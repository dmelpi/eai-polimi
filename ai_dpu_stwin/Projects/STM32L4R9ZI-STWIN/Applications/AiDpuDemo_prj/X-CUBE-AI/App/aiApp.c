/**
 *********************************************************************************
 * @file    aiApp.c
 * @author  STMicroelectronics - AIS - MCD Team
 * @version V2.0.0
 * @date    31-March-2022
 *********************************************************************************
 * @attention
 *
 * <h2><center>COPYRIGHT &copy; 2021 STMicroelectronics</center></h2>
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file in
 * the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *********************************************************************************
 */

/*
 * Description:
 */

/* System headers */
#include <stdlib.h>
#include <stdio.h>


/* header files */
#include "aiTestHelper.h"

/* AI Run-time header files */
#include "ai_platform_interface.h"

/* AI x-cube-ai files */
#include "app_x-cube-ai.h"

#include "services/sysdebug.h"

#define SYS_DEBUGF(level, message)      SYS_DEBUGF3(SYS_DBG_AI, level, message)

#define AI_NETWORK_CTX_IDX 0

#define AI_MNETWORK_OUT_NUM_MAX 2
#if (AI_MNETWORK_OUT_NUM > AI_MNETWORK_OUT_NUM_MAX  )
#error Number of output is too high
#endif

/* Global variables */

FILE * plcPrintFile;

/* -----------------------------------------------------------------------------
 * AI-related functions
 * -----------------------------------------------------------------------------
 */

//DEF_DATA_IN;

//DEF_DATA_OUT;

struct ai_network_exec_ctx {
  ai_handle handle;
  ai_network_report report;
} net_exec_ctx[AI_MNETWORK_NUMBER] = {0};

static int aiBootstrap(struct ai_network_exec_ctx *ctx, const char *nn_name)
{
  ai_error err;

  /* Creating the instance of the  network ------------------------- */
  //LC_PRINT("Creating the network \"%s\"..\r\n", nn_name);

  err = ai_mnetwork_create(nn_name, &ctx->handle, NULL);
  if (err.type) {
    aiLogErr(err, "ai_mnetwork_create");
    return -1;
  }

  /* Initialize the instance --------------------------------------- */
  LC_PRINT("Initializing the network\r\n");

  if (!ai_mnetwork_get_report(ctx->handle, &ctx->report)) {
    err = ai_mnetwork_get_error(ctx->handle);
    aiLogErr(err, "ai_mnetwork_get_info");
    ai_mnetwork_destroy(ctx->handle);
    ctx->handle = AI_HANDLE_NULL;
    return -2;
  }

  if (!ai_mnetwork_init(ctx->handle)) {
    err = ai_mnetwork_get_error(ctx->handle);
    aiLogErr(err, "ai_mnetwork_init");
    ai_mnetwork_destroy(ctx->handle);
    ctx->handle = AI_HANDLE_NULL;
    return -4;
  }

  /* Display the network info -------------------------------------- */
  if (ai_mnetwork_get_report(ctx->handle, &ctx->report)) {
    aiPrintNetworkInfo(&ctx->report);
  } else {
    err = ai_mnetwork_get_error(ctx->handle);
    aiLogErr(err, "ai_mnetwork_get_info");
    ai_mnetwork_destroy(ctx->handle);
    ctx->handle = AI_HANDLE_NULL;
    return -2;
  }

  return 0;
}

int aiInit(const char* nn_name)
{
  int res = -1;
  plcPrintFile = stdout;

  aiPlatformVersion();

  /* Reset the contexts -------------------------------------------- */

  if (strncmp(nn_name,AI_NETWORK_MODEL_NAME, 10)==0)  {
	 net_exec_ctx[AI_NETWORK_CTX_IDX].handle = AI_HANDLE_NULL;
	 res = aiBootstrap(&net_exec_ctx[AI_NETWORK_CTX_IDX],nn_name) ;
  }
  else  {
	//LC_PRINT("\r\nAI Error : Did not Found network \"%s\"\r\n", nn_name);
  }
  return res;
}

void aiDeInit(const char* nn_name)
{
  ai_error err;
  if (strncmp(nn_name,AI_NETWORK_MODEL_NAME, 10)==0){
	LC_PRINT("Releasing %s...\r\n",nn_name);
	if (net_exec_ctx[AI_NETWORK_CTX_IDX].handle != AI_HANDLE_NULL) {
	  if (ai_mnetwork_destroy(net_exec_ctx[AI_NETWORK_CTX_IDX].handle)
		  != AI_HANDLE_NULL) {
		err = ai_mnetwork_get_error(net_exec_ctx[AI_NETWORK_CTX_IDX].handle);
		aiLogErr(err, "ai_mnetwork_destroy");
	  }
	}
	net_exec_ctx[AI_NETWORK_CTX_IDX].handle = AI_HANDLE_NULL;
  }
}

int aiProcess(const char* nn_name, float * p_inData, float p_out_data[2])
{
  ai_i32 batch;
  int idx; /* only one fixed network */

  ai_buffer ai_input;
  ai_buffer ai_output[AI_MNETWORK_OUT_NUM_MAX];

  if (strncmp(nn_name,AI_NETWORK_MODEL_NAME, 10)==0){
	idx = AI_NETWORK_CTX_IDX;
  }
  else return -1;

  if(net_exec_ctx[idx].handle == AI_HANDLE_NULL)
  {
	//LC_PRINT("E: network handle is NULL\r\n");
    return -1;
  }
  if ((net_exec_ctx[idx].report.n_inputs > AI_MNETWORK_IN_NUM) ||
	  (net_exec_ctx[idx].report.n_outputs > AI_MNETWORK_OUT_NUM))
  {
	//LC_PRINT("E: AI_MNETWORK_IN/OUT_NUM definition are incoherent\r\n");
	HAL_Delay(100);
	return -1;
  }

  /* we are dealing only with  1/1 or 1/2 networks  */
  ai_input          = net_exec_ctx[idx].report.inputs[0];
  ai_input.data     = AI_HANDLE_PTR(p_inData);
  ai_output[0]      = net_exec_ctx[idx].report.outputs[0];
  ai_output[0].data = AI_HANDLE_PTR(data_outs[0]);
  if (net_exec_ctx[idx].report.n_outputs == 2)
  {
	ai_output[1]      = net_exec_ctx[idx].report.outputs[1];
	ai_output[1].data = AI_HANDLE_PTR(data_outs[1]);
  }

  batch = ai_mnetwork_run(net_exec_ctx[idx].handle, &ai_input, ai_output);
  if (batch != 1) {
    aiLogErr(ai_mnetwork_get_error(net_exec_ctx[idx].handle),"ai_mnetwork_run");
  }
  if (AI_NETWORK_CTX_IDX == idx )
  {
    /* check correct init  */
    if( net_exec_ctx[idx].report.n_outputs == 2)
    {
	  float *p_out0 = (float*) ai_output[0].data;
	  float *p_out1 = (float*) ai_output[1].data;

	  p_out_data[0] = p_out0[0];
	  p_out_data[1] = p_out1[(int) p_out0[0]] * 100.0;
    }
    else if(net_exec_ctx[idx].report.n_outputs == 1)
    {
	  float *p_out0 = (float*) ai_output[0].data;
	  float max_out = *p_out0;
	  int max_idx = 0;
	  for(int i = 1; i < AI_BUFFER_SIZE(&ai_output[0]); i++)
	  {
	    if(p_out0[i] > max_out)
		{
		  max_idx = i;
		  max_out = p_out0[i];
		}
	  }
	  p_out_data[0] = max_idx;
	  p_out_data[1] = max_out * 100.0;
    }
    SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("Class: %d ,  Accuracy: %f   \r\n", (int) p_out_data[0] , p_out_data[1]));
    //SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("Class: %d  \r\n", (int) p_out_data[0]));
  }
  return 0;
}

void aiPrintNetworkInfoToFile(const char* nn_name, FILE *out)
{

  struct ai_network_exec_ctx *p_ctx = NULL;
  FILE *plcPrintFileSave = plcPrintFile;
  plcPrintFile = out;

  if (strncmp(nn_name,AI_NETWORK_MODEL_NAME, 10)==0)  {
	p_ctx = &net_exec_ctx[AI_NETWORK_CTX_IDX];
  }

  if (p_ctx) {
	if (ai_mnetwork_get_report(p_ctx->handle, &p_ctx->report)) {
	    aiPrintNetworkInfo(&p_ctx->report);
	  }else{
		LC_PRINT("\r\nAI Error : Cannot print network \"%s\"\r\n", nn_name);
	  }
  }
  else{
	LC_PRINT("\r\nAI Error : Did not Found network \"%s\"\r\n", nn_name);
  }
  plcPrintFile = plcPrintFileSave;

  return;
}

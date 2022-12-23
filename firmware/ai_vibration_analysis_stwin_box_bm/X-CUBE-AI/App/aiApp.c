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
//#include "arm_math.h"

/* AI Run-time header files */
#include "ai_platform_interface.h"
#include "network.h"
#include "network_data.h"
#include "arm_math.h"

#define SYS_DEBUGF(level, message)      SYS_DEBUGF3(SYS_DBG_AI, level, message)

//#define AI_NETWORK_CTX_IDX 0

#define AI_MNETWORK_OUT_NUM_MAX 2
#if (AI_MNETWORK_OUT_NUM > AI_MNETWORK_OUT_NUM_MAX  )
#error Number of output is too high
#endif


#if(AI_NETWORK_OUT_NUM == 1)
#define AI_NETWORK_OUT_2_SIZE 1
static ai_float out_1_data[AI_NETWORK_OUT_1_SIZE];
static ai_float* out_data[AI_NETWORK_OUT_NUM] = {
&out_1_data[0]
};
#elif(AI_NETWORK_OUT_NUM == 2)
static ai_float out_1_data[AI_NETWORK_OUT_1_SIZE];
static ai_float out_2_data[AI_NETWORK_OUT_2_SIZE];

/* C-table to store the @ of the output buffers */
static ai_float* out_data[AI_NETWORK_OUT_NUM] = {
	&out_1_data[0],
	&out_2_data[0]
	};
#endif


/* Global handle to reference the instantiated C-model */
ai_handle network = AI_HANDLE_NULL;

/* Global c-array to handle the activations buffer */
AI_ALIGNED(32)
ai_u8 activations[AI_NETWORK_DATA_ACTIVATIONS_SIZE];

ai_buffer *ai_input;
ai_buffer *ai_output;
float32_t *p_out0;
float32_t *p_out1;



int aiInit(void)
{
  ai_error err;

  /* Create and initialize the c-model */
  const ai_handle acts[] = { activations };
  err = ai_network_create_and_init(&network, acts, NULL);
  if (err.type != AI_ERROR_NONE) {

  };

  /* Retrieve pointers to the model's input/output tensors */
  ai_input = ai_network_inputs_get(network , NULL);
  ai_output = ai_network_outputs_get(network, NULL);

  return 0;
}



int aiProcess(float *p_inData, float p_out_data[2])
{
  ai_i32 n_batch;

  /* 1 - Update IO handlers with the data payload */
  ai_input[0].data = AI_HANDLE_PTR(p_inData);
  ai_output[0].data = AI_HANDLE_PTR(out_data[0]);

  if (AI_NETWORK_OUT_NUM == 2){
	  ai_output[1].data = AI_HANDLE_PTR(out_data[1]);
  }

  if (AI_NETWORK_OUT_NUM == 1){
	  /* 2 - Perform the inference */
	  n_batch = ai_network_run(network, &ai_input[0], &ai_output[0]);
	  p_out0 = (float32_t*) ai_output[0].data;
	  float32_t max_out = *p_out0;
	  uint32_t max_idx = 0;

	  arm_max_f32(p_out0,AI_NETWORK_OUT_1_SIZE, &max_out, &max_idx);
	  p_out_data[0] = (float32_t)max_idx;
	  p_out_data[1] = max_out * 100.0;

  }
  else if (AI_NETWORK_OUT_NUM == 2){
	  n_batch = ai_network_run(network, &ai_input[0], &ai_output[0]);
	  p_out0 = (float32_t*) ai_output[0].data;
	  p_out1 = (float32_t*) ai_output[1].data;

	  p_out_data[0] = (float32_t)p_out0[0];
	  p_out_data[1] = (float32_t)p_out1[(int) p_out0[0]];

  }

  if (n_batch != 1) {
	  ai_network_get_error(network);
  };


  return 0;
}






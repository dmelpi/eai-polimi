/**
  ******************************************************************************
  * @file    AiDPU.c
  * @author  STMicroelectronics - AIS - MCD Team
  * @brief
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#include "AiDPU.h"
#include "AiDPU_vtbl.h"
#include "params.h"
#include "services/sysdebug.h"
#include "mfcc_params.h"
#include "arm_math.h"


// set the AIDPU_G_TP_MS_2 as 9.81 if acceleration data inside the DPU are needed in [m/s^2] otherwise set it to 1.0
//#define AIDPU_G_TO_MS_2 (9.81F)
#define AIDPU_G_TO_MS_2 (1.0F)

#define SYS_DEBUGF(level, message)      SYS_DEBUGF3(SYS_DBG_AI, level, message)

/**
 * Specified the virtual table for the AiDPU_t class.
 */
static const IDPU_vtbl sAiDPU_vtbl = {
    AiDPU_vtblInit,
    ADPU_AttachToSensor_vtbl,
    ADPU_DetachFromSensor_vtbl,
    ADPU_AttachInputADPU_vtbl,
    ADPU_DetachFromADPU_vtbl,
    ADPU_DispatchEvents_vtbl,
    ADPU_RegisterNotifyCallbacks_vtbl,
    AiDPU_vtblProcess,
};


//pre_processing_data_t pre_processing_data;
static arm_mfcc_instance_f32 mfcc_inst;
static float32_t samples_buf[N_FFT];	// contains all samples needed for MFCC
static float32_t samples_hist_buf[HOP_LEN];	// since MFCC is destructive, needed to store last slice of data
static float32_t mfcc_temp_buf[N_FFT+2];

static float32_t nn_input[AI_NETWORK_IN_1_SIZE];
//static float32_t nn_output[AI_NETWORK_OUT_1_SIZE];
/* Private member function declaration */
/***************************************/

/**
 * Check if the DPU is initialized and ready to receive and process data.
 *
 * @param _this [IN] specifies a pointer to the object.
 * @return TRUE if the object is initialized, FALSE otherwise.
 */
static inline boolean_t AiDPUAreStreamsInitialized(AiDPU_t *_this);


/* Public API functions definition */
/***********************************/

IDPU *AiDPUAlloc() {
  IDPU *p_obj = (IDPU*) SysAlloc(sizeof(AiDPU_t));

  if (p_obj != NULL)
  {
    p_obj->vptr = &sAiDPU_vtbl;
  }
  return p_obj;
}

IDPU *AiDPUStaticAlloc(void *p_mem_block)
{
  IDPU *p_obj = (IDPU*)p_mem_block;
  if (p_obj != NULL)
  {
    p_obj->vptr = &sAiDPU_vtbl;
  }

  return p_obj;
}

sys_error_code_t AiDPUSetSensitivity(AiDPU_t *_this, float sensi)
{
  assert_param(_this != NULL);

  _this->scale = sensi * AIDPU_G_TO_MS_2;

  return SYS_NO_ERROR_CODE;
}



uint16_t AiDPUSetStreamsParam(AiDPU_t *_this, uint16_t signal_size, uint8_t axes, uint8_t cb_items)
{
  assert_param(_this != NULL);

  if (!AiDPUAreStreamsInitialized(_this))
  {
    /* DPU has been already initialized, so first reset it, if needed */
  }

  /* DPU converts input data in float */
  _this->super.dpuWorkingStream.packet.payload_type = AI_FMT;
  _this->super.dpuWorkingStream.packet.payload_fmt  = AI_SP_FMT_FLOAT32_RESET();

  /* the shape is 1D the the microphone as 1 axis (X)*/
  _this->super.dpuWorkingStream.packet.shape.n_shape                          = 1 ;
  _this->super.dpuWorkingStream.packet.shape.shapes[AI_LOGGING_SHAPES_WIDTH]  = axes;
  _this->super.dpuWorkingStream.packet.shape.shapes[AI_LOGGING_SHAPES_HEIGHT] = signal_size;

  /* Initialize the out stream */
  _this->super.dpuOutStream.packet.shape.n_shape                         = 1;
  _this->super.dpuOutStream.packet.shape.shapes[AI_LOGGING_SHAPES_WIDTH] = 2;
  _this->super.dpuOutStream.packet.payload_type = AI_FMT;
  _this->super.dpuOutStream.packet.payload_fmt  = AI_SP_FMT_FLOAT32_RESET();
  _this->super.dpuOutStream.packet.payload_size = sizeof(_this->ai_out);
  _this->super.dpuOutStream.packet.payload      = (uint8_t*)&_this->ai_out[0];

  /* compute the size in byte of one cb item, */
  _this->super.n_bytes_for_item = axes * signal_size * sizeof(float);
  _this->super.cb_items         = cb_items;

  _this->stream_ready = TRUE;

  return (cb_items * _this->super.n_bytes_for_item);
}

sys_error_code_t AiDPUPrepareToProcessData(AiDPU_t *_this)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  ADPU_Reset((ADPU*)_this);
  _this->ai_out[0] = 0;
  _this->ai_out[1] = 0;

  return res;
}


/* IDPU virtual functions definition */
/*************************************/

sys_error_code_t AiDPU_vtblInit(IDPU *_this) {
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  AiDPU_t *p_obj =(AiDPU_t*)_this;

  res = ADPU_Init_vtbl(_this);
  if (!SYS_IS_ERROR_CODE(res))
  {
    p_obj->ai_out[0] = 0;
    p_obj->ai_out[1] = 0;
    p_obj->stream_ready = FALSE;
    // take the ownership of the Sensor Event IF
    IEventListenerSetOwner((IEventListener *) ADPU_GetEventListenerIF(&p_obj->super), &p_obj->super);

    /* Free signal pre-processing functions */
//    pre_processing_free(&pre_processing_data);

    /* Initialize signal pre-processing functions */
//    pre_processing_init(&pre_processing_data);
    arm_mfcc_init_f32(&mfcc_inst, N_FFT, N_MELS, N_DCT,
    		mfcc_dct_coefs_config1_f32,
			mfcc_filter_pos_config1_f32,
			mfcc_filter_len_config1_f32,
			mfcc_filter_coefs_config1_f32,
			mfcc_window_coefs_config1_f32);

    /* Initialize AI library */
    if (aiInit(NETWORK_NAME)==0)
    {
	  /* set the initial mode to process */
	  p_obj->ai_processing_f = aiProcess;
    }
  }

  return res;
}

sys_error_code_t AiDPU_vtblProcess(IDPU *_this)
{
  assert_param(_this != NULL);
  sys_error_code_t xRes = SYS_NO_ERROR_CODE;
  ADPU *super = (ADPU*) _this;
  AiDPU_t *p_obj = (AiDPU_t*) _this;
  CBItem **p_consumer_buff = NULL;
  CircularBuffer *p_circular_buffer = NULL;

  static uint32_t slice_cnt = 1;

  //DPU has the priority
  if(!super->isADPUattached)
  {
    for(int i = 0; i < ADPU_CFG_MAX_SENSOR; i++)
    {
      if(super->sensors[i].sensorIF != NULL)
      { //TODO sensors[i].pConsumerDataBuff could be not necessary
        uint32_t sensor_ready = CB_GetReadyItemFromTail(super->sensors[i].cb_handle.pCircularBuffer, &super->sensors[i].cb_handle.pConsumerDataBuff);
        if(sensor_ready == SYS_CB_NO_READY_ITEM_ERROR_CODE)
        {
          return SYS_CB_NO_READY_ITEM_ERROR_CODE;
        }
        p_consumer_buff = &super->sensors[i].cb_handle.pConsumerDataBuff;
        p_circular_buffer = super->sensors[i].cb_handle.pCircularBuffer;
      }
    }
  }
  else
  {
    uint32_t process_ready = CB_GetReadyItemFromTail(super->AttachedAdpu.cb_handle.pCircularBuffer, &super->AttachedAdpu.cb_handle.pConsumerDataBuff);
    if(process_ready == SYS_CB_NO_READY_ITEM_ERROR_CODE)
    {
//      return SYS_NO_ERROR_CODE;
      return SYS_CB_NO_READY_ITEM_ERROR_CODE;
    }
    p_consumer_buff = &super->AttachedAdpu.cb_handle.pConsumerDataBuff;
    p_circular_buffer = super->AttachedAdpu.cb_handle.pCircularBuffer;
  }

  if((*p_consumer_buff) != NULL)
  {
    assert_param(AIDPU_NB_AXIS == p_obj->super.dpuWorkingStream.packet.shape.shapes[AI_LOGGING_SHAPES_WIDTH]);
    assert_param(INPUT_BUFFER_SIZE == p_obj->super.dpuWorkingStream.packet.shape.shapes[AI_LOGGING_SHAPES_HEIGHT]);

    float32_t *p_in = (float32_t*) CB_GetItemData((*p_consumer_buff));
    float32_t scale = p_obj->scale;

    // TODO use DMA
    // TODO make generic copy for any hop_len
    for(int i=0; i<HOP_LEN; i++)
    {
    	samples_buf[i] = samples_hist_buf[i];
    }

    for(int i = 0; i < HOP_LEN; i++)
    {
    	float32_t num = p_in[i] * scale;
    	samples_buf[i+HOP_LEN] = num;
    	samples_hist_buf[i] = num;	// store history
    }

    // release the buffer as soon as possible
 	CB_ReleaseItem(p_circular_buffer, (*p_consumer_buff));
	(*p_consumer_buff) = NULL;

	// shift input history by one
	for(int i=0; i<N_DCT*(N_MFCC_HIST-1); i++)
	{
		nn_input[i] = nn_input[i+N_DCT];	//TODO DMA
	}


	// pre-process every new sample slice and store MFCC
    arm_mfcc_f32(&mfcc_inst, samples_buf, &nn_input[N_DCT*(N_MFCC_HIST-1)], mfcc_temp_buf);

    // call the AI at a desired rate (here every 10s).
    // TODO: history shift is necessary only once per AI call, can optimize
    if(!(slice_cnt%(N_MFCC_HIST-1)))
	{
		ProcessEvent evt_acc;
		ProcessEventInit((IEvent*) &evt_acc, super->pProcessEventSrc, (ai_logging_packet_t*) &super->dpuOutStream, ADPU_GetTag(super));
		IDPU_DispatchEvents(_this, &evt_acc);
		p_obj->ai_processing_f(NETWORK_NAME, nn_input, p_obj->ai_out);
	}
	slice_cnt++;

  }
  return xRes;
}

/* Private member function definition */
/**************************************/

static inline boolean_t AiDPUAreStreamsInitialized(AiDPU_t *_this)
{
  assert_param(_this != NULL);

  return _this->stream_ready;
}

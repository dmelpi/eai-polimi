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
#include "pre_processing_app.h"

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


pre_processing_data_t pre_processing_data;


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
    pre_processing_free(&pre_processing_data);

    /* Initialize signal pre-processing functions */
    pre_processing_init(&pre_processing_data);

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
//          return SYS_NO_ERROR_CODE;
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
	  /*-----August  experiment for data acquisition
	  assert_param(p_obj->scale != 0.0F);
	  float *p_in = (float*) CB_GetItemData((*p_consumer_buff));
	  float scale = p_obj->scale;

	  float32_t raw_data[INPUT_BUFFER_SIZE];
	  //monodimensional_data_t raw_data[INPUT_BUFFER_SIZE];


	  for(int i = 0; i < INPUT_BUFFER_SIZE; i++)
		  {
			raw_data[i] = *p_in++;
		  }

	  for(int i = 0; i < INPUT_BUFFER_SIZE; i++)  //AL MOMENTO CB size = 16
		  {
			  SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE,("%f, \r\n",(float32_t)(raw_data[i])));
			  //p_in=p_in+7; //undersampling to keep up with pace of the acquisition??
			  // previously done as p_in=p_in+4*sizeof(p_in);
		  }


	  CB_ReleaseItem(p_circular_buffer, (*p_consumer_buff));
	  (*p_consumer_buff) = NULL; */

	/*starting original code--> now modified to accommodate new pre-processing */
	tridimensional_data_t raw_data[INPUT_BUFFER_SIZE];

    //assert_param(p_obj->scale != 0.0F); ---> scale parameter is not used with the microphone
    assert_param(AIDPU_NB_AXIS == p_obj->super.dpuWorkingStream.packet.shape.shapes[AI_LOGGING_SHAPES_WIDTH]);
    assert_param(INPUT_BUFFER_SIZE == p_obj->super.dpuWorkingStream.packet.shape.shapes[AI_LOGGING_SHAPES_HEIGHT]);

    float *p_in = (float*) CB_GetItemData((*p_consumer_buff));
    //float scale = p_obj->scale;
    /*Modified on 20/09 */

    for(int i = 0; i < INPUT_BUFFER_SIZE; i++)
    {
      raw_data[i].x = *p_in++;
      raw_data[i].y = 0;
      raw_data[i].z = 0;
    }

    /*dummy data creation: sine wave as acquired by the sensor
    float32_t dummy_data[INPUT_BUFFER_SIZE]={-321, -335, -345, -330, -281, -224, -188, -132, -60, 11, 69, 116, 166, 200, 229, 250, 269, 297, 315, 331, 340, 336, 299, 240, 199, 147, 81, 7, -56, -104, -162, -203, -232, -249, -267, -294, -312, -326, -338, -335, -296, -237, -197, -149, -82, -10, 53, 102, 153, 194, 225, 244, 265, 294, 314, 328, 339, 345, 319, 261, 217, 173, 107, 36, -38, -90, -138, -188, -223, -247, -263, -287, -311, -327, -338, -336, -313, -257, -207, -169, -104, -32, 35, 85, 131, 179, 215, 235, 251, 277, 304, 322, 334, 344, 333, 286, 230, 190, 129, 56, -14, -73, -121, -176, -215, -243, -258, -277, -306, -325, -335, -342, -331, -281, -224, -186, -136, -69, 6, 65, 110, 150, 201, 229, 224, 259, 295, 305, 310, 339, 344, 291, 243, 192, 146, 79, -5, -60, -118, -172, -200, -237, -257, -273, -296, -321, -327, -334, -345, -305, -249, -195, -156, -100, -18, 46, 93, 139, 178, 209, 230, 251, 281, 301, 320, 335, 343, 320, 270, 228, 177, 105, 41, -26, -86, -140, -189, -226, -252, -266, -292, -317, -326, -336, -345, -323, -258, -207, -174, -114, -36, 38, 84, 126, 184, 216, 233, 256, 278, 305, -321, -335, -345, -330, -281, -224, -188, -132, -60, 11, 69, 116, 166, 200, 229, 250, 269, 297, 315, 331, 340, 336, 299, 240, 199, 147, 81, 7, -56, -104, -162, -203, -232, -249, -267, -294, -312, -326, -338, -335, -296, -237, -197, -149, -82, -10, 53, 102, 153, 194, 225, 244, 265, 294, 314, 328};
    for(int i = 0; i < INPUT_BUFFER_SIZE; i++)
    {
      raw_data[i].x = dummy_data[i];
      raw_data[i].y = 0;
      raw_data[i].z = 0;
    }*/


    /*dummy data creation: ramp from 0 to 255
    for(int i = 0; i < INPUT_BUFFER_SIZE; i++)
    {
      raw_data[i].x = i;
      raw_data[i].y = 0;
      raw_data[i].z = 0;
    }*/



    /*dummy data creation: sin440 hz as acquired by the firmware*/
     float32_t preprocessing_output_array[AI_NETWORK_IN_1_SIZE];

    // call preprocessing function
     pre_processing_process(raw_data, INPUT_BUFFER_SIZE, preprocessing_output_array, AI_NETWORK_IN_1_SIZE, &pre_processing_data);

     // call Ai library.
    p_obj->ai_processing_f(NETWORK_NAME, (float*) preprocessing_output_array, p_obj->ai_out);
    SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("[AiDPU_vtblProcess()] label_id: %d, accuracy: %f.\r\n", (int) (p_obj->ai_out[0]), (float) (p_obj->ai_out[1])));

    // release the buffer as soon as possible
 	CB_ReleaseItem(p_circular_buffer, (*p_consumer_buff));
	(*p_consumer_buff) = NULL;

    ProcessEvent evt_acc;
    ProcessEventInit((IEvent*) &evt_acc, super->pProcessEventSrc, (ai_logging_packet_t*) &super->dpuOutStream, ADPU_GetTag(super));
    IDPU_DispatchEvents(_this, &evt_acc);


	  /* ORIGINAL CODE:
	  	tridimensional_data_t raw_data[INPUT_BUFFER_SIZE];

	      assert_param(p_obj->scale != 0.0F);
	      assert_param(AIDPU_NB_AXIS == p_obj->super.dpuWorkingStream.packet.shape.shapes[AI_LOGGING_SHAPES_WIDTH]);
	      assert_param(INPUT_BUFFER_SIZE == p_obj->super.dpuWorkingStream.packet.shape.shapes[AI_LOGGING_SHAPES_HEIGHT]);

	      float *p_in = (float*) CB_GetItemData((*p_consumer_buff));
	      float scale = p_obj->scale;

	      for(int i = 0; i < INPUT_BUFFER_SIZE; i++)
	      {
	        raw_data[i].x = *p_in++ * scale;
	        raw_data[i].y = *p_in++ * scale;
	        raw_data[i].z = *p_in++ * scale;
	      }


	       float32_t preprocessing_output_array[AI_NETWORK_IN_1_SIZE];

	      // call preprocessing function
	       pre_processing_process(raw_data, INPUT_BUFFER_SIZE, preprocessing_output_array, AI_NETWORK_IN_1_SIZE, &pre_processing_data);
	      // call Ai library.
	      p_obj->ai_processing_f(NETWORK_NAME, (float*) preprocessing_output_array, p_obj->ai_out);
	      SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("[AiDPU_vtblProcess()] label_id: %d, accuracy: %f.\r\n", (int) (p_obj->ai_out[0]), (float) (p_obj->ai_out[1])));

	      // release the buffer as soon as possible
	   	CB_ReleaseItem(p_circular_buffer, (*p_consumer_buff));
	  	(*p_consumer_buff) = NULL;

	      ProcessEvent evt_acc;
	      ProcessEventInit((IEvent*) &evt_acc, super->pProcessEventSrc, (ai_logging_packet_t*) &super->dpuOutStream, ADPU_GetTag(super));
	      IDPU_DispatchEvents(_this, &evt_acc);
	  */

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

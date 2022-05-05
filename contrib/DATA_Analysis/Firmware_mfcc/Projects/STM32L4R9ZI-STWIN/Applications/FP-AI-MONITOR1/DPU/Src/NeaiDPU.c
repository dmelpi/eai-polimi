/**
******************************************************************************
* @file    NeaiDPU.c
* @author  SRA - MCD
* @version 1.0.0
* @date    6-Sep-2021
*
* @brief
*
* <DESCRIPTIOM>
*
******************************************************************************
* @attention
*
* <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
* All rights reserved.</center></h2>
*
* This software component is licensed by ST under ODE SOFTWARE LICENSE AGREEMENT
* SLA0094, the "License"; You may not use this file except in compliance with
* the License. You may obtain a copy of the License at:
*                             www.st.com/SLA0094
*
******************************************************************************
*/

#include "NeaiDPU.h"
#include "NeaiDPU_vtbl.h"
#include "services/sysdebug.h"


#ifndef NEAI_DPU_CFG_LEARN_NORMALIZATION_FACTOR
#define NEAI_DPU_CFG_LEARN_NORMALIZATION_FACTOR    1.0f
#endif


/**
* Specified the virtual table for the NeaiDPU_t class.
*/
static const IDPU_vtbl sNeaiDPU_vtbl = {
  NeaiDPU_vtblInit,
  ADPU_AttachToSensor_vtbl,
  ADPU_DetachFromSensor_vtbl,
  ADPU_AttachInputADPU_vtbl,
  ADPU_DetachFromADPU_vtbl,
  ADPU_DispatchEvents_vtbl,
  ADPU_RegisterNotifyCallbacks_vtbl,
  NeaiDPU_vtblProcess
};


/* Inline functions definition */
/*******************************/


/* GCC requires one function forward declaration in only one .c source
* in order to manage the inline.
* See also http://stackoverflow.com/questions/26503235/c-inline-function-and-gcc
*/
#if defined (__GNUC__) || defined (__ICCARM__)
extern ENeaiMode_t NeaiDPUGetProcessingMode(NeaiDPU_t *_this);
extern float NeaiDPUGetProcessResult(NeaiDPU_t *_this);
#endif


/* Private member function declaration */
/***************************************/

/**
* Check if the DPU is initialized and ready to receive and process data.
*
* @param _this [IN] specifies a pointer to the object.
* @return TRUE if the object is initialized, FALSE otherwise.
*/
static inline boolean_t NeaiDPUAreStreamsInitialized(NeaiDPU_t *_this);


/* Public API functions definition */
/***********************************/

IDPU *NeaiDPUAlloc() {
  IDPU *p_obj = (IDPU*) pvPortMalloc(sizeof(NeaiDPU_t));
  
  if (p_obj != NULL)
  {
    p_obj->vptr = &sNeaiDPU_vtbl;
  }
  
  return p_obj;
}

IDPU *NeaiDPUStaticAlloc(void *p_mem_block)
{
  IDPU *p_obj = (IDPU*)p_mem_block;
  if (p_obj != NULL)
  {
    p_obj->vptr = &sNeaiDPU_vtbl;
  }
  
  return p_obj;
}

sys_error_code_t NeaiDPUSetProcessingMode(NeaiDPU_t *_this, ENeaiMode_t mode)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  
  if (mode == E_NEAI_DETECTION)
  {
    _this->neai_processing_f = NanoEdgeAI_detect;
  }
  else if (mode == E_NEAI_LEARNING)
  {
    _this->neai_processing_f = NanoEdgeAI_learn;
  }
  else
  {
    _this->neai_processing_f = NULL;
  }
  
  return res;
}

uint32_t NeaiDPUSetStreamsParam(NeaiDPU_t *_this, uint16_t signal_size, uint8_t axes, uint8_t cb_items) {
  assert_param(_this != NULL);
  uint32_t working_buffer_size_byte = 0;
  
  if (!NeaiDPUAreStreamsInitialized(_this))
  {
    /* DPU has been already initialized, so first reset it, if needed */
  }
  
  /* Nanoedge wants data interleaved [x0,y0,z0, x1,y1,z1, ...] to make the inference (or learn) */
  _this->super.dpuWorkingStream.mode = AI_SP_MODE_COLUMN;
  /* DPU converts input data in float */
  _this->super.dpuWorkingStream.packet.payload_type = AI_FMT;
  _this->super.dpuWorkingStream.packet.payload_fmt = AI_SP_FMT_FLOAT32_RESET();
  
  /* the shape is always 1d because NanoEdgAI wants interleaved data */
  ai_logging_create_shape_2d(&_this->super.dpuWorkingStream.packet.shape, axes, signal_size);
  
  /* compute the size in byte of one cb item, */
  uint16_t rows =  _this->super.dpuWorkingStream.packet.shape.shapes[AI_LOGGING_SHAPES_HEIGHT] ;
  uint16_t columns = _this->super.dpuWorkingStream.packet.shape.shapes[AI_LOGGING_SHAPES_WIDTH] ;
  _this->super.n_bytes_for_item =  rows * columns * sizeof(float);
  
  /* Initialize the out stream */
  ai_logging_create_shape_0d(&_this->super.dpuOutStream.packet.shape);
  _this->super.dpuOutStream.packet.payload_type = AI_FMT;
  _this->super.dpuOutStream.packet.payload_fmt = AI_SP_FMT_FLOAT32_RESET();
  _this->super.dpuOutStream.packet.payload_size = sizeof(float);
  _this->super.dpuOutStream.packet.payload = (uint8_t*)&_this->neai_out;
  _this->super.cb_items = cb_items;
  
  _this->stream_ready = TRUE;
  
  working_buffer_size_byte = cb_items * _this->super.n_bytes_for_item;
  
  return working_buffer_size_byte;
}

sys_error_code_t NeaiDPUPrepareToProcessData(NeaiDPU_t *_this)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  
  ADPU_Reset((ADPU*)_this);
  _this->neai_out = 0.0f;
  
  return res;
}

sys_error_code_t NeaiDPUSetSensitivity(NeaiDPU_t *_this, float sensitivity)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  
  _this->sensitivity = sensitivity;
  
  NanoEdgeAI_set_sensitivity(_this->sensitivity);
  
  return res;
}


/* IDPU virtual functions definition */
/*************************************/

sys_error_code_t NeaiDPU_vtblInit(IDPU *_this) {
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  NeaiDPU_t *p_obj =(NeaiDPU_t*)_this;
  
  res = ADPU_Init_vtbl(_this);
  if (!SYS_IS_ERROR_CODE(res)) {
    p_obj->neai_out = 0;
    p_obj->stream_ready = FALSE;
    /* take the ownership of the Sensor Event IF */
    IEventListenerSetOwner((IEventListener *) ADPU_GetEventListenerIF(&p_obj->super), &p_obj->super);
    
    /* initialize NanoEdge AI library */
    NanoEdgeAI_initialize();
    
    /* set the initial mode to learn */
    p_obj->neai_processing_f = NanoEdgeAI_learn;
  }
  
  return res;
}

sys_error_code_t NeaiDPU_vtblProcess(IDPU *_this)
{
  assert_param(_this != NULL);
  sys_error_code_t xRes = SYS_NO_ERROR_CODE;
  ADPU * super = (ADPU*)_this;
  NeaiDPU_t *p_obj = (NeaiDPU_t*)_this;
  
  CBItem **p_consumer_buff = NULL;
  CircularBuffer *p_circular_buffer = NULL;
  
  /* DPU has the priority */
  if(!super->isADPUattached)
  {
    for(int i=0; i < ADPU_CFG_MAX_SENSOR; i++)
    {
      if(super->sensors[i].sensorIF != NULL)
      {
        uint32_t sensor_ready = CB_GetReadyItemFromTail(super->sensors[i].cb_handle.pCircularBuffer, &super->sensors[i].cb_handle.pConsumerDataBuff);
        if(sensor_ready == CB_NO_READY_ITEM_ERROR_CODE)
        {
          return SYS_NO_ERROR_CODE;
        }
        p_consumer_buff = &super->sensors[i].cb_handle.pConsumerDataBuff;
        p_circular_buffer = super->sensors[i].cb_handle.pCircularBuffer;
      }
    }
  }
  else
  {
    uint32_t process_ready = CB_GetReadyItemFromTail(super->AttachedAdpu.cb_handle.pCircularBuffer, &super->AttachedAdpu.cb_handle.pConsumerDataBuff);
    if(process_ready == CB_NO_READY_ITEM_ERROR_CODE)
    {
      return xRes;
    }
    p_consumer_buff = &super->AttachedAdpu.cb_handle.pConsumerDataBuff;
    p_circular_buffer = super->AttachedAdpu.cb_handle.pCircularBuffer;
  }
  
  if ((*p_consumer_buff) != NULL)
  {
    float *p_signal =  (float*) CB_GetItemData((*p_consumer_buff));
    p_obj->neai_out = p_obj->neai_processing_f(p_signal);
    if (p_obj->neai_processing_f == NanoEdgeAI_learn)
    {
      /* normalize the output */
      p_obj->neai_out *= NEAI_DPU_CFG_LEARN_NORMALIZATION_FACTOR;
    }
    
    /* release the buffer as soon as possible */
    CB_ReleaseItem(p_circular_buffer, (*p_consumer_buff));
    (*p_consumer_buff) = NULL;
    
    ProcessEvent evt_acc;
    ProcessEventInit((IEvent*)&evt_acc, super->pProcessEventSrc, (ai_logging_packet_t*)&super->dpuOutStream, ADPU_GetTag(super));
    IDPU_DispatchEvents(_this, &evt_acc);
  }
  
  return xRes;
}

/* Private member function definition */
/**************************************/

static inline boolean_t NeaiDPUAreStreamsInitialized(NeaiDPU_t *_this)
{
  assert_param(_this != NULL);
  
  return _this->stream_ready;
}

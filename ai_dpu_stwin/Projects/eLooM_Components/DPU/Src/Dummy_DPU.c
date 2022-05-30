/**
  ******************************************************************************
  * @file    Dummy_DPU.c
  * @author  SRA - MCD
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

#include "Dummy_DPU.h"
#include "Dummy_DPU_vtbl.h"
#include "services/sysdebug.h"




static const IDPU_vtbl DummyDPU_IDPU_vtbl = {
    Dummy_DPUInit,
    ADPU_AttachToSensor_vtbl,
    ADPU_DetachFromSensor_vtbl,
    ADPU_AttachInputADPU_vtbl,
    ADPU_DetachFromADPU_vtbl,
    ADPU_DispatchEvents_vtbl,
    ADPU_RegisterNotifyCallbacks_vtbl,
    Dummy_Process_vtbl
};



/**
 *  Dummy_DPU internal structure.
 */
struct _Dummy_DPU {
  /**
   * Base class object.
   */
  ADPU super;

  uint16_t size;

  uint8_t nAxis;

  int16_t *test;
};



// Private member function declaration
// ***********************************


// Inline function forward declaration
// ***********************************

#if defined (__GNUC__)
// Inline function defined inline in the header file Dummy_DPU.h must be declared here as extern function.
#endif

IDPU *Dummy_DPUAlloc()
{
  IDPU *pxObj = (IDPU*) SysAlloc(sizeof(Dummy_DPU));

  if (pxObj != NULL) {
    pxObj->vptr = &DummyDPU_IDPU_vtbl;
  }

  return pxObj;
}

sys_error_code_t Dummy_DPUInit(IDPU *_this) {
  assert_param(_this != NULL);
  sys_error_code_t xRes = SYS_NO_ERROR_CODE;
  ADPU * pObj = (ADPU*) _this;

  ADPU_Init_vtbl(_this);

  // take the ownership of the Sensor Event IF
  IEventListenerSetOwner((IEventListener *) ADPU_GetEventListenerIF(pObj), pObj);

  return xRes;

}

uint32_t Dummy_DPUSetInputParam(Dummy_DPU *_this, uint16_t size, uint8_t nAxis, uint8_t cb_items) {
  assert_param(_this != NULL);
  Dummy_DPU * pObj = (Dummy_DPU*) _this;


  pObj->size = size;
  pObj->nAxis = nAxis;

  pObj->super.dpuWorkingStream.mode = AI_SP_MODE_COLUMN;//AI_SP_MODE_LINE;
  pObj->super.dpuWorkingStream.packet.payload_fmt = AI_SP_FMT_INT16_RESET();


  if( pObj->nAxis == 1)
  {
    ai_logging_create_shape_1d(&pObj->super.dpuWorkingStream.packet.shape, size);
  }
  else if( pObj->nAxis > 1)
  {
    ai_logging_create_shape_2d(&pObj->super.dpuWorkingStream.packet.shape, size, nAxis);
  }
  else
  {
    return 0;
  }

  pObj->super.n_bytes_for_item =  pObj->super.dpuWorkingStream.packet.shape.shapes[AI_LOGGING_SHAPES_HEIGHT] *pObj->super.dpuWorkingStream.packet.shape.shapes[AI_LOGGING_SHAPES_WIDTH] *sizeof(int16_t);
  pObj->super.cb_items = cb_items;

  int numberOfRows =  pObj->super.dpuWorkingStream.packet.shape.shapes[AI_LOGGING_SHAPES_HEIGHT] ;
  int numberOfColumns = pObj->super.dpuWorkingStream.packet.shape.shapes[AI_LOGGING_SHAPES_WIDTH] ;

  /* define the stream for fft out */
  memcpy(&pObj->super.dpuOutStream, &pObj->super.dpuWorkingStream, sizeof(AI_SP_Stream_t));


  /* Allocate output buffer */
  pObj->test = (int16_t *) SysAlloc(sizeof (int16_t) * numberOfRows* numberOfColumns);
  pObj->super.dpuOutStream.packet.payload = (uint8_t *)pObj->test;


  return cb_items*pObj->super.n_bytes_for_item;
}


sys_error_code_t Dummy_Process_vtbl(IDPU *_this)
{
  assert_param(_this != NULL);
  sys_error_code_t xRes = SYS_NO_ERROR_CODE;
  ADPU * pObj = (ADPU*) _this;

  CBItem **pconsumer_buff = NULL;
  CircularBuffer *pcircular_buffer =NULL;

  //DPU has the priority
  if(!pObj->isADPUattached)
  {
    for(int i=0; i < ADPU_CFG_MAX_SENSOR; i++)
    {
      if(pObj->sensors[i].sensorIF != NULL)
      {
        uint32_t sensor_ready = CB_GetReadyItemFromTail(pObj->sensors[i].cb_handle.pCircularBuffer, &pObj->sensors[i].cb_handle.pConsumerDataBuff);
        if(sensor_ready == CB_NO_READY_ITEM_ERROR_CODE)
        {
          return SYS_NO_ERROR_CODE;
        }
        pconsumer_buff = &pObj->sensors[i].cb_handle.pConsumerDataBuff;
        pcircular_buffer = pObj->sensors[i].cb_handle.pCircularBuffer;
      }
    }
  }
  else
  {
    uint32_t process_ready = CB_GetReadyItemFromTail(pObj->AttachedAdpu.cb_handle.pCircularBuffer, &pObj->AttachedAdpu.cb_handle.pConsumerDataBuff);
    if(process_ready == CB_NO_READY_ITEM_ERROR_CODE)
    {
      return 0;
    }
    pconsumer_buff = &pObj->AttachedAdpu.cb_handle.pConsumerDataBuff;
    pcircular_buffer = pObj->AttachedAdpu.cb_handle.pCircularBuffer;
  }

  if ((*pconsumer_buff) != NULL)
  {
    int16_t *temp =  (int16_t *) CB_GetItemData((*pconsumer_buff));

    int16_t *dest = &((Dummy_DPU*)pObj)->test[0];
    for (int i = 0; i < pObj->dpuWorkingStream.packet.shape.shapes[AI_LOGGING_SHAPES_HEIGHT]; i++)
    {
      int16_t *raw = &temp[pObj->dpuWorkingStream.packet.shape.shapes[AI_LOGGING_SHAPES_WIDTH] *i];

      for (int j = 0; j < pObj->dpuWorkingStream.packet.shape.shapes[AI_LOGGING_SHAPES_WIDTH] ; j++)
      {
        (*dest++)  =  (*raw++);
      }
    }

    // release the buffer as soon as possible
    CB_ReleaseItem(pcircular_buffer, (*pconsumer_buff));
    (*pconsumer_buff) = NULL;

    ProcessEvent evt_acc;
    ProcessEventInit((IEvent*)&evt_acc, pObj->pProcessEventSrc, (ai_logging_packet_t*)&pObj->dpuOutStream, ADPU_GetTag(pObj));
    IDPU_DispatchEvents(_this, &evt_acc);
  }

  return xRes;
}



// Private function definition
// ***************************



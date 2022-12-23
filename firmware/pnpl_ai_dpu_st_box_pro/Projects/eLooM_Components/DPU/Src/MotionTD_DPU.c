/**
 ******************************************************************************
 * @file    MotionTD_DPU.c
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

#include "MotionTD_DPU.h"
#include "MotionTD_DPU_vtbl.h"
#include "services/sysdebug.h"


static const IDPU_vtbl MotionTD_DPU_IDPU_vtbl = {
    MotionTD_DPUInit,
    ADPU_AttachToSensor_vtbl,
    ADPU_DetachFromSensor_vtbl,
    ADPU_AttachInputADPU_vtbl,
    ADPU_DetachFromADPU_vtbl,
    ADPU_DispatchEvents_vtbl,
    ADPU_RegisterNotifyCallbacks_vtbl,
    MotionTD_Process_vtbl
};

/**
 *  MotionTD_DPU internal structure.
 */
struct _MotionTD_DPU_t {
  /**
   * Base class object.
   */
  ADPU super;

  uint8_t n_axis;

  float *MotionTD_out;

  xMotionTD_t MotionTD_instance;
};


// Private member function declaration
// ***********************************
/**
* Check if the DPU is initialized and ready to receive and process data.
*
* @param _this [IN] specifies a pointer to the object.
* @return TRUE if the object is initialized, FALSE otherwise.
*/
static inline boolean_t MotionTD_DPUIsFullInitialized(MotionTD_DPU_t *_this);

// Inline function forward declaration
// ***********************************

#if defined (__GNUC__)
// Inline function defined inline in the header file MotionTD_DPU.h must be declared here as extern function.
#endif

IDPU *MotionTD_DPUAlloc() {
  IDPU *pxObj = (IDPU*) pvPortMalloc(sizeof(MotionTD_DPU_t));

  if (pxObj != NULL) {
    pxObj->vptr = &MotionTD_DPU_IDPU_vtbl;
  }

  return pxObj;
}

sys_error_code_t MotionTD_DPUInit(IDPU *_this) {
  assert_param(_this != NULL);
  sys_error_code_t xRes = SYS_NO_ERROR_CODE;
  MotionTD_DPU_t * pObj = (MotionTD_DPU_t*) _this;

  ADPU_Init_vtbl(_this);

  /* take the ownership of the Sensor Event IF */
  IEventListenerSetOwner((IEventListener *) ADPU_GetEventListenerIF((ADPU*)pObj), (ADPU*)pObj);

  return xRes;
}

uint32_t MotionTD_DPUSetInputParam(MotionTD_DPU_t *_this, float odr, float sensitivity, MotionTD_AnalysisType_t analysis_type, uint16_t window_length, uint8_t n_axis, uint8_t cb_items) {
  assert_param(_this != NULL);
  MotionTD_DPU_t * pObj = (MotionTD_DPU_t *) _this;
  
  if(MotionTD_DPUIsFullInitialized(_this))
  {
    return cb_items*pObj->super.n_bytes_for_item;
  }

  pObj->n_axis = n_axis;

  /* init library */
  pObj->MotionTD_instance.Param.AnalysisType = analysis_type;
  pObj->MotionTD_instance.Param.ODR = odr;
  pObj->MotionTD_instance.Param.sensitivity = sensitivity;
  pObj->MotionTD_instance.Param.SamplesNumber = window_length;
  MotionTD_Init(&pObj->MotionTD_instance);

  pObj->super.dpuWorkingStream.mode = AI_SP_MODE_LINE;
  pObj->super.dpuWorkingStream.packet.payload_fmt = AI_SP_FMT_INT16_RESET();

  if( pObj->n_axis == 1)
   {
     ai_logging_create_shape_1d(&pObj->super.dpuWorkingStream.packet.shape, window_length);
   }
   else if( pObj->n_axis > 1)
   {
     ai_logging_create_shape_2d(&pObj->super.dpuWorkingStream.packet.shape, window_length, n_axis);
   }
   else
   {
     return 0;
   }

  pObj->super.n_bytes_for_item =  pObj->super.dpuWorkingStream.packet.shape.shapes[AI_LOGGING_SHAPES_HEIGHT] *pObj->super.dpuWorkingStream.packet.shape.shapes[AI_LOGGING_SHAPES_WIDTH] *sizeof(int16_t);

  /* define the stream for output data */
  memcpy(&pObj->super.dpuOutStream, &pObj->super.dpuWorkingStream, sizeof(AI_SP_Stream_t));
  pObj->super.dpuOutStream.packet.shape.shapes[AI_LOGGING_SHAPES_WIDTH] = 0;

  if(pObj->MotionTD_instance.Param.AnalysisType & MOTIONTD_PK_ACCEL_MASK)
  {
    pObj->super.dpuOutStream.packet.shape.shapes[AI_LOGGING_SHAPES_WIDTH]++;
  }
  if(pObj->MotionTD_instance.Param.AnalysisType & MOTIONTD_RMS_ACCEL_MASK)
  {
    pObj->super.dpuOutStream.packet.shape.shapes[AI_LOGGING_SHAPES_WIDTH]++;
  }
  if(pObj->MotionTD_instance.Param.AnalysisType & MOTIONTD_RMS_SPD_MASK)
  {
    pObj->super.dpuOutStream.packet.shape.shapes[AI_LOGGING_SHAPES_WIDTH]++;
  }

  /* Allocate output buffer */
  pObj->MotionTD_out = (float *) pvPortMalloc(sizeof (float) * pObj->super.dpuOutStream.packet.shape.shapes[AI_LOGGING_SHAPES_WIDTH]);

  pObj->super.dpuOutStream.packet.payload = (uint8_t *)pObj->MotionTD_out;
  pObj->super.cb_items = cb_items;

  return cb_items*pObj->super.n_bytes_for_item;
}


sys_error_code_t MotionTD_Process_vtbl(IDPU *_this)
{
  assert_param(_this != NULL);
  sys_error_code_t xRes = SYS_NO_ERROR_CODE;
  ADPU * pObj = (ADPU*) _this;

  CBItem *pconsumer_buff;
  int i=0;

  /* DPU has the priority */
  for( i=0; i < ADPU_CFG_MAX_SENSOR; i++)
    {
      if(pObj->sensors[i].sensorIF != NULL)
    {
      uint32_t sensor_ready = CB_GetReadyItemFromTail(pObj->sensors[i].cb_handle.pCircularBuffer, &pconsumer_buff);
        if(sensor_ready == CB_NO_READY_ITEM_ERROR_CODE)
        {
          return SYS_NO_ERROR_CODE;
        }
      break;
    }
  }

  MotionTD_DPU_t * DPUObj = (MotionTD_DPU_t*) _this;

  if (pconsumer_buff != NULL)
  {
    int16_t *temp =  (int16_t *) CB_GetItemData(pconsumer_buff);

    for (int i = 0; i < pObj->dpuWorkingStream.packet.shape.shapes[AI_LOGGING_SHAPES_HEIGHT]; i++)
    {
      int16_t *raw = &temp[pObj->dpuWorkingStream.packet.shape.shapes[AI_LOGGING_SHAPES_WIDTH] * i];
      float *out = &DPUObj->MotionTD_out[pObj->dpuOutStream.packet.shape.shapes[AI_LOGGING_SHAPES_WIDTH] * i ];

      MotionTD_Process(&DPUObj->MotionTD_instance, raw, out);
    }

    /* release the buffer as soon as possible */
    CB_ReleaseItem(pObj->sensors[i].cb_handle.pCircularBuffer, pconsumer_buff);

    ProcessEvent evt_acc;
    ProcessEventInit((IEvent*)&evt_acc, pObj->pProcessEventSrc, (ai_logging_packet_t*)&pObj->dpuOutStream, ADPU_GetTag(pObj));
    IDPU_DispatchEvents(_this, &evt_acc);
  }

  return xRes;
}



// Private function definition
// ***************************
static inline boolean_t MotionTD_DPUIsFullInitialized(MotionTD_DPU_t *_this)
{
  assert_param(_this != NULL);
  boolean_t res = FALSE;

  if ((_this->MotionTD_instance.Param.AnalysisType != 0) && (_this->MotionTD_instance.Param.SamplesNumber != 0) && (_this->MotionTD_out != NULL))
  {
    res = TRUE;
  }

  return res;
}


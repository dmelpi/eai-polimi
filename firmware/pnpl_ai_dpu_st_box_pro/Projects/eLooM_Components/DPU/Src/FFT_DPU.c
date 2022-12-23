/**
  ******************************************************************************
  * @file    FFT_DPU.c
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

#include "FFT_DPU.h"
#include "FFT_DPU_vtbl.h"
#include "services/sysdebug.h"
#include "services/sysmem.h"
#include "fft.h"


static const IDPU_vtbl FFT_DPU_IDPU_vtbl = {
  FFT_DPUInit,
  ADPU_AttachToSensor_vtbl,
  ADPU_DetachFromSensor_vtbl,
  ADPU_AttachInputADPU_vtbl,
  ADPU_DetachFromADPU_vtbl,
  ADPU_DispatchEvents_vtbl,
  ADPU_RegisterNotifyCallbacks_vtbl,
  FFT_DPU_Process_vtbl
};

/**
*  FFT_DPU internal structure.
*/
struct _FFT_DPU_t {
  /**
  * Base class object.
  */
  ADPU super;
  
  FFT_instance_t fft_instance;
  
  uint8_t n_average;
  
  uint8_t *avg_counter;
  
  float *fft_avg;
  
  float *fft_in_copy;
  
  float *fft_out;
};


/* Private member function declaration */
/***************************************/

/**
* Check if the DPU is initialized and ready to receive and process data.
*
* @param _this [IN] specifies a pointer to the object.
* @return TRUE if the object is initialized, FALSE otherwise.
*/
static inline boolean_t FFT_DPUIsFullInitialized(FFT_DPU_t *_this);

/*  Public member function declaration */
// ***********************************

IDPU *FFT_DPUAlloc() {
  IDPU *pxObj = (IDPU*) SysAlloc(sizeof(FFT_DPU_t));
  
  if (pxObj != NULL) {
    pxObj->vptr = &FFT_DPU_IDPU_vtbl;
  }
  
  return pxObj;
}


/* IDPU virtual functions definition */
/*************************************/
sys_error_code_t FFT_DPUInit(IDPU *_this) {
  assert_param(_this != NULL);
  sys_error_code_t xRes = SYS_NO_ERROR_CODE;
  FFT_DPU_t * pObj = (FFT_DPU_t*) _this;
  
  ADPU_Init_vtbl(_this);
  
  /* take the ownership of the Sensor Event IF */
  IEventListenerSetOwner((IEventListener *) ADPU_GetEventListenerIF((ADPU*)pObj), (ADPU*)pObj);
  
  FFT_set_allocation_functions(SysAlloc, SysFree);
  pObj->fft_out = NULL;
  return xRes;
}

sys_error_code_t FFT_DPU_Process_vtbl(IDPU *_this)
{
  assert_param(_this != NULL);
  sys_error_code_t xRes = SYS_NO_ERROR_CODE;
  ADPU * pObj = (ADPU*) _this;
  
  CBItem *pconsumer_buff;	
  int i=0;
  
  /* DPU has the priority */
  for(i=0; i < ADPU_CFG_MAX_SENSOR; i++)
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
  
  FFT_DPU_t * FFTObj = (FFT_DPU_t*) _this;
  uint16_t numberOfAxis =  pObj->dpuWorkingStream.packet.shape.shapes[AI_LOGGING_SHAPES_HEIGHT];
  uint16_t FFTlength = pObj->dpuOutStream.packet.shape.shapes[AI_LOGGING_SHAPES_WIDTH];
  uint16_t inlength = pObj->dpuWorkingStream.packet.shape.shapes[AI_LOGGING_SHAPES_WIDTH];
  
  if (pconsumer_buff!= NULL)
  {
    int16_t *temp =  (int16_t *) CB_GetItemData(pconsumer_buff);
    
    for (int i = 0; i < numberOfAxis; i++)
    {
      if (FFTObj->n_average == 0)
      {
        int16_t *raw = &temp[inlength * i];
        float *out = &FFTObj->fft_out[FFTlength * i];
        
        FFT_Direct_Process(&FFTObj->fft_instance, raw, out);
      }
      else
      {
        int16_t *raw = &temp[inlength * i];
        float *avg_in = &FFTObj->fft_in_copy[FFTlength * i ];
        float *avg_sum = &FFTObj->fft_avg[FFTlength * i ];
        
        FFT_Direct_Process(&FFTObj->fft_instance, raw, avg_in);
        
        arm_add_f32(avg_in, avg_sum, avg_sum, FFTlength);
        FFTObj->avg_counter[i] = FFTObj->avg_counter[i] + 1;
      }
    }
    
    /* release the buffer as soon as possible */
    CB_ReleaseItem(pObj->sensors[i].cb_handle.pCircularBuffer, pconsumer_buff);
    pconsumer_buff = NULL;
    
    if (FFTObj->n_average == 0)
    {
      ProcessEvent evt_acc;
      ProcessEventInit((IEvent*)&evt_acc, pObj->pProcessEventSrc, (ai_logging_packet_t*)&pObj->dpuOutStream, ADPU_GetTag(pObj));
      IDPU_DispatchEvents(_this, &evt_acc);
    }
    else
    {
      if (FFTObj->avg_counter[numberOfAxis-1] == FFTObj->n_average)
      {
        arm_scale_f32(FFTObj->fft_avg, 2.0f/(float)((float)FFTObj->avg_counter[numberOfAxis-1] * FFTlength), FFTObj->fft_out, numberOfAxis * FFTlength);
        
        ProcessEvent evt_acc;
        ProcessEventInit((IEvent*)&evt_acc, pObj->pProcessEventSrc, (ai_logging_packet_t*)&pObj->dpuOutStream, ADPU_GetTag(pObj));
        IDPU_DispatchEvents(_this, &evt_acc);
        
        memset(FFTObj->avg_counter, 0, sizeof(uint8_t) * numberOfAxis);
        memset(FFTObj->fft_avg, 0, sizeof(float) * numberOfAxis * FFTlength);
        memset(FFTObj->fft_in_copy, 0, sizeof(float)* numberOfAxis * FFTlength);
      }
    }
    
  }
  else /** no circular buffer **/
  {
    xRes = ADPU_NOT_IMPLEMENTED;
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(xRes);
  }
  
  return xRes;
}


uint32_t FFT_DPUSetInputParam(FFT_DPU_t *_this, uint16_t fft_size, uint8_t nAxis, uint8_t nAverage, uint8_t cb_items) {
  assert_param(_this != NULL);
  FFT_DPU_t * pObj = (FFT_DPU_t*) _this;
  
  if(FFT_DPUIsFullInitialized(_this))
  {
    return cb_items*pObj->super.n_bytes_for_item;
  }
  
  /* Perform fft line by line*/
  pObj->super.dpuWorkingStream.mode = AI_SP_MODE_LINE;
  pObj->super.dpuWorkingStream.packet.payload_fmt = AI_SP_FMT_INT16_RESET();
  
  if( nAxis == 1)
  {
    ai_logging_create_shape_1d(&pObj->super.dpuWorkingStream.packet.shape, fft_size);
  }
  else if( nAxis > 1)
  {
    ai_logging_create_shape_2d(&pObj->super.dpuWorkingStream.packet.shape, fft_size, nAxis);
  }
  else
  {
    return 0;
  }
  
  uint16_t numberOfRows =  pObj->super.dpuWorkingStream.packet.shape.shapes[AI_LOGGING_SHAPES_HEIGHT] ;
  uint16_t numberOfColumns = pObj->super.dpuWorkingStream.packet.shape.shapes[AI_LOGGING_SHAPES_WIDTH] ; 
  
  /* byte for cb item*/
  pObj->super.n_bytes_for_item =  numberOfRows * numberOfColumns *sizeof(int16_t);
  /* number of cb item*/
  pObj->super.cb_items = cb_items;
  
  /* define the stream for fft out */
  memcpy(&pObj->super.dpuOutStream, &pObj->super.dpuWorkingStream, sizeof(AI_SP_Stream_t));
  pObj->super.dpuOutStream.packet.payload_fmt = AI_SP_FMT_FLOAT32_RESET();
  pObj->super.dpuOutStream.packet.shape.shapes[AI_LOGGING_SHAPES_WIDTH] = fft_size/2;
  
  /* Allocate intermediate buffer (for averaging output result if nAverage > 0)*/
  if (nAverage == 0)
  {
    pObj->n_average = 0;
    pObj->avg_counter = 0;
    pObj->fft_avg = 0;
    pObj->fft_in_copy = 0;
  }
  else
  {
    pObj->n_average = nAverage;
    pObj->avg_counter = (uint8_t *) SysAlloc(sizeof (uint8_t) * nAxis);
    pObj->fft_avg = (float *) SysAlloc(sizeof (float) * numberOfRows * (numberOfColumns/2)); /* the output of fft is fft_size/2 */
    pObj->fft_in_copy = (float *) SysAlloc(sizeof (float) * numberOfRows * (numberOfColumns/2)); /* the output of fft is fft_size/2 */
  }
  /* Allocate output and buffer */
  pObj->fft_out = (float *) SysAlloc(sizeof (float) * numberOfRows * (numberOfColumns/2)); /* the output of fft is fft_size/2 */
  pObj->super.dpuOutStream.packet.payload = (uint8_t *)pObj->fft_out;  
  
  /* init fft library */
  pObj->fft_instance.init_params.FFT_len = numberOfColumns;
  pObj->fft_instance.init_params.overlap = 0.5f;
  pObj->fft_instance.init_params.win_type = FFT_HANNING_WIN;
  pObj->fft_instance.init_params.output_type = MAGNITUDE;
  pObj->fft_instance.init_params.data_type = INT16;
  pObj->fft_instance.init_params.use_direct_process = DIRECT_PROCESS_ENABLED;
  FFT_Init(&pObj->fft_instance);
  
  /* return the byte for cb */
  return cb_items*pObj->super.n_bytes_for_item;
}



/* Private member function definition */
/**************************************/

static inline boolean_t FFT_DPUIsFullInitialized(FFT_DPU_t *_this)
{
  assert_param(_this != NULL);
  boolean_t res = FALSE;
  
  if ((_this->fft_instance.init_params.FFT_len != 0) && (_this->fft_out != NULL))
  {
    res = TRUE;
  }
  
  return res;
}



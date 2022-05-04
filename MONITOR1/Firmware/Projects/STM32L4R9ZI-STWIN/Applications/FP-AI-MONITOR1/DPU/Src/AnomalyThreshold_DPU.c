/**
 ******************************************************************************
 * @file    AnomalyThreshold_DPU.c
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

#include "AnomalyThreshold_DPU.h"
#include "AnomalyThreshold_DPU_vtbl.h"
#include "services/sysdebug.h"
#include <stdio.h>

#define IS_IN_RANGE(low,high,x) (((x-high)*(x-low) <= 0)? 1:0)   //(x >= a && x <= b) ? 1:0
#define PRECISION2(n) floorf(100*n)/100
#define PRECISION4(n) floorf(10000*n)/10000
#define MAX_LEN_MSG 256
#define MAX_LEN_MEA 20
#define OFFSET 2
#define MAX_SOURCES 3


typedef enum
{
  MONITORING = 0,
  FAULTED,
  NO_FAULTED
} Status_evt_t;

typedef struct
{
  Status_evt_t status;
  Status_evt_t status_old;
} Status_Detect_t;

typedef struct
{
  ThValue_t th;
  int32_t old_bin;
} Th_t;

typedef struct
{
  Th_t axis_th[4];
  Status_Detect_t status;
  uint32_t tag;
} Th_axis_t;


static const IDPU_vtbl AnomalyThreshold_IDPU_vtbl = {
    AnomalyThreshold_DPUInit,
    ADPU_AttachToSensor_vtbl,
    ADPU_DetachFromSensor_vtbl,
    ADPU_AttachInputADPU_vtbl,
    ADPU_DetachFromADPU_vtbl,
    ADPU_DispatchEvents_vtbl,
    ADPU_RegisterNotifyCallbacks_vtbl,
    AnomalyThreshold_DPU_Process_vtbl
};

/**
 *  AnomalyThreshold_DPU internal structure.
 */
struct _AnomalyThreshold_DPU_t {
  /**
   * Base class object.
   */
  ADPU super;

  uint16_t size;

  uint8_t nAxis;

  uint16_t index_axis;

  Th_axis_t axis[MAX_SOURCES];

  float bfs;

  uint8_t is_lower; /* is_lower = 1 -> set an alarm if data is lower than threshold */

};


// Private member function declaration
// ***********************************
static int Anomaly_detection(AnomalyThreshold_DPU_t *_this, float *data, Th_t *th_w, Th_t *th_a, Th_t *th_c, char axis, Status_Detect_t *status, uint32_t tag, float bin_index);
static void send_ine_event(AnomalyThreshold_DPU_t *_this, char *msg_, Threshold_type_t severity, char *measure_, float freq,float mag, uint32_t tag);
static float *looking_for_element(uint16_t low_f, uint16_t high_f, float min, float * vet, uint8_t is_lower);

// Inline function forward declaration
// ***********************************

#if defined (__GNUC__)
// Inline function defined inline in the header file AnomalyThreshold_DPU.h must be declared here as extern function.
#endif

IDPU *AnomalyThreshold_DPUAlloc() {
  IDPU *pxObj = (IDPU*) pvPortMalloc(sizeof(AnomalyThreshold_DPU_t));

  if (pxObj != NULL) {
    pxObj->vptr = &AnomalyThreshold_IDPU_vtbl;
  }
  
  return pxObj;
}

sys_error_code_t AnomalyThreshold_DPUInit(IDPU *_this) {
  assert_param(_this != NULL);
  sys_error_code_t xRes = SYS_NO_ERROR_CODE;
  ADPU * pObj = (ADPU*) _this;

  ADPU_Init_vtbl(_this);

  // take the ownership of the Sensor Event IF
  IEventListenerSetOwner((IEventListener *) ADPU_GetEventListenerIF(pObj), pObj);

  return xRes;
}

sys_error_code_t AnomalyThreshold_DPUSetInputThreshold(AnomalyThreshold_DPU_t *_this, ThValue_t warning, ThValue_t alert, ThValue_t critical, uint32_t tag) {
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  for(int i = 0; i < _this->index_axis; i++ )
  {
    if(_this->axis[i].tag == tag)
    {
      _this->axis[i].axis_th[WARNING].old_bin = -1;
      _this->axis[i].axis_th[WARNING].th = warning;
      _this->axis[i].axis_th[ALERT].old_bin = -1;
      _this->axis[i].axis_th[ALERT].th = alert;
      _this->axis[i].axis_th[CRITICAL].old_bin = -1;
      _this->axis[i].axis_th[CRITICAL].th = critical;
      return res;
    }
  }

  if(_this->index_axis >= MAX_SOURCES)
  {
    res = ADPU_BASE_ERROR_CODE;
  }
  else
  {
    _this->axis[_this->index_axis].tag = tag;
    _this->axis[_this->index_axis].axis_th[WARNING].old_bin = -1;
    _this->axis[_this->index_axis].axis_th[WARNING].th = warning;
    _this->axis[_this->index_axis].axis_th[ALERT].old_bin = -1;
    _this->axis[_this->index_axis].axis_th[ALERT].th = alert;
    _this->axis[_this->index_axis].axis_th[CRITICAL].old_bin = -1;
    _this->axis[_this->index_axis].axis_th[CRITICAL].th = critical;

    _this->index_axis++;
  }

  return res;
}

sys_error_code_t AnomalyThreshold_DPU_UpdateThreshold(AnomalyThreshold_DPU_t *_this, ThValue_t warning, ThValue_t alert, ThValue_t critical, uint32_t tag) {
  assert_param(_this != NULL);
  sys_error_code_t xRes = SYS_NO_ERROR_CODE;

  for(int i=0 ; i < _this->index_axis; i++)
  {

  }
  
  return xRes;
}

uint32_t AnomalyThreshold_DPUSetInputParam(AnomalyThreshold_DPU_t *_this, uint16_t size, uint8_t nAxis, uint8_t cb_items, float bfs, uint8_t is_lower) {
  assert_param(_this != NULL);
  AnomalyThreshold_DPU_t * pObj = (AnomalyThreshold_DPU_t*) _this;

  pObj->size = size;
  pObj->nAxis = nAxis;
  pObj->bfs = bfs;
  pObj->is_lower = is_lower;

  pObj->super.dpuWorkingStream.mode = AI_SP_MODE_LINE;
  pObj->super.dpuWorkingStream.packet.payload_fmt = AI_SP_FMT_FLOAT32_RESET();

  pObj->index_axis = 0;

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

  pObj->super.n_bytes_for_item =  pObj->super.dpuWorkingStream.packet.shape.shapes[AI_LOGGING_SHAPES_HEIGHT] *pObj->super.dpuWorkingStream.packet.shape.shapes[AI_LOGGING_SHAPES_WIDTH] *sizeof(float);
  pObj->super.cb_items = cb_items;

  return cb_items*pObj->super.n_bytes_for_item;
}


sys_error_code_t AnomalyThreshold_DPU_Process_vtbl(IDPU *_this)
{
  assert_param(_this != NULL);
  sys_error_code_t xRes = SYS_NO_ERROR_CODE;
  ADPU * pObj = (ADPU*) _this;

  CBItem **pconsumer_buff = NULL;
  CircularBuffer *pcircular_buffer =NULL;
  uint8_t cb_available = 0;

  /* DPU has the priority */
  if(!pObj->isADPUattached)
  {
    for(int i=0; i < ADPU_CFG_MAX_SENSOR; i++)
    {
      if(pObj->sensors[i].sensorIF != NULL)
      {
        if (pObj->sensors[i].cb_handle.pCircularBuffer != NULL)
        {
          cb_available = 1;
          uint32_t sensor_ready = CB_GetReadyItemFromTail(pObj->sensors[i].cb_handle.pCircularBuffer, &pObj->sensors[i].cb_handle.pConsumerDataBuff);
          if(sensor_ready == CB_NO_READY_ITEM_ERROR_CODE)
          {
            return SYS_NO_ERROR_CODE;
          }
          pconsumer_buff = &pObj->sensors[i].cb_handle.pConsumerDataBuff;
          pcircular_buffer = pObj->sensors[i].cb_handle.pCircularBuffer;
        }
      }
      else
      {
        cb_available = 0;
      }
    }
  }
  else
  {
    if (pObj->AttachedAdpu.cb_handle.pCircularBuffer != NULL)
    {
      cb_available = 1;
      uint32_t process_ready = CB_GetReadyItemFromTail(pObj->AttachedAdpu.cb_handle.pCircularBuffer, &pObj->AttachedAdpu.cb_handle.pConsumerDataBuff);
      if(process_ready == CB_NO_READY_ITEM_ERROR_CODE)
      {
        return 0;
      }
      pconsumer_buff = &pObj->AttachedAdpu.cb_handle.pConsumerDataBuff;
      pcircular_buffer = pObj->AttachedAdpu.cb_handle.pCircularBuffer;
    }
    else
    {
      cb_available = 0;
    }
  }


  AnomalyThreshold_DPU_t * AT_DPUObj = (AnomalyThreshold_DPU_t *)_this;

  if (cb_available == 1)
  {
    float *temp =  (float *) CB_GetItemData((*pconsumer_buff));

    for (int i = 0; i < pObj->dpuWorkingStream.packet.shape.shapes[AI_LOGGING_SHAPES_HEIGHT]; i++)
    {
      float *raw = &temp[pObj->dpuWorkingStream.packet.shape.shapes[AI_LOGGING_SHAPES_WIDTH] *i];
      Anomaly_detection(AT_DPUObj, raw, &AT_DPUObj->axis[i].axis_th[1], &AT_DPUObj->axis[i].axis_th[2], &AT_DPUObj->axis[i].axis_th[3], 'x', &AT_DPUObj->axis[i].status, AT_DPUObj->axis[i].tag, AT_DPUObj->bfs);
    }
    /* release the buffer as soon as possible */
    CB_ReleaseItem(pcircular_buffer, (*pconsumer_buff));
    (*pconsumer_buff) = NULL;
  }
  else
  {
    float *temp = (float *) AT_DPUObj->super.sourceStream.packet.payload;

    for (int i = 0; i < pObj->dpuWorkingStream.packet.shape.shapes[AI_LOGGING_SHAPES_HEIGHT]; i++)
    {
      float *raw = &temp[pObj->dpuWorkingStream.packet.shape.shapes[AI_LOGGING_SHAPES_WIDTH] *i];
      Anomaly_detection(AT_DPUObj, raw, &AT_DPUObj->axis[i].axis_th[1], &AT_DPUObj->axis[i].axis_th[2], &AT_DPUObj->axis[i].axis_th[3], 'x', &AT_DPUObj->axis[i].status, AT_DPUObj->axis[i].tag, AT_DPUObj->bfs);
    }
  }
  return xRes;
}



// Private function definition
// ***************************
static float *looking_for_element(uint16_t low_f, uint16_t high_f, float min, float * vet, uint8_t is_lower)
{
  float *start = &vet[low_f];
  float *end = &vet[high_f];

  if (is_lower == 1) /* is_lower = 1 -> set an alarm if data is lower than threshold */
  {
    while(start <= end)
    {
      if(*start++ < min)
      {
        start--;
        return start;
      }
    }
  }
  else  /* set an alarm if data is higher than threshold */
  {
    while(start <= end)
    {
      if(*start++ > min)
      {
        start--;
        return start;
      }
    }
  }
  return 0;
}

static void send_ine_event(AnomalyThreshold_DPU_t *_this, char *msg_, Threshold_type_t severity, char *measure_, float freq, float mag, uint32_t tag)
{
  assert_param(_this != NULL);
  AnomalyEvent_t ine_event;

  ine_event.evt.freq = freq;
  ine_event.evt.mag = mag;
  ine_event.evt.severity = severity;
  ine_event.packet.payload =  (uint8_t *) &ine_event;
  ai_logging_create_shape_0d(&ine_event.packet.shape);

  ProcessEvent evt_anomaly;
  ProcessEventInit((IEvent*)&evt_anomaly, _this->super.pProcessEventSrc, (ai_logging_packet_t*)&ine_event, tag);
  IDPU_DispatchEvents((IDPU*)_this, &evt_anomaly);

}

static int Anomaly_detection(AnomalyThreshold_DPU_t *_this, float *data, Th_t *th_w, Th_t *th_a, Th_t *th_c, char axis, Status_Detect_t *status, uint32_t tag, float bin_index)
{
  assert_param(_this != NULL);
  uint16_t low_f = 0;
  uint16_t high_f = 0;
  float *result[4]={0,0,0,0};
  uint32_t idx = 0;
  float mag = 0.0f;
  char temp[50];

  if(status->status == MONITORING)
  {
      /**warning**/
      if (bin_index != 0)
      {
        if(th_w->th.frequency_range.Min > OFFSET*bin_index)
        {
          low_f= (uint16_t)  floor(th_w->th.frequency_range.Min/bin_index) -OFFSET ;
        }
        else
        {
          low_f=0;
        }
        high_f= (uint16_t) floor(th_w->th.frequency_range.Max/bin_index) +OFFSET ;
      }
      result[1]=looking_for_element( low_f,  high_f, th_w->th.Value, data, _this->is_lower);

      /**alert**/
      if (bin_index != 0)
      {
        if(th_w->th.frequency_range.Min > OFFSET*bin_index)
        {
          low_f= (uint16_t) floor(th_a->th.frequency_range.Min/bin_index) -OFFSET ;
        }
        else
        {
          low_f=0;
        }
        high_f= (uint16_t) floor(th_a->th.frequency_range.Max/bin_index) +OFFSET ;
      }
      result[2]=looking_for_element( low_f,  high_f,  th_a->th.Value, data, _this->is_lower);

      /**critical**/
      if (bin_index != 0)
      {
        if(th_w->th.frequency_range.Min > OFFSET*bin_index)
        {
          low_f= (uint16_t) floor(th_c->th.frequency_range.Min/bin_index) -OFFSET ;//offset of 5
        }
        else
        {
          low_f=0;
        }
        high_f= (uint16_t) floor(th_c->th.frequency_range.Max/bin_index) +OFFSET ;
      }
      result[3]=looking_for_element( low_f,  high_f, th_c->th.Value, data, _this->is_lower);

    if(!(result[1] || result[2] || result[3]))
    {
      status->status = NO_FAULTED;
    }
    else
    {
      status->status = FAULTED;
      status->status_old = FAULTED;
    }
  }

  if(status->status == FAULTED)
  {
    if(result[3] != 0)
    {
      idx= ( (int32_t) result[3] - (int32_t) data)/sizeof(float);
      mag=* (float *)result[3];
      if(th_c->old_bin != idx)
      {
        send_ine_event(_this, temp,CRITICAL, NULL, bin_index*idx,mag, tag);
        th_c->old_bin=idx;

        th_w->old_bin=-1; /* reset older alarms */
        th_a->old_bin=-1;
      }
    }
    else if(result[2] != 0)
    {
      idx= ( (int32_t) result[2] - (int32_t) data)/sizeof(float);
      mag=* (float *)result[2];
      if(th_a->old_bin != idx)
      {
        send_ine_event(_this, temp,ALERT, NULL, bin_index*idx,mag, tag);
        th_a->old_bin=idx;

        th_w->old_bin=-1; /* reset older alarms */
        th_c->old_bin=-1;
      }
    }
    else if(result[1] != 0)
    {
      idx= ( (int32_t) result[1] - (int32_t) data)/sizeof(float);
      mag=* (float *)result[1];
      if(th_w->old_bin != idx)
      {
        send_ine_event(_this, temp , WARNING, NULL, bin_index*idx,mag, tag);
        th_w->old_bin=idx;

        th_a->old_bin=-1; /* reset older alarms */
        th_c->old_bin=-1;
      }
    }

    status->status= MONITORING;
  }

  if(status->status == NO_FAULTED)
  {
    if (status->status_old == FAULTED)
    {
      status->status_old=NO_FAULTED;
      send_ine_event(_this, NULL, TH_OK, NULL, bin_index*idx, mag, tag);
      th_w->old_bin=-1;
      th_a->old_bin=-1;
      th_c->old_bin=-1;
    }
    status->status= MONITORING;
  }

  return 1;
}


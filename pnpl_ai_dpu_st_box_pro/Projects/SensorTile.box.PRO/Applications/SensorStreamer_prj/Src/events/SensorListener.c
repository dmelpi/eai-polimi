/**
 ******************************************************************************
 * @file    SensorListener.h
 * @author  STMicroelectronics - AIS - MCD Team
 * @version v1.0.0
 * @date    Jan 31, 2022
 *
 * @brief   Simple object that listen the ::SensorEvent_t and print the value
 *          in the debug log.
 *
 *
 *********************************************************************************
 * @attention
 *
 * Copyright (c) 2021 STMicroelectronics
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file in
 * the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *********************************************************************************
 */


#include "events/SensorListener.h"
#include "events/SensorListener_vtbl.h"
#include "events/IEventSrc.h"
#include "events/IEventSrc_vtbl.h"
#include "services/sysdebug.h"

#define SL_NULL_SENSOR_ID                              0xFFFF

#define SYS_DEBUGF(level, message)                     SYS_DEBUGF3(SYS_DBG_SL, level, message)

/**
 * Class object declaration. The class object encapsulates members that are shared between
 * all instance of the class.
 */
typedef struct _SensorListenerClass_t {
  /**
   * SensorEventListener interface virtual table.
   */
  IDataEventListener_vtbl vtbl;

} SensorListenerClass_t;

/* GCC requires one function forward declaration in only one .c source
 * in order to manage the inline.
 * See also http://stackoverflow.com/questions/26503235/c-inline-function-and-gcc
 */
#if defined (__GNUC__) || defined (__ICCARM__)
#endif


/**
 * The class object.
 */
static const SensorListenerClass_t sTheClass = {
  /* ISensorEventLestener virtual table */
  {
    SensorListener_vtblOnStatusChange,
    SensorListener_vtblSetOwner,
    SensorListener_vtblGetOwner,
    SensorListener_vtblOnNewDataReady
  }
};


/* Private member function declaration */
/***************************************/

/**
 * Default sensors configuration function. It does nothing.
 * @sa ::SLSensorsConfigF
 */
static sys_error_code_t SensorListenerDefSensorsConfig(SensorManager_t *p_sm, uint16_t *p_active_sensor_id);

static sys_error_code_t SensorListenerStreamFloatData(SensorListener_t *_this,  EMData_t *p_data);

static sys_error_code_t SensorListenerStreamIntData(SensorListener_t *_this,  EMData_t *p_data);

static sys_error_code_t SensorListenerStreamData(SensorListener_t *_this,  EMData_t *p_data);


/* Public API definition */
/*************************/

SensorListener_t *SensorListenerStaticAlloc(SensorListener_t *p_instance)
{
  assert_param(p_instance != NULL);

  p_instance->super.vptr = &sTheClass.vtbl;

  return p_instance;
}

sys_error_code_t SensorListenerInit(SensorListener_t *_this)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  _this->p_owner = NULL;
  _this->active_sensor = SL_NULL_SENSOR_ID;
  _this->p_default_snsors_config = SensorListenerDefSensorsConfig;

  return res;
}

sys_error_code_t SensorListenerSetDefSensorConfifF(SensorListener_t *_this, SLSensorsConfigF p_sensor_config_f)
{
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  _this->p_default_snsors_config = p_sensor_config_f == NULL ? SensorListenerDefSensorsConfig : p_sensor_config_f;

  return res;
}

sys_error_code_t SensorListenerUpdateSnsorConfiguration(SensorListener_t *_this)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  if (_this->active_sensor != SL_NULL_SENSOR_ID)
  {
    /* remove the listener from the previous active sensor*/
    ISourceObservable *p_sensor = SMGetSensorObserver(_this->active_sensor);
    if (p_sensor != NULL)
    {
      IEventSrcRemoveEventListener(ISourceGetEventSrcIF(p_sensor), (IEventListener*)_this);
    }
  }

  if (_this->p_default_snsors_config != NULL)
  {
    res = _this->p_default_snsors_config(SMGetSensorManager(), &_this->active_sensor);
  }

  if (_this->active_sensor != SL_NULL_SENSOR_ID)
  {
    /* add the listener from the previous active sensor*/
    ISourceObservable *p_sensor = SMGetSensorObserver(_this->active_sensor);
    if (p_sensor != NULL)
    {
      IEventSrcAddEventListener(ISourceGetEventSrcIF(p_sensor), (IEventListener*)_this);
    }
  }

  return res;
}

/* IListener virtual functions definition */
/******************************************/

sys_error_code_t SensorListener_vtblOnStatusChange(IListener *this)
{
  /* not implemented */

  SYS_DEBUGF(SYS_DBG_LEVEL_WARNING, ("CTRL: IListener::OnStatusChange() not implemented.\r\n"));

  return SYS_NO_ERROR_CODE;
}

/* IEventListener virtual functions definition */
/***********************************************/

void *SensorListener_vtblGetOwner(IEventListener *_this)
{
  assert_param(_this != NULL);
  SensorListener_t *p_obj = (SensorListener_t *)_this;

  return p_obj->p_owner;
}

void SensorListener_vtblSetOwner(IEventListener *_this, void *p_owner)
{
  assert_param(_this != NULL);
  SensorListener_t *p_obj = (SensorListener_t *)_this;

  p_obj->p_owner = p_owner;
}


/* IEventListener virtual functions definition */
/***********************************************/
sys_error_code_t SensorListener_vtblOnNewDataReady(IEventListener *_this, const DataEvent_t *p_evt)
{
  assert_param(_this != NULL);
  assert_param(p_evt != NULL);
  SensorListener_t *p_obj = (SensorListener_t *)_this;
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  res = SensorListenerStreamData(p_obj, (EMData_t*)p_evt->p_data);

  return res;
}


/* Private function definition */
/*******************************/

static sys_error_code_t SensorListenerDefSensorsConfig(SensorManager_t *p_sm, uint16_t *p_active_sensor_id)
{
  UNUSED(p_sm);
  UNUSED(p_active_sensor_id);

  SYS_DEBUGF(SYS_DBG_LEVEL_WARNING, ("SL: warning! Need to set the sensor confing function\r\n"));

  return SYS_NO_ERROR_CODE;
}

static sys_error_code_t SensorListenerStreamData(SensorListener_t *_this, EMData_t *p_data)
{
  assert_param(_this != NULL);
  assert_param(p_data != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  /*check the data type*/
  switch (EMD_GetType(p_data))
  {
    case E_EM_UINT8:
    case E_EM_UINT16:
    case E_EM_UINT32:
      SYS_DEBUGF(SYS_DBG_LEVEL_WARNING, ("CTRL: UINT data type not supported by stream function!\r\n"));
      break;
    case E_EM_INT8:
    case E_EM_INT16:
    case E_EM_INT32:
      res = SensorListenerStreamIntData(_this, p_data);
      break;
    case E_EM_FLOAT:
      res = SensorListenerStreamFloatData(_this, p_data);
      break;
    default:
      SYS_DEBUGF(SYS_DBG_LEVEL_WARNING, ("CTRL: data type not supported by stream function!\r\n"));
      break;
  }

  return res;
}

static sys_error_code_t SensorListenerStreamFloatData(SensorListener_t *_this, EMData_t *p_data)
{
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  if (EMD_GetDimensions(p_data) == 1U)
  {
    float *p_val = (float*)EMD_Data(p_data);
    for (uint16_t i=0; i<EMD_GetShape(p_data, 0); ++i)
    {
     	//fprintf(SCLI_TASK_CFG_OUT_CH, "[%f]\r\n", p_val[i]);
     	SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE,("[%f]\r\n", (*p_val)));
    }
  }
  else if(EMD_GetDimensions(p_data) == 2U)
  {
    /* display 2D data*/
    uint16_t dim0 = EMD_GetShape(p_data, 0);
    uint16_t dim1 = EMD_GetShape(p_data, 1);
    register float *p_val;
    for (uint16_t i=0; i<dim0; ++i)
    {
      //fprintf(SCLI_TASK_CFG_OUT_CH, "[");
      SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE,("["));
      for (uint16_t j=0; j<dim1-1; ++j)
      {
        p_val = (float*)EMD_2dDataAt(p_data, i, j);
        //fprintf(SCLI_TASK_CFG_OUT_CH, "%f, ", (*p_val));
        SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE,("%f", (*p_val)));
      }
      p_val = (float*)EMD_2dDataAt(p_data, i, dim1-1);
      SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE,("%f]\r\n", (*p_val)));
      //fprintf(SCLI_TASK_CFG_OUT_CH, "%f]\r\n", (*p_val));
    }
  }

  return res;
}

static sys_error_code_t SensorListenerStreamIntData(SensorListener_t *_this, EMData_t *p_data)
{
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  if (EMD_GetDimensions(p_data) == 1U)
  {
    int16_t *p_val = (int16_t*)EMD_Data(p_data);
    for (uint16_t i=0; i<EMD_GetShape(p_data, 0); ++i)
    {
      //fprintf(SCLI_TASK_CFG_OUT_CH, "[%d]\r\n", p_data[i]);
      SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE,("[%d]\r\n", (p_val[i])));
    }
  }
  else if(EMD_GetDimensions(p_data) == 2U)
  {
    /* display 2D data*/
    uint16_t dim0 = EMD_GetShape(p_data, 0);
    uint16_t dim1 = EMD_GetShape(p_data, 1);
    register int16_t *p_val;
    for (uint16_t i=0; i<dim0; ++i)
    {
      //fprintf(SCLI_TASK_CFG_OUT_CH, "[");
      SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE,("["));
      for (uint16_t j=0; j<dim1-1; ++j)
      {
    	p_val = (int16_t*)EMD_2dDataAt(p_data, i, j);
    	SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE,("%d, ", (*p_val)));
        //fprintf(SCLI_TASK_CFG_OUT_CH, "%d, ", (*p_val));
      }
      p_val = (int16_t*)EMD_2dDataAt(p_data, i, dim1-1);
      //fprintf(SCLI_TASK_CFG_OUT_CH, "%d]\r\n", (*p_val));
      SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE,("%d]\r\n", (*p_val)));
    }
  }

  return res;
}

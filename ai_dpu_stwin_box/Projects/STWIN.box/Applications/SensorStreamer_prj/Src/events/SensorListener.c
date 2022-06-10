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
  ISensorEventListener_vtbl vtbl;

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
sys_error_code_t SensorListener_vtblOnNewDataReady(IEventListener *_this, const SensorEvent *p_evt)
{
  assert_param(_this != NULL);
  assert_param(p_evt != NULL);
//  SensorListener_t *p_obj = (SensorListener_t *)_this;
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  char * sensor_name = SMSensorGetDescription(p_evt->nSensorID).Name;
  uint16_t n_shape = p_evt->stream->shape.n_shape;

  SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("SL: %s[", sensor_name));
  for (uint16_t i=0; i< n_shape; ++i)
  {
    SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("%d", p_evt->stream->shape.shapes[i]));
  }
  /* assume that the raw sensor data are or uint6_t or float*/
  if (p_evt->stream->payload_fmt == AI_SP_FMT_FLOAT32_RESET())
  {
    const float *p_sensor_raw_data = (const float*)p_evt->stream->payload;
    SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("] new raw data[] = %.2f\r\n", *p_sensor_raw_data));
  }
  else if (p_evt->stream->payload_fmt == AI_SP_FMT_INT16_RESET())
  {
    const uint16_t *p_sensor_raw_data = (const uint16_t*)p_evt->stream->payload;
    SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("] new raw data[] = %d\r\n", *p_sensor_raw_data));
  }


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

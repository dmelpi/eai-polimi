/**
 ******************************************************************************
 * @file    LPS22HHClass.c
 * @author  SRA - MCD
 * @version 1.0.0
 * @date    6-Sep-2021
 * @brief   This file provides a set of functions to handle lps22hh sensor
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

/* Includes ------------------------------------------------------------------*/
#include "LPS22HHClass.h"
#include "ISensor.h"
#include "ISensor_vtbl.h"
#include "SMUtilTask.h"
#include <string.h>
#include "services/sysdebug.h"

/* Private includes ----------------------------------------------------------*/

#define SYS_DEBUGF(level, message)                SYS_DEBUGF3(SYS_DBG_LPS22HH, level, message)

#ifndef HSD_USE_DUMMY_DATA
#define HSD_USE_DUMMY_DATA 0
#endif

#if (HSD_USE_DUMMY_DATA == 1)
static uint16_t dummyDataCounter_press = 0;
static uint16_t dummyDataCounter_temp = 0;
#endif



/**
 * The only instance of the task object.
 */
static LPS22HHClass sClassObj;


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


// Public API definition
// *********************

ISourceObservable *LPS22HHClassGetTempSensorIF(LPS22HHClass *_this){
  return (ISourceObservable *)&(_this->temp_sensor_if);
}

ISourceObservable *LPS22HHClassGetPressSensorIF(LPS22HHClass *_this){
  return (ISourceObservable *)&(_this->press_sensor_if);
}

I2CBusIF *LPS22HHClassGetSensorIF(LPS22HHClass *_this) {
  assert_param(_this);

  return &_this->sensor_if;
}

IEventSrc *LPS22HHClassGetTempEventSrcIF(LPS22HHClass *_this) {
  assert_param(_this);

  return _this->p_temp_event_src;
}

IEventSrc *LPS22HHClassGetPressEventSrcIF(LPS22HHClass *_this) {
  assert_param(_this);

  return _this->p_press_event_src;
}

LPS22HHClass* LPS22HHClassGetInstance(){
  return (LPS22HHClass*)&sClassObj;
}

sys_error_code_t LPS22HHClassSensorInit(LPS22HHClass *_this) {
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  stmdev_ctx_t *p_sensor_drv = (stmdev_ctx_t*) &_this->sensor_if.super.m_xConnector;

  uint8_t reg0 = 0;
  int32_t ret_val = 0;
  float lps22hh_odr = 0.0f;

  /* Power Down */
  ret_val = lps22hh_device_id_get( p_sensor_drv, (uint8_t *)&reg0);
  if (!ret_val) {
    I2CBusIFSetWhoAmI(&_this->sensor_if, reg0);
  }
  SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("LPS22HH: sensor - I am 0x%x.\r\n", reg0));

  /* Disable MIPI I3C(SM) interface */
  lps22hh_i3c_interface_set(p_sensor_drv, LPS22HH_I3C_DISABLE);

  /* Power down the device, set Low Noise Enable (bit 5), clear One Shot (bit 4) */
  lps22hh_data_rate_set(p_sensor_drv, (lps22hh_odr_t)(LPS22HH_POWER_DOWN | 0x10));

  /* Disable low-pass filter on LPS22HH pressure data */
  lps22hh_lp_bandwidth_set(p_sensor_drv, LPS22HH_LPF_ODR_DIV_2);

  /* Set block data update mode */
  lps22hh_block_data_update_set(p_sensor_drv, PROPERTY_ENABLE);

  /* Set autoincrement for multi-byte read/write */
  lps22hh_auto_increment_set(p_sensor_drv, PROPERTY_ENABLE);

  lps22hh_reset_set(p_sensor_drv,1);

  /* Set fifo mode */
  lps22hh_fifo_mode_set(p_sensor_drv, LPS22HH_STREAM_MODE);

  if (_this->sensor_status.pSubSensorStatus[1].IsActive == TRUE)
  {
    lps22hh_odr = _this->sensor_status.pSubSensorStatus[1].ODR;
    _this->sensor_status.pSubSensorStatus[0].ODR = _this->sensor_status.pSubSensorStatus[1].ODR;
  }
  else
  {
    lps22hh_odr = _this->sensor_status.pSubSensorStatus[0].ODR;
    _this->sensor_status.pSubSensorStatus[1].ODR = _this->sensor_status.pSubSensorStatus[0].ODR;
  }

  if(lps22hh_odr < 2.0f)
  {
    lps22hh_data_rate_set(p_sensor_drv, LPS22HH_1_Hz);
    _this->task_delay = 1000;
  }
  else if(lps22hh_odr < 11.0f)
  {
    lps22hh_data_rate_set(p_sensor_drv, LPS22HH_10_Hz);
    _this->task_delay = 1000;
  }
  else if(lps22hh_odr < 26.0f)
  {
    lps22hh_data_rate_set(p_sensor_drv, LPS22HH_25_Hz);
    _this->task_delay = 1000;
  }
  else if(lps22hh_odr < 51.0f)
  {
    lps22hh_data_rate_set(p_sensor_drv, LPS22HH_50_Hz);
    _this->task_delay = 1000;
  }
  else if(lps22hh_odr < 76.0f)
  {
    lps22hh_data_rate_set(p_sensor_drv, LPS22HH_75_Hz);
    _this->task_delay = 1000;
  }
  else if(lps22hh_odr < 101.0f)
  {
    lps22hh_data_rate_set(p_sensor_drv, LPS22HH_100_Hz);
    _this->task_delay = 1000;
  }
  else
  {
    lps22hh_data_rate_set(p_sensor_drv, LPS22HH_200_Hz);
    _this->task_delay = 500;
  }

  return res;
}

sys_error_code_t LPS22HHClassSensorReadData(LPS22HHClass *_this) {
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  stmdev_ctx_t *p_sensor_drv = (stmdev_ctx_t*) &_this->sensor_if.super.m_xConnector;

  lps22hh_fifo_data_level_get(p_sensor_drv, (uint8_t *)&_this->fifo_level);
  lps22hh_read_reg(p_sensor_drv, LPS22HH_FIFO_DATA_OUT_PRESS_XL, (uint8_t *) _this->p_sensor_data_buff, 5*_this->fifo_level);
  uint16_t i = 0;

  for(i = 0; i < _this->fifo_level; i++)
  {
    uint32_t press = (((uint32_t)_this->p_sensor_data_buff[5 * i + 0])) | (((uint32_t)_this->p_sensor_data_buff[5 * i + 1]) << (8 * 1)) | (((uint32_t)_this->p_sensor_data_buff[5 * i + 2]) << (8 * 2));

    /* convert the 2's complement 24 bit to 2's complement 32 bit */
    if(press & 0x00800000)
      press |= 0xFF000000;

    uint16_t temp = *((uint16_t *)(&_this->p_sensor_data_buff[5 * i + 3]));

    if(_this->sensor_status.pSubSensorStatus[0].IsActive && !_this->sensor_status.pSubSensorStatus[1].IsActive) /* Only Pressure */
    {
      _this->p_press_data_buff[i] = (float)press/4096.0f; /* Pressure */
    }
    else if(!_this->sensor_status.pSubSensorStatus[0].IsActive && _this->sensor_status.pSubSensorStatus[1].IsActive) /* Only Temperature */
    {
      _this->p_temp_data_buff[i] = (float)temp/100.0f; /* Temperature */
    }
    else if(_this->sensor_status.pSubSensorStatus[0].IsActive && _this->sensor_status.pSubSensorStatus[1].IsActive) /* Both Sub Sensors */
    {
      _this->p_press_data_buff[i] = (float)press/4096.0f; /* Pressure */
      _this->p_temp_data_buff[i] = (float)temp/100.0f; /* Temperature */
    }
  }

#if (HSD_USE_DUMMY_DATA == 1)
  for (i = 0; i < _this->fifo_level ; i++)
  {
    _this->p_press_data_buff[i]  = (float)dummyDataCounter_press++;
    _this->p_temp_data_buff[i] =(float)dummyDataCounter_temp++;
  }
#endif

  return res;
}


sys_error_code_t LPS22HHClassSensorInitClassParams(LPS22HHClass *_this) {
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  /* SENSOR DESCRIPTOR */
  strcpy(_this->sensor_descriptor.Name, "LPS22HH");
  _this->sensor_descriptor.NumberSubSensors = 2;

  /* SUBSENSOR 0 DESCRIPTOR */
  _this->sensor_descriptor.pSubSensorDescriptor[0].IdSub = _this->press_id;
  _this->sensor_descriptor.pSubSensorDescriptor[0].SensorType = COM_TYPE_PRESS;
  _this->sensor_descriptor.pSubSensorDescriptor[0].Dimensions = 1;
  strcpy(_this->sensor_descriptor.pSubSensorDescriptor[0].DimensionsLabel[0], "prs");
  _this->sensor_descriptor.pSubSensorDescriptor[0].DataType = DATA_TYPE_FLOAT;
  _this->sensor_descriptor.pSubSensorDescriptor[0].pODR[0] = 1.0f;
  _this->sensor_descriptor.pSubSensorDescriptor[0].pODR[1] = 10.0f;
  _this->sensor_descriptor.pSubSensorDescriptor[0].pODR[2] = 25.0f;
  _this->sensor_descriptor.pSubSensorDescriptor[0].pODR[3] = 50.0f;
  _this->sensor_descriptor.pSubSensorDescriptor[0].pODR[4] = 75.0f;
  _this->sensor_descriptor.pSubSensorDescriptor[0].pODR[5] = 100.0f;
  _this->sensor_descriptor.pSubSensorDescriptor[0].pODR[6] = 200.0f;
  _this->sensor_descriptor.pSubSensorDescriptor[0].pODR[7] = COM_END_OF_LIST_FLOAT;
  _this->sensor_descriptor.pSubSensorDescriptor[0].pSamplesPerTimestamp[0] = 0;
  _this->sensor_descriptor.pSubSensorDescriptor[0].pSamplesPerTimestamp[1] = 1000;
  strcpy(_this->sensor_descriptor.pSubSensorDescriptor[0].unit, "hPa");
  _this->sensor_descriptor.pSubSensorDescriptor[0].pFS[0] = 1260.0f;
  _this->sensor_descriptor.pSubSensorDescriptor[0].pFS[1] = COM_END_OF_LIST_FLOAT;

  /* SUBSENSOR 0 STATUS */
  _this->sensor_status.pSubSensorStatus[0].IsActive = TRUE;
  _this->sensor_status.pSubSensorStatus[0].FS = 1260.0f;
  _this->sensor_status.pSubSensorStatus[0].Sensitivity = 1.0f;
  _this->sensor_status.pSubSensorStatus[0].ODR = 10.0f;
  _this->sensor_status.pSubSensorStatus[0].MeasuredODR = 0.0f;
  _this->sensor_status.pSubSensorStatus[0].InitialOffset = 0.0f;
#if (HSD_USE_DUMMY_DATA == 1)
  _this->sensor_status.pSubSensorStatus[0].SamplesPerTimestamp = 0;
#else
  _this->sensor_status.pSubSensorStatus[0].SamplesPerTimestamp = 200;
#endif

  /* SUBSENSOR 1 DESCRIPTOR */
  _this->sensor_descriptor.pSubSensorDescriptor[1].IdSub = _this->temp_id;
  _this->sensor_descriptor.pSubSensorDescriptor[1].SensorType = COM_TYPE_TEMP;
  _this->sensor_descriptor.pSubSensorDescriptor[1].Dimensions = 1;
  strcpy(_this->sensor_descriptor.pSubSensorDescriptor[1].DimensionsLabel[0], "tem");
  _this->sensor_descriptor.pSubSensorDescriptor[1].DataType = DATA_TYPE_FLOAT;
  _this->sensor_descriptor.pSubSensorDescriptor[1].pODR[0] = 1.0f;
  _this->sensor_descriptor.pSubSensorDescriptor[1].pODR[1] = 10.0f;
  _this->sensor_descriptor.pSubSensorDescriptor[1].pODR[2] = 25.0f;
  _this->sensor_descriptor.pSubSensorDescriptor[1].pODR[3] = 50.0f;
  _this->sensor_descriptor.pSubSensorDescriptor[1].pODR[4] = 75.0f;
  _this->sensor_descriptor.pSubSensorDescriptor[1].pODR[5] = 100.0f;
  _this->sensor_descriptor.pSubSensorDescriptor[1].pODR[6] = 200.0f;
  _this->sensor_descriptor.pSubSensorDescriptor[1].pODR[7] = COM_END_OF_LIST_FLOAT;
  _this->sensor_descriptor.pSubSensorDescriptor[1].pSamplesPerTimestamp[0] = 0;
  _this->sensor_descriptor.pSubSensorDescriptor[1].pSamplesPerTimestamp[1] = 1000;
  strcpy(_this->sensor_descriptor.pSubSensorDescriptor[1].unit, "Celsius");
  _this->sensor_descriptor.pSubSensorDescriptor[1].pFS[0] = 85.0f;
  _this->sensor_descriptor.pSubSensorDescriptor[1].pFS[1] = COM_END_OF_LIST_FLOAT;

  /* SUBSENSOR 1 STATUS */
  _this->sensor_status.pSubSensorStatus[1].IsActive = TRUE;
  _this->sensor_status.pSubSensorStatus[1].FS = 85.0f;
  _this->sensor_status.pSubSensorStatus[1].Sensitivity = 1.0f;
  _this->sensor_status.pSubSensorStatus[1].ODR = 10.0f;
  _this->sensor_status.pSubSensorStatus[1].MeasuredODR = 0.0f;
  _this->sensor_status.pSubSensorStatus[1].InitialOffset = 0.0f;
#if (HSD_USE_DUMMY_DATA == 1)
  _this->sensor_status.pSubSensorStatus[1].SamplesPerTimestamp = 0;
#else
  _this->sensor_status.pSubSensorStatus[1].SamplesPerTimestamp = 200;
#endif

  return res;
}

sys_error_code_t LPS22HHClassSensorStart(LPS22HHClass *_this){
  assert_param(_this);
  sys_error_code_t res = SYS_NOT_IMPLEMENTED_ERROR_CODE;

  return res;
}

sys_error_code_t LPS22HHClassSensorStop(LPS22HHClass *_this){
  assert_param(_this);
  sys_error_code_t res = SYS_NOT_IMPLEMENTED_ERROR_CODE;

  return res;
}

sys_error_code_t LPS22HHClassSensorSetODR(LPS22HHClass *_this, float ODR, uint8_t id){
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  stmdev_ctx_t *p_sensor_drv = (stmdev_ctx_t*) &_this->sensor_if.super.m_xConnector;

  if(id == _this->temp_id || id == _this->press_id)
  {
    if(ODR < 2.0f)
    {
      lps22hh_data_rate_set(p_sensor_drv, LPS22HH_1_Hz);
      ODR = 1.0f;
      _this->task_delay = 1000;
    }
    else if(ODR < 11.0f)
    {
      lps22hh_data_rate_set(p_sensor_drv, LPS22HH_10_Hz);
      ODR = 10.0f;
      _this->task_delay = 1000;
    }
    else if(ODR < 26.0f)
    {
      lps22hh_data_rate_set(p_sensor_drv, LPS22HH_25_Hz);
      ODR = 25.0f;
      _this->task_delay = 1000;
    }
    else if(ODR < 51.0f)
    {
      lps22hh_data_rate_set(p_sensor_drv, LPS22HH_50_Hz);
      ODR = 50.0f;
      _this->task_delay = 1000;
    }
    else if(ODR < 76.0f)
    {
      lps22hh_data_rate_set(p_sensor_drv, LPS22HH_75_Hz);
      ODR = 75.0f;
      _this->task_delay = 1000;
    }
    else if(ODR < 101.0f)
    {
      lps22hh_data_rate_set(p_sensor_drv, LPS22HH_100_Hz);
      ODR = 100.0f;
      _this->task_delay = 1000;
    }
    else
    {
      lps22hh_data_rate_set(p_sensor_drv, LPS22HH_200_Hz);
      ODR = 200.0f;
      _this->task_delay = 500;
    }

    if (!SYS_IS_ERROR_CODE(res))
    {
      if(id == _this->press_id)
        _this->sensor_status.pSubSensorStatus[0].ODR = ODR;
      else
        _this->sensor_status.pSubSensorStatus[1].ODR = ODR;
    }
  }
  else
  {
    res = SYS_INVALID_PARAMETER_ERROR_CODE;
  }

  return res;
}

sys_error_code_t LPS22HHClassSensorSetFS(LPS22HHClass *_this, float FS, uint8_t id){
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  if(id == _this->temp_id)
  {
    if(FS != 85.0f)
      res = SYS_INVALID_PARAMETER_ERROR_CODE;

    if (!SYS_IS_ERROR_CODE(res))
    {
      _this->sensor_status.pSubSensorStatus[1].FS = FS;
    }
  }
  else if(id == _this->press_id)
  {
    if(FS != 1260.0f)
      res = SYS_INVALID_PARAMETER_ERROR_CODE;

    if (!SYS_IS_ERROR_CODE(res))
    {
      _this->sensor_status.pSubSensorStatus[0].FS = FS;
    }
  }
  else
  {
    res = SYS_INVALID_PARAMETER_ERROR_CODE;
  }

  return res;
}

sys_error_code_t LPS22HHClassSensorEnable(LPS22HHClass *_this, uint8_t id){
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  if(id == _this->temp_id)
    _this->sensor_status.pSubSensorStatus[1].IsActive = TRUE;
  else if(id == _this->press_id)
    _this->sensor_status.pSubSensorStatus[0].IsActive = TRUE;
  else
    res = SYS_INVALID_PARAMETER_ERROR_CODE;

  return res;
}

sys_error_code_t LPS22HHClassSensorDisable(LPS22HHClass *_this, uint8_t id){
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  if(id == _this->temp_id)
    _this->sensor_status.pSubSensorStatus[1].IsActive = FALSE;
  else if(id == _this->press_id)
    _this->sensor_status.pSubSensorStatus[0].IsActive = FALSE;
  else
    res = SYS_INVALID_PARAMETER_ERROR_CODE;

  return res;
}

boolean_t LPS22HHClassSensorIsActive(const LPS22HHClass *_this) {
  assert_param(_this);
  boolean_t res = FALSE;

  // check if at least one sub sensor is active
  for (uint8_t i=0; i<_this->sensor_descriptor.NumberSubSensors; ++i) {
    if (_this->sensor_status.pSubSensorStatus[i].IsActive) {
      res = TRUE;
      break;
    }
  }

  return res;
}

sys_error_code_t LPS22HHClassEnterLowPowerMode(const LPS22HHClass *_this) {
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  stmdev_ctx_t *p_sensor_drv = (stmdev_ctx_t*) &_this->sensor_if.super.m_xConnector;

  if (lps22hh_data_rate_set(p_sensor_drv, (lps22hh_odr_t)(LPS22HH_POWER_DOWN | 0x10))) {
    res = SYS_SENSOR_TASK_OP_ERROR_CODE;
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_SENSOR_TASK_OP_ERROR_CODE);
  }

  return res;
}


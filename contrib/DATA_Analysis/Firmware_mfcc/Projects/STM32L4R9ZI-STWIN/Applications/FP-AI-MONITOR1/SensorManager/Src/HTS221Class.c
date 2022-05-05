/**
 ******************************************************************************
 * @file    HTS221Class.c
 * @author  SRA - MCD
 * @version 1.0.0
 * @date    6-Sep-2021
 * @brief   This file provides a set of functions to handle hts221 sensor
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
#include "HTS221Class.h"
#include "ISensor.h"
#include "ISensor_vtbl.h"
#include "SMUtilTask.h"
#include <string.h>
#include "services/sysdebug.h"

/* Private includes ----------------------------------------------------------*/

#define SYS_DEBUGF(level, message)                SYS_DEBUGF3(SYS_DBG_HTS221, level, message)

#if defined(DEBUG) || defined (SYS_DEBUG)
#define sTaskObj                        sHTS221TaskObj
#endif

#ifndef HSD_USE_DUMMY_DATA
#define HSD_USE_DUMMY_DATA 0
#endif

#if (HSD_USE_DUMMY_DATA == 1)
static uint16_t dummyDataCounter_temp = 0;
static uint16_t dummyDataCounter_hum = 0;
#endif



/**
 * The only instance of the task object.
 */
static HTS221Class sTaskObj;


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


// Public API definition
// *********************

ISourceObservable *HTS221ClassGetTempSensorIF(HTS221Class *_this){
  return (ISourceObservable *)&(_this->temp_sensor_if);
}

ISourceObservable *HTS221ClassGetHumSensorIF(HTS221Class *_this){
  return (ISourceObservable *)&(_this->hum_sensor_if);
}

I2CBusIF *HTS221ClassGetSensorIF(HTS221Class *_this) {
  assert_param(_this);

  return &_this->sensor_if;
}

IEventSrc *HTS221ClassGetTempEventSrcIF(HTS221Class *_this) {
  assert_param(_this);

  return _this->p_temp_event_src;
}

IEventSrc *HTS221ClassGetHumEventSrcIF(HTS221Class *_this) {
  assert_param(_this);

  return _this->p_hum_event_src;
}


HTS221Class* HTS221ClassGetInstance(){
  return (HTS221Class*)&sTaskObj;
}

sys_error_code_t HTS221ClassSensorInit(HTS221Class *_this) {
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  stmdev_ctx_t *p_sensor_drv = (stmdev_ctx_t*) &_this->sensor_if.super.m_xConnector;

  uint8_t reg0 = 0;
  int32_t ret_val = 0;
  float hts221_odr = 0.0f;

  typedef struct	/* From hts221.h - HL PID */
  {
    float x0;
    float y0;
    float x1;
    float y1;
  } lin_t;

  lin_t lin_temp;
  lin_t lin_hum;

  ret_val = hts221_device_id_get( p_sensor_drv, (uint8_t *)&reg0);
  if (!ret_val) {
    I2CBusIFSetWhoAmI(&_this->sensor_if, reg0);
  }
  SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("HTS221: sensor - I am 0x%x.\r\n", reg0));

  typedef union
  { /* From hts221.h - HL PID */
    int16_t i16bit;
    uint8_t u8bit[2];
  } hts221_axis1bit16_t;

  hts221_axis1bit16_t data_raw_humidity;
  hts221_axis1bit16_t data_raw_temperature;

  /* Read the output registers to reset the interrupt pin */
  /* Without these instructions, INT PIN remains HIGH and */
  /* it never swithces anymore */
  hts221_temperature_raw_get(p_sensor_drv, &data_raw_temperature.i16bit);
  hts221_humidity_raw_get(p_sensor_drv, &data_raw_humidity.i16bit);

  /* Power on and reset the component */
  hts221_power_on_set(p_sensor_drv, PROPERTY_DISABLE);
  hts221_boot_set(p_sensor_drv, PROPERTY_ENABLE);

  /* Set BDU */
  hts221_block_data_update_set(p_sensor_drv, PROPERTY_ENABLE);
  /* Enable Interrupt */
  hts221_drdy_on_int_set(p_sensor_drv, PROPERTY_ENABLE);

  /* Set Data Rate */
  if (_this->sensor_status.pSubSensorStatus[1].IsActive == TRUE)
  {
    hts221_odr = _this->sensor_status.pSubSensorStatus[1].ODR;
    _this->sensor_status.pSubSensorStatus[0].ODR = _this->sensor_status.pSubSensorStatus[1].ODR;
  }
  else
  {
    hts221_odr = _this->sensor_status.pSubSensorStatus[0].ODR;
    _this->sensor_status.pSubSensorStatus[1].ODR = _this->sensor_status.pSubSensorStatus[0].ODR;
  }

  if(hts221_odr < 2.0f)
  {
    hts221_data_rate_set(p_sensor_drv, HTS221_ODR_1Hz);
  }
  else if(hts221_odr < 8.0f)
  {
    hts221_data_rate_set(p_sensor_drv, HTS221_ODR_7Hz);
  }
  else
  {
    hts221_data_rate_set(p_sensor_drv, HTS221_ODR_12Hz5);
  }

  /* Get calibration values (only first time) */
  hts221_temp_adc_point_0_get(p_sensor_drv, &lin_temp.x0);
  _this->x0_t = lin_temp.x0;

  hts221_temp_deg_point_0_get(p_sensor_drv, &lin_temp.y0);
  _this->y0_t = lin_temp.y0;

  hts221_temp_adc_point_1_get(p_sensor_drv, &lin_temp.x1);
  _this->x1_t = lin_temp.x1;

  hts221_temp_deg_point_1_get(p_sensor_drv, &lin_temp.y1);
  _this->y1_t = lin_temp.y1;

  hts221_hum_adc_point_0_get(p_sensor_drv, &lin_hum.x0);
  _this->x0_h = lin_hum.x0;

  hts221_hum_rh_point_0_get(p_sensor_drv, &lin_hum.y0);
  _this->y0_h = lin_hum.y0;

  hts221_hum_adc_point_1_get(p_sensor_drv, &lin_hum.x1);
  _this->x1_h = lin_hum.x1;

  hts221_hum_rh_point_1_get(p_sensor_drv, &lin_hum.y1);
  _this->y1_h = lin_hum.y1;

  /* Power Up */
  hts221_power_on_set(p_sensor_drv, PROPERTY_ENABLE);

  return res;
}

sys_error_code_t HTS221ClassSensorReadData(HTS221Class *_this) {
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  stmdev_ctx_t *p_sensor_drv = (stmdev_ctx_t*) &_this->sensor_if.super.m_xConnector;

  typedef union{			/* From hts221.h - HL PID */
    int16_t i16bit;
    uint8_t u8bit[2];
  } hts221_axis1bit16_t;

  hts221_axis1bit16_t data_raw_humidity;
  hts221_axis1bit16_t data_raw_temperature;

  hts221_temperature_raw_get(p_sensor_drv, &data_raw_temperature.i16bit);
  hts221_humidity_raw_get(p_sensor_drv, &data_raw_humidity.i16bit);

  /* Apply calibration */ /* To be optimized eventually */
  _this->p_sensor_data_buff[0] = (((_this->y1_t - _this->y0_t) * (float)(data_raw_temperature.i16bit)) + ((_this->x1_t * _this->y0_t) - (_this->x0_t * _this->y1_t))) / (_this->x1_t - _this->x0_t);

  /* Apply calibration */ /* To be optimized eventually */
  _this->p_sensor_data_buff[1] = (((_this->y1_h - _this->y0_h) * (float)(data_raw_humidity.i16bit)) + ((_this->x1_h * _this->y0_h) - (_this->x0_h * _this->y1_h))) / (_this->x1_h - _this->x0_h);

#if (HSD_USE_DUMMY_DATA == 1)
  _this->p_sensor_data_buff[0] = (float)(dummyDataCounter_temp++);
  _this->p_sensor_data_buff[1] = (float)(dummyDataCounter_hum++);
#endif

  return res;
}


sys_error_code_t HTS221ClassSensorInitClassParams(HTS221Class *_this) {
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

    /* SENSOR DESCRIPTOR */
  strcpy(_this->sensor_descriptor.Name, "HTS221");
  _this->sensor_descriptor.NumberSubSensors = 2;

    /* SUBSENSOR 0 DESCRIPTOR */
  _this->sensor_descriptor.pSubSensorDescriptor[0].IdSub = _this->temp_id;
  _this->sensor_descriptor.pSubSensorDescriptor[0].SensorType = COM_TYPE_TEMP;
  _this->sensor_descriptor.pSubSensorDescriptor[0].Dimensions = 1;
  strcpy(_this->sensor_descriptor.pSubSensorDescriptor[0].DimensionsLabel[0], "tem");
  _this->sensor_descriptor.pSubSensorDescriptor[0].DataType = DATA_TYPE_FLOAT;
  _this->sensor_descriptor.pSubSensorDescriptor[0].pODR[0] = 1.0f;
  _this->sensor_descriptor.pSubSensorDescriptor[0].pODR[1] = 7.0f;
  _this->sensor_descriptor.pSubSensorDescriptor[0].pODR[2] = 12.5f;
  _this->sensor_descriptor.pSubSensorDescriptor[0].pODR[3] = COM_END_OF_LIST_FLOAT;
  _this->sensor_descriptor.pSubSensorDescriptor[0].pSamplesPerTimestamp[0] = 0;
  _this->sensor_descriptor.pSubSensorDescriptor[0].pSamplesPerTimestamp[1] = 1000;
  strcpy(_this->sensor_descriptor.pSubSensorDescriptor[0].unit, "Celsius");
  _this->sensor_descriptor.pSubSensorDescriptor[0].pFS[0] = 120.0f;
  _this->sensor_descriptor.pSubSensorDescriptor[0].pFS[1] = COM_END_OF_LIST_FLOAT;

    /* SUBSENSOR 0 STATUS */
  _this->sensor_status.pSubSensorStatus[0].IsActive = TRUE;
  _this->sensor_status.pSubSensorStatus[0].FS = 120.0f;
  _this->sensor_status.pSubSensorStatus[0].Sensitivity = 1.0f;
  _this->sensor_status.pSubSensorStatus[0].ODR = 12.5f;
  _this->sensor_status.pSubSensorStatus[0].MeasuredODR = 0.0f;
  _this->sensor_status.pSubSensorStatus[0].InitialOffset = 0.0f;
#if (HSD_USE_DUMMY_DATA == 1)
  _this->sensor_status.pSubSensorStatus[0].SamplesPerTimestamp = 0;
#else
  _this->sensor_status.pSubSensorStatus[0].SamplesPerTimestamp = 50;
#endif

    /* SUBSENSOR 1 DESCRIPTOR */
  _this->sensor_descriptor.pSubSensorDescriptor[1].IdSub = _this->hum_id;
  _this->sensor_descriptor.pSubSensorDescriptor[1].SensorType = COM_TYPE_HUM;
  _this->sensor_descriptor.pSubSensorDescriptor[1].Dimensions = 1;
  strcpy(_this->sensor_descriptor.pSubSensorDescriptor[1].DimensionsLabel[0], "hum");
  _this->sensor_descriptor.pSubSensorDescriptor[1].DataType = DATA_TYPE_FLOAT;
  _this->sensor_descriptor.pSubSensorDescriptor[1].pODR[0] = 1.0f;
  _this->sensor_descriptor.pSubSensorDescriptor[1].pODR[1] = 7.0f;
  _this->sensor_descriptor.pSubSensorDescriptor[1].pODR[2] = 12.5f;
  _this->sensor_descriptor.pSubSensorDescriptor[1].pODR[3] = COM_END_OF_LIST_FLOAT;
  _this->sensor_descriptor.pSubSensorDescriptor[1].pSamplesPerTimestamp[0] = 0;
  _this->sensor_descriptor.pSubSensorDescriptor[1].pSamplesPerTimestamp[1] = 1000;
  strcpy(_this->sensor_descriptor.pSubSensorDescriptor[1].unit, "%");
  _this->sensor_descriptor.pSubSensorDescriptor[1].pFS[0] = 100.0f;
  _this->sensor_descriptor.pSubSensorDescriptor[1].pFS[1] = COM_END_OF_LIST_FLOAT;

    /* SUBSENSOR 1 STATUS */
  _this->sensor_status.pSubSensorStatus[1].IsActive = TRUE;
  _this->sensor_status.pSubSensorStatus[1].FS = 100.0f;
  _this->sensor_status.pSubSensorStatus[1].Sensitivity = 1.0f;
  _this->sensor_status.pSubSensorStatus[1].ODR = 12.5f;
  _this->sensor_status.pSubSensorStatus[1].MeasuredODR = 0.0f;
  _this->sensor_status.pSubSensorStatus[1].InitialOffset = 0.0f;
#if (HSD_USE_DUMMY_DATA == 1)
  _this->sensor_status.pSubSensorStatus[1].SamplesPerTimestamp = 0;
#else
  _this->sensor_status.pSubSensorStatus[1].SamplesPerTimestamp = 50;
#endif

  return res;
}

sys_error_code_t HTS221ClassSensorStart(HTS221Class *_this){
  assert_param(_this);
  sys_error_code_t res = SYS_NOT_IMPLEMENTED_ERROR_CODE;

  return res;
}

sys_error_code_t HTS221ClassSensorStop(HTS221Class *_this){
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  stmdev_ctx_t *p_sensor_drv = (stmdev_ctx_t*) &_this->sensor_if.super.m_xConnector;
  hts221_power_on_set(p_sensor_drv, PROPERTY_DISABLE);
  return res;
}

sys_error_code_t HTS221ClassSensorSetODR(HTS221Class *_this, float ODR, uint8_t id){
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  stmdev_ctx_t *p_sensor_drv = (stmdev_ctx_t*) &_this->sensor_if.super.m_xConnector;

  if(id == _this->temp_id || id == _this->hum_id)
  {
    if(ODR < 2.0f)
    {
      hts221_data_rate_set(p_sensor_drv, HTS221_ODR_1Hz);
      ODR = 1.0f;
    }
    else if(ODR < 8.0f)
    {
      hts221_data_rate_set(p_sensor_drv, HTS221_ODR_7Hz);
      ODR = 7.0f;
    }
    else
    {
      hts221_data_rate_set(p_sensor_drv, HTS221_ODR_12Hz5);
      ODR = 12.5f;
    }

    if (!SYS_IS_ERROR_CODE(res))
    {
      if(id == _this->temp_id)
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

sys_error_code_t HTS221ClassSensorSetFS(HTS221Class *_this, float FS, uint8_t id){
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  if(id == _this->temp_id)
  {
    if(FS != 120.0f)
      res = SYS_INVALID_PARAMETER_ERROR_CODE;

    if (!SYS_IS_ERROR_CODE(res))
    {
      _this->sensor_status.pSubSensorStatus[0].FS = FS;
    }
  }
  else if(id == _this->hum_id)
  {
    if(FS != 100.0f)
      res = SYS_INVALID_PARAMETER_ERROR_CODE;

    if (!SYS_IS_ERROR_CODE(res))
    {
      _this->sensor_status.pSubSensorStatus[1].FS = FS;
    }
  }
  else
  {
    res = SYS_INVALID_PARAMETER_ERROR_CODE;
  }

  return res;
}

sys_error_code_t HTS221ClassSensorEnable(HTS221Class *_this, uint8_t id){
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  if(id == _this->temp_id)
    _this->sensor_status.pSubSensorStatus[0].IsActive = TRUE;
  else if(id == _this->hum_id)
    _this->sensor_status.pSubSensorStatus[1].IsActive = TRUE;
  else
    res = SYS_INVALID_PARAMETER_ERROR_CODE;

  return res;
}

sys_error_code_t HTS221ClassSensorDisable(HTS221Class *_this, uint8_t id){
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  if(id == _this->temp_id)
    _this->sensor_status.pSubSensorStatus[0].IsActive = FALSE;
  else if(id == _this->hum_id)
    _this->sensor_status.pSubSensorStatus[1].IsActive = FALSE;
  else
    res = SYS_INVALID_PARAMETER_ERROR_CODE;

  return res;
}

boolean_t HTS221ClassSensorIsActive(const HTS221Class *_this) {
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

sys_error_code_t HTS221ClassEnterLowPowerMode(const HTS221Class *_this) {
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  stmdev_ctx_t *p_sensor_drv = (stmdev_ctx_t*) &_this->sensor_if.super.m_xConnector;

  if (hts221_power_on_set(p_sensor_drv, PROPERTY_DISABLE)) {
    res = SYS_SENSOR_TASK_OP_ERROR_CODE;
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_SENSOR_TASK_OP_ERROR_CODE);
  }

  return res;
}

sys_error_code_t HTS221ClassConfigureIrqPin(const HTS221Class *_this, boolean_t LowPower) {
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  if (!LowPower) {

	  HTS221_INT_GPIO_ADDITIONAL();
	  HTS221_INT_GPIO_CLK_ENABLE();
	  /*Configure GPIO pins : HTS221_INT_Pin hts221_INT1_Pin */
	  GPIO_InitStruct.Pin =  HTS221_INT_Pin ;
	  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  HAL_GPIO_Init(HTS221_INT_GPIO_Port, &GPIO_InitStruct);

	  /* EXTI interrupt init*/
	  HAL_NVIC_SetPriority(HTS221_INT_EXTI_IRQn, 5, 0);
  }
  else {
    // first disable the IRQ to avoid spurious interrupt to wake the MCU up.
    HAL_NVIC_DisableIRQ(HTS221_INT_EXTI_IRQn);
    HAL_NVIC_ClearPendingIRQ(HTS221_INT_EXTI_IRQn);
    // then reconfigure the PIN in analog high impedance to reduce the power consumption.
    GPIO_InitStruct.Pin =  HTS221_INT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(HTS221_INT_GPIO_Port, &GPIO_InitStruct);
  }

  return res;
}


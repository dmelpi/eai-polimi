/**
  ******************************************************************************
  * @file    SIterator.c
  * @author  SRA - MCD
 * @version 1.1.0
 * @date    10-Dec-2021
  *
  * @brief   Sensor Iterator declaration.
  *
  * Definition of the ::SIterator public API.
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

#include "services/SIterator.h"

/* Public functions definition */
/*******************************/

sys_error_code_t SIInit(SIterator_t *_this, SensorManager_t *p_sm)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  /* parameters validation */
  if (p_sm == NULL)
  {
    res = SYS_INVALID_PARAMETER_ERROR_CODE;
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_INVALID_PARAMETER_ERROR_CODE);
  }
  else
  {
    _this->p_sm = p_sm;
    _this->sensor_idx = 0;
    _this->sensors_count = SMGetNsensor();
  }

  return res;
}

bool SIHasNext(SIterator_t *_this)
{
  assert_param(_this != NULL);

  return _this->sensor_idx < _this->sensors_count ? true : false;
}

uint16_t SINext(SIterator_t *_this)
{
  assert_param(_this != NULL);
  uint16_t sensor_id = SI_NULL_SENSOR_ID;

  if (_this->sensor_idx < _this->sensors_count)
  {
    sensor_id = _this->sensor_idx++;
  }

  return sensor_id;
}

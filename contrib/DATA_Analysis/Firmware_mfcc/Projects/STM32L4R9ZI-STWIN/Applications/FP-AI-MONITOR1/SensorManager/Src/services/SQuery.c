/**
 ******************************************************************************
 * @file    SQuery.c
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

#include "services/SQuery.h"
#include <string.h>

sys_error_code_t SQInit(SQuery_t *_this, SensorManager_t *p_sm)
{
  assert_param(_this != NULL);

  return SIInit(&_this->iterator, p_sm);
}

uint16_t SQNextByName(SQuery_t *_this, const char *sensor_name)
{
  assert_param(_this != NULL);
  uint16_t sensor_id = SI_NULL_SENSOR_ID;
  bool found_next_sensor = false;

  while (SIHasNext(&_this->iterator) && !found_next_sensor) {
    uint16_t next_sensor_id = SINext(&_this->iterator);
    SensorDescriptor_t descriptor = SMSensorGetDescription(next_sensor_id);
    /* check if the name match the query */

    if (strncmp(sensor_name, descriptor.Name, SM_MAX_DIM_LABELS) == 0)
    {
      sensor_id = next_sensor_id;
      found_next_sensor = true;
    }
  }

  return sensor_id;
}

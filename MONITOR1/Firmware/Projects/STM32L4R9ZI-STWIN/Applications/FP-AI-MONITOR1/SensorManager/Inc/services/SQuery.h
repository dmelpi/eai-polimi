/**
 ******************************************************************************
 * @file    SQuery.h
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
#ifndef SENSORMANAGER_INC_SERVICES_SQUERY_H_
#define SENSORMANAGER_INC_SERVICES_SQUERY_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "services/SIterator.h"


  /**
   * Create a type name for ::_SQuery_t
   */
  typedef struct _SQuery_t SQuery_t;

  /**
   * Sensor Query internal state.
   */
  struct _SQuery_t
  {
    /**
     * Specifies the iterator used to to iterate trough a sensors.
     */
    SIterator_t iterator;
  };

  /**
   *  Initialize the query based on a sensor manager instance.
   *
   * @param _this [IN] specifies an ::SIterator_t object.
   * @param p_sm [IN] specifies a ::SensorManager_t instance.
   * @return SYS_NO_ERROR_CODE if success, an error code otherwise.
   */
  sys_error_code_t SQInit(SQuery_t *_this, SensorManager_t *p_sm);

  /**
   * Return the sensor id of the next sensor in the iteration that match the given name.
   *
   * @param _this [IN] specifies an ::SIterator_t object.
   * @param sensor_name [IN] specify the name parameter for the query.
   * @return the sensor id of the next sensor in the iteration matching the query or SI_NULL_SENSOR
   */
  uint16_t SQNextByName(SQuery_t *_this, const char *sensor_name);

#ifdef __cplusplus
}
#endif

#endif /* SENSORMANAGER_INC_SERVICES_SQUERY_H_ */

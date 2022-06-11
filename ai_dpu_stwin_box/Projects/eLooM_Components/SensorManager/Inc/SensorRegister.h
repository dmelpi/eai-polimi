/**
 ******************************************************************************
 * @file    SensorRegister.h
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
#ifndef HSDCORE_INC_SENSORREGISTER_H_
#define HSDCORE_INC_SENSORREGISTER_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "ISensor.h"
#include "ISensor_vtbl.h"

#define SM_INVALID_SENSOR_ID (0xFF) /**< Specifies the invalid sensor ID.  */

  /* Public API declaration */
  /**************************/

  /**
   * Register a sensor with the SensorManager. During the registration the SensorManager assigns an ID to the sensor.
   * This ID is unique during the application lifecycle, and it can be used with the other public API of the
   * SensorManager to operate the sensor.
   *
   * @param pSensor [IN] specifies a sensor interface to be registered with the SensorManager.
   * @return the sensor ID if success, SM_INVALID_SENSOR_ID otherwise.
   */
  uint8_t SMAddSensor(ISensor_t *pSensor);

  /**
   * Remove a sensor from the SensorManager.
   * @param id [IN] specifies the Sensor ID to be removed.
   * @return the sensor ID if success, SM_INVALID_SENSOR_ID otherwise.
   */
  sys_error_code_t SMRemoveSensor(ISensor_t *pSensor);


  /* Inline functions definition */
  /*******************************/


#ifdef __cplusplus
}
#endif

#endif /* HSDCORE_INC_SENSORREGISTER_H_ */

/**
 ******************************************************************************
 * @file    SensorManager.h
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
#ifndef HSDCORE_INC_SENSORMANAGER_H_
#define HSDCORE_INC_SENSORMANAGER_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "services/systp.h"
#include "services/syserror.h"
#include "SensorDef.h"
#include "SensorRegister.h"


  /**
   * Create  type name for _SensorManager_t.
   */
  typedef struct _SensorManager_t SensorManager_t;

  /**
   *  SensorManager_t internal structure.
   */
  struct _SensorManager_t {

    /**
     * Describes the sensor capabilities.
     */
    ISensor_t *Sensors[COM_MAX_SENSORS];

    /**
     * Indicates the number of sensors available.
     */
    uint16_t n_sensors;
  };


  /* Public API declaration */
  /**************************/
  ISourceObservable * SMGetSensorObserver(uint8_t id);
  uint16_t SMGetNsensor(void);
  sys_error_code_t SMRemoveSensor(uint8_t id);
  sys_error_code_t SMSensorStart(uint8_t id);
  sys_error_code_t SMSensorStop(uint8_t id);
  sys_error_code_t SMSensorSetODR(uint8_t id, float ODR);
  sys_error_code_t SMSensorSetFS(uint8_t id, float FS);
  sys_error_code_t SMSensorEnable(uint8_t id);
  sys_error_code_t SMSensorDisable(uint8_t id);
  SensorDescriptor_t SMSensorGetDescription(uint8_t id);
  sys_error_code_t SMDeviceGetDescription(SensorDescriptor_t *device_description);
  SensorManager_t * SMGetSensorManager(void);


  /* Inline functions definition */
  /*******************************/


#ifdef __cplusplus
}
#endif

#endif /* HSDCORE_INC_SENSORMANAGER_H_ */

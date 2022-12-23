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

#ifndef INC_SENSORLISTENER_H_
#define INC_SENSORLISTENER_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "services/systp.h"
#include "services/systypes.h"
#include "services/syserror.h"
#include "events/IDataEventListener.h"
#include "events/IDataEventListener_vtbl.h"
#include "SensorManager.h"


/**
 * Create  type name for _SensorListener_t.
 */
typedef struct _SensorListener_t SensorListener_t;

/**
 * Function prototype to set a configuration for the SensorManager.This function configures the sensors
 * available through the SensorManager and it specifies the active sensor used by the SensorListener
 * to visualize the sensor data.
 *
 * @param p_sm [IN] specifies a pointer to the application SensorManager object.
 * @param p_active_sensor_id [OUT] specifies the
 * @return SYS_NO_ERROR_CODE if success, an application error code otherwise.
 */
typedef sys_error_code_t (*SLSensorsConfigF)(SensorManager_t *p_sm, uint16_t *p_active_sensor_id);

/**
 * ::SensorListener_t internal state.
 * It declares only the virtual table used to implement the inheritance.
 */
struct _SensorListener_t {
  /**
   * Specifies the listener interface to receive the sensor event.
   */
  IDataEventListener_t super;

  /**
   * Specifies the owner of the object. It is present for backward compatibility with
   * the ::IEventListener interface. It can be used as an application specific tag.
   */
  void *p_owner;

  /**
   * Function to set the defaukt configuration of teh sensor and to select the active sensor.
   */
  SLSensorsConfigF p_default_snsors_config;

  /**
   * Active sensor.
   */
  uint16_t active_sensor;
};



/* Public API declaration */
/**************************/

/**
 * Allocate an instance of SensorListener_t. This static allocator delegates the memory allocation
 * and it only initialize the actual type of the object.
 *
 * @return a pointer to the an object ::SensorListener_t if success,
 * or NULL if out of memory error occurs.
 */
SensorListener_t *SensorListenerStaticAlloc(SensorListener_t *p_instance);

/**
 * Initialize the interface SensorListener_t. It should be called after the object allocation and before using the object.
 *
 * @param _this [IN] specifies a pointer to the object.
 * @return SYS_NO_ERROR_CODE if success, an error code otherwise.
 */
sys_error_code_t SensorListenerInit(SensorListener_t *_this);

/**
 * Set the function to provide the default sensor configuration.
 * @param _this [IN] specifies a pointer to the object.
 * @param p_sensor_config_f [IN] specifies a pointer to the default configuration function. It can be NULL.
 * @return SYS_NO_ERROR_CODE if success, an error code otherwise.
 */
sys_error_code_t SensorListenerSetDefSensorConfifF(SensorListener_t *_this, SLSensorsConfigF p_sensor_config_f);

/**
 * Update the sensors configuration.
 *
 * @param _this [IN] specifies a pointer to the object.
 * @return SYS_NO_ERROR_CODE if success, an error code otherwise.
 */
sys_error_code_t SensorListenerUpdateSnsorConfiguration(SensorListener_t *_this);


/* Inline functions definition */
/*******************************/


#ifdef __cplusplus
}
#endif

#endif /* INC_SENSORLISTENER_H_ */

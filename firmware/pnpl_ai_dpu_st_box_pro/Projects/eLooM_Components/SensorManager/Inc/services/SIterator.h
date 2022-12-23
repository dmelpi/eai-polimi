/**
  ******************************************************************************
  * @file    SIterator.h
  * @author  SRA - MCD
 * @version 1.1.0
 * @date    10-Dec-2021
  *
  * @brief   Sensor Iterator declaration.
  *
  * A sensor iterator (::SIterator_t) allow to iterate trough a sensors
  * collection managed by a ::SensorManager_t.
  *
  * \code
  * SIterator_t iterator;
  * SIInit(&iterator, SMGetSensorManager());
  * while(SIHasNext(&iterator))
  * {
  *   uint16_t sensor_id = SINext(&iterator);
  *   // do something with the sensor
  *   ISourceObservable *p_sensor_observer = SMGetSensorObserver(sensor_id);
  *   float odr = ISourceGetODR(p_sensor_observer);
  *   SMSensorSetODR(sensor_id,  odr+1000);
  * }
  * \endcode
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
#ifndef SENSORMANAGER_SRC_SERVICES_SITERATOR_H_
#define SENSORMANAGER_SRC_SERVICES_SITERATOR_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "SensorManager.h"

#define SI_NULL_SENSOR_ID        0xFFFF

/**
  * Create a type name for ::_SIterator_t
  */
typedef struct _SIterator_t SIterator_t;

/**
  * Sensor Iterator internal state.
  */
struct _SIterator_t
{
  /**
    * Specifies the SensorManager instance containing the sensors collection.
    */
  SensorManager_t *p_sm;

  /**
    * Specifies the number of sensors in the collection.
    */
  uint16_t sensors_count;

  /**
    * Specifies the index of the next sensor
    */
  uint16_t sensor_idx;
};


/* Public functions declaration */
/********************************/

/**
  *  Initialize the iterator based on a sensor manager instance.
  *
  * @param _this [IN] specifies an ::SIterator_t object.
  * @param p_sm [IN] specifies a ::SensorManager_t instance.
  * @return SYS_NO_ERROR_CODE if success, an error code otherwise.
  */
sys_error_code_t SIInit(SIterator_t *_this, SensorManager_t *p_sm);

/**
  * Return true if the iteration has more elements.
  * In other words, returns true if next() would return an element rather than NULL.
  *
  * @param _this [IN] specifies an ::SIterator_t object.
  * @return true if the iteration has more elements, false otherwise
  */
bool SIHasNext(SIterator_t *_this);

/**
  * Return the sensor id of the next sensor in the iteration.
  *
  * @param _this [IN] specifies an ::SIterator_t object.
  * @return the sensor id of the next sensor in the iteration or SI_NULL_SENSOR
  */
uint16_t SINext(SIterator_t *_this);

#ifdef __cplusplus
}
#endif

#endif /* SENSORMANAGER_SRC_SERVICES_SITERATOR_H_ */

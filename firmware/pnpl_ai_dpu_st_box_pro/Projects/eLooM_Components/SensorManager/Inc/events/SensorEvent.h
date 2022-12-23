/**
  ******************************************************************************
  * @file    SensorEvent.h
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
#ifndef INCLUDE_EVENTS_SENSOREVENT_H_
#define INCLUDE_EVENTS_ISENSOREVENT_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "events/IEvent.h"
#include "services/em_data_format.h"


/**
  * Create  type name for _SensorEvent.
  */
typedef struct _SensorEvent SensorEvent;

/**
  * Specifies a generic Event. Each event has a pointer to the ::IEventSrc object
  * that has generated the event.
  */
struct _SensorEvent
{
  /**
    * Specifies a pointer to the Event Source that generated the event.
    */
  IEvent super;

  /**
    * Specify the sensor data normalized according to the internal data format.
    */
  const EMData_t *p_data;

  /**
    * Timestamp
    */
  double fTimeStamp;

  /**
    * Specifies the sensor ID.
    */
  uint16_t sensor_id;
};


// Public API declaration
//***********************

/**
  * Initialize the interface ISensorEvent. It should be called after the object allocation and before using the object.
  *
  * @param _this [IN] specifies a pointer to the object.
  * @return SYS_NO_ERROR_CODE if success, an error code otherwise.
  */

/**
  *
  * @param _this [IN] Initialize the interface ISensorEvent. It should be called after the object allocation and before using the object.
  * @param pSource [IN] specifies the source of the event.
  * @param pData [IN] specifies the buffer containing the raw data form the sensor.
  * @param nDataSize [IN] specifies the size in byt of the data coming from the sensor.
  * @param fTimeStamp [IN] specifies the timestamp of the data.
  * @param nSensorID [IN] specifies the ID of the sensor in the sensor DB.
  * @param nSubSensorID [IN] specifies the ID of the subsensor.
  * @return SYS_NO_ERROR_CODE if success, an error code otherwise.
  */
static inline sys_error_code_t SensorEventInit(IEvent *_this, const IEventSrc *pSource, const EMData_t *p_data, double timestamp, uint16_t sensor_id);


// Inline functions definition
// ***************************

SYS_DEFINE_STATIC_INLINE
sys_error_code_t SensorEventInit(IEvent *_this, const IEventSrc *pSource, const EMData_t *p_data, double timestamp, uint16_t sensor_id)
{
  assert_param(_this != NULL);
  SensorEvent *p_obj = (SensorEvent *)_this;

  IEventInit(_this, pSource);
  p_obj->p_data = p_data;
  p_obj->fTimeStamp = timestamp;
  p_obj->sensor_id = sensor_id;

  return SYS_NO_ERROR_CODE;
}

#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_EVENTS_SENSOREVENT_H_ */

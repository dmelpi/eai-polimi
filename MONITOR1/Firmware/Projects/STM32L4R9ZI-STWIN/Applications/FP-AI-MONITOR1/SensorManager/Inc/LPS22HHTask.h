/**
 ******************************************************************************
 * @file    LPS22HHTask.h
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
#ifndef LPS22HHTASK_H_
#define LPS22HHTASK_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "services/systp.h"
#include "services/syserror.h"
#include "services/AManagedTaskEx.h"
#include "services/AManagedTaskEx_vtbl.h"
#include "I2CBusIF.h"
#include "lps22hh_reg.h"
#include "events/SensorEventSrc.h"
#include "events/SensorEventSrc_vtbl.h"
#include "ISensor.h"
#include "ISensor_vtbl.h"
#include "queue.h"


#define LPS22HH_CFG_MAX_LISTENERS         2

/**
 * Create a type name for _LPS22HHTask.
 */
typedef struct _LPS22HHTask LPS22HHTask;


// Public API declaration
//***********************

/**
 * Get the ISourceObservable interface for the accelerometer.
 * @param _this [IN] specifies a pointer to a task object.
 * @return a pointer to the generic object ::ISourceObservable if success,
 * or NULL if out of memory error occurs.
 */
ISourceObservable *LPS22HHTaskGetTempSensorIF(LPS22HHTask *_this);

/**
 * Get the ISourceObservable interface for the gyroscope.
 * @param _this [IN] specifies a pointer to a task object.
 * @return a pointer to the generic object ::ISourceObservable if success,
 * or NULL if out of memory error occurs.
 */
ISourceObservable *LPS22HHTaskGetPressSensorIF(LPS22HHTask *_this);

/**
 * Allocate an instance of LPS22HHTask.
 *
 * @return a pointer to the generic object ::AManagedTaskEx if success,
 * or NULL if out of memory error occurs.
 */
AManagedTaskEx *LPS22HHTaskAlloc(void);

/**
 * Get the SPI interface for the sensor task.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return a pointer to the SPI interface of the sensor.
 */
I2CBusIF *LPS22HHTaskGetSensorIF(LPS22HHTask *_this);

/**
 * Get the ::IEventSrc interface for the sensor task.
 * @param _this [IN] specifies a pointer to a task object.
 * @return a pointer to the ::IEventSrc interface of the sensor.
 */
IEventSrc *LPS22HHTaskGetTempEventSrcIF(LPS22HHTask *_this);

/**
 * Get the ::IEventSrc interface for the sensor task.
 * @param _this [IN] specifies a pointer to a task object.
 * @return a pointer to the ::IEventSrc interface of the sensor.
 */
IEventSrc *LPS22HHTaskGetPressEventSrcIF(LPS22HHTask *_this);

// Inline functions definition
// ***************************


#ifdef __cplusplus
}
#endif

#endif /* LPS22HHTASK_H_ */

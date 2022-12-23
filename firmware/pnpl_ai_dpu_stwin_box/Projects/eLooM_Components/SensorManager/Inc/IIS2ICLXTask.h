/**
 ******************************************************************************
 * @file    IIS2ICLXTask.h
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
#ifndef IIS2ICLXTASK_H_
#define IIS2ICLXTASK_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "services/AManagedTaskEx.h"
#include "services/AManagedTaskEx_vtbl.h"
#include "ABusIF.h"
#include "events/SensorEventSrc.h"
#include "events/SensorEventSrc_vtbl.h"
#include "ISensor.h"
#include "ISensor_vtbl.h"

//TODO: STF.Begin - where should these be defined ?
#define IIS2ICLX_MAX_DRDY_PERIOD           (1.0)
#ifndef IIS2ICLX_MAX_WTM_LEVEL
#define IIS2ICLX_MAX_WTM_LEVEL             (256)
#endif
#define IIS2ICLX_MIN_WTM_LEVEL             (16)
#define IIS2ICLX_MAX_SAMPLES_PER_IT        (IIS2ICLX_MAX_WTM_LEVEL)
//STF.End

#define IIS2ICLX_CFG_MAX_LISTENERS           2

/**
 * Create  type name for _IIS2ICLXTask.
 */
typedef struct _IIS2ICLXTask IIS2ICLXTask;


// Public API declaration
//***********************

/**
 * Get the ISourceObservable interface for the accelerometer.
 * @param _this [IN] specifies a pointer to a task object.
 * @return a pointer to the generic object ::ISourceObservable if success,
 * or NULL if out of memory error occurs.
 */
ISourceObservable *IIS2ICLXTaskGetAccSensorIF(IIS2ICLXTask *_this);

/**
 * Allocate an instance of IIS2ICLXTask.
 *
 * @param pIRQConfig [IN] specifies a ::MX_GPIOParams_t instance declared in the mx.h file.
 *        It must be a GPIO connected to the IIS2ICLX sensor and configured in EXTI mode.
 *        If it is NULL then the sensor is configured in polling mode.
 * @param pCSConfig [IN] specifies a ::MX_GPIOParams_t instance declared in the mx.h file.
 *        It must be a GPIO identifying the SPI CS Pin.
 * @return a pointer to the generic object ::AManagedTaskEx if success,
 * or NULL if out of memory error occurs.
 */
AManagedTaskEx *IIS2ICLXTaskAlloc(const void *pIRQConfig, const void *pCSConfig);

/**
 * Get the Bus interface for the sensor task.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return a pointer to the Bus interface of the sensor.
 */
ABusIF *IIS2ICLXTaskGetSensorIF(IIS2ICLXTask *_this);

/**
 * Get the ::IEventSrc interface for the sensor task.
 * @param _this [IN] specifies a pointer to a task object.
 * @return a pointer to the ::IEventSrc interface of the sensor.
 */
IEventSrc *IIS2ICLXTaskGetEventSrcIF(IIS2ICLXTask *_this);


// Inline functions definition
// ***************************


#ifdef __cplusplus
}
#endif

#endif /* IIS2ICLXTASK_H_ */

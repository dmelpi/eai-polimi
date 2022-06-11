/**
 ******************************************************************************
 * @file    IIS2DLPCTask.h
 * @author  SRA - MCD
 * @version 1.0.0
 * @date    30-Jul-2021
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
#ifndef IIS2DLPCTASK_H_
#define IIS2DLPCTASK_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "services/systp.h"
#include "services/syserror.h"
#include "services/AManagedTaskEx.h"
#include "services/AManagedTaskEx_vtbl.h"
#include "ABusIF.h"
#include "events/SensorEventSrc.h"
#include "events/SensorEventSrc_vtbl.h"
#include "ISensor.h"
#include "ISensor_vtbl.h"

#define IIS2DLPC_MAX_DRDY_PERIOD           (1.0)    /* seconds */

#ifndef IIS2DLPC_MAX_WTM_LEVEL
#define IIS2DLPC_MAX_WTM_LEVEL             (16)    /* samples */
#endif

#define IIS2DLPC_CFG_MAX_LISTENERS         2

/**
 * Create  type name for _IIS2DLPCTask.
 */
typedef struct _IIS2DLPCTask IIS2DLPCTask;

// Public API declaration
//***********************

/**
 * Get the ISourceObserver interface for the accelerometer.
 * @param _this [IN] specifies a pointer to a task object.
 * @return a pointer to the generic object ::ISourceObserver if success,
 * or NULL if out of memory error occurs.
 */
ISourceObservable* IIS2DLPCTaskGetAccSensorIF(IIS2DLPCTask *_this);

/**
 * Allocate an instance of IIS3DWBTask.
 *
 * @param pIRQConfig [IN] specifies a ::MX_GPIOParams_t instance declared in the mx.h file.
 *        It must be a GPIO connected to the IIS2DLPC sensor and configured in EXTI mode.
 *        If it is NULL then the sensor is configured in polling mode.
 * @param pCSConfig [IN] specifies a ::MX_GPIOParams_t instance declared in the mx.h file.
 *        It must be a GPIO identifying the SPI CS Pin.
 * @param eBusType [IN] specifies the bus type (SPI, I2C...).
 * @return a pointer to the generic object ::AManagedTaskEx if success,
 * or NULL if out of memory error occurs.
 */
AManagedTaskEx* IIS2DLPCTaskAlloc(const void *pIRQConfig, const void *pCSConfig, EBusType eBusType);

/**
 * Get the Bus interface for the sensor task.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return a pointer to the Bus interface of the sensor.
 */
ABusIF* IIS2DLPCTaskGetSensorIF(IIS2DLPCTask *_this);

/**
 * Get the ::IEventSrc interface for the sensor task.
 * @param _this [IN] specifies a pointer to a task object.
 * @return a pointer to the ::IEventSrc interface of the sensor.
 */
IEventSrc* IIS2DLPCTaskGetEventSrcIF(IIS2DLPCTask *_this);

// Inline functions definition
// ***************************

#ifdef __cplusplus
}
#endif

#endif /* IIS2DLPCTASK_H_ */

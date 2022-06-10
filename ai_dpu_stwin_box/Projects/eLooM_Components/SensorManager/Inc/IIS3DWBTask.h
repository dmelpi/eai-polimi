/**
 ******************************************************************************
 * @file    IIS3DWBTask.h
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
#ifndef IIS3DWBTASK_H_
#define IIS3DWBTASK_H_

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
#define IIS3DWB_MAX_DRDY_PERIOD           (1.0)
#ifndef IIS3DWB_MAX_WTM_LEVEL
#define IIS3DWB_MAX_WTM_LEVEL             (256)
#endif
#define IIS3DWB_MIN_WTM_LEVEL             (16)
#define IIS3DWB_MAX_SAMPLES_PER_IT        (IIS3DWB_MAX_WTM_LEVEL)
//STF.End

#define IIS3DWB_CFG_MAX_LISTENERS           2

/**
 * Create  type name for _IIS3DWBTask.
 */
typedef struct _IIS3DWBTask IIS3DWBTask;


// Public API declaration
//***********************

/**
 * Get the ISourceObservable interface for the accelerometer.
 * @param _this [IN] specifies a pointer to a task object.
 * @return a pointer to the generic object ::ISourceObservable if success,
 * or NULL if out of memory error occurs.
 */
ISourceObservable *IIS3DWBTaskGetAccSensorIF(IIS3DWBTask *_this);

/**
 * Allocate an instance of IIS3DWBTask.
 *
 * @param pIRQConfig [IN] specifies a ::MX_GPIOParams_t instance declared in the mx.h file.
 *        It must be a GPIO connected to the IIS3DWB sensor and configured in EXTI mode.
 *        If it is NULL then the sensor is configured in polling mode.
 * @param pCSConfig [IN] specifies a ::MX_GPIOParams_t instance declared in the mx.h file.
 *        It must be a GPIO identifying the SPI CS Pin.
 * @param eBusType [IN] specifies the bus type (SPI, I2C...).
 * @return a pointer to the generic object ::AManagedTaskEx if success,
 * or NULL if out of memory error occurs.
 */
AManagedTaskEx *IIS3DWBTaskAlloc(const void *pIRQConfig, const void *pCSConfig, EBusType eBusType);

/**
 * Get the SPI interface for the sensor task.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return a pointer to the SPI interface of the sensor.
 */
ABusIF *IIS3DWBTaskGetSensorIF(IIS3DWBTask *_this);

/**
 * Get the ::IEventSrc interface for the sensor task.
 * @param _this [IN] specifies a pointer to a task object.
 * @return a pointer to the ::IEventSrc interface of the sensor.
 */
IEventSrc *IIS3DWBTaskGetEventSrcIF(IIS3DWBTask *_this);


// Inline functions definition
// ***************************


#ifdef __cplusplus
}
#endif

#endif /* IIS3DWBTASK_H_ */

/**
 ******************************************************************************
 * @file    ISM330DHCXTask.h
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
#ifndef ISM330DHCXTASK_H_
#define ISM330DHCXTASK_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "services/systp.h"
#include "services/syserror.h"
#include "services/AManagedTaskEx.h"
#include "services/AManagedTaskEx_vtbl.h"
#include "SPIBusIF.h"
#include "ism330dhcx_reg.h"
#include "events/SensorEventSrc.h"
#include "events/SensorEventSrc_vtbl.h"
#include "ISensor.h"
#include "ISensor_vtbl.h"
#include "queue.h"

//TODO: STF.Begin - where should these be defined ?
#define ISM330DHCX_MAX_DRDY_PERIOD           (1.0)    /* seconds */

#ifndef ISM330DHCX_MAX_WTM_LEVEL
#define ISM330DHCX_MAX_WTM_LEVEL             (256)    /* samples */
#endif

#define ISM330DHCX_MIN_WTM_LEVEL             (16)     /* samples */
#define ISM330DHCX_MAX_SAMPLES_PER_IT        (ISM330DHCX_MAX_WTM_LEVEL)
//STF.End

#define ISM330DHCX_CFG_MAX_LISTENERS         2

/**
 * Create a type name for _ISM330DHCXTask.
 */
typedef struct _ISM330DHCXTask ISM330DHCXTask;


// Public API declaration
//***********************

/**
 * Get the ISourceObservable interface for the accelerometer.
 * @param _this [IN] specifies a pointer to a task object.
 * @return a pointer to the generic object ::ISourceObservable if success,
 * or NULL if out of memory error occurs.
 */
ISourceObservable *ISM330DHCXTaskGetAccSensorIF(ISM330DHCXTask *_this);

/**
 * Get the ISourceObservable interface for the gyroscope.
 * @param _this [IN] specifies a pointer to a task object.
 * @return a pointer to the generic object ::ISourceObservable if success,
 * or NULL if out of memory error occurs.
 */
ISourceObservable *ISM330DHCXTaskGetGyroSensorIF(ISM330DHCXTask *_this);

/**
 * Allocate an instance of ISM330DHCXTask.
 *
 * @return a pointer to the generic object ::AManagedTaskEx if success,
 * or NULL if out of memory error occurs.
 */
AManagedTaskEx *ISM330DHCXTaskAlloc(void);

/**
 * Get the SPI interface for the sensor task.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return a pointer to the SPI interface of the sensor.
 */
SPIBusIF *ISM330DHCXTaskGetSensorIF(ISM330DHCXTask *_this);

/**
 * Get the ::IEventSrc interface for the sensor task.
 * @param _this [IN] specifies a pointer to a task object.
 * @return a pointer to the ::IEventSrc interface of the sensor.
 */
IEventSrc *ISM330DHCXTaskGetAccEventSrcIF(ISM330DHCXTask *_this);

/**
 * Get the ::IEventSrc interface for the sensor task.
 * @param _this [IN] specifies a pointer to a task object.
 * @return a pointer to the ::IEventSrc interface of the sensor.
 */
IEventSrc *ISM330DHCXTaskGetGyroEventSrcIF(ISM330DHCXTask *_this);

// Inline functions definition
// ***************************


#ifdef __cplusplus
}
#endif

#endif /* ISM330DHCXTASK_H_ */

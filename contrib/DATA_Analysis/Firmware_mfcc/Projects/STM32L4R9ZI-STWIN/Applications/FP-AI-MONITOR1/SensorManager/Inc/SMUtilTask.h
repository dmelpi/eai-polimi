/**
 ******************************************************************************
 * @file    SMUtilTask.h
 * @author  SRA - MCD
 * @version 1.0.0
 * @date    6-Sep-2021
 *
 * @brief   Utility API for the Sensor Manager module.
 *
 * This utility module of the Sensor Manager is in charge of:
 * - Generate the timestamp
 *
 * It uses an hardware timer.
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
#ifndef UTILTASK_H_
#define UTILTASK_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "services/systp.h"
#include "services/syserror.h"
#include "drivers/SMUtilityDriver.h"


/* Public API declaration */
/**************************/

/**
 * The same as SMUtilTaskGetTimeStamp() but without a task object. It implicitly uses the only
 * instance of ::SMUtilTask.
 *
 * @return the system time stamp in tick.
 */
uint32_t SMUtilGetTimeStamp(void);

/**
 * Get the instance of ::SMUtilityDriver.
 *
 * @return a pointer to the application ::SMUtilityDriver
 */
__weak SMUtilityDriver_t *GetSMUtilityDriver(void);


// Inline functions definition
// ***************************


#ifdef __cplusplus
}
#endif

#endif /* UTILTASK_H_ */

/**
 ******************************************************************************
 * @file    sysconfig.h
 * @author  STMicroelectronics - AIS - MCD Team
 * @version $Version$
 * @date    $Date$
 * @brief   Global System configuration file
 *
 * This file include some configuration parameters grouped here for user
 * convenience. This file override the default configuration value, and it is
 * used in the "Preinclude file" section of the "compiler > prepocessor"
 * options.
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

#ifndef SYSCONFIG_H_
#define SYSCONFIG_H_

/* Sensor Manager PIN configuration for the STWIN board */
#include "SMPinConfig.h"


// Drivers configuration
// *********************

// Other hardware configuration
// ****************************

#define SYS_DBG_AUTO_START_TA4                    0

// Services configuration
// **********************

// files syslowpower.h, SysDefPowerModeHelper.c
#define SYS_CFG_USE_DEFAULT_PM_HELPER             0
#define SYS_CFG_DEF_PM_HELPER_STANDBY             0  ///< if defined to 1 then the MCU goes in STANDBY mode when the system enters in SLEEP_1.

// file CircularBuffer.h
#define CB_HEAP_ALLOC                             pvPortMalloc
#define CB_HEAP_FREE                              vPortFree


// Tasks configuration
// *******************

// file IManagedTask.h
#define MT_ALLOWED_ERROR_COUNT                    0x2

// file sysinit.c
#define INIT_TASK_CFG_ENABLE_BOOT_IF              0
#define INIT_TASK_CFG_STACK_SIZE                  (configMINIMAL_STACK_SIZE*4)

// file HelloWorldTask.c
// uncomment the following lines to change the task common parameters
#define HW_TASK_CFG_STACK_DEPTH                   ((configMINIMAL_STACK_SIZE*2) + (configMINIMAL_STACK_SIZE/2))
#define HW_TASK_CFG_PRIORITY                      (tskIDLE_PRIORITY+1)

// file UtilTask.c
#define UTIL_TASK_CFG_STACK_DEPTH                 ((configMINIMAL_STACK_SIZE*3) + (0*configMINIMAL_STACK_SIZE/2))
#define UTIL_TASK_CFG_PRIORITY                    (tskIDLE_PRIORITY+1)


// SensorManager configuration

// file SensorManager.h
#define COM_MAX_SENSORS                           2


// file HTS211Task.c
#define HTS221_TASK_CFG_STACK_DEPTH               ((configMINIMAL_STACK_SIZE*5) + (0*configMINIMAL_STACK_SIZE/2))
#define HTS221_TASK_CFG_PRIORITY                  (tskIDLE_PRIORITY+4)

// file I2CBusTask.c
#define I2CBUS_TASK_CFG_STACK_DEPTH               ((configMINIMAL_STACK_SIZE*2) + (configMINIMAL_STACK_SIZE/2))
#define I2CBUS_TASK_CFG_PRIORITY                  (tskIDLE_PRIORITY+4)


#endif /* SYSCONFIG_H_ */

/**
 ******************************************************************************
 * @file    sysconfig.h
 * @author  STMicroelectronics - AIS - MCD Team
 * @version V1.0.0
 * @date    15-September-2021
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


// Tasks configuration
// *******************

// file IManagedTask.h
#define MT_ALLOWED_ERROR_COUNT                    0x2

// file sysinit.c
#define INIT_TASK_CFG_ENABLE_BOOT_IF              0
#define INIT_TASK_CFG_STACK_SIZE                  (configMINIMAL_STACK_SIZE*6)

// file HelloWorldTask.c
// uncomment the following lines to change the task common parameters
#define HW_TASK_CFG_STACK_DEPTH                   360
#define HW_TASK_CFG_PRIORITY                      (tskIDLE_PRIORITY+1)

// file NeaiTask.c
#define NAI_TASK_CFG_STACK_DEPTH                  (configMINIMAL_STACK_SIZE*4)
#define NAI_TASK_CFG_PRIORITY                     (tskIDLE_PRIORITY+2)

// file AITask.c
#define AI_TASK_CFG_STACK_DEPTH                   (configMINIMAL_STACK_SIZE*40)
#define AI_TASK_CFG_PRIORITY                      (tskIDLE_PRIORITY+2)

// file AppController.c
#define CTRL_TASK_CFG_STACK_DEPTH                 (configMINIMAL_STACK_SIZE*12)
#define CTRL_TASK_CFG_PRIORITY                    (tskIDLE_PRIORITY+3)

// file UtilTask.c
#define UTIL_TASK_CFG_STACK_DEPTH                 ((configMINIMAL_STACK_SIZE*2) + (configMINIMAL_STACK_SIZE/2))
#define UTIL_TASK_CFG_PRIORITY                    (tskIDLE_PRIORITY+1)

// SensorManager configuration

// file SensorManager.h
#define COM_MAX_SENSORS                           3

// file ISM330DHCXTask.c
#define ISM330DHCX_TASK_CFG_STACK_DEPTH           ((configMINIMAL_STACK_SIZE*3) + (configMINIMAL_STACK_SIZE/2))
#define ISM330DHCX_TASK_CFG_PRIORITY              (tskIDLE_PRIORITY+4)

// file IIS3DWBTask.c
#define IIS3DWB_TASK_CFG_STACK_DEPTH              (configMINIMAL_STACK_SIZE*3)
#define IIS3DWB_TASK_CFG_PRIORITY                 (tskIDLE_PRIORITY+4)

// file IMP23ABSUTask.c
#define IMP23ABSU_TASK_CFG_STACK_DEPTH            (configMINIMAL_STACK_SIZE*5)
#define IMP23ABSU_TASK_CFG_PRIORITY               (tskIDLE_PRIORITY+4)


// file SPIBusTask.c
#define SPIBUS_TASK_CFG_STACK_DEPTH               ((configMINIMAL_STACK_SIZE*2) + (configMINIMAL_STACK_SIZE/2))
#define SPIBUS_TASK_CFG_PRIORITY                  (tskIDLE_PRIORITY+4)

// DPU configuration

// file CircularBuffer.h
#define CB_HEAP_ALLOC                             pvPortMalloc
#define CB_HEAP_FREE                              vPortFree


#endif /* SYSCONFIG_H_ */

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

// file SysTimestamp.c
#define SYS_TS_CFG_ENABLE_SERVICE                 1
/**
 * Configuration parameter for the timer used for the eLooM timestamp service.
 * Valid value are:
 * - SYS_TS_USE_SW_TSDRIVER to use the RTOS tick
 * - The configuration structure for an hardware timer. It must be compatible with SysTimestamp_t type.
 */
//#define SYS_TS_CFG_TSDRIVER_PARAMS                &MX_TIM5InitParams
#define SYS_TS_CFG_TSDRIVER_PARAMS                SYS_TS_USE_SW_TSDRIVER

//#define SYS_TS_CFG_TSDRIVER_FREQ_HZ               SystemCoreClock ///< hardware timer clock frequency in Hz
#define SYS_TS_CFG_TSDRIVER_FREQ_HZ               configTICK_RATE_HZ ///< FreeRTOS clock frequency in Hz

// Tasks configuration
// *******************

// file IManagedTask.h
#define MT_ALLOWED_ERROR_COUNT                    (0x2)

// file sysinit.c
#define INIT_TASK_CFG_ENABLE_BOOT_IF              (0)
#define INIT_TASK_CFG_STACK_SIZE                  (configMINIMAL_STACK_SIZE*6)

// file AppTask.c
#define APP_TASK_CFG_STACK_DEPTH                  (360)
#define APP_TASK_CFG_PRIORITY                     (tskIDLE_PRIORITY+1)

// file UsbCdcTask.c
#define USB_CDC_TASK_CFG_STACK_DEPTH              (2000)
#define USB_CDC_TASK_CFG_PRIORITY                 (tskIDLE_PRIORITY+1)

// file ProcessTask.c
#define PRC_TASK_CFG_STACK_DEPTH                  (configMINIMAL_STACK_SIZE*120)
#define PRC_TASK_CFG_PRIORITY                     (tskIDLE_PRIORITY+2)

// SensorManager configuration

// file SensorManager.h
#define COM_MAX_SENSORS                           (3)

// file ISM330DHCXTask.c
#define ISM330DHCX_TASK_CFG_STACK_DEPTH           ((configMINIMAL_STACK_SIZE*4) + (configMINIMAL_STACK_SIZE/2))
#define ISM330DHCX_TASK_CFG_PRIORITY              (tskIDLE_PRIORITY+4)

// file IIS3DWBTask.c
#define IIS3DWB_TASK_CFG_STACK_DEPTH              ((configMINIMAL_STACK_SIZE*4) + (configMINIMAL_STACK_SIZE/2))
#define IIS3DWB_TASK_CFG_PRIORITY                 (tskIDLE_PRIORITY+4)

// file SPIBusTask.c
#define SPIBUS_TASK_CFG_STACK_DEPTH               ((configMINIMAL_STACK_SIZE*4) + (configMINIMAL_STACK_SIZE/2))
#define SPIBUS_TASK_CFG_PRIORITY                  (tskIDLE_PRIORITY+4)


// DPU configuration

// file ADPU.h
#define ADPU_CFG_MAX_SENSOR                       (COM_MAX_SENSORS)

// SensorStreamerCLI configuration

// file SCLIUtilTask.c
#define SCLIUTIL_TASK_CFG_STACK_DEPTH             ((configMINIMAL_STACK_SIZE*10) + (configMINIMAL_STACK_SIZE/2))
#define SCLIUTIL_TASK_CFG_PRIORITY                (tskIDLE_PRIORITY+1)


#endif /* SYSCONFIG_H_ */

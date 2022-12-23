/**
 ******************************************************************************
 * @file    sysconfig.h
 * @author  STMicroelectronics - ST-Korea - MCD Team
 * @version 1.2.0
 * @date    Jan 5, 2017
 * @brief   Global System configuration file
 *
 * This file include some configuration parameters grouped here for user
 * convenience. This file override the default configuration value, and it is
 * used in the "Preinclude file" section of the "compiler > prepocessor"
 * options.
 *
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
 *
 * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *        http://www.st.com/software_license_agreement_liberty_v2
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************
 */

#ifndef SYSCONFIG_H_
#define SYSCONFIG_H_

//#include "SMPinConfig.h"

// Drivers configuration
// *********************

// file NucleoDriver.c
// uncomment the following line to change the drive common parameters
//#define NUCLEO_DRV_CFG_IRQ_PRIORITY             13

// Other hardware configuration
// ****************************

#define SYS_DBG_AUTO_START_TA4                    0

// file SysTimestamp.c
#define SYS_TS_CFG_ENABLE_SERVICE                 1
/**
 * Configuration parameter for the timer used for the eLooM timestamp service.
 * Valid value are:
 * - SYS_TS_USE_SW_TSDRIVER to use the RTOS tick
 * - The configuration structure for an hardware timer. It must be compatible with SysTimestamp_t type.
 */
//#define SYS_TS_CFG_TSDRIVER_PARAMS      &MX_TIM3InitParams
#define SYS_TS_CFG_TSDRIVER_PARAMS                SYS_TS_USE_SW_TSDRIVER
//#define SYS_TS_CFG_TSDRIVER_FREQ_HZ     SystemCoreClock ///< hardware timer clock frequency in Hz
#define SYS_TS_CFG_TSDRIVER_FREQ_HZ               TX_TIMER_TICKS_PER_SECOND ///< ThreadX clock frequency in Hz



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
#define INIT_TASK_CFG_STACK_SIZE                  (TX_MINIMUM_STACK*10)
#define INIT_TASK_CFG_HEAP_SYZE                   (450*1024)


// HSDCore configuration

// file sensor_db.h
#define COM_MAX_SENSORS                           16

// file ISM330DHCXTask.c
#define ISM330DHCX_TASK_CFG_STACK_DEPTH           (TX_MINIMUM_STACK*8)
#define ISM330DHCX_TASK_CFG_PRIORITY              (4)

// file IIS3DWBTask.c
#define IIS3DWB_TASK_CFG_STACK_DEPTH              (TX_MINIMUM_STACK*8)
#define IIS3DWB_TASK_CFG_PRIORITY                 (4)

// file IIS2ICLXTask.c
#define IIS2ICLX_TASK_CFG_STACK_DEPTH              (TX_MINIMUM_STACK*8)
#define IIS2ICLX_TASK_CFG_PRIORITY                 (4)

// file IMP23ABSUTask.c
#define IMP23ABSU_TASK_CFG_STACK_DEPTH            (TX_MINIMUM_STACK*4)
#define IMP23ABSU_TASK_CFG_PRIORITY               (4)

// file HTS221Task.c
//#define HTS221_TASK_CFG_STACK_DEPTH               (TX_MINIMUM_STACK*8)
//#define HTS221_TASK_CFG_PRIORITY                  (4)

// file STTS22HTask.c
//#define STTS22H_TASK_CFG_STACK_DEPTH               (TX_MINIMUM_STACK*8)
//#define STTS22H_TASK_CFG_PRIORITY                  (4)
#define STTS22H_TASK_CFG_I2C_ADDRESS              STTS22H_I2C_ADD_H

// file LPS22DFTask.c
//#define LPS22DF_TASK_CFG_STACK_DEPTH              (TX_MINIMUM_STACK*10)
//#define LPS22DF_TASK_CFG_PRIORITY                 (4)
#define LPS22DF_TASK_CFG_I2C_ADDRESS              LPS22DF_I2C_ADD_H

// file IIS2MDCTask.c
#define IIS2MDC_TASK_CFG_STACK_DEPTH              (TX_MINIMUM_STACK*8)
#define IIS2MDC_TASK_CFG_PRIORITY                 (4)

// file IMP34DT05Task.c
#define IMP34DT05_TASK_CFG_STACK_DEPTH            (TX_MINIMUM_STACK*4)
#define IMP34DT05_TASK_CFG_PRIORITY               (4)

// file IIS2DLCPTask.c
#define IIS2DLPC_TASK_CFG_STACK_DEPTH            (TX_MINIMUM_STACK*8)
#define IIS2DLPC_TASK_CFG_PRIORITY               (4)

// file I2CBusTask.c
#define I2CBUS_TASK_CFG_STACK_DEPTH               (TX_MINIMUM_STACK*7)
#define I2CBUS_TASK_CFG_PRIORITY                  (4)

// file SPIBusTask.c
#define SPIBUS_TASK_CFG_STACK_DEPTH               (TX_MINIMUM_STACK*10)
#define SPIBUS_TASK_CFG_PRIORITY                  (4)

// file UtilTask.c
#define UTIL_TASK_CFG_STACK_DEPTH               (TX_MINIMUM_STACK*9)
#define UTIL_TASK_CFG_PRIORITY                  (14)

// App configuration

// file HelloWorldTask.c
// uncomment the following lines to change the task common parameters
#define HW_TASK_CFG_STACK_DEPTH                    (TX_MINIMUM_STACK*7)
#define HW_TASK_CFG_PRIORITY                       (14)

#endif /* SYSCONFIG_H_ */

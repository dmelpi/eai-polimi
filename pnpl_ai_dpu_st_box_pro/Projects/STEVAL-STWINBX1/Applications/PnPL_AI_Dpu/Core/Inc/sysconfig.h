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

#include "SMPinConfig.h"

// Drivers configuration
// *********************

#define SYS_TP_RTOS_THREADX

// Board ID for STWIN.box
#define BOARD_ID                                  0x0E

// Generic FW ID
#define FW_ID                                     0xFF


// Other hardware configuration
// ****************************

#define SYS_DBG_AUTO_START_TA4                    0

// Services configuration
// **********************

// files syslowpower.h, SysDefPowerModeHelper.c
#define SYS_CFG_USE_DEFAULT_PM_HELPER             0
#define SYS_CFG_DEF_PM_HELPER_STANDBY             0  ///< if defined to 1 then the MCU goes in STANDBY mode when the system enters in SLEEP_1.

// file SysTimestamp.c
#define SYS_TS_CFG_ENABLE_SERVICE 1
/**
* Configuration parameter for the timer used for the eLooM timestamp service.
* Valid value are:
* - SYS_TS_USE_SW_TSDRIVER to use the RTOS tick
* - The configuration structure for an hardware timer. It must be compatible with SysTimestamp_t type.
*/
//#define SYS_TS_CFG_TSDRIVER_PARAMS &MX_TIM3InitParams
#define SYS_TS_CFG_TSDRIVER_PARAMS SYS_TS_USE_SW_TSDRIVER

//#define SYS_TS_CFG_TSDRIVER_FREQ_HZ SystemCoreClock ///< hardware timer clock frequency in Hz
#define SYS_TS_CFG_TSDRIVER_FREQ_HZ TX_TIMER_TICKS_PER_SECOND ///< ThreadX clock frequency in Hz

// Tasks configuration
// *******************

// file IManagedTask.h
#define MT_ALLOWED_ERROR_COUNT                    0x2

// file sysinit.c
#define INIT_TASK_CFG_ENABLE_BOOT_IF              0
#define INIT_TASK_CFG_STACK_SIZE                  (TX_MINIMUM_STACK*11)
#define INIT_TASK_CFG_HEAP_SYZE                   (250*1024)

// file sensor_db.h
#define COM_MAX_SENSORS                           4

// file ADPU.h
#define ADPU_CFG_MAX_SENSOR                       COM_MAX_SENSORS

// file ISM330DHCXTask.c
#define ISM330DHCX_TASK_CFG_STACK_DEPTH           (TX_MINIMUM_STACK*12)
#define ISM330DHCX_TASK_CFG_PRIORITY              (8)

// file IIS3DWBTask.c
#define IIS3DWB_TASK_CFG_STACK_DEPTH              (TX_MINIMUM_STACK*7)
#define IIS3DWB_TASK_CFG_PRIORITY                 (8)

// file IMP34DT05Task.c
#define IMP34DT05_TASK_CFG_STACK_DEPTH            (TX_MINIMUM_STACK*20)
#define IMP34DT05_TASK_CFG_PRIORITY               (8)

// file SPIBusTask.c
#define SPIBUS_TASK_CFG_STACK_DEPTH               (TX_MINIMUM_STACK*4)
#define SPIBUS_TASK_CFG_PRIORITY                  (4)

// file UsbCdcTask.c
#define USB_CDC_TASK_CFG_STACK_DEPTH              (TX_MINIMUM_STACK*10)
#define USB_CDC_TASK_CFG_PRIORITY                 (7)

// file ProcessTask.c
#define PRC_TASK_CFG_STACK_DEPTH                  (TX_MINIMUM_STACK*100)
#define PRC_TASK_CFG_PRIORITY                     (6)

// file AppTask.c
#define APP_TASK_CFG_STACK_DEPTH                  (TX_MINIMUM_STACK*40)
#define APP_TASK_CFG_PRIORITY                     (TX_MAX_PRIORITIES-2)

// OS memory management.
#include "services/sysmem.h"


#endif /* SYSCONFIG_H_ */

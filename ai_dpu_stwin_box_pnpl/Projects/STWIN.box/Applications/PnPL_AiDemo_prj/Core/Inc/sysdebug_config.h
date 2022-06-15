/**
 ******************************************************************************
 * @file    sysdebug_config.h
 * @author  STMicroelectronics - ST-Korea - MCD Team
 * @version 1.2.0
 * @date    Oct 10, 2016
 * @brief   Configure the debug log functionality
 *
 * Each logic module of the application should define a DEBUG control byte
 * used to turn on/off the log for the module.
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

#ifndef SYSDEBUG_CONFIG_H_
#define SYSDEBUG_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#define SYS_DBG_LEVEL                      SYS_DBG_LEVEL_VERBOSE /*!< set the level of the system log: all log messages with minor level are discharged. */

/* Example */
/*#define SYS_DBG_MODULE1     SYS_DBG_ON|GTS_DBG_HALT  ///< Module 1 debug control byte */
/*#define SYS_DBG_MODULE2     SYS_DBG_ON               ///< Module 2 debug control byte */


#define SYS_DBG_INIT                       SYS_DBG_ON                 ///< Init task debug control byte
#define SYS_DBG_DRIVERS                    SYS_DBG_OFF                ///< Drivers debug control byte
#define SYS_DBG_APP                        SYS_DBG_ON                ///< Generic Application debug control byte
#define SYS_DBG_APMH                       SYS_DBG_ON                 ///< Application Power Mode Helper debug control byte
#define SYS_DBG_HW                         SYS_DBG_ON                 ///< Hello World task debug control byte
#define SYS_DBG_SPIBUS                     SYS_DBG_OFF                 ///< SPIBus task debug control byte
#define SYS_DBG_I2CBUS                     SYS_DBG_ON                 ///< I2CBus task debug control byte
#define SYS_DBG_ISM330DHCX                 SYS_DBG_ON                 ///< ISM330DHCX sensor task debug control byte
#define SYS_DBG_IIS3DWB                    SYS_DBG_OFF                ///< IIS3DWB sensor task debug control byte
#define SYS_DBG_HTS221                     SYS_DBG_ON                 ///< HTS221 sensor task debug control byte
#define SYS_DBG_LPS22HH                    SYS_DBG_OFF                 ///< LPS22HH sensor task debug control byte
#define SYS_DBG_ENV                        SYS_DBG_OFF                 ///< ENV sensor task debug control byte
#define SYS_DBG_SMUTIL                     SYS_DBG_ON                 ///< Utility task debug control byte
#define SYS_DBG_SDC                        SYS_DBG_OFF                 ///< SDCARD task debug control byte
#define SYS_DBG_IMP23ABSU                  SYS_DBG_OFF                 ///< IMP23ABSU task debug control byte
#define SYS_DBG_IMP34DT05                  SYS_DBG_OFF                 ///< IMP34DT05 task debug control byte
#define SYS_DBG_IIS2MDC                    SYS_DBG_OFF                 ///< IIS2MDC task debug control byte
#define SYS_DBG_IIS2DH                     SYS_DBG_OFF                 ///< IIS2DH task debug control byte
#define SYS_DBG_STTS751                    SYS_DBG_OFF                 ///< STTS751 task debug control byte
#define SYS_DBG_UTIL                       SYS_DBG_ON                 ///< UTIL task debug control byte
#define SYS_DBG_PRC                        SYS_DBG_ON                 ///< Process task debug control byte
#define SYS_DBG_AI                         SYS_DBG_ON                 ///< CubeAI plus ML extension task debug control byte
#define SYS_DBG_USB                        SYS_DBG_ON                 ///< USB_CDC task debug control byte
#define SYS_DBG_SYSTS                      SYS_DBG_OFF                ///< TimeStamp service debug control byte

/* ODeV - hardware configuration for the debug services provided by the framework */
/**********************************************************************************/

#include "mx.h"

/* ODeV test point PINs */
//#define SYS_DBG_TP1_PORT                   SYS_DBG_TP1_GPIO_Port
//#define SYS_DBG_TP1_PIN                    SYS_DBG_TP1_Pin
//#define SYS_DBG_TP1_CLK_ENABLE             __HAL_RCC_GPIOG_CLK_ENABLE
//#define SYS_DBG_TP2_PORT                   SYS_DBG_TP2_GPIO_Port
//#define SYS_DBG_TP2_PIN                    SYS_DBG_TP2_Pin
//#define SYS_DBG_TP2_CLK_ENABLE             __HAL_RCC_GPIOG_CLK_ENABLE

/* ODeV DBG UART used for the system log */
extern UART_HandleTypeDef huart2;
void MX_USART2_UART_Init(void);

#define SYS_DBG_UART                       huart2
#define SYS_DBG_UART_INIT                  MX_USART2_UART_Init
#define SYS_DBG_UART_TIMEOUT_MS            5000

/* ODeV runtime statistic timer configuration for FreeRTOS */
extern TIM_HandleTypeDef htim6;
void MX_TIM6_Init(void);

#define SYS_DBG_TIM                        htim6
#define SYS_DBG_TIM_INIT                   MX_TIM6_Init
#define SYS_DBG_TIM_IRQ_N                  TIM6_DAC_IRQn
#define SYS_DBG_TIM_IRQ_HANDLER            TIM6_DACUNDER_IRQHandler
#endif /* SYS_DEBUG */


#ifdef __cplusplus
}
#endif


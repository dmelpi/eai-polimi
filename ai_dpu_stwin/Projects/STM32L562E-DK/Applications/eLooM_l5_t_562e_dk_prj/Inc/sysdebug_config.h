/**
 ******************************************************************************
 * @file    sysdebug_config.h
 * @author  STMicroelectronics - AIS - MCD Team
 * @version V1.0.0
 * @date    15-September-2021
 * @brief   Configure the debug log functionality
 *
 * Each logic module of the application should define a DEBUG control byte
 * used to turn on/off the log for the module.
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
#define SYS_DBG_APP                        SYS_DBG_OFF                ///< Generic Application debug control byte
#define SYS_DBG_APMH                       SYS_DBG_ON                 ///< Application Power Mode Helper debug control byte
#define SYS_DBG_HW                         SYS_DBG_ON                 ///< Hello World task debug control byte


/* eLooM - hardware configuration for the debug services provided by the framework */
/**********************************************************************************/

#include "mx.h"

/* eLooM test point PINs */
#define SYS_DBG_TP1_PORT                   SYS_DBG_TP1_GPIO_Port
#define SYS_DBG_TP1_PIN                    SYS_DBG_TP1_Pin
#define SYS_DBG_TP1_CLK_ENABLE             __HAL_RCC_GPIOG_CLK_ENABLE
#define SYS_DBG_TP2_PORT                   SYS_DBG_TP2_GPIO_Port
#define SYS_DBG_TP2_PIN                    SYS_DBG_TP2_Pin
#define SYS_DBG_TP2_CLK_ENABLE             __HAL_RCC_GPIOD_CLK_ENABLE

/* eLooM DBG UART used for the system log */
extern UART_HandleTypeDef huart1;
void MX_USART1_UART_Init(void);

#define SYS_DBG_UART                       huart1
#define SYS_DBG_UART_INIT                  MX_USART1_UART_Init
#define SYS_DBG_UART_TIMEOUT_MS            5000

/* eLooM runtime statistic timer configuration for FreeRTOS */
extern TIM_HandleTypeDef htim16;
void MX_TIM16_Init(void);

#define SYS_DBG_TIM                        htim16
#define SYS_DBG_TIM_INIT                   MX_TIM16_Init
#define SYS_DBG_TIM_IRQ_N                  TIM16_IRQn
#define SYS_DBG_TIM_IRQ_HANDLER            TIM16_IRQHandler
#endif /* SYS_DEBUG */


#ifdef __cplusplus
}
#endif


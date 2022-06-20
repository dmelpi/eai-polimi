/**
 ******************************************************************************
 * @file    mx.h
 * @author  STMicroelectronics - AIS - MCD Team
 * @version 1.0.0
 * @date    Jul 3, 2020
 *
 * @brief Project peripherals configuration files.
 *
 * This files declares the peripherals configuration parameters that can be used
 * in the Low Level API of an eLooM driver - IDrvInit(). It creates a link
 * between the initialization code generated by CubeMX (see the .ioc file
 * in the tools folder) and the eLooM framework.
 *
 * This example show how a managed task allocates an SPI driver that must be
 * connected to the SPI3 (that has been configured using CubeMX):
 * ~~~{.c}
 * p_task->p_driver = SPIMasterDriverAlloc();
 * if (p_task->p_driver != NULL)
 * {
 *   res = IDrvInit((IDriver*)p_task->p_driver, (void*)&MX_SPI3InitParams);
 *   if (SYS_IS_ERROR_CODE(res)) {
 *     SYS_DEBUGF(SYS_DBG_LEVEL_SEVERE, ("SPIBus task: error during driver initialization\r\n"));
 *   }
 * }
 * ~~~
 *
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  ******************************************************************************
  */
#ifndef INCLUDE_MX_MX_H_
#define INCLUDE_MX_MX_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32wbxx_hal.h"

/* Private defines -----------------------------------------------------------*/
#define SPI_IIS3DWB_CS_Pin GPIO_PIN_15
#define SPI_IIS3DWB_CS_GPIO_Port GPIOA
#define BUTTON_USER_Pin GPIO_PIN_13
#define BUTTON_USER_GPIO_Port GPIOB
#define BUTTON_USER_EXTI_IRQn EXTI15_10_IRQn
#define ISM330DHCX_INT1_Pin GPIO_PIN_4
#define ISM330DHCX_INT1_GPIO_Port GPIOE
#define ISM330DHCX_INT1_EXTI_IRQn EXTI4_IRQn
#define SPI_ISM330DHCX_CS_Pin GPIO_PIN_4
#define SPI_ISM330DHCX_CS_GPIO_Port GPIOA
#define LED_BLUE_Pin GPIO_PIN_1
#define LED_BLUE_GPIO_Port GPIOH
#define LED_GREEN_Pin GPIO_PIN_2
#define LED_GREEN_GPIO_Port GPIOD
#define IIS3DWB_INT1_Pin GPIO_PIN_3
#define IIS3DWB_INT1_GPIO_Port GPIOE
#define IIS3DWB_INT1_EXTI_IRQn EXTI3_IRQn
#define LED_RED_Pin GPIO_PIN_8
#define LED_RED_GPIO_Port GPIOD
/* PB1 is not connected, so TP2 are defined to NULL*/
/* TP1 is mapped to LED BLUE*/
#define SYS_DBG_TP1_Pin GPIO_PIN_1
#define SYS_DBG_TP1_GPIO_Port GPIOH
#define SYS_DBG_TP2_Pin GPIO_PIN_1
#define SYS_DBG_TP2_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */


/* Driver initialization parameters */
/************************************/

/**
 * TIM configuration parameters.
 */
typedef struct _MX_TIMParams_t
{
  TIM_HandleTypeDef *p_tim; /*!< HAL TIM handle */
  IRQn_Type irq_n; /*!< External interrupt number. */
  void (*p_mx_init_f)(void); /*!< MX TIM initialization function */
} MX_TIMParams_t;

/**
 * GPIO Configuration parameters.
 */
typedef struct _MX_GPIOParams_t
{
  void (*p_mx_init_f)(void); /*!< MX GPIO initialization function */
  IRQn_Type irq_n; /*!< External interrupt number. */
  uint32_t pin;
  GPIO_TypeDef *port;
} MX_GPIOParams_t;

/**
 * TIM5 configuration parameters.
 */
extern const MX_TIMParams_t MX_TIM2InitParams;

/**
 * PB13 configuration parameters.
 */
extern const MX_GPIOParams_t MX_GPIO_PB13InitParams;


#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_MX_MX_H_ */

/**
  ******************************************************************************
  * @file    stm32l4xx_it.c
  * @brief   Interrupt Service Routines.
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

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "stm32l4xx.h"
#include "FreeRTOS.h"
#include "task.h"
#include "drivers/EXTIPinMap.h"

// External variables
// *******************
extern DMA_HandleTypeDef hdma_spi3_rx;
extern DMA_HandleTypeDef hdma_spi3_tx;
extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim7;
extern TIM_HandleTypeDef htim16;
extern PCD_HandleTypeDef hpcd_USB_OTG_FS;


// Forward function declarations
// ****************************

extern void xPortSysTickHandler(void);

/**
 * Map one EXTI to n callback based on the GPIO PIN.
 */
static inline void ExtiDefISR(void);


// External variables
// ******************

EXTI_DECLARE_PIN2F_MAP()

// Private function definition
// ***************************

void ExtiDefISR() {
  EXTIPin2CallbckMap xMap = EXTI_GET_P2F_MAP();
  for (int i=0; xMap[i].pfCallback != NULL; i++) {
    if (__HAL_GPIO_EXTI_GET_IT(xMap[i].nPin)) {
      /* EXTI line interrupt detected */
      __HAL_GPIO_EXTI_CLEAR_IT(xMap[i].nPin);
      xMap[i].pfCallback(xMap[i].nPin);
    }
  }
}

/******************************************************************************/
/*            Cortex-M4 Processor Interruption and Exception Handlers         */
/******************************************************************************/

/**
 * @brief This function handles System tick timer.
 */
void SysTick_Handler(void)
{
#if (INCLUDE_xTaskGetSchedulerState  == 1 )
  if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
  {
#endif  /* INCLUDE_xTaskGetSchedulerState */
    xPortSysTickHandler();
#if (INCLUDE_xTaskGetSchedulerState  == 1 )
  }
#endif  /* INCLUDE_xTaskGetSchedulerState */
}

/******************************************************************************/
/* STM32L4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32l4xx.s).                    */
/******************************************************************************/

/**
 * @brief This function handles WWDG interrupt.
 */

void WWDG_IRQHandler(void) {
  while (1) {
    __NOP();
  }
}

/**
  * @brief This function handles DMA1 channel1 global interrupt.
  */
void DMA1_CH1_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel1_IRQn 0 */

  /* USER CODE END DMA1_Channel1_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_spi3_rx);
  /* USER CODE BEGIN DMA1_Channel1_IRQn 1 */

  /* USER CODE END DMA1_Channel1_IRQn 1 */
}

/**
  * @brief This function handles DMA1 channel2 global interrupt.
  */
void DMA1_CH2_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel2_IRQn 0 */

  /* USER CODE END DMA1_Channel2_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_spi3_tx);
  /* USER CODE BEGIN DMA1_Channel2_IRQn 1 */

  /* USER CODE END DMA1_Channel2_IRQn 1 */
}

/**
  * @brief This function handles EXTI line0 interrupt.
  */
void EXTI0_IRQHandler(void)
{
  ExtiDefISR();
}

/**
  * @brief This function handles EXTI line[9:5] interrupts.
  */
void EXTI9_5_IRQHandler(void)
{
  ExtiDefISR();
}

/**
  * @brief This function handles EXTI line[10:15] interrupts.
  */
void EXTI15_10_IRQHandler(void)
{
  ExtiDefISR();
}

/**
  * @brief This function handles TIM7 global interrupt.
  */
void TIM7_IRQHandler(void)
{
  /* USER CODE BEGIN TIM7_IRQn 0 */

  /* USER CODE END TIM7_IRQn 0 */
  HAL_TIM_IRQHandler(&htim7);
  /* USER CODE BEGIN TIM7_IRQn 1 */

  /* USER CODE END TIM7_IRQn 1 */
}

/**
  * @brief This function handles TIM1 update interrupt and TIM16 global interrupt.
  */
void TIM1_UP_TIM16_IRQHandler(void)
{
  /* USER CODE BEGIN TIM1_UP_TIM16_IRQn 0 */

  /* USER CODE END TIM1_UP_TIM16_IRQn 0 */
  HAL_TIM_IRQHandler(&htim16);
  /* USER CODE BEGIN TIM1_UP_TIM16_IRQn 1 */

  /* USER CODE END TIM1_UP_TIM16_IRQn 1 */
}

/**
  * @brief This function handles TIM5 global interrupt.
  */
void TIM5_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&htim5);
}

/**
  * @brief This function handles USB OTG FS global interrupt.
  */
void OTG_FS_IRQHandler(void)
{
  HAL_PCD_IRQHandler(&hpcd_USB_OTG_FS);
}

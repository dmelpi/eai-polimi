/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32u5xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32u5xx_it.h"
#include "drivers/EXTIPinMap.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
/**
 * Map one EXTI to n callback based on the GPIO PIN.
 */
static inline void ExtiDefISR(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern TIM_HandleTypeDef htim6;
extern DMA_HandleTypeDef handle_GPDMA1_Channel1;
extern DMA_HandleTypeDef handle_GPDMA1_Channel0;
extern SPI_HandleTypeDef hspi2;

EXTI_DECLARE_PIN2F_MAP()

/* USER CODE BEGIN EV */
void ExtiDefISR()
{
  EXTIPin2CallbckMap xMap = EXTI_GET_P2F_MAP();
  for(int i = 0; xMap[i].pfCallback != NULL; i++)
  {
    if(__HAL_GPIO_EXTI_GET_IT(xMap[i].nPin))
    {
      /* EXTI line interrupt detected */
      __HAL_GPIO_EXTI_CLEAR_IT(xMap[i].nPin);
      xMap[i].pfCallback(xMap[i].nPin);
    }
  }
}
/* USER CODE END EV */

/******************************************************************************/
/*           Cortex Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
  while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Prefetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/******************************************************************************/
/* STM32U5xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32u5xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles TIM6 global interrupt.
  */
void TIM6_IRQHandler(void)
{
  /* USER CODE BEGIN TIM6_IRQn 0 */

  /* USER CODE END TIM6_IRQn 0 */
  HAL_TIM_IRQHandler(&htim6);
  /* USER CODE BEGIN TIM6_IRQn 1 */

  /* USER CODE END TIM6_IRQn 1 */
}

/**
 * @brief This function handles EXTI Line0 interrupt.
 */
void EXTI0_IRQHandler(void)
{
  ExtiDefISR();
}

/**
 * @brief This function handles EXTI Line0 interrupt.
 */
void EXTI8_IRQHandler(void)
{
  ExtiDefISR();
}

/**
 * @brief This function handles EXTI Line0 interrupt.
 */
void EXTI15_IRQHandler(void)
{
  ExtiDefISR();
}



/**
 * @brief This function handles GPDMA1 Channel 0 global interrupt.
 */
void GPDMA1_Channel0_IRQHandler(void)
{
  /* USER CODE BEGIN GPDMA1_Channel0_IRQn 0 */

  /* USER CODE END GPDMA1_Channel0_IRQn 0 */
  HAL_DMA_IRQHandler(&handle_GPDMA1_Channel0);
  /* USER CODE BEGIN GPDMA1_Channel0_IRQn 1 */

  /* USER CODE END GPDMA1_Channel0_IRQn 1 */
}

/**
 * @brief This function handles GPDMA1 Channel 1 global interrupt.
 */
void GPDMA1_Channel1_IRQHandler(void)
{
  /* USER CODE BEGIN GPDMA1_Channel1_IRQn 0 */

  /* USER CODE END GPDMA1_Channel1_IRQn 0 */
  HAL_DMA_IRQHandler(&handle_GPDMA1_Channel1);
  /* USER CODE BEGIN GPDMA1_Channel1_IRQn 1 */

  /* USER CODE END GPDMA1_Channel1_IRQn 1 */
}


void SPI2_IRQHandler(void)
{
  HAL_SPI_IRQHandler(&hspi2);
}


///**
// * @brief This function handles GPDMA1 Channel 2 global interrupt.
// */
//void GPDMA1_Channel2_IRQHandler(void)
//{
//  /* USER CODE BEGIN GPDMA1_Channel2_IRQn 0 */
//
//  /* USER CODE END GPDMA1_Channel2_IRQn 0 */
//  HAL_DMA_IRQHandler(&handle_GPDMA1_Channel2);
//  /* USER CODE BEGIN GPDMA1_Channel2_IRQn 1 */
//
//  /* USER CODE END GPDMA1_Channel2_IRQn 1 */
//}
//
///**
// * @brief This function handles GPDMA1 Channel 3 global interrupt.
// */
//void GPDMA1_Channel3_IRQHandler(void)
//{
//  /* USER CODE BEGIN GPDMA1_Channel3_IRQn 0 */
//
//  /* USER CODE END GPDMA1_Channel3_IRQn 0 */
//  HAL_DMA_IRQHandler(&handle_GPDMA1_Channel3);
//  /* USER CODE BEGIN GPDMA1_Channel3_IRQn 1 */
//
//  /* USER CODE END GPDMA1_Channel3_IRQn 1 */
//}
//
///**
// * @brief This function handles GPDMA1 Channel 4 global interrupt.
// */
//void GPDMA1_Channel4_IRQHandler(void)
//{
//  /* USER CODE BEGIN GPDMA1_Channel4_IRQn 0 */
//
//  /* USER CODE END GPDMA1_Channel4_IRQn 0 */
//  HAL_DMA_IRQHandler(&handle_GPDMA1_Channel4);
//  /* USER CODE BEGIN GPDMA1_Channel4_IRQn 1 */
//
//  /* USER CODE END GPDMA1_Channel4_IRQn 1 */
//}
//
///**
// * @brief This function handles GPDMA1 Channel 5 global interrupt.
// */
//void GPDMA1_Channel5_IRQHandler(void)
//{
//  /* USER CODE BEGIN GPDMA1_Channel5_IRQn 0 */
//
//  /* USER CODE END GPDMA1_Channel5_IRQn 0 */
//  HAL_DMA_IRQHandler(&handle_GPDMA1_Channel5);
//  /* USER CODE BEGIN GPDMA1_Channel5_IRQn 1 */
//
//  /* USER CODE END GPDMA1_Channel5_IRQn 1 */
//}
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins
     PA14 (JTCK/SWCLK)   ------> DEBUG_JTCK-SWCLK
     PC14-OSC32_IN (PC14)   ------> RCC_OSC32_IN
     PB3 (JTDO/TRACESWO)   ------> DEBUG_JTDO-SWO
     PC15-OSC32_OUT (PC15)   ------> RCC_OSC32_OUT
     PA13 (JTMS/SWDIO)   ------> DEBUG_JTMS-SWDIO
     PH0-OSC_IN (PH0)   ------> RCC_OSC_IN
     PH1-OSC_OUT (PH1)   ------> RCC_OSC_OUT
*/

/* USER CODE BEGIN 2 */
// BUTTON
void MX_GPIO_PC13_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
   __HAL_RCC_GPIOC_CLK_ENABLE();

  /*Configure GPIO pin : PC13Pin */
  GPIO_InitStruct.Pin = USER_BUTTON_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USER_BUTTON_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(USER_BUTTON_EXTI_IRQn, 15, 0);
//  HAL_NVIC_EnableIRQ(USER_BUTTON_EXTI_IRQn);
}

void MX_GPIO_PE11_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  HAL_PWREx_EnableVddIO2();
  /* GPIO Ports Clock Enable */
   __HAL_RCC_GPIOE_CLK_ENABLE();

  /*Configure GPIO pin : PE11Pin */
  GPIO_InitStruct.Pin = INT_HTS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(INT_HTS_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(INT_HTS_EXTI_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(INT_HTS_EXTI_IRQn);
}

//STTS22H (temperature)
void MX_GPIO_PB15_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  HAL_PWREx_EnableVddIO2();
  /* GPIO Ports Clock Enable */
   __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin : PB15Pin */
  GPIO_InitStruct.Pin = INT_STT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(INT_STT_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(INT_STT_EXTI_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(INT_STT_EXTI_IRQn);
}

//LPS22DF (pressure)
void MX_GPIO_PE8_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  HAL_PWREx_EnableVddIO2();
  /* GPIO Ports Clock Enable */
   __HAL_RCC_GPIOE_CLK_ENABLE();

  /*Configure GPIO pin : PE8Pin */
  GPIO_InitStruct.Pin = INT_LPS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(INT_LPS_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(INT_LPS_EXTI_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(INT_LPS_EXTI_IRQn);
}

//LIS2MDL (magnetometer)
void MX_GPIO_PE6_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  HAL_PWREx_EnableVddIO2();
  /* GPIO Ports Clock Enable */
   __HAL_RCC_GPIOE_CLK_ENABLE();

  /*Configure GPIO pin : PE8Pin */
  GPIO_InitStruct.Pin = INT_MAG_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(INT_MAG_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(INT_MAG_EXTI_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(INT_MAG_EXTI_IRQn);
}

//LIS2DU12 (accelerometer)
void MX_GPIO_PF2_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  HAL_PWREx_EnableVddIO2();
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOF_CLK_ENABLE();

  /*Configure GPIO pin : PF2Pin */
  GPIO_InitStruct.Pin = INT1_ACC_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(INT1_ACC_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(INT1_ACC_EXTI_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(INT1_ACC_EXTI_IRQn);
}

//LSM6DSV16X (acc+gyro)
void MX_GPIO_PA4_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  HAL_PWREx_EnableVddIO2();
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin : PA4Pin */
  GPIO_InitStruct.Pin = INT1_DSV16X_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(INT1_DSV16X_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(INT1_DSV16X_EXTI_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(INT1_DSV16X_EXTI_IRQn);
}

void MX_GPIO_PF9_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  HAL_PWREx_EnableVddIO2();
  /* GPIO Ports Clock Enable */
   __HAL_RCC_GPIOF_CLK_ENABLE();

  /*Configure GPIO pin : PF9Pin */
  GPIO_InitStruct.Pin = INT_MAG_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(INT_MAG_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(INT_MAG_EXTI_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(INT_MAG_EXTI_IRQn);
}

void MX_GPIO_PF3_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  HAL_PWREx_EnableVddIO2();
  /* GPIO Ports Clock Enable */
   __HAL_RCC_GPIOF_CLK_ENABLE();

  /*Configure GPIO pin : PF3Pin */
  GPIO_InitStruct.Pin = INT1_ICLX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(INT1_ICLX_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(INT1_ICLX_EXTI_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(INT1_ICLX_EXTI_IRQn);
}

void MX_GPIO_PI0_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  HAL_PWREx_EnableVddIO2();
  __HAL_RCC_GPIOI_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(MCU_SEL_GPIO_Port, MCU_SEL_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : PI0 */
  GPIO_InitStruct.Pin = MCU_SEL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(MCU_SEL_GPIO_Port, &GPIO_InitStruct);
}

void MX_GPIO_PI5_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  HAL_PWREx_EnableVddIO2();
  __HAL_RCC_GPIOI_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(CS_DSV16X_GPIO_Port, CS_DSV16X_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : PI5 */
  GPIO_InitStruct.Pin = CS_DSV16X_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(CS_DSV16X_GPIO_Port, &GPIO_InitStruct);
}

void MX_GPIO_PI7_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  HAL_PWREx_EnableVddIO2();
  __HAL_RCC_GPIOI_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(CS_ACC_GPIO_Port, CS_ACC_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : PI7 */
  GPIO_InitStruct.Pin = CS_ACC_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(CS_ACC_GPIO_Port, &GPIO_InitStruct);
}

void MX_GPIO_PH6_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  HAL_PWREx_EnableVddIO2();
  __HAL_RCC_GPIOH_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(CS_DLPC_GPIO_Port, CS_DLPC_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : PH6 */
  GPIO_InitStruct.Pin = CS_DLPC_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(CS_DLPC_GPIO_Port, &GPIO_InitStruct);
}

void MX_GPIO_PB8_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  HAL_PWREx_EnableVddIO2();
  /* GPIO Ports Clock Enable */
   __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin : PB8Pin */
  GPIO_InitStruct.Pin = INT1_DHCX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(INT1_DHCX_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(INT1_DHCX_EXTI_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(INT1_DHCX_EXTI_IRQn);
}

void MX_GPIO_PF4_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  HAL_PWREx_EnableVddIO2();
  /* GPIO Ports Clock Enable */
   __HAL_RCC_GPIOF_CLK_ENABLE();

  /*Configure GPIO pin : PF4Pin */
  GPIO_InitStruct.Pin = INT2_DHCX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(INT2_DHCX_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(INT2_DHCX_EXTI_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(INT2_DHCX_EXTI_IRQn);
}

void MX_GPIO_PH15_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  HAL_PWREx_EnableVddIO2();
  __HAL_RCC_GPIOH_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(CS_DHCX_GPIO_Port, CS_DHCX_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : PH15 */
  GPIO_InitStruct.Pin = CS_DHCX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(CS_DHCX_GPIO_Port, &GPIO_InitStruct);
}

void MX_GPIO_PF15_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  HAL_PWREx_EnableVddIO2();
  /* GPIO Ports Clock Enable */
   __HAL_RCC_GPIOF_CLK_ENABLE();

  /*Configure GPIO pin : PF15Pin */
  GPIO_InitStruct.Pin = INT1_DWB_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(INT1_DWB_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(INT1_DWB_EXTI_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(INT1_DWB_EXTI_IRQn);
}

void MX_GPIO_PF12_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  HAL_PWREx_EnableVddIO2();
  __HAL_RCC_GPIOF_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(CS_DWB_GPIO_Port, CS_DWB_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : PF12 */
  GPIO_InitStruct.Pin = CS_DWB_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(CS_DWB_GPIO_Port, &GPIO_InitStruct);
}

/* USER CODE END 2 */

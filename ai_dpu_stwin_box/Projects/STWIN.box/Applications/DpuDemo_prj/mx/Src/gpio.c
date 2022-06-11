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
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOH, CS_DHCX_Pin|CS_DLPC_Pin|WIFI_CS_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOI, EX_SPI_NSS_Pin|CS_ICLX_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(BLE_SPI_CS_GPIO_Port, BLE_SPI_CS_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(CS_DWB_GPIO_Port, CS_DWB_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = INT_HTS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(INT_HTS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PGPin PG10 PGPin PGPin
                           PG0 PGPin */
  GPIO_InitStruct.Pin = WIFI_FLOW_Pin|GPIO_PIN_10|SD_DETECT_Pin|INT_EX_Pin
                          |GPIO_PIN_0|INT_EXG5_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pins : PA15 PA10 PAPin PA9
                           PA0 PA1 PA2 PA3
                           PA6 */
  GPIO_InitStruct.Pin = GPIO_PIN_15|GPIO_PIN_10|SW_SEL_Pin|GPIO_PIN_9
                          |GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PHPin PHPin PHPin */
  GPIO_InitStruct.Pin = CS_DHCX_Pin|CS_DLPC_Pin|WIFI_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

  /*Configure GPIO pins : PIPin PI4 PIPin */
  GPIO_InitStruct.Pin = DCDC_2_EN_Pin|GPIO_PIN_4|SPI2_MISO_p2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

  /*Configure GPIO pins : PDPin PD4 PD7 PD1
                           PD14 PDPin PDPin PD8
                           PDPin */
  GPIO_InitStruct.Pin = C_EN_Pin|GPIO_PIN_4|GPIO_PIN_7|GPIO_PIN_1
                          |GPIO_PIN_14|INT_EXD15_Pin|BLE_RST_Pin|GPIO_PIN_8
                          |WIFI_BOOT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : PIPin PIPin */
  GPIO_InitStruct.Pin = EX_SPI_NSS_Pin|CS_ICLX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

  /*Configure GPIO pins : PHPin PHPin PH14 PH13
                           PHPin PH11 PH8 PHPin
                           PH5 PH2 */
  GPIO_InitStruct.Pin = LED1_Pin|BOOT0_Pin|GPIO_PIN_14|GPIO_PIN_13
                          |LED2_Pin|GPIO_PIN_11|GPIO_PIN_8|STSAFE_RESE_Pin
                          |GPIO_PIN_5|GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = BLE_SPI_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(BLE_SPI_CS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PEPin PEPin */
  GPIO_InitStruct.Pin = USER_BUTTON_Pin|WIFI_NOTIFY_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : PBPin PBPin */
  GPIO_InitStruct.Pin = INT1_DHCX_Pin|NFC_INT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = INT_LPS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(INT_LPS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PFPin PFPin PFPin PF13 */
  GPIO_InitStruct.Pin = GPIO2_EX_Pin|BLE_TEST9_Pin|BLE_TEST8_Pin|GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pins : PFPin PFPin PFPin PFPin
                           PFPin PFPin PFPin PFPin
                           PFPin */
  GPIO_InitStruct.Pin = INT1_DLPC_Pin|INT2_DLPC_Pin|INT_MAG_Pin|INT_STT_Pin
                          |INT1_ICLX_Pin|INT2_DHCX_Pin|BLE_INT_Pin|INT2_ICLX_Pin
                          |INT1_DWB_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pins : PB7 PB0 PB10 PB2
                           PB11 PB12 PBPin PB14
                           PBPin */
  GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_0|GPIO_PIN_10|GPIO_PIN_2
                          |GPIO_PIN_11|GPIO_PIN_12|SPI2_MOSI_p2_Pin|GPIO_PIN_14
                          |GPIO1_EX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PC7 PCPin PCPin PC4
                           PC5 */
  GPIO_InitStruct.Pin = GPIO_PIN_7|uC_ADC_BATT_Pin|ADC_EX_Pin|GPIO_PIN_4
                          |GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = CS_DWB_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(CS_DWB_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PE8 PE14 PE13 PE11
                           PEPin PEPin */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_14|GPIO_PIN_13|GPIO_PIN_11
                          |LDO_EN_Pin|WIFI_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = BUTTON_PWR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BUTTON_PWR_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
//  HAL_NVIC_SetPriority(EXTI0_IRQn, 15, 0);
//  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

//  HAL_NVIC_SetPriority(EXTI1_IRQn, 7, 0);
//  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

  HAL_NVIC_SetPriority(EXTI2_IRQn, 7, 0);
  HAL_NVIC_EnableIRQ(EXTI2_IRQn);

//  HAL_NVIC_SetPriority(EXTI3_IRQn, 7, 0);
//  HAL_NVIC_EnableIRQ(EXTI3_IRQn);

//  HAL_NVIC_SetPriority(EXTI4_IRQn, 7, 0);
//  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

//  HAL_NVIC_SetPriority(EXTI5_IRQn, 7, 0);
//  HAL_NVIC_EnableIRQ(EXTI5_IRQn);

//  HAL_NVIC_SetPriority(EXTI6_IRQn, 7, 0);
//  HAL_NVIC_EnableIRQ(EXTI6_IRQn);

  HAL_NVIC_SetPriority(EXTI7_IRQn, 7, 0);
  HAL_NVIC_EnableIRQ(EXTI7_IRQn);

//  HAL_NVIC_SetPriority(EXTI8_IRQn, 7, 0);
//  HAL_NVIC_EnableIRQ(EXTI8_IRQn);

  HAL_NVIC_SetPriority(EXTI9_IRQn, 7, 0);
  HAL_NVIC_EnableIRQ(EXTI9_IRQn);

  HAL_NVIC_SetPriority(EXTI10_IRQn, 7, 0);
  HAL_NVIC_EnableIRQ(EXTI10_IRQn);

  HAL_NVIC_SetPriority(EXTI11_IRQn, 7, 0);
  HAL_NVIC_EnableIRQ(EXTI11_IRQn);

//  HAL_NVIC_SetPriority(EXTI12_IRQn, 7, 0);
//  HAL_NVIC_EnableIRQ(EXTI12_IRQn);

  HAL_NVIC_SetPriority(EXTI13_IRQn, 7, 0);
  HAL_NVIC_EnableIRQ(EXTI13_IRQn);

  HAL_NVIC_SetPriority(EXTI14_IRQn, 7, 0);
  HAL_NVIC_EnableIRQ(EXTI14_IRQn);

}

/* USER CODE BEGIN 2 */
void MX_GPIO_PE0_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
   __HAL_RCC_GPIOE_CLK_ENABLE();

  /*Configure GPIO pin : PA5Pin */
  GPIO_InitStruct.Pin = USER_BUTTON_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USER_BUTTON_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 15, 0);
//  HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}

void MX_GPIO_PI6_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  HAL_PWREx_EnableVddIO2();
  /* GPIO Ports Clock Enable */
   __HAL_RCC_GPIOI_CLK_ENABLE();

  /*Configure GPIO pin : PI6Pin */
  GPIO_InitStruct.Pin = INT_HTS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(INT_HTS_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(INT_HTS_EXTI_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(INT_HTS_EXTI_IRQn);
}

void MX_GPIO_PF5_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  HAL_PWREx_EnableVddIO2();
  /* GPIO Ports Clock Enable */
   __HAL_RCC_GPIOF_CLK_ENABLE();

  /*Configure GPIO pin : PF5Pin */
  GPIO_InitStruct.Pin = INT_STT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(INT_STT_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(INT_STT_EXTI_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(INT_STT_EXTI_IRQn);
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

void MX_GPIO_PI7_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  HAL_PWREx_EnableVddIO2();
  __HAL_RCC_GPIOI_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(CS_ICLX_GPIO_Port, CS_ICLX_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : PI7 */
  GPIO_InitStruct.Pin = CS_ICLX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(CS_ICLX_GPIO_Port, &GPIO_InitStruct);
}

void MX_GPIO_PF1_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  HAL_PWREx_EnableVddIO2();
  /* GPIO Ports Clock Enable */
   __HAL_RCC_GPIOF_CLK_ENABLE();

  /*Configure GPIO pin : PF1Pin */
  GPIO_InitStruct.Pin = INT1_DLPC_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(INT1_DLPC_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(INT1_DLPC_EXTI_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(INT1_DLPC_EXTI_IRQn);
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
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
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

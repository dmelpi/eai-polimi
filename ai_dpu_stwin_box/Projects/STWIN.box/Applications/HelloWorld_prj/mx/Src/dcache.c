/**
  ******************************************************************************
  * @file    dcache.c
  * @brief   This file provides code for the configuration
  *          of the DCACHE instances.
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

/* Includes ------------------------------------------------------------------*/
#include "dcache.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

DCACHE_HandleTypeDef hdcache1;

/* DCACHE1 init function */
void MX_DCACHE1_Init(void)
{

  /* USER CODE BEGIN DCACHE1_Init 0 */

  /* USER CODE END DCACHE1_Init 0 */

  /* USER CODE BEGIN DCACHE1_Init 1 */

  /* USER CODE END DCACHE1_Init 1 */
  hdcache1.Instance = DCACHE1;
  hdcache1.Init.ReadBurstType = DCACHE_READ_BURST_WRAP;
  if (HAL_DCACHE_Init(&hdcache1) != HAL_OK)
  {
    Error_Handler();
  }
  HAL_DCACHE_Enable(&hdcache1);
  /* USER CODE BEGIN DCACHE1_Init 2 */

  /* USER CODE END DCACHE1_Init 2 */

}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

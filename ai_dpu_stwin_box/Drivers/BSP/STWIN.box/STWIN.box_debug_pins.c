/**
 ******************************************************************************
 * @file    STWIN.box_debug_pins.c
 * @author  SRA
 * @brief   This file provides low level functionalities for STWIN.box board
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

/* Includes ------------------------------------------------------------------*/
#include "STWIN.box_debug_pins.h"


/** @addtogroup BSP
* @{
*/ 

/** @addtogroup STWIN.box
* @{
*/

/** @defgroup STWIN.box_LOW_LEVEL_Private_Variables STWIN.box_LOW_LEVEL Private Variables 
* @{
*/
static const uint16_t  DEBUG_PIN[DEBUG_PINn] = {DEBUG_PIN14,
												DEBUG_PIN15,
												DEBUG_PIN18,
												DEBUG_PIN19,
												DEBUG_PIN20,
												DEBUG_PIN21,
												DEBUG_PIN22
                                                };

static GPIO_TypeDef  *DEBUG_PIN_PORT[DEBUG_PINn] =   {DEBUG_PIN_14_GPIO_PORT, 
                                                      DEBUG_PIN_15_GPIO_PORT, 
                                                      DEBUG_PIN_18_GPIO_PORT, 
                                                      DEBUG_PIN_19_GPIO_PORT, 
                                                      DEBUG_PIN_20_GPIO_PORT, 
                                                      DEBUG_PIN_21_GPIO_PORT, 
                                                      DEBUG_PIN_22_GPIO_PORT
                                                      };                              
                                     
/**
* @}
*/


/** @defgroup STWIN.box_LOW_LEVEL_Exported_Functions STWIN.box_LOW_LEVEL Exported Functions
* @{
*/

/**
* @brief  Configure general purpose debug pins on STWIN.box DIL24 connector
*         Pin name also identifies the pin number on DIL24.
* @param  None
* @retval None
*/  
void BSP_DEBUG_PIN_Init_All(void)
{
  BSP_DEBUG_PIN_Off(DEBUG_PIN14);
  BSP_DEBUG_PIN_Off(DEBUG_PIN15);
  BSP_DEBUG_PIN_Off(DEBUG_PIN18);
  BSP_DEBUG_PIN_Off(DEBUG_PIN19);
  BSP_DEBUG_PIN_Off(DEBUG_PIN20);
  BSP_DEBUG_PIN_Off(DEBUG_PIN21);
  BSP_DEBUG_PIN_Off(DEBUG_PIN22);
  
  BSP_DEBUG_PIN_Init(DEBUG_PIN14);
  BSP_DEBUG_PIN_Init(DEBUG_PIN15);
  BSP_DEBUG_PIN_Init(DEBUG_PIN18);
  BSP_DEBUG_PIN_Init(DEBUG_PIN19);  
  BSP_DEBUG_PIN_Init(DEBUG_PIN20);
  BSP_DEBUG_PIN_Init(DEBUG_PIN21);  
  BSP_DEBUG_PIN_Init(DEBUG_PIN22);
}


void BSP_DEBUG_PIN_Init(Debug_Pin_TypeDef Pin)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  /* Enable the GPIO clock */
  DEBUG_PINx_GPIO_CLK_ENABLE(Pin);
  
  /* Configure the GPIO pin */
  GPIO_InitStructure.Pin = DEBUG_PIN[Pin];
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;    
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  
  HAL_GPIO_WritePin(DEBUG_PIN_PORT[Pin], DEBUG_PIN[Pin], GPIO_PIN_RESET);  
  
  HAL_GPIO_Init(DEBUG_PIN_PORT[Pin], &GPIO_InitStructure);
}

void  BSP_DEBUG_PIN_DeInit(Debug_Pin_TypeDef Pin)
{
  HAL_GPIO_DeInit(DEBUG_PIN_PORT[Pin], DEBUG_PIN[Pin]);
  DEBUG_PINx_GPIO_CLK_DISABLE(Pin);
}

void BSP_DEBUG_PIN_On(Debug_Pin_TypeDef Pin)
{
  HAL_GPIO_WritePin(DEBUG_PIN_PORT[Pin], DEBUG_PIN[Pin], GPIO_PIN_SET);
}

void BSP_DEBUG_PIN_Off(Debug_Pin_TypeDef Pin)
{
  HAL_GPIO_WritePin(DEBUG_PIN_PORT[Pin], DEBUG_PIN[Pin], GPIO_PIN_RESET);
}

void BSP_DEBUG_PIN_Toggle(Debug_Pin_TypeDef Pin)
{
  HAL_GPIO_TogglePin(DEBUG_PIN_PORT[Pin], DEBUG_PIN[Pin]);
}    


/**
* @}
*/ 

/**
* @}
*/

/**
* @}
*/    


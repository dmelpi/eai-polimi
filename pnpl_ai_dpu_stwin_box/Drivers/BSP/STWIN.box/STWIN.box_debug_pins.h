/**
 ******************************************************************************
 * @file    STWIN.box_debug_pins.h
 * @author  SRA
 * @brief   This file contains definitions for STWIN.box debug pins on
 *          DIL24 connector
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

  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STWINBOX_DEBUG_PINS_H
#define STWINBOX_DEBUG_PINS_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32u5xx_hal.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STWIN.box
  * @{
  */
      
  /** @addtogroup STWIN.box_LOW_LEVEL
  * @{
  */
   

/** @defgroup STWIN.box_LOW_LEVEL_Exported_Types STWIN.box_LOW_LEVEL Exported Types
  * @{
  */
typedef enum
{
  DEBUG_PIN14 = 0,
  DEBUG_PIN15,
  DEBUG_PIN18,
  DEBUG_PIN19,
  DEBUG_PIN20,
  DEBUG_PIN21,
  DEBUG_PIN22
} Debug_Pin_TypeDef;


/**
  * @}
  */ 

/** @defgroup STWIN.box_LOW_LEVEL_Exported_Constants STWIN.box_LOW_LEVEL Exported Constants
  * @{
  */ 


/** @defgroup STWIN.box_LOW_LEVEL_DEBUG_PINS PINS
  * @{
  */
#define DEBUG_PINn 7
  
#define DEBUG_PIN_14_PIN                                 GPIO_PIN_5
#define DEBUG_PIN_14_GPIO_PORT                           GPIOG
#define DEBUG_PIN_14_GPIO_CLK_ENABLE()                   __HAL_RCC_GPIOG_CLK_ENABLE()
#define DEBUG_PIN_14_GPIO_CLK_DISABLE()                  __HAL_RCC_GPIOG_CLK_DISABLE()
  
#define DEBUG_PIN_15_PIN                                 GPIO_PIN_1
#define DEBUG_PIN_15_GPIO_PORT                           GPIOB
#define DEBUG_PIN_15_GPIO_CLK_ENABLE()                   __HAL_RCC_GPIOB_CLK_ENABLE()
#define DEBUG_PIN_15_GPIO_CLK_DISABLE()                  __HAL_RCC_GPIOB_CLK_DISABLE()
  
#define DEBUG_PIN_18_PIN                                 GPIO_PIN_8
#define DEBUG_PIN_18_GPIO_PORT                           GPIOF
#define DEBUG_PIN_18_GPIO_CLK_ENABLE()                   __HAL_RCC_GPIOF_CLK_ENABLE()
#define DEBUG_PIN_18_GPIO_CLK_DISABLE()                  __HAL_RCC_GPIOF_CLK_DISABLE()
  
#define DEBUG_PIN_19_PIN                                 GPIO_PIN_0
#define DEBUG_PIN_19_GPIO_PORT                           GPIOI
#define DEBUG_PIN_19_GPIO_CLK_ENABLE()                   __HAL_RCC_GPIOI_CLK_ENABLE()
#define DEBUG_PIN_19_GPIO_CLK_DISABLE()                  __HAL_RCC_GPIOI_CLK_DISABLE()
  
#define DEBUG_PIN_20_PIN                                 GPIO_PIN_1
#define DEBUG_PIN_20_GPIO_PORT                           GPIOI
#define DEBUG_PIN_20_GPIO_CLK_ENABLE()                   __HAL_RCC_GPIOI_CLK_ENABLE()
#define DEBUG_PIN_20_GPIO_CLK_DISABLE()                  __HAL_RCC_GPIOI_CLK_DISABLE()
  
#define DEBUG_PIN_21_PIN                                 GPIO_PIN_8
#define DEBUG_PIN_21_GPIO_PORT                           GPIOG
#define DEBUG_PIN_21_GPIO_CLK_ENABLE()                   __HAL_RCC_GPIOG_CLK_ENABLE()
#define DEBUG_PIN_21_GPIO_CLK_DISABLE()                  __HAL_RCC_GPIOG_CLK_DISABLE()
  
#define DEBUG_PIN_22_PIN                                 GPIO_PIN_3
#define DEBUG_PIN_22_GPIO_PORT                           GPIOD
#define DEBUG_PIN_22_GPIO_CLK_ENABLE()                   __HAL_RCC_GPIOD_CLK_ENABLE()
#define DEBUG_PIN_22_GPIO_CLK_DISABLE()                  __HAL_RCC_GPIOD_CLK_DISABLE()

#define DEBUG_PINx_GPIO_CLK_ENABLE(__DEBUG_PIN__)   do { if((__DEBUG_PIN__) == DEBUG_PIN14) { HAL_PWREx_EnableVddIO2(); DEBUG_PIN_14_GPIO_CLK_ENABLE(); } else \
                                                         if((__DEBUG_PIN__) == DEBUG_PIN15) { DEBUG_PIN_15_GPIO_CLK_ENABLE(); } else \
                                                         if((__DEBUG_PIN__) == DEBUG_PIN18) { DEBUG_PIN_18_GPIO_CLK_ENABLE(); } else \
                                                         if((__DEBUG_PIN__) == DEBUG_PIN19) { HAL_PWREx_EnableVddIO2(); DEBUG_PIN_19_GPIO_CLK_ENABLE(); } else \
                                                         if((__DEBUG_PIN__) == DEBUG_PIN20) { HAL_PWREx_EnableVddIO2(); DEBUG_PIN_20_GPIO_CLK_ENABLE(); } else \
                                                         if((__DEBUG_PIN__) == DEBUG_PIN21) { HAL_PWREx_EnableVddIO2(); DEBUG_PIN_21_GPIO_CLK_ENABLE(); } else \
                                                         if((__DEBUG_PIN__) == DEBUG_PIN22) { DEBUG_PIN_22_GPIO_CLK_ENABLE(); } else \
                                                         { /*No more pin available on DIL24 connector*/ } } while(0)
                                                           
#define DEBUG_PINx_GPIO_CLK_DISABLE(__DEBUG_PIN__)  do { if((__DEBUG_PIN__) == DEBUG_PIN14) { DEBUG_PIN_14_GPIO_CLK_DISABLE(); } else \
                                                         if((__DEBUG_PIN__) == DEBUG_PIN15) { DEBUG_PIN_15_GPIO_CLK_DISABLE(); } else \
                                                         if((__DEBUG_PIN__) == DEBUG_PIN18) { DEBUG_PIN_18_GPIO_CLK_DISABLE(); } else \
                                                         if((__DEBUG_PIN__) == DEBUG_PIN19) { DEBUG_PIN_19_GPIO_CLK_DISABLE(); } else \
                                                         if((__DEBUG_PIN__) == DEBUG_PIN20) { DEBUG_PIN_20_GPIO_CLK_DISABLE(); } else \
                                                         if((__DEBUG_PIN__) == DEBUG_PIN21) { DEBUG_PIN_21_GPIO_CLK_DISABLE(); } else \
                                                         if((__DEBUG_PIN__) == DEBUG_PIN22) { DEBUG_PIN_22_GPIO_CLK_DISABLE(); } else \
                                                         { /*No more pin available on DIL24 connector*/ } } while(0)


/**
  * @}
  */ 
  
/**
  * @}
  */ 

/** @defgroup STWIN.box_LOW_LEVEL_Exported_Functions STWIN.box_LOW_LEVEL Exported Functions
  * @{
  */

void BSP_DEBUG_PIN_Init_All(void);
void BSP_DEBUG_PIN_Init(Debug_Pin_TypeDef Pin);
void BSP_DEBUG_PIN_DeInit(Debug_Pin_TypeDef Pin);
void BSP_DEBUG_PIN_On(Debug_Pin_TypeDef Pin);
void BSP_DEBUG_PIN_Off(Debug_Pin_TypeDef Pin);
void BSP_DEBUG_PIN_Toggle(Debug_Pin_TypeDef Pin);

/**
  * @}
  */

/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* STWINBOX_DEBUG_PINS_H */


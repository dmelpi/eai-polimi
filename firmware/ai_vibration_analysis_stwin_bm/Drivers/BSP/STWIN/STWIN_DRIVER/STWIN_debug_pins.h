/**
******************************************************************************
* @file    STWIN_debug_pins.h
* @author  SRA
* @version v1.4.0
* @date    11-Sep-2020
* @brief   This file contains definitions for STWIN debug pins on
*          STMOD+ connector
******************************************************************************
* @attention
*
* <h2><center>&copy; Copyright (c) 2019 STMicroelectronics. 
* All rights reserved.</center></h2>
*
* This software component is licensed by ST under BSD 3-Clause license,
* the "License"; You may not use this file except in compliance with the 
* License. You may obtain a copy of the License at:
*                        opensource.org/licenses/BSD-3-Clause
*
******************************************************************************
*/ 

  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STWIN_DEBUG_PINS_H
#define STWIN_DEBUG_PINS_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STWIN
  * @{
  */
      
  /** @addtogroup STWIN_LOW_LEVEL
  * @{
  */
   

/** @defgroup STWIN_LOW_LEVEL_Exported_Types STWIN_LOW_LEVEL Exported Types
  * @{
  */
typedef enum
{
  DEBUG_PIN7  = 0,
  DEBUG_PIN8,
  DEBUG_PIN9,
  DEBUG_PIN10,
  DEBUG_PIN11,
  DEBUG_PIN12,
  DEBUG_PIN13,
  DEBUG_PIN14,
  DEBUG_PIN17,
  DEBUG_PIN18,
  DEBUG_PIN19,
  DEBUG_PIN20
} Debug_Pin_TypeDef;


/**
  * @}
  */ 

/** @defgroup STWIN_LOW_LEVEL_Exported_Constants STWIN_LOW_LEVEL Exported Constants
  * @{
  */ 


/** @defgroup STWIN_LOW_LEVEL_DEBUG_PINS PINS
  * @{
  */
#define DEBUG_PINn 12

#define DEBUG_PIN_7_PIN                                 GPIO_PIN_12
#define DEBUG_PIN_7_GPIO_PORT                           GPIOD
#define DEBUG_PIN_7_GPIO_CLK_ENABLE()                   __HAL_RCC_GPIOD_CLK_ENABLE()
#define DEBUG_PIN_7_GPIO_CLK_DISABLE()                  __HAL_RCC_GPIOD_CLK_DISABLE()
  
#define DEBUG_PIN_8_PIN                                 GPIO_PIN_15
#define DEBUG_PIN_8_GPIO_PORT                           GPIOB
#define DEBUG_PIN_8_GPIO_CLK_ENABLE()                   __HAL_RCC_GPIOB_CLK_ENABLE()
#define DEBUG_PIN_8_GPIO_CLK_DISABLE()                  __HAL_RCC_GPIOB_CLK_DISABLE()
  
#define DEBUG_PIN_9_PIN                                 GPIO_PIN_2
#define DEBUG_PIN_9_GPIO_PORT                           GPIOC
#define DEBUG_PIN_9_GPIO_CLK_ENABLE()                   __HAL_RCC_GPIOC_CLK_ENABLE()
#define DEBUG_PIN_9_GPIO_CLK_DISABLE()                  __HAL_RCC_GPIOC_CLK_DISABLE()
  
#define DEBUG_PIN_10_PIN                                 GPIO_PIN_13
#define DEBUG_PIN_10_GPIO_PORT                           GPIOD
#define DEBUG_PIN_10_GPIO_CLK_ENABLE()                   __HAL_RCC_GPIOD_CLK_ENABLE()
#define DEBUG_PIN_10_GPIO_CLK_DISABLE()                  __HAL_RCC_GPIOD_CLK_DISABLE()
  
#define DEBUG_PIN_11_PIN                                 GPIO_PIN_5
#define DEBUG_PIN_11_GPIO_PORT                           GPIOC
#define DEBUG_PIN_11_GPIO_CLK_ENABLE()                   __HAL_RCC_GPIOC_CLK_ENABLE()
#define DEBUG_PIN_11_GPIO_CLK_DISABLE()                  __HAL_RCC_GPIOC_CLK_DISABLE()
  
#define DEBUG_PIN_12_PIN                                 GPIO_PIN_11
#define DEBUG_PIN_12_GPIO_PORT                           GPIOD 
#define DEBUG_PIN_12_GPIO_CLK_ENABLE()                   __HAL_RCC_GPIOD_CLK_ENABLE()
#define DEBUG_PIN_12_GPIO_CLK_DISABLE()                  __HAL_RCC_GPIOD_CLK_DISABLE()
  
#define DEBUG_PIN_13_PIN                                 GPIO_PIN_5
#define DEBUG_PIN_13_GPIO_PORT                           GPIOA
#define DEBUG_PIN_13_GPIO_CLK_ENABLE()                   __HAL_RCC_GPIOA_CLK_ENABLE()
#define DEBUG_PIN_13_GPIO_CLK_DISABLE()                  __HAL_RCC_GPIOA_CLK_DISABLE()
  
#define DEBUG_PIN_14_PIN                                 GPIO_PIN_15
#define DEBUG_PIN_14_GPIO_PORT                           GPIOA
#define DEBUG_PIN_14_GPIO_CLK_ENABLE()                   __HAL_RCC_GPIOA_CLK_ENABLE()
#define DEBUG_PIN_14_GPIO_CLK_DISABLE()                  __HAL_RCC_GPIOA_CLK_DISABLE()
  
#define DEBUG_PIN_17_PIN                                 GPIO_PIN_14
#define DEBUG_PIN_17_GPIO_PORT                           GPIOB
#define DEBUG_PIN_17_GPIO_CLK_ENABLE()                   __HAL_RCC_GPIOB_CLK_ENABLE()
#define DEBUG_PIN_17_GPIO_CLK_DISABLE()                  __HAL_RCC_GPIOB_CLK_DISABLE()
  
#define DEBUG_PIN_18_PIN                                 GPIO_PIN_9
#define DEBUG_PIN_18_GPIO_PORT                           GPIOG
#define DEBUG_PIN_18_GPIO_CLK_ENABLE()                   __HAL_RCC_GPIOG_CLK_ENABLE()
#define DEBUG_PIN_18_GPIO_CLK_DISABLE()                  __HAL_RCC_GPIOG_CLK_DISABLE()
  
#define DEBUG_PIN_19_PIN                                 GPIO_PIN_10
#define DEBUG_PIN_19_GPIO_PORT                           GPIOG
#define DEBUG_PIN_19_GPIO_CLK_ENABLE()                   __HAL_RCC_GPIOG_CLK_ENABLE()
#define DEBUG_PIN_19_GPIO_CLK_DISABLE()                  __HAL_RCC_GPIOG_CLK_DISABLE()
  
#define DEBUG_PIN_20_PIN                                 GPIO_PIN_12
#define DEBUG_PIN_20_GPIO_PORT                           GPIOG
#define DEBUG_PIN_20_GPIO_CLK_ENABLE()                   __HAL_RCC_GPIOG_CLK_ENABLE()
#define DEBUG_PIN_20_GPIO_CLK_DISABLE()                  __HAL_RCC_GPIOG_CLK_DISABLE()

#define DEBUG_PINx_GPIO_CLK_ENABLE(__DEBUG_PIN__)     do { if((__DEBUG_PIN__) == DEBUG_PIN7 ) { DEBUG_PIN_7_GPIO_CLK_ENABLE(); } else \
                                                         if((__DEBUG_PIN__) == DEBUG_PIN8 ) { DEBUG_PIN_8_GPIO_CLK_ENABLE(); } else \
                                                         if((__DEBUG_PIN__) == DEBUG_PIN9 ) { DEBUG_PIN_9_GPIO_CLK_ENABLE(); } else \
                                                         if((__DEBUG_PIN__) == DEBUG_PIN10) { DEBUG_PIN_10_GPIO_CLK_ENABLE(); } else \
                                                         if((__DEBUG_PIN__) == DEBUG_PIN11) { DEBUG_PIN_11_GPIO_CLK_ENABLE(); } else \
                                                         if((__DEBUG_PIN__) == DEBUG_PIN12) { DEBUG_PIN_12_GPIO_CLK_ENABLE(); } else \
                                                         if((__DEBUG_PIN__) == DEBUG_PIN13) { DEBUG_PIN_13_GPIO_CLK_ENABLE(); } else \
                                                         if((__DEBUG_PIN__) == DEBUG_PIN14) { DEBUG_PIN_14_GPIO_CLK_ENABLE(); } else \
                                                         if((__DEBUG_PIN__) == DEBUG_PIN17) { DEBUG_PIN_17_GPIO_CLK_ENABLE(); } else \
                                                         if((__DEBUG_PIN__) == DEBUG_PIN18) { HAL_PWREx_EnableVddIO2(); DEBUG_PIN_18_GPIO_CLK_ENABLE(); } else \
                                                         if((__DEBUG_PIN__) == DEBUG_PIN19) { HAL_PWREx_EnableVddIO2(); DEBUG_PIN_19_GPIO_CLK_ENABLE(); } else \
                                                         if((__DEBUG_PIN__) == DEBUG_PIN20) { HAL_PWREx_EnableVddIO2(); DEBUG_PIN_20_GPIO_CLK_ENABLE(); } else \
                                                         { /*No more pin available on STMOD+ connector*/ } } while(0)
                                                           
#define DEBUG_PINx_GPIO_CLK_DISABLE(__DEBUG_PIN__)  do { if((__DEBUG_PIN__) == DEBUG_PIN7 ) { DEBUG_PIN_7_GPIO_CLK_DISABLE(); } else \
                                                         if((__DEBUG_PIN__) == DEBUG_PIN8 ) { DEBUG_PIN_8_GPIO_CLK_DISABLE(); } else \
                                                         if((__DEBUG_PIN__) == DEBUG_PIN9 ) { DEBUG_PIN_9_GPIO_CLK_DISABLE(); } else \
                                                         if((__DEBUG_PIN__) == DEBUG_PIN10) { DEBUG_PIN_10_GPIO_CLK_DISABLE(); } else \
                                                         if((__DEBUG_PIN__) == DEBUG_PIN11) { DEBUG_PIN_11_GPIO_CLK_DISABLE(); } else \
                                                         if((__DEBUG_PIN__) == DEBUG_PIN12) { DEBUG_PIN_12_GPIO_CLK_DISABLE(); } else \
                                                         if((__DEBUG_PIN__) == DEBUG_PIN13) { DEBUG_PIN_13_GPIO_CLK_DISABLE(); } else \
                                                         if((__DEBUG_PIN__) == DEBUG_PIN14) { DEBUG_PIN_14_GPIO_CLK_DISABLE(); } else \
                                                         if((__DEBUG_PIN__) == DEBUG_PIN17) { DEBUG_PIN_17_GPIO_CLK_DISABLE(); } else \
                                                         if((__DEBUG_PIN__) == DEBUG_PIN18) { DEBUG_PIN_18_GPIO_CLK_DISABLE(); } else \
                                                         if((__DEBUG_PIN__) == DEBUG_PIN19) { DEBUG_PIN_19_GPIO_CLK_DISABLE(); } else \
                                                         if((__DEBUG_PIN__) == DEBUG_PIN20) { DEBUG_PIN_20_GPIO_CLK_DISABLE(); } else \
                                                         { /*No more pin available on STMOD+ connector*/ } } while(0)


/**
  * @}
  */ 
  
/**
  * @}
  */ 

/** @defgroup STWIN_LOW_LEVEL_Exported_Functions STWIN_LOW_LEVEL Exported Functions
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

#endif /* STWIN_DEBUG_PINS_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

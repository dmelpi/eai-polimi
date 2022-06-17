/**
  ******************************************************************************
  * @file    stm32_assert.h
  * @author  MCD Application Team
  * @version V1.7.0
  * @date    31-May-2016
  * @brief   STM32 assert template file.
  *          This file should be copied to the application folder and renamed
  *          to stm32_assert.h.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32_ASSERT_H
#define __STM32_ASSERT_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Includes ------------------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

  void assert_failed(uint8_t* file, uint32_t line);

#ifdef __cplusplus
}
#endif

#endif /* __STM32_ASSERT_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

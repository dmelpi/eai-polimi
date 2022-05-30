/**
 ******************************************************************************
 * @file    CircularBuffer_freertos.c
 * @author  STMicroelectronics - AIS - MCD Team
 * @version M.m.b
 * @date    May 12, 2022
 *
 * @brief   Define the CircularBuffer API that are specific for a given
 *          target platform: FreeRTOS
 *
 *
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file in
 * the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 ******************************************************************************
 */

#include "services/CircularBuffer.h"

#ifdef SYS_TP_RTOS_FREERTOS

#include "FreeRTOS.h"
#include "task.h"

#ifndef SYS_IS_CALLED_FROM_ISR
#define SYS_IS_CALLED_FROM_ISR() ((SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk) != 0 ? 1 : 0)
#endif


// Private functions definition
// ****************************

void CB_EnterCritical(void)
{
  if(SYS_IS_CALLED_FROM_ISR())
  {
    taskENTER_CRITICAL_FROM_ISR();
  }
  else
  {
    taskENTER_CRITICAL();
  }
}

void CB_ExitCritical(void)
{
  if(SYS_IS_CALLED_FROM_ISR())
  {
    taskEXIT_CRITICAL_FROM_ISR(0);
  }
  else
  {
    taskEXIT_CRITICAL();
  }
}

#endif // SYS_TP_RTOS_FREERTOS

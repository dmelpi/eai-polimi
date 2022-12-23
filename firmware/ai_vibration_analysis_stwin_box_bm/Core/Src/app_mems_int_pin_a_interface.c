/**
  ******************************************************************************
  * @file    app_mems_int_pin_a_interface.c
  * @author  MEMS Application Team
  * @brief   This file contains the MEMS INT pin A interface
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
#include "app_mems_int_pin_a_interface.h"
#include "STWIN.box.h"


EXTI_HandleTypeDef hexti8 = {.Line = EXTI_LINE_8};
extern volatile uint8_t MemsEventDetected;
extern volatile HAL_StatusTypeDef ret_hal_status;
static void mems_int_pin_a_hardware_event_isr(void);
volatile int Mems_Interrupt = 0;

void set_mems_int_pin_a_exti(void)
{
	/* register event irq handler */
  ret_hal_status = HAL_EXTI_GetHandle(&hexti8, EXTI_LINE_8);
  ret_hal_status = HAL_EXTI_RegisterCallback(&hexti8, HAL_EXTI_COMMON_CB_ID, mems_int_pin_a_hardware_event_isr);
  HAL_NVIC_SetPriority(EXTI8_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI8_IRQn);
}

static void mems_int_pin_a_hardware_event_isr(void)
{

	MemsEventDetected = 1;

}


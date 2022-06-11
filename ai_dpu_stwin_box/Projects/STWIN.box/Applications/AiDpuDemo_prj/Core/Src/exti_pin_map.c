/**
 ******************************************************************************
 * @file    exti_pin_map.c
 * @author  SRA
 * @version 1.0.0
 * @date    6-Sep-2021
 * @brief   Application level file. It defines the PIN to callback
 *          mapping function for the external interrupt.
 *
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under ODE SOFTWARE LICENSE AGREEMENT
 * SLA0094, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0094
 *
 ******************************************************************************
 */

#include "drivers/EXTIPinMap.h"

// Forward function declaration.
void Def_EXTI_Callback(uint16_t nPin){};
void HW_PB_EXTI_Callback(uint16_t pin);
void INT2_DHCX_EXTI_Callback(uint16_t nPin);
void ISM330DHCXTask_EXTI_Callback(uint16_t nPin);
void IIS3DWBTask_EXTI_Callback(uint16_t nPin);

EXTI_BEGIN_P2F_MAP() EXTI_P2F_MAP_ENTRY(GPIO_PIN_0, HW_PB_EXTI_Callback)
EXTI_P2F_MAP_ENTRY(GPIO_PIN_8, ISM330DHCXTask_EXTI_Callback)
EXTI_P2F_MAP_ENTRY(GPIO_PIN_15, IIS3DWBTask_EXTI_Callback)
EXTI_END_P2F_MAP()

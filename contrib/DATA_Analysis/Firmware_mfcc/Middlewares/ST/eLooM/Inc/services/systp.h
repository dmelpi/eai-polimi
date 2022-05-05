/**
 ******************************************************************************
 * @file    systp.h
 * @author  STMicroelectronics - ST-Korea - MCD Team
 * @version 3.0.0
 * @date    Mar 22, 2017
 * @brief   Target platform definition.
 *
 * This file include definitions depending on the target platform.
 * A target platform is a tuple Hardware + Compiler.
 *
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2016 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file in
 * the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 ******************************************************************************
 */
#ifndef SYSTARGETPLATFORM_H_
#define SYSTARGETPLATFORM_H_

/* MCU specific include */
/************************/

#ifdef SYS_TP_MCU_STM32L4
#include "stm32l4xx.h"
#elif defined (SYS_TP_MCU_STM32L0)
#include "stm32l0xx.h"
#elif defined(SYS_TP_MCU_STM32U5)
#include "stm32u5xx.h"
#elif defined(SYS_TP_MCU_STM32WB)
#include "stm32wbxx.h"
#else
#error "no target platform defined in the project options."
#endif

/* Compiler specific define */
/****************************/

/*
 * GCC, IAR and KEIL compiler support the inline keyword, so we do not need to redefine it
 * for each compiler. For example, in teh past, IAR used the #pragma inline directive.
 * We decided to keep the SYS_DEFINE_INLINE macro for backward compatibility.
 *
 */
#define SYS_DEFINE_INLINE inline

/*
 *
 * This section defines some symbol specific to the STM32L4 memory map.
 * See bugtabs4 #5265
 */
#ifdef SYS_TP_MCU_STM32L4
#ifdef STM32L431xx

#define SKP_PRWR_SCR_CWUF_1_5    0x1FU
#define SKP_FLASH_SR_OPTVERR     (0x1U << (15U))

#endif
#endif

#endif /* SYSTARGETPLATFORM_H_ */

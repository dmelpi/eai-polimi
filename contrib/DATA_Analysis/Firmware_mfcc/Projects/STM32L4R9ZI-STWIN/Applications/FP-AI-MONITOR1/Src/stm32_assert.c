/**
 ******************************************************************************
 * @file    stm32_assert.c
 * @author  STMicroelectronics - AIS - MCD Team
 * @version V1.0.0
 * @date    15-September-2021
 * @brief
 *
 * TODO - insert here the file description
 *
 *********************************************************************************
 * @attention
 *
 * Copyright (c) 2021 STMicroelectronics
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file in
 * the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *********************************************************************************
 */

#include "services/syserror.h"

void assert_failed(uint8_t* file, uint32_t line) {
	sys_error_handler();
}


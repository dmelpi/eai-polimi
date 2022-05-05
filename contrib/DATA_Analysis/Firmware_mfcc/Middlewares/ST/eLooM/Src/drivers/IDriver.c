/**
 ******************************************************************************
 * @file    IDriver.c
 * @author  STMicroelectronics - ST-Korea - MCD Team
 * @version 3.0.0
 * @date    Mar 20, 2017
 * @brief
 *
 * <DESCRIPTIOM>
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

#include "drivers/IDriver.h"
#include "drivers/IDriver_vtbl.h"

// GCC requires one function forward declaration in only one .c source
// in order to manage the inline.
// See also http://stackoverflow.com/questions/26503235/c-inline-function-and-gcc
#if defined (__GNUC__) || defined (__ICCARM__)
extern sys_error_code_t IDrvInit(IDriver *_this, void *pParams);
extern sys_error_code_t IDrvStart(IDriver *_this);
extern sys_error_code_t IDrvStop(IDriver *_this);
extern sys_error_code_t IDrvDoEnterPowerMode(IDriver *this, const EPowerMode eActivePowerMode, const EPowerMode eNewPowerMode);
extern sys_error_code_t IDrvReset(IDriver *_this, void *pParams);
#endif

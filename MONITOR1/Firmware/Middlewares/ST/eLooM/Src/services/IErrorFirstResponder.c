/**
 ******************************************************************************
 * @file    IErrorFirstResponder.c
 * @author  STMicroelectronics - ST-Korea - MCD Team
 * @version 3.0.0
 * @date    Aug 12, 2017
 *
 * @brief
 *
 * <DESCRIPTIOM>
 *
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2017 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file in
 * the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 ******************************************************************************
 */

#include "services/IErrorFirstResponder.h"
#include "services/IErrorFirstResponderVtbl.h"

#if defined (__GNUC__) || defined (__ICCARM__)

extern void IErrFirstResponderSetOwner(IErrFirstResponder *this, void *pxOwner);
extern void *IErrFirstResponderGetOwner(IErrFirstResponder *this);
sys_error_code_t IErrorFirstResponderNewError(IErrFirstResponder *this, SysEvent xError, boolean_t bIsCalledFromISR);

#endif

// Private member function declaration
// ***********************************


// Public API definition
// *********************


// Private function definition
// ***************************

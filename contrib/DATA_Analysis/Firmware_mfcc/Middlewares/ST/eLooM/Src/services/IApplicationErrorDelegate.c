/**
 ******************************************************************************
 * @file    IApplicationErrorDelegate.c
 * @author  STMicroelectronics - ST-Korea - MCD Team
 * @version 3.0.0
 * @date    Aug 4, 2017
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

#include "services/IApplicationErrorDelegate.h"
#include "services/IApplicationErrorDelegateVtbl.h"

// GCC requires one function forward declaration in only one .c source
// in order to manage the inline.
// See also http://stackoverflow.com/questions/26503235/c-inline-function-and-gcc
#if defined (__GNUC__) || defined (__ICCARM__)
extern sys_error_code_t IAEDInit(IApplicationErrorDelegate *this, void *pParams);
extern sys_error_code_t IAEDOnStartApplication(IApplicationErrorDelegate *this, ApplicationContext *pxContext);
extern sys_error_code_t IAEDProcessEvent(IApplicationErrorDelegate *this, ApplicationContext *pxContext, SysEvent xEvent);
extern sys_error_code_t IAEDOnNewErrEvent(IApplicationErrorDelegate *this, SysEvent xEvent);
extern boolean_t IAEDIsLastErrorPending(IApplicationErrorDelegate *this);
extern sys_error_code_t IAEDAddFirstResponder(IApplicationErrorDelegate *this, IErrFirstResponder *pFirstResponder, uint8_t nPriority);
extern sys_error_code_t IAEDRemoveFirstResponder(IApplicationErrorDelegate *this, IErrFirstResponder *pFirstResponder);
extern uint8_t IAEDGetMaxFirstResponderPriority(const IApplicationErrorDelegate *this);
extern void IAEDResetCounter(IApplicationErrorDelegate *this);
#endif


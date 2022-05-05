/**
 ******************************************************************************
 * @file    NullErrorDelegate.c
 * @author  STMicroelectronics - ST-Korea - MCD Team
 * @version 3.0.0
 * @date    Nov 14, 2017
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

#include "services/NullErrorDelegate.h"

// Private member function declaration
// ***********************************


// Inline function farward declaration
// ***********************************

#if defined (__GNUC__) || defined (__ICCARM__)
extern sys_error_code_t NullAEDInit(IApplicationErrorDelegate *this, void *pParams);
extern sys_error_code_t NullAEDOnStartApplication(IApplicationErrorDelegate *this, ApplicationContext *pxContext);
extern sys_error_code_t NullAEDProcessEvent(IApplicationErrorDelegate *this, ApplicationContext *pxContext, SysEvent xEvent);
extern sys_error_code_t NullAEDOnNewErrEvent(IApplicationErrorDelegate *this, SysEvent xEvent);
extern boolean_t NullAEDIsLastErrorPending(IApplicationErrorDelegate *this);
extern sys_error_code_t NullAEDAddFirstResponder(IApplicationErrorDelegate *this, IErrFirstResponder *pFirstResponder, uint8_t nPriority);
extern sys_error_code_t NullAEDRemoveFirstResponder(IApplicationErrorDelegate *this, IErrFirstResponder *pFirstResponder);
extern uint8_t NullAEDGetMaxFirstResponderPriority(const IApplicationErrorDelegate *this);
extern void NullAEDResetCounter(IApplicationErrorDelegate *this);
#endif


// Public API definition
// *********************

IApplicationErrorDelegate *NullAEDAlloc(void) {
  static IApplicationErrorDelegate_vtbl s_xNullAED_vtbl = {
      NullAEDInit,
      NullAEDOnStartApplication,
      NullAEDProcessEvent,
      NullAEDOnNewErrEvent,
      NullAEDIsLastErrorPending,
      NullAEDAddFirstResponder,
      NullAEDRemoveFirstResponder,
      NullAEDGetMaxFirstResponderPriority,
      NullAEDResetCounter
  };

  static NullErrorDelegate s_xNullAED;
  s_xNullAED.super.vptr = &s_xNullAED_vtbl;

  return (IApplicationErrorDelegate*) &s_xNullAED;
}

// Private function definition
// ***************************

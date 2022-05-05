/**
 ******************************************************************************
 * @file    IBoot.c
 * @author  STMicroelectronics - ST-Korea - MCD Team
 * @version 3.0.0
 * @date    Nov 22, 2017
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

#include "services/IBoot.h"
#include "services/IBootVtbl.h"

// GCC requires one function forward declaration in only one .c source
// in order to manage the inline.
// See also http://stackoverflow.com/questions/26503235/c-inline-function-and-gcc
#if defined (__GNUC__) || defined (__ICCARM__)
extern sys_error_code_t IBootInit(IBoot *this);
extern boolean_t IBootCheckDFUTrigger(IBoot *this);
extern uint32_t IBootGetAppAdderss(IBoot *this);
extern sys_error_code_t IBootOnJampToApp(IBoot *this, uint32_t nAppDress);
#endif

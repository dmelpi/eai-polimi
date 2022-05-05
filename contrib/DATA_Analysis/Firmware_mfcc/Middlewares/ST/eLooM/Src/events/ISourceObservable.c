/**
 ******************************************************************************
 * @file    ISourceObservable.c
 * @author  STMicroelectronics - AIS - MCD Team
 * @version 3.0.0
 * @date    Jun 8, 2021
 *
 * @brief   Stream data source generic interface.
 *
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file in
 * the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 ******************************************************************************
 */

#include "events/ISourceObservable.h"
#include "events/ISourceObservable_vtbl.h"


// GCC requires one function forward declaration in only one .c source
// in order to manage the inline.
// See also http://stackoverflow.com/questions/26503235/c-inline-function-and-gcc
#if defined (__GNUC__) || defined (__ICCARM__)
extern uint8_t ISourceGetId(ISourceObservable *_this);
extern IEventSrc     *  ISourceGetEventSrcIF(ISourceObservable *_this);
extern sys_error_code_t ISourceGetODR(ISourceObservable *_this, float *p_measured, float *p_nominal);
extern float ISourceGetFS(ISourceObservable *_this);
extern float ISourceGetSensitivity(ISourceObservable *_this);
#endif

/**
 ******************************************************************************
 * @file    ABusIF.c
 * @author  SRA - MCD
 * @version 1.0.0
 * @date    6-Sep-2021
 *
 * @brief
 *
 * <DESCRIPTIOM>
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

#include "ABusIF.h"


// GCC requires one function forward declaration in only one .c source
// in order to manage the inline.
// See also http://stackoverflow.com/questions/26503235/c-inline-function-and-gcc
#if defined (__GNUC__) || defined (__ICCARM__)
extern sys_error_code_t ABusIFInit(ABusIF *_this);
extern sys_error_code_t ABusIFSetHandle(ABusIF *_this, void *pxHandle);
extern void *ABusIFGetHandle(const ABusIF *_this);
#endif

int32_t ABusIFNullRW(void *pxSensor, uint8_t nRegAddr, uint8_t* pnData, uint16_t nSize) {
  UNUSED(pxSensor);
  UNUSED(nRegAddr);
  UNUSED(pnData);
  UNUSED(nSize);

  return 0;
}

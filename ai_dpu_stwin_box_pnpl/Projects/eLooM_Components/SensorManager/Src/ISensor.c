/**
  ******************************************************************************
  * @file    ISensor.c
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

#include "ISensor.h"
#include "ISensor_vtbl.h"

// GCC requires one function forward declaration in only one .c source
// in order to manage the inline.
// See also http://stackoverflow.com/questions/26503235/c-inline-function-and-gcc
#if defined (__GNUC__) || defined(__ICCARM__)
extern sys_error_code_t ISensorStart(ISensor_t *_this);
extern sys_error_code_t ISensorStop(ISensor_t *_this);
extern sys_error_code_t ISensorSetODR(ISensor_t *_this, float ODR);
extern sys_error_code_t ISensorSetFS(ISensor_t *_this, float FS);
extern sys_error_code_t ISensorEnable(ISensor_t *_this);
extern sys_error_code_t ISensorDisable(ISensor_t *_this);
extern boolean_t ISensorIsEnabled(ISensor_t *_this);
extern SensorDescriptor_t ISensorGetDescription(ISensor_t *_this);
extern SensorStatus_t ISensorGetStatus(ISensor_t *_this);
#endif

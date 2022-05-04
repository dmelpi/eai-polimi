/**
 ******************************************************************************
 * @file   IDPU.c
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
#include "IDPU.h"
#include "IDPUVtbl.h"


// GCC requires one function forward declaration in only one .c source
// in order to manage the inline.
// See also http://stackoverflow.com/questions/26503235/c-inline-function-and-gcc
#if defined (__GNUC__) || defined (__ICCARM__)
extern sys_error_code_t IDPU_Init(IDPU *_this);
extern sys_error_code_t IDPU_AttachToSensor(IDPU *_this, ISourceObservable *s, void *buffer);
extern sys_error_code_t IDPU_DetachFromSensor(IDPU *_this, ISourceObservable *s);
extern sys_error_code_t IDPU_AttachInputDPU(IDPU *_this, IDPU *in_adpu, void *buffer);
extern sys_error_code_t IDPU_DetachFromDPU(IDPU *_this);
extern sys_error_code_t IDPU_DispatchEvents(IDPU *_this,  ProcessEvent *pxEvt);
extern sys_error_code_t IDPU_RegisterNotifyCallback(IDPU *_this, DPU_ReadyToProcessCallback_t callback, void *p_param);
extern sys_error_code_t IDPU_Process(IDPU *_this);
#endif

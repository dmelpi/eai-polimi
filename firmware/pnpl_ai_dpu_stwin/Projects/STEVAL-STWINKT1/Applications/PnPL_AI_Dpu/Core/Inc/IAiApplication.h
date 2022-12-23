/**
 ******************************************************************************
 * @file    IAiApplication.h
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

#ifndef INCLUDE_IAIAPPLICATION_H_
#define INCLUDE_IAIAPPLICATION_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "services/syserror.h"

/**
 * Create  type name for IApplication.
 */
typedef struct _IAiApplication_t IAiApplication_t;

// Public API declaration
//***********************
/** Public interface **/
inline sys_error_code_t IAiApplication_stop(IAiApplication_t *_this);
inline sys_error_code_t IAiApplication_start(IAiApplication_t *_this);
inline sys_error_code_t IAiApplication_switch_bank(IAiApplication_t *_this);

#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_IAIAPPLICATION_H_ */

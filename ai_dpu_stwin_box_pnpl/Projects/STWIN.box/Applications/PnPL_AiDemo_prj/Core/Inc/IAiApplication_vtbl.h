/**
 ******************************************************************************
 * @file    IAiApplication_vtbl.h
 * @author  SRA - MCD
 * @version 1.0.0
 * @date    09-jun-2022
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

#ifndef INCLUDE_IAIAPPLICATION_VTBL_H_
#define INCLUDE_IAIAPPLICATION_VTBL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "services/syserror.h"

/**
 * Create a type name for IAiApplication_vtbl.
 */
typedef struct _IAiApplication_vtbl IAiApplication_vtbl;

struct _IAiApplication_vtbl
{
  sys_error_code_t (*ai_application_start)(IAiApplication_t *_this);
  sys_error_code_t (*ai_application_stop)(IAiApplication_t *_this);
  sys_error_code_t (*ai_application_switch_bank)(IAiApplication_t *_this);
};

struct _IAiApplication_t
{
  /**
   * Pointer to the virtual table for the class.
   */
  const IAiApplication_vtbl *vptr;
};

// Inline functions definition
// ***************************

inline sys_error_code_t IAiApplication_start(IAiApplication_t *_this)
{
  return _this->vptr->ai_application_start(_this);
}

inline sys_error_code_t IAiApplication_stop(IAiApplication_t *_this)
{
  return _this->vptr->ai_application_stop(_this);
}

inline sys_error_code_t IAiApplication_switch_bank(IAiApplication_t *_this)
{
  return _this->vptr->ai_application_switch_bank(_this);
}

#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_IAIAPPLICATION_VTBL_H_ */

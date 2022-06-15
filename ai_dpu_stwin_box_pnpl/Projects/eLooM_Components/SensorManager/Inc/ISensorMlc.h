/**
  ******************************************************************************
  * @file    ISensorMlc.h
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

#ifndef INCLUDE_ISENSORMLC_H_
#define INCLUDE_ISENSORMLC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "services/systypes.h"
#include "services/syserror.h"
#include "services/systp.h"
#include "SensorDef.h"

/**
  * Create  type name for ISensorMlc.
  */
typedef struct _ISensorMlc_t ISensorMlc_t;

// Public API declaration
//***********************
/** Public interface **/
inline sys_error_code_t ISensorMlcLoadUcf(ISensorMlc_t *_this, uint32_t size, const char *ucf);
inline boolean_t ISensorMlcIsEnabled(ISensorMlc_t *_this);

#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_ISENSORMLC_H_ */

/**
 ******************************************************************************
 * @file    ISensor.h
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
 
#ifndef INCLUDE_ISENSOR_H_
#define INCLUDE_ISENSOR_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "services/systypes.h"
#include "services/syserror.h"
#include "services/systp.h"
#include "SensorDef.h"
#include "events/ISourceObservable.h"
  

/**
 * Create  type name for ISensor.
 */
typedef struct _ISensor_t ISensor_t;


// Public API declaration
//***********************
/** Public interface **/
inline sys_error_code_t ISensorStart(ISensor_t *_this);
inline sys_error_code_t ISensorStop(ISensor_t *_this);
inline sys_error_code_t ISensorSetODR(ISensor_t *_this, float ODR);
inline sys_error_code_t ISensorSetFS(ISensor_t *_this, float FS);
inline sys_error_code_t ISensorEnable(ISensor_t *_this);
inline sys_error_code_t ISensorDisable(ISensor_t *_this);
inline boolean_t ISensorIsEnabled(ISensor_t *_this);
inline SensorDescriptor_t ISensorGetDescription(ISensor_t *_this);


#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_ISENSOR_H_ */

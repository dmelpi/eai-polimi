/**
  ******************************************************************************
  * @file    IMP23ABSUTask.h
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
#ifndef IMP23ABSUTASK_H_
#define IMP23ABSUTASK_H_

#ifdef __cplusplus
extern "C" {
#endif



#include "services/systp.h"
#include "services/syserror.h"
#include "services/AManagedTaskEx.h"
#include "services/AManagedTaskEx_vtbl.h"
#include "events/SensorEventSrc.h"
#include "events/SensorEventSrc_vtbl.h"
#include "ISensor.h"
#include "ISensor_vtbl.h"

/**
  * Create  type name for _IMP23ABSUTask.
  */
typedef struct _IMP23ABSUTask IMP23ABSUTask;




// Public API declaration
//***********************

/**
  * Get the ISourceObservable interface for the accelerometer.
  * @param _this [IN] specifies a pointer to a task object.
  * @return a pointer to the generic object ::ISourceObservable if success,
  * or NULL if out of memory error occurs.
  */
ISourceObservable *IMP23ABSUTaskGetMicSensorIF(IMP23ABSUTask *_this);

/**
  * Allocate an instance of IMP23ABSUTask.
  *
  * @param p_mx_mdf_cfg [IN] specifies a ::MX_MDFParams_t instance declared in the mx.h file.
  * @param p_mx_adc_cfg [IN] specifies a ::MX_ADCParams_t instance declared in the mx.h file.
  * @return a pointer to the generic object ::AManagedTaskEx if success,
  * or NULL if out of memory error occurs.
  */
AManagedTaskEx *IMP23ABSUTaskAlloc(const void *p_mx_mdf_cfg, const void *p_mx_adc_cfg);

IEventSrc *IMP23ABSUTaskGetEventSrcIF(IMP23ABSUTask *_this);

// Inline functions definition
// ***************************


#ifdef __cplusplus
}
#endif

#endif /* IMP23ABSUTASK_H_ */

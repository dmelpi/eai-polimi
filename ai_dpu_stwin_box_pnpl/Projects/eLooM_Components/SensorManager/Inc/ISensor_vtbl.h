/**
  ******************************************************************************
  * @file    ISensor_vtbl.h
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

#ifndef INCLUDE_ISENSOR_VTBL_H_
#define INCLUDE_ISENSOR_VTBL_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "events/ISourceObservable.h"
#include "events/ISourceObservable_vtbl.h"

/**
  * Create a type name for ISensor_vtbl.
  */
typedef struct _ISensor_vtbl ISensor_vtbl;


struct _ISensor_vtbl
{
  uint8_t (*GetId)(ISourceObservable *_this);
  IEventSrc          *(*GetEventSourceIF)(ISourceObservable *_this);
  sys_error_code_t (*SensorGetODR)(ISourceObservable *_this, float *p_measured, float *p_nominal);
  float (*SensorGetFS)(ISourceObservable *_this);
  float (*SensorGetSensitivity)(ISourceObservable *_this);
  uint8_t (*SensorGetDataType)(ISourceObservable *_this);
  uint16_t (*SensorGetDimensions)(ISourceObservable *_this);
  sys_error_code_t (*SensorSetODR)(ISensor_t *_this, float ODR);
  sys_error_code_t (*SensorSetFS)(ISensor_t *_this, float FS);
  sys_error_code_t (*SensorEnable)(ISensor_t *_this);
  sys_error_code_t (*SensorDisable)(ISensor_t *_this);
  boolean_t (*SensorIsEnabled)(ISensor_t *_this);
  SensorDescriptor_t (*SensorGetDescription)(ISensor_t *_this);
  SensorStatus_t (*SensorGetStatus)(ISensor_t *_this);
};


struct _ISensor_t
{
  /**
    * Pointer to the virtual table for the class.
    */
  const ISensor_vtbl *vptr;
};


// Inline functions definition
// ***************************

inline sys_error_code_t ISensorSetODR(ISensor_t *_this, float ODR)
{
  return _this->vptr->SensorSetODR(_this, ODR);
}

inline sys_error_code_t ISensorSetFS(ISensor_t *_this, float FS)
{
  return _this->vptr->SensorSetFS(_this, FS);
}

inline sys_error_code_t ISensorEnable(ISensor_t *_this)
{
  return _this->vptr->SensorEnable(_this);
}

inline sys_error_code_t ISensorDisable(ISensor_t *_this)
{
  return _this->vptr->SensorDisable(_this);
}

inline boolean_t ISensorIsEnabled(ISensor_t *_this)
{
  return _this->vptr->SensorIsEnabled(_this);
}

inline SensorDescriptor_t ISensorGetDescription(ISensor_t *_this)
{
  return _this->vptr->SensorGetDescription(_this);
}

inline SensorStatus_t ISensorGetStatus(ISensor_t *_this)
{
  return _this->vptr->SensorGetStatus(_this);
}


#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_ISENSOR_VTBL_H_ */

/**
  ******************************************************************************
  * @file    ISensorMlc_vtbl.h
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

#ifndef INCLUDE_ISENSORMLC_VTBL_H_
#define INCLUDE_ISENSORMLC_VTBL_H_

#ifdef __cplusplus
extern "C" {
#endif


/**
  * Create a type name for ISensorMlc_vtbl.
  */
typedef struct _ISensorMlc_vtbl ISensorMlc_vtbl;


struct _ISensorMlc_vtbl
{
  sys_error_code_t (*SensorMlcLoadUcf)(ISensorMlc_t *_this, uint32_t size, const char *ucf);
  boolean_t (*SensorMlcIsEnabled)(ISensorMlc_t *_this);
};


struct _ISensorMlc_t
{
  /**
    * Pointer to the virtual table for the class.
    */
  const ISensorMlc_vtbl *vptr;
};


// Inline functions definition
// ***************************
static inline sys_error_code_t ISensorMlcLoadUcf(ISensorMlc_t *_this, uint32_t size, const char *ucf)
{
  return _this->vptr->SensorMlcLoadUcf(_this, size, ucf);
}

static inline boolean_t ISensorMlcIsEnabled(ISensorMlc_t *_this)
{
  return _this->vptr->SensorMlcIsEnabled(_this);
}


#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_ISENSORMLC_VTBL_H_ */

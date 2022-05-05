/**
 ******************************************************************************
 * @file    SensorRegister.h
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
#ifndef HSDCORE_INC_SENSORREGISTER_H_
#define HSDCORE_INC_SENSORREGISTER_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "services/systp.h"
#include "services/syserror.h"
#include "ISensor.h"
#include "ISensor_vtbl.h"


  /* Public API declaration */
  /**************************/
  uint8_t SMAddSensor(ISensor_t *pSensor);


  /* Inline functions definition */
  /*******************************/


#ifdef __cplusplus
}
#endif

#endif /* HSDCORE_INC_SENSORREGISTER_H_ */

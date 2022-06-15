/**
  ******************************************************************************
  * @file    SensorEventSrc_vtbl.h
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
#ifndef INCLUDE_EVENTS_SENSOREVENTSRC_VTBL_H_
#define INCLUDE_EVENTS_SENSOREVENTSRC_VTBL_H_

#ifdef __cplusplus
extern "C" {
#endif


// Inline functions definition
// ***************************

// IEventSrc virtual functions
sys_error_code_t SensorEventSrc_vtblSendEvent(const IEventSrc *this, const IEvent *pxEvent, void *pvParams);


#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_EVENTS_SENSOREVENTSRC_VTBL_H_ */

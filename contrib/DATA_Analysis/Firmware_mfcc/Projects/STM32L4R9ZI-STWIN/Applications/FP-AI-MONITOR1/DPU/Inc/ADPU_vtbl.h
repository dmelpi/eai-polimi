/**
 ******************************************************************************
 * @file    ADPU_vtbl.h
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
 
#ifndef INCLUDE_ADPU_VTBL_H_
#define INCLUDE_ADPU_VTBL_H_

#ifdef __cplusplus
extern "C" {
#endif


  // IDPU virtual functions
  sys_error_code_t ADPU_Init_vtbl(IDPU *_this);
  sys_error_code_t ADPU_AttachToSensor_vtbl(IDPU *_this, ISourceObservable *s, void *buffer);
  sys_error_code_t ADPU_DetachFromSensor_vtbl(IDPU *_this, ISourceObservable *s);
  sys_error_code_t ADPU_AttachInputADPU_vtbl(IDPU *_this, IDPU *adpu, void *buffer);
  sys_error_code_t ADPU_DetachFromADPU_vtbl(IDPU *_this);
  sys_error_code_t ADPU_DispatchEvents_vtbl(IDPU *_this,  ProcessEvent *pxEvt);
  sys_error_code_t ADPU_RegisterNotifyCallbacks_vtbl(IDPU *_this, DPU_ReadyToProcessCallback_t callback, void *p_param);
//  sys_error_code_t ADPU_NotifyDPUDataReady_vtbl(IDPU *_this,  ProcessEvent *pxEvt);


  // IEventListener virtual functions
  void *ADPU_GetOwner_vtbl(IEventListener *_this);
  void ADPU_SetOwner_vtbl(IEventListener *_this, void *pxOwner);

  // ISensorEventListener virtual functions
  sys_error_code_t ADP_OnNewDataReady_vtbl(IEventListener *_this, const SensorEvent *pxEvt);


#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_ADPU_VTBL_H_ */

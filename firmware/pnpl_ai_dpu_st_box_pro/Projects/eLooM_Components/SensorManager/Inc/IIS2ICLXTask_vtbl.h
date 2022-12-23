/**
  ******************************************************************************
  * @file    IIS2ICLXTask_vtbl.h
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
#ifndef IIS2ICLXTASK_VTBL_H_
#define IIS2ICLXTASK_VTBL_H_

#ifdef __cplusplus
extern "C" {
#endif




/* AManagedTask virtual functions */
sys_error_code_t IIS2ICLXTask_vtblHardwareInit(AManagedTask *_this, void *pParams); ///< @sa AMTHardwareInit
sys_error_code_t IIS2ICLXTask_vtblOnCreateTask(AManagedTask *_this, tx_entry_function_t *pvTaskCode, CHAR **pcName,
                                               VOID **pvStackStart, ULONG *pnStackSize, UINT *pnPriority, UINT *pnPreemptThreshold, ULONG *pnTimeSlice,
                                               ULONG *pnAutoStart, ULONG *pnParams); ///< @sa AMTOnCreateTask
sys_error_code_t IIS2ICLXTask_vtblDoEnterPowerMode(AManagedTask *_this, const EPowerMode ActivePowerMode,
                                                   const EPowerMode NewPowerMode); ///< @sa AMTDoEnterPowerMode
sys_error_code_t IIS2ICLXTask_vtblHandleError(AManagedTask *_this, SysEvent Error); ///< @sa AMTHandleError
sys_error_code_t IIS2ICLXTask_vtblOnEnterTaskControlLoop(AManagedTask *this); ///< @sa AMTOnEnterTaskControlLoop

/* AManagedTaskEx virtual functions */
sys_error_code_t IIS2ICLXTask_vtblForceExecuteStep(AManagedTaskEx *_this,
                                                   EPowerMode ActivePowerMode); ///< @sa AMTExForceExecuteStep
sys_error_code_t IIS2ICLXTask_vtblOnEnterPowerMode(AManagedTaskEx *_this, const EPowerMode ActivePowerMode,
                                                   const EPowerMode NewPowerMode); ///< @sa AMTExOnEnterPowerMode

uint8_t IIS2ICLXTask_vtblAccGetId(ISourceObservable *_this);
IEventSrc *IIS2ICLXTask_vtblGetEventSourceIF(ISourceObservable *_this);
sys_error_code_t IIS2ICLXTask_vtblAccGetODR(ISourceObservable *_this, float *p_measured, float *p_nominal);
float IIS2ICLXTask_vtblAccGetFS(ISourceObservable *_this);
float IIS2ICLXTask_vtblAccGetSensitivity(ISourceObservable *_this);
uint8_t IIS2ICLXTask_vtblAccGetDataType(ISourceObservable *_this);
uint16_t IIS2ICLXTask_vtblAccGetDimensions(ISourceObservable *_this);

sys_error_code_t IIS2ICLXTask_vtblSensorSetODR(ISensor_t *_this, float ODR);
sys_error_code_t IIS2ICLXTask_vtblSensorSetFS(ISensor_t *_this, float FS);
sys_error_code_t IIS2ICLXTask_vtblSensorEnable(ISensor_t *_this);
sys_error_code_t IIS2ICLXTask_vtblSensorDisable(ISensor_t *_this);
boolean_t IIS2ICLXTask_vtblSensorIsEnabled(ISensor_t *_this);
SensorDescriptor_t IIS2ICLXTask_vtblSensorGetDescription(ISensor_t *_this);
SensorStatus_t IIS2ICLXTask_vtblSensorGetStatus(ISensor_t *_this);

#ifdef __cplusplus
}
#endif

#endif /* IIS2ICLXTASK_VTBL_H_ */

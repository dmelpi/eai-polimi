/**
 ******************************************************************************
 * @file    IMP23ABSUTask_vtbl.h
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
#ifndef IMP23ABSUTASK_VTBL_H_
#define IMP23ABSUTASK_VTBL_H_

#ifdef __cplusplus
extern "C" {
#endif



/* AManagedTask virtual functions */
sys_error_code_t IMP23ABSUTask_vtblHardwareInit(AManagedTask *_this, void *pParams); ///< @sa AMTHardwareInit
sys_error_code_t IMP23ABSUTask_vtblOnCreateTask(AManagedTask *_this, tx_entry_function_t *pvTaskCode, CHAR **pcName, VOID **pvStackStart, ULONG *pnStackSize, UINT *pnPriority, UINT *pnPreemptThreshold, ULONG *pnTimeSlice, ULONG *pnAutoStart, ULONG *pnParams); ///< @sa AMTOnCreateTask
sys_error_code_t IMP23ABSUTask_vtblDoEnterPowerMode(AManagedTask *_this, const EPowerMode ActivePowerMode, const EPowerMode NewPowerMode); ///< @sa AMTDoEnterPowerMode
sys_error_code_t IMP23ABSUTask_vtblHandleError(AManagedTask *_this, SysEvent Error); ///< @sa AMTHandleError
sys_error_code_t IMP23ABSUTask_vtblOnEnterTaskControlLoop(AManagedTask *this); ///< @sa AMTOnEnterTaskControlLoop

/* AManagedTaskEx virtual functions */
sys_error_code_t IMP23ABSUTask_vtblForceExecuteStep(AManagedTaskEx *_this, EPowerMode ActivePowerMode); ///< @sa AMTExForceExecuteStep
sys_error_code_t IMP23ABSUTask_vtblOnEnterPowerMode(AManagedTaskEx *_this, const EPowerMode ActivePowerMode, const EPowerMode NewPowerMode); ///< @sa AMTExOnEnterPowerMode

uint8_t IMP23ABSUTask_vtblMicGetId(ISourceObservable *_this);
IEventSrc *IMP23ABSUTask_vtblGetEventSourceIF(ISourceObservable *_this);
sys_error_code_t IMP23ABSUTask_vtblMicGetODR(ISourceObservable *_this, float *p_measured, float *p_nominal);
float IMP23ABSUTask_vtblMicGetFS(ISourceObservable *_this);
float IMP23ABSUTask_vtblMicGetSensitivity(ISourceObservable *_this);
uint8_t IMP23ABSUTask_vtblMicGetDataType(ISourceObservable *_this);
uint16_t IMP23ABSUTask_vtblMicGetDimensions(ISourceObservable *_this);

sys_error_code_t IMP23ABSUTask_vtblSensorSetODR(ISensor_t *_this, float ODR);
sys_error_code_t IMP23ABSUTask_vtblSensorSetFS(ISensor_t *_this, float FS);
sys_error_code_t IMP23ABSUTask_vtblSensorEnable(ISensor_t *_this);
sys_error_code_t IMP23ABSUTask_vtblSensorDisable(ISensor_t *_this);
boolean_t IMP23ABSUTask_vtblSensorIsEnabled(ISensor_t *_this);
SensorDescriptor_t IMP23ABSUTask_vtblSensorGetDescription(ISensor_t *_this);
SensorStatus_t IMP23ABSUTask_vtblSensorGetStatus(ISensor_t *_this);

#ifdef __cplusplus
}
#endif

#endif /* IMP23ABSUTASK_VTBL_H_ */

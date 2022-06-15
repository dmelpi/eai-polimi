/**
  ******************************************************************************
  * @file    IMP34DT05Task_vtbl.h
  * @author  SRA - MCD
  * @version 1.0.0
  * @date    30-Jul-2021
  *
  * @brief
  *
  * <DESCRIPTIOM>
  *
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
#ifndef IMP34DT05TASK_VTBL_H_
#define IMP34DT05TASK_VTBL_H_

#ifdef __cplusplus
extern "C" {
#endif



/* AManagedTask virtual functions */
sys_error_code_t IMP34DT05Task_vtblHardwareInit(AManagedTask *_this, void *pParams); ///< @sa AMTHardwareInit
sys_error_code_t IMP34DT05Task_vtblOnCreateTask(AManagedTask *_this, tx_entry_function_t *pvTaskCode, CHAR **pcName,
                                                VOID **pvStackStart, ULONG *pnStackSize, UINT *pnPriority, UINT *pnPreemptThreshold, ULONG *pnTimeSlice,
                                                ULONG *pnAutoStart, ULONG *pnParams); ///< @sa AMTOnCreateTask
sys_error_code_t IMP34DT05Task_vtblDoEnterPowerMode(AManagedTask *_this, const EPowerMode ActivePowerMode,
                                                    const EPowerMode NewPowerMode); ///< @sa AMTDoEnterPowerMode
sys_error_code_t IMP34DT05Task_vtblHandleError(AManagedTask *_this, SysEvent Error); ///< @sa AMTHandleError
sys_error_code_t IMP34DT05Task_vtblOnEnterTaskControlLoop(AManagedTask *this); ///< @sa AMTOnEnterTaskControlLoop

/* AManagedTaskEx virtual functions */
sys_error_code_t IMP34DT05Task_vtblForceExecuteStep(AManagedTaskEx *_this,
                                                    EPowerMode ActivePowerMode); ///< @sa AMTExForceExecuteStep
sys_error_code_t IMP34DT05Task_vtblOnEnterPowerMode(AManagedTaskEx *_this, const EPowerMode ActivePowerMode,
                                                    const EPowerMode NewPowerMode); ///< @sa AMTExOnEnterPowerMode

uint8_t IMP34DT05Task_vtblMicGetId(ISourceObservable *_this);
IEventSrc *IMP34DT05Task_vtblGetEventSourceIF(ISourceObservable *_this);
sys_error_code_t IMP34DT05Task_vtblMicGetODR(ISourceObservable *_this, float *p_measured, float *p_nominal);
float IMP34DT05Task_vtblMicGetFS(ISourceObservable *_this);
float IMP34DT05Task_vtblMicGetSensitivity(ISourceObservable *_this);
uint8_t IMP34DT05Task_vtblMicGetDataType(ISourceObservable *_this);
uint16_t IMP34DT05Task_vtblMicGetDimensions(ISourceObservable *_this);

sys_error_code_t IMP34DT05Task_vtblSensorSetODR(ISensor_t *_this, float ODR);
sys_error_code_t IMP34DT05Task_vtblSensorSetFS(ISensor_t *_this, float FS);
sys_error_code_t IMP34DT05Task_vtblSensorEnable(ISensor_t *_this);
sys_error_code_t IMP34DT05Task_vtblSensorDisable(ISensor_t *_this);
boolean_t IMP34DT05Task_vtblSensorIsEnabled(ISensor_t *_this);
SensorDescriptor_t IMP34DT05Task_vtblSensorGetDescription(ISensor_t *_this);
SensorStatus_t IMP34DT05Task_vtblSensorGetStatus(ISensor_t *_this);

#ifdef __cplusplus
}
#endif

#endif /* IMP34DT05TASK_VTBL_H_ */

/**
  ******************************************************************************
  * @file    LIS2MDLTask_vtbl.h
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
#ifndef LIS2MDLTASK_VTBL_H_
#define LIS2MDLTASK_VTBL_H_

#ifdef __cplusplus
extern "C" {
#endif




// AManagedTaskEx virtual functions
sys_error_code_t LIS2MDLTask_vtblHardwareInit(AManagedTask *_this, void *pParams); ///< @sa AMTHardwareInit
sys_error_code_t LIS2MDLTask_vtblOnCreateTask(AManagedTask *_this, tx_entry_function_t *pvTaskCode, CHAR **pcName,
                                              VOID **pvStackStart, ULONG *pnStackSize, UINT *pnPriority, UINT *pnPreemptThreshold, ULONG *pnTimeSlice,
                                              ULONG *pnAutoStart, ULONG *pnParams); ///< @sa AMTOnCreateTask
sys_error_code_t LIS2MDLTask_vtblDoEnterPowerMode(AManagedTask *_this, const EPowerMode eActivePowerMode,
                                                  const EPowerMode eNewPowerMode); ///< @sa AMTDoEnterPowerMode
sys_error_code_t LIS2MDLTask_vtblHandleError(AManagedTask *_this, SysEvent xError); ///< @sa AMTHandleError
sys_error_code_t LIS2MDLTask_vtblOnEnterTaskControlLoop(AManagedTask *this); ///< @sa AMTOnEnterTaskControlLoop

/* AManagedTaskEx virtual functions */
sys_error_code_t LIS2MDLTask_vtblForceExecuteStep(AManagedTaskEx *_this,
                                                  EPowerMode ActivePowerMode); ///< @sa AMTExForceExecuteStep
sys_error_code_t LIS2MDLTask_vtblOnEnterPowerMode(AManagedTaskEx *_this, const EPowerMode ActivePowerMode,
                                                  const EPowerMode NewPowerMode); ///< @sa AMTExOnEnterPowerMode

uint8_t LIS2MDLTask_vtblMagGetId(ISourceObservable *_this);
IEventSrc *LIS2MDLTask_vtblMagGetEventSourceIF(ISourceObservable *_this);
sys_error_code_t LIS2MDLTask_vtblMagGetODR(ISourceObservable *_this, float *p_measured, float *p_nominal);
float LIS2MDLTask_vtblMagGetFS(ISourceObservable *_this);
float LIS2MDLTask_vtblMagGetSensitivity(ISourceObservable *_this);
uint8_t LIS2MDLTask_vtblMagGetDataType(ISourceObservable *_this);
uint16_t LIS2MDLTask_vtblMagGetDimensions(ISourceObservable *_this);

sys_error_code_t LIS2MDLTask_vtblSensorSetODR(ISensor_t *_this, float ODR);
sys_error_code_t LIS2MDLTask_vtblSensorSetFS(ISensor_t *_this, float FS);
sys_error_code_t LIS2MDLTask_vtblSensorEnable(ISensor_t *_this);
sys_error_code_t LIS2MDLTask_vtblSensorDisable(ISensor_t *_this);
boolean_t LIS2MDLTask_vtblSensorIsEnabled(ISensor_t *_this);
SensorDescriptor_t LIS2MDLTask_vtblSensorGetDescription(ISensor_t *_this);
SensorStatus_t LIS2MDLTask_vtblSensorGetStatus(ISensor_t *_this);

#ifdef __cplusplus
}
#endif

#endif /* LIS2MDLTASK_VTBL_H_ */

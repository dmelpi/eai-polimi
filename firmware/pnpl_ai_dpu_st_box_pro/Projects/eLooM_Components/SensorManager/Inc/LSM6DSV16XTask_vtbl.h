/**
  ******************************************************************************
  * @file    LSM6DSV16XTask_vtbl.h
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
#ifndef LSM6DSV16XTASK_VTBL_H_
#define LSM6DSV16XTASK_VTBL_H_

#ifdef __cplusplus
extern "C" {
#endif

/* AManagedTask virtual functions */

/**
  * Initialize the hardware resource for the task.
  * This task doesn't need a driver extending the ::IDriver interface because:
  * - it manages two GPIO pins, that are the CS connected to the sensor SPI IF and the EXTI line.
  * - it uses the common sensor driver provided by the ST Sensor Solutions Software Team .
  *
  * @param _this [IN] specifies a task object.
  * @param pParams [IN] specifies task specific parameters. Not used
  * @return SYS_NO_ERROR_CODE if success, an error code otherwise.
  * @sa AMTHardwareInit
  */
sys_error_code_t LSM6DSV16XTask_vtblHardwareInit(AManagedTask *_this, void *pParams);
sys_error_code_t LSM6DSV16XTask_vtblOnCreateTask(AManagedTask *_this, tx_entry_function_t *pvTaskCode, CHAR **pcName,
                                                 VOID **pvStackStart, ULONG *pnStackSize,
                                                 UINT *pnPriority, UINT *pnPreemptThreshold, ULONG *pnTimeSlice, ULONG *pnAutoStart,
                                                 ULONG *pnParams); ///< @sa AMTOnCreateTask
sys_error_code_t LSM6DSV16XTask_vtblDoEnterPowerMode(AManagedTask *_this, const EPowerMode ActivePowerMode,
                                                     const EPowerMode NewPowerMode); ///< @sa AMTDoEnterPowerMode
sys_error_code_t LSM6DSV16XTask_vtblHandleError(AManagedTask *_this, SysEvent Error); ///< @sa AMTHandleError
sys_error_code_t LSM6DSV16XTask_vtblOnEnterTaskControlLoop(AManagedTask *this); ///< @sa AMTOnEnterTaskControlLoop

/* AManagedTaskEx virtual functions */
sys_error_code_t LSM6DSV16XTask_vtblForceExecuteStep(AManagedTaskEx *_this,
                                                     EPowerMode ActivePowerMode); ///< @sa AMTExForceExecuteStep
sys_error_code_t LSM6DSV16XTask_vtblOnEnterPowerMode(AManagedTaskEx *_this, const EPowerMode ActivePowerMode,
                                                     const EPowerMode NewPowerMode); ///< @sa AMTExOnEnterPowerMode

uint8_t LSM6DSV16XTask_vtblAccGetId(ISourceObservable *_this);
IEventSrc *LSM6DSV16XTask_vtblAccGetEventSourceIF(ISourceObservable *_this);
sys_error_code_t LSM6DSV16XTask_vtblAccGetODR(ISourceObservable *_this, float *p_measured, float *p_nominal);
float LSM6DSV16XTask_vtblAccGetFS(ISourceObservable *_this);
float LSM6DSV16XTask_vtblAccGetSensitivity(ISourceObservable *_this);
uint8_t LSM6DSV16XTask_vtblAccGetDataType(ISourceObservable *_this);
uint16_t LSM6DSV16XTask_vtblAccGetDimensions(ISourceObservable *_this);
uint8_t LSM6DSV16XTask_vtblGyroGetId(ISourceObservable *_this);
IEventSrc *LSM6DSV16XTask_vtblGyroGetEventSourceIF(ISourceObservable *_this);
sys_error_code_t LSM6DSV16XTask_vtblGyroGetODR(ISourceObservable *_this, float *p_measured, float *p_nominal);
float LSM6DSV16XTask_vtblGyroGetFS(ISourceObservable *_this);
float LSM6DSV16XTask_vtblGyroGetSensitivity(ISourceObservable *_this);
uint8_t LSM6DSV16XTask_vtblGyroGetDataType(ISourceObservable *_this);
uint16_t LSM6DSV16XTask_vtblGyroGetDimensions(ISourceObservable *_this);

sys_error_code_t LSM6DSV16XTask_vtblSensorSetODR(ISensor_t *_this, float ODR);
sys_error_code_t LSM6DSV16XTask_vtblSensorSetFS(ISensor_t *_this, float FS);
sys_error_code_t LSM6DSV16XTask_vtblSensorEnable(ISensor_t *_this);
sys_error_code_t LSM6DSV16XTask_vtblSensorDisable(ISensor_t *_this);
boolean_t LSM6DSV16XTask_vtblSensorIsEnabled(ISensor_t *_this);
SensorDescriptor_t LSM6DSV16XTask_vtblAccGetDescription(ISensor_t *_this);
SensorDescriptor_t LSM6DSV16XTask_vtblGyroGetDescription(ISensor_t *_this);
SensorStatus_t LSM6DSV16XTask_vtblAccGetStatus(ISensor_t *_this);
SensorStatus_t LSM6DSV16XTask_vtblGyroGetStatus(ISensor_t *_this);

boolean_t LSM6DSV16XTask_vtblSensorMlcIsEnabled(ISensorMlc_t *_this);
sys_error_code_t LSM6DSV16XTask_vtblSensorMlcLoadUcf(ISensorMlc_t *_this, uint32_t size, const char *ucf);

uint8_t LSM6DSV16XTask_vtblMlcGetId(ISourceObservable *_this);
IEventSrc *LSM6DSV16XTask_vtblMlcGetEventSourceIF(ISourceObservable *_this);
sys_error_code_t LSM6DSV16XTask_vtblMlcGetODR(ISourceObservable *_this, float *p_measured, float *p_nominal);
SensorDescriptor_t LSM6DSV16XTask_vtblMlcGetDescription(ISensor_t *_this);
SensorStatus_t LSM6DSV16XTask_vtblMlcGetStatus(ISensor_t *_this);

#ifdef __cplusplus
}
#endif

#endif /* LSM6DSV16XTASK_VTBL_H_ */

/**
  ******************************************************************************
  * @file    ILPS22QSTask_vtbl.h
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
#ifndef ILPS22QSTASK_VTBL_H_
#define ILPS22QSTASK_VTBL_H_

#ifdef __cplusplus
extern "C" {
#endif

// AManagedTaskEx virtual functions

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
sys_error_code_t ILPS22QSTask_vtblHardwareInit(AManagedTask *_this, void *pParams);
sys_error_code_t ILPS22QSTask_vtblOnCreateTask(AManagedTask *_this, tx_entry_function_t *pvTaskCode, CHAR **pcName,
                                               VOID **pvStackStart, ULONG *pnStackSize,
                                               UINT *pnPriority, UINT *pnPreemptThreshold, ULONG *pnTimeSlice, ULONG *pnAutoStart,
                                               ULONG *pnParams); ///< @sa AMTOnCreateTask
sys_error_code_t ILPS22QSTask_vtblDoEnterPowerMode(AManagedTask *_this, const EPowerMode ActivePowerMode,
                                                   const EPowerMode NewPowerMode); ///< @sa AMTDoEnterPowerMode
sys_error_code_t ILPS22QSTask_vtblHandleError(AManagedTask *_this, SysEvent Error); ///< @sa AMTHandleError
sys_error_code_t ILPS22QSTask_vtblOnEnterTaskControlLoop(AManagedTask *this); ///< @sa AMTOnEnterTaskControlLoop

/* AManagedTaskEx virtual functions */
sys_error_code_t ILPS22QSTask_vtblForceExecuteStep(AManagedTaskEx *_this,
                                                   EPowerMode ActivePowerMode); ///< @sa AMTExForceExecuteStep
sys_error_code_t ILPS22QSTask_vtblOnEnterPowerMode(AManagedTaskEx *_this, const EPowerMode ActivePowerMode,
                                                   const EPowerMode NewPowerMode); ///< @sa AMTExOnEnterPowerMode

uint8_t ILPS22QSTask_vtblPressGetId(ISourceObservable *_this);
IEventSrc *ILPS22QSTask_vtblPressGetEventSourceIF(ISourceObservable *_this);
sys_error_code_t ILPS22QSTask_vtblPressGetODR(ISourceObservable *_this, float *p_measured, float *p_nominal);
float ILPS22QSTask_vtblPressGetFS(ISourceObservable *_this);
float ILPS22QSTask_vtblPressGetSensitivity(ISourceObservable *_this);
uint8_t ILPS22QSTask_vtblPressGetDataType(ISourceObservable *_this);
uint16_t ILPS22QSTask_vtblPressGetDimensions(ISourceObservable *_this);

sys_error_code_t ILPS22QSTask_vtblSensorSetODR(ISensor_t *_this, float ODR);
sys_error_code_t ILPS22QSTask_vtblSensorSetFS(ISensor_t *_this, float FS);
sys_error_code_t ILPS22QSTask_vtblSensorEnable(ISensor_t *_this);
sys_error_code_t ILPS22QSTask_vtblSensorDisable(ISensor_t *_this);
boolean_t ILPS22QSTask_vtblSensorIsEnabled(ISensor_t *_this);
SensorDescriptor_t ILPS22QSTask_vtblPressGetDescription(ISensor_t *_this);
SensorStatus_t ILPS22QSTask_vtblPressGetStatus(ISensor_t *_this);

#ifdef __cplusplus
}
#endif

#endif /* ILPS22QSTASK_VTBL_H_ */

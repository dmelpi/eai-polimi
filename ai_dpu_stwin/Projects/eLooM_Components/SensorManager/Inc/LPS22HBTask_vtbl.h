/**
 ******************************************************************************
 * @file    LPS22HBTask_vtbl.h
 * @author  SRA - MCD
 * @version 1.1.0
 * @date    10-Dec-2021
 *
 * @brief
 *
 * <DESCRIPTIOM>
 *
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *                             
 *
 ******************************************************************************
 */
#ifndef LPS22HBTASK_VTBL_H_
#define LPS22HBTASK_VTBL_H_

#ifdef __cplusplus
extern "C" {
#endif


//#include "ISensor.h"


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
sys_error_code_t LPS22HBTask_vtblHardwareInit(AManagedTask *_this, void *pParams);
sys_error_code_t LPS22HBTask_vtblOnCreateTask(AManagedTask *_this, TaskFunction_t *pTaskCode, const char **pName, unsigned short *pStackDepth, void **pParams, UBaseType_t *pPriority); ///< @sa AMTOnCreateTask
sys_error_code_t LPS22HBTask_vtblDoEnterPowerMode(AManagedTask *_this, const EPowerMode ActivePowerMode, const EPowerMode NewPowerMode); ///< @sa AMTDoEnterPowerMode
sys_error_code_t LPS22HBTask_vtblHandleError(AManagedTask *_this, SysEvent Error); ///< @sa AMTHandleError
sys_error_code_t LPS22HBTask_vtblOnEnterTaskControlLoop(AManagedTask *this); ///< @sa AMTOnEnterTaskControlLoop

/* AManagedTaskEx virtual functions */
sys_error_code_t LPS22HBTask_vtblForceExecuteStep(AManagedTaskEx *_this, EPowerMode ActivePowerMode); ///< @sa AMTExForceExecuteStep
sys_error_code_t LPS22HBTask_vtblOnEnterPowerMode(AManagedTaskEx *_this, const EPowerMode ActivePowerMode, const EPowerMode NewPowerMode); ///< @sa AMTExOnEnterPowerMode


uint8_t LPS22HBTask_vtblTempGetId(ISourceObservable *_this);
uint8_t LPS22HBTask_vtblPressGetId(ISourceObservable *_this);
IEventSrc *LPS22HBTask_vtblTempGetEventSourceIF(ISourceObservable *_this);
IEventSrc *LPS22HBTask_vtblPressGetEventSourceIF(ISourceObservable *_this);
sys_error_code_t LPS22HBTask_vtblPressGetODR(ISourceObservable *_this, float *p_measured, float *p_nominal);
float LPS22HBTask_vtblPressGetFS(ISourceObservable *_this);
float LPS22HBTask_vtblPressGetSensitivity(ISourceObservable *_this);
uint8_t LPS22HBTask_vtblPressGetDataType(ISourceObservable *_this);
uint16_t LPS22HBTask_vtblPressGetDimensions(ISourceObservable *_this);
sys_error_code_t LPS22HBTask_vtblTempGetODR(ISourceObservable *_this, float *p_measured, float *p_nominal);
float LPS22HBTask_vtblTempGetFS(ISourceObservable *_this);
float LPS22HBTask_vtblTempGetSensitivity(ISourceObservable *_this);
uint8_t LPS22HBTask_vtblTempGetDataType(ISourceObservable *_this);
uint16_t LPS22HBTask_vtblTempGetDimensions(ISourceObservable *_this);

sys_error_code_t LPS22HBTask_vtblSensorSetODR(ISensor_t *_this, float ODR);
sys_error_code_t LPS22HBTask_vtblSensorSetFS(ISensor_t *_this, float FS);
sys_error_code_t LPS22HBTask_vtblSensorEnable(ISensor_t *_this);
sys_error_code_t LPS22HBTask_vtblSensorDisable(ISensor_t *_this);
boolean_t LPS22HBTask_vtblSensorIsEnabled(ISensor_t *_this);
SensorDescriptor_t LPS22HBTask_vtblTempGetDescription(ISensor_t *_this);
SensorDescriptor_t LPS22HBTask_vtblPressGetDescription(ISensor_t *_this);
SensorStatus_t LPS22HBTask_vtblTempGetStatus(ISensor_t *_this);
SensorStatus_t LPS22HBTask_vtblPressGetStatus(ISensor_t *_this);

#ifdef __cplusplus
}
#endif

#endif /* LPS22HBTASK_VTBL_H_ */

/**
 ******************************************************************************
 * @file    StreamerCLI_vtbl.h
 * @author  STMicroelectronics - AIS - MCD Team
 * @version $Version$
 * @date    $Date$
 *
 * @brief
 *
 * <DESCRIPTIOM>
 *
 *********************************************************************************
 * @attention
 *
 * Copyright (c) 2021 STMicroelectronics
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file in
 * the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *********************************************************************************
 */
#ifndef INC_APPCONTROLLER_VTBL_H_
#define INC_APPCONTROLLER_VTBL_H_

#ifdef __cplusplus
extern "C" {
#endif


/* AManagedTask virtual functions */
sys_error_code_t StreamerCLI_vtblHardwareInit(AManagedTask *_this, void *p_params); /*!< @sa AMTHardwareInit */
sys_error_code_t StreamerCLI_vtblOnCreateTask(AManagedTask *_this, TaskFunction_t *p_task_code, const char **p_name, unsigned short *p_stack_depth, void **p_params, UBaseType_t *p_priority); /*!< @sa AMTOnCreateTask */
sys_error_code_t StreamerCLI_vtblDoEnterPowerMode(AManagedTask *_this, const EPowerMode active_power_mode, const EPowerMode new_power_mode); /*!< @sa AMTDoEnterPowerMode */
sys_error_code_t StreamerCLI_vtblHandleError(AManagedTask *_this, SysEvent error); /*!< @sa AMTHandleError */
sys_error_code_t StreamerCLI_vtblOnEnterTaskControlLoop(AManagedTask *this); ///< @sa AMTOnEnterTaskControlLoop

/* AManagedTaskEx virtual functions */
sys_error_code_t StreamerCLI_vtblForceExecuteStep(AManagedTaskEx *_this, EPowerMode active_power_mode); /*!< @sa AMTExForceExecuteStep */
sys_error_code_t StreamerCLI_vtblOnEnterPowerMode(AManagedTaskEx *_this, const EPowerMode active_power_mode, const EPowerMode new_power_mode); /*!< @sa AMTExOnEnterPowerMode */

/* IListener virtual functions */
sys_error_code_t StreamerCLI_vtblOnStatusChange(IListener *this); ///< @sa IListener_OnStatusChange

/* IEventListener virtual functions */
void *StreamerCLI_vtblGetOwner(IEventListener *_this); ///< @sa IEventListener_GetOwner
void StreamerCLI_vtblSetOwner(IEventListener *_this, void *p_owner); ///< @sa IEventListener_SetOwner

/* ISensorEventListener virtual functions */
sys_error_code_t StreamerCLI_vtblOnNewDataReady(IEventListener *_this, const SensorEvent *p_evt); ///< @sa ISensorEventListener_OnNewDataReady

#ifdef __cplusplus
}
#endif

#endif /* INC_APPCONTROLLER_VTBL_H_ */

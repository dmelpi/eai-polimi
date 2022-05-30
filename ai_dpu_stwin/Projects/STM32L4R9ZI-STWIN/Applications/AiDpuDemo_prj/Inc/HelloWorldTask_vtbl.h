/**
 ******************************************************************************
 * @file    HelloWorldTask_vtbl.h
 * @author  STMicroelectronics - AIS - MCD Team
 * @version V1.0.0
 * @date    15-September-2021
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
#ifndef HELLOWORLDTASK_VTBL_H_
#define HELLOWORLDTASK_VTBL_H_

#ifdef __cplusplus
extern "C" {
#endif


// AManagedTask virtual functions
sys_error_code_t HelloWorldTask_vtblHardwareInit(AManagedTask *this, void *pParams); ///< @sa AMTHardwareInit
sys_error_code_t HelloWorldTask_vtblOnCreateTask(AManagedTask *this, TaskFunction_t *pvTaskCode, const char **pcName, unsigned short *pnStackDepth, void **pParams, UBaseType_t *pxPriority); ///< @sa AMTOnCreateTask
sys_error_code_t HelloWorldTask_vtblDoEnterPowerMode(AManagedTask *this, const EPowerMode eActivePowerMode, const EPowerMode eNewPowerMode); ///< @sa AMTDoEnterPowerMode
sys_error_code_t HelloWorldTask_vtblHandleError(AManagedTask *this, SysEvent xError); ///< @sa AMTHandleError
sys_error_code_t HelloWorldTask_vtblOnEnterTaskControlLoop(AManagedTask *this); ///< @sa AMTOnEnterTaskControlLoop

/* IListener virtual functions */
sys_error_code_t HelloWorldTask_vtblOnStatusChange(IListener *_this);                                          ///< @sa IListenerOnStatusChange

/* IEventListener virtual functions */
void HelloWorldTask_vtblSetOwner(IEventListener *_this, void *pxOwner);                                        ///< @sa IEventListenerSetOwner
void *HelloWorldTask_vtblGetOwner(IEventListener *_this);                                                      ///< @sa IEventListenerGetOwner

/* IProcessEventListener virtual functions */
sys_error_code_t HelloWorldTask_vtblOnProcessedDataReady(IEventListener *_this, const ProcessEvent *pxEvt);   ///< @sa IProcessEventListenerOnProcessedDataReady


#ifdef __cplusplus
}
#endif

#endif /* HELLOWORLDTASK_VTBL_H_ */

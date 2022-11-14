/**
 ******************************************************************************
 * @file    AppTask_vtbl.h
 * @author  STMicroelectronics
 * @version 1.0.0
 * @date    Jun 14, 2022
 *
 * @brief Application Controller Task
 *
 *
 ******************************************************************************
 * @attention
 *
 * <h2><center>COPYRIGHT &copy; 2022 STMicroelectronics</center></h2>
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file in
 * the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
#ifndef APPTASK_VTBL_H_
#define APPTASK_VTBL_H_

#ifdef __cplusplus
extern "C" {
#endif


// IManagedTask virtual functions
sys_error_code_t AppTask_vtblHardwareInit(AManagedTask *this, void *pParams); 	///< @sa AMTHardwareInit
sys_error_code_t AppTask_vtblOnCreateTask(AManagedTask *_this, tx_entry_function_t *pTaskCode, CHAR **pName, VOID **pStackStart, ULONG *pStackDepth, UINT *pPriority, UINT *pPreemptThreshold, ULONG *pTimeSlice, ULONG *pAutoStart, ULONG *pParams); ///< @sa AMTOnCreateTask
sys_error_code_t AppTask_vtblDoEnterPowerMode(AManagedTask *this, const EPowerMode eActivePowerMode, const EPowerMode eNewPowerMode); ///< @sa AMTDoEnterPowerMode
sys_error_code_t AppTask_vtblHandleError(AManagedTask *this, SysEvent xError);	///< @sa AMTHandleError
sys_error_code_t AppTask_vtblOnEnterTaskControlLoop(AManagedTask *this); 	    ///< @sa AMTOnEnterTaskControlLoop

/* IListener virtual functions */
sys_error_code_t AppTask_vtblOnStatusChange(IListener *_this);                  ///< @sa IListenerOnStatusChange

/* IEventListener virtual functions */
void AppTask_vtblSetOwner(IEventListener *_this, void *pxOwner);                ///< @sa IEventListenerSetOwner
void *AppTask_vtblGetOwner(IEventListener *_this);                              ///< @sa IEventListenerGetOwner

/* IUsbDelegate virtual functions */
sys_error_code_t AppTask_vtblOnUsbData(IUsbDelegate *_this, const uint8_t *buffer, uint32_t length);    ///< @sa IUsbDelegateOnNewData

/* IApplication virtual functions */
sys_error_code_t AppTask_vtblStart(IAiApplication_t *_this);                                               ///< @sa IApplication_start
sys_error_code_t AppTask_vtblStop(IAiApplication_t *_this);                                                ///< @sa IApplication_stop
sys_error_code_t AppTask_vtblSwitchBank(IAiApplication_t *_this);                                          ///< @sa IApplication_switch_bank

/* IProcessEventListener virtual functions */
sys_error_code_t AppTask_vtblOnProcessedDataReady(IEventListener *_this, const ProcessEvent *pxEvt);   ///< @sa IProcessEventListenerOnProcessedDataReady

#ifdef __cplusplus
}
#endif

#endif /* APPTASK_VTBL_H_ */

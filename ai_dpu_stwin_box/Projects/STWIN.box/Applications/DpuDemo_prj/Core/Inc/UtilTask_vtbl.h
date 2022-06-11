/**
 ******************************************************************************
 * @file    UtilTask_vtbl.h
 * @author  SRA
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
#ifndef INC_UTILTASK_VTBL_H_
#define INC_UTILTASK_VTBL_H_

#ifdef __cplusplus
extern "C" {
#endif

/* AManagedTask virtual functions */
sys_error_code_t UtilTask_vtblHardwareInit(AManagedTask *_this, void *p_params); /*!< @sa AMTHardwareInit */
sys_error_code_t UtilTask_vtblOnCreateTask(AManagedTask *_this, tx_entry_function_t *pvTaskCode, CHAR **pcName, VOID **pvStackStart, ULONG *pnStackSize,
                                           UINT *pnPriority, UINT *pnPreemptThreshold, ULONG *pnTimeSlice, ULONG *pnAutoStart, ULONG *pnParams); ///< @sa AMTOnCreateTask
sys_error_code_t UtilTask_vtblDoEnterPowerMode(AManagedTask *_this, const EPowerMode active_power_mode, const EPowerMode new_power_mode); /*!< @sa AMTDoEnterPowerMode */
sys_error_code_t UtilTask_vtblHandleError(AManagedTask *_this, SysEvent error); /*!< @sa AMTHandleError */
sys_error_code_t UtilTask_vtblOnEnterTaskControlLoop(AManagedTask *this); ///< @sa AMTOnEnterTaskControlLoop

/* AManagedTaskEx virtual functions */
sys_error_code_t UtilTask_vtblForceExecuteStep(AManagedTaskEx *_this, EPowerMode active_power_mode); /*!< @sa AMTExForceExecuteStep */
sys_error_code_t UtilTask_vtblOnEnterPowerMode(AManagedTaskEx *_this, const EPowerMode active_power_mode, const EPowerMode new_power_mode); /*!< @sa AMTExOnEnterPowerMode */

#ifdef __cplusplus
}
#endif

#endif /* INC_UTILTASK_VTBL_H_ */

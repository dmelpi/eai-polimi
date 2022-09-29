/*
 * HelloTask_vtbl.h
 *
 *  Created on: Sep 29, 2022
 *      Author: andre
 */

#ifndef CORE_INC_HELLOTASK_VTBL_H_
#define CORE_INC_HELLOTASK_VTBL_H_

#ifdef __cplusplus
extern "C" {
#endif


sys_error_code_t HelloTask_vtblHardwareInit(AManagedTask *this, void *pParams); 	///< @sa AMTHardwareInit
sys_error_code_t HelloTask_vtblOnCreateTask(AManagedTask *_this, tx_entry_function_t *pTaskCode, CHAR **pName, VOID **pStackStart, ULONG *pStackDepth, UINT *pPriority, UINT *pPreemptThreshold, ULONG *pTimeSlice, ULONG *pAutoStart, ULONG *pParams); ///< @sa AMTOnCreateTask
sys_error_code_t HelloTask_vtblDoEnterPowerMode(AManagedTask *this, const EPowerMode eActivePowerMode, const EPowerMode eNewPowerMode); ///< @sa AMTDoEnterPowerMode
sys_error_code_t HelloTask_vtblHandleError(AManagedTask *this, SysEvent xError);	///< @sa AMTHandleError
sys_error_code_t HelloTask_vtblOnEnterTaskControlLoop(AManagedTask *this); 	    ///< @sa AMTOnEnterTaskControlLoop


#ifdef __cplusplus
}
#endif

#endif /* CORE_INC_HELLOTASK_VTBL_H_ */

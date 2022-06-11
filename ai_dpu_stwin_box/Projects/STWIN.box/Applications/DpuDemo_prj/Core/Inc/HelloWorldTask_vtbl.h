/**
 ******************************************************************************
 * @file    HelloWorldTask_vtbl.h
 * @author  STMicroelectronics - ST-Korea - MCD Team
 * @version 1.0.0
 * @date    Nov 2, 2018
 *
 * @brief
 *
 * <DESCRIPTIOM>
 *
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT 2017 STMicroelectronics</center></h2>
 *
 * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *        http://www.st.com/software_license_agreement_liberty_v2
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************
 */
#ifndef HELLOWORLDTASK_VTBL_H_
#define HELLOWORLDTASK_VTBL_H_

#ifdef __cplusplus
extern "C" {
#endif


// IManagedTask virtual functions
sys_error_code_t HelloWorldTask_vtblHardwareInit(AManagedTask *this, void *pParams); ///< @sa AMTHardwareInit
sys_error_code_t HelloWorldTask_vtblOnCreateTask(AManagedTask *_this, tx_entry_function_t *pTaskCode, CHAR **pName, VOID **pStackStart, ULONG *pStackDepth, UINT *pPriority, UINT *pPreemptThreshold, ULONG *pTimeSlice, ULONG *pAutoStart, ULONG *pParams); ///< @sa AMTOnCreateTask
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

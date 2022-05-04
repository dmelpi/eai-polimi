/**
 ******************************************************************************
 * @file    AManagedTask.c
 * @author  STMicroelectronics - ST-Korea - MCD Team
 * @version 3.0.0
 * @date    Mar 20, 2017
 * @brief
 *
 * <DESCRIPTIOM>
 *
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2016 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file in
 * the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 ******************************************************************************
 */

#include "services/AManagedTask.h"
#include "services/AManagedTask_vtbl.h"

/* GCC requires one function forward declaration in only one .c source
 * in order to manage the inline.
 * See also http://stackoverflow.com/questions/26503235/c-inline-function-and-gcc
 */
#if defined (__GNUC__) || defined (__ICCARM__)
extern sys_error_code_t AMTHardwareInit(AManagedTask *this, void *pParams);
extern sys_error_code_t AMTOnCreateTask(AManagedTask *this, TaskFunction_t *pvTaskCode, const char **pcName, unsigned short *pnStackDepth, void **pParams, UBaseType_t *pxPriority);
extern sys_error_code_t AMTDoEnterPowerMode(AManagedTask *this, const EPowerMode eActivePowerMode, const EPowerMode eNewPowerMode);
extern sys_error_code_t AMTHandleError(AManagedTask *this, SysEvent xError);
extern sys_error_code_t AMTOnEnterTaskControlLoop(AManagedTask *_this);
extern sys_error_code_t AMTInit(AManagedTask *this);
extern sys_error_code_t AMTNotifyIsStillRunning(AManagedTask *this, sys_error_code_t nStepError);
extern void AMTResetAEDCounter(AManagedTask *this);
extern EPowerMode AMTGetSystemPowerMode(void);
extern EPowerMode AMTGetTaskPowerMode(AManagedTask *_this);
extern boolean_t AMTIsPowerModeSwitchPending(AManagedTask *this);
extern void AMTReportErrOnStepExecution(AManagedTask *this, sys_error_code_t nStepError);
extern sys_error_code_t AMTSetPMStateRemapFunc(AManagedTask *_this, EPowerMode *pPMState2PMStateMap);
#endif


/* Public API definition */
/*************************/

void AMTRun(void *pParams) {
  sys_error_code_t xRes;
  AManagedTask *_this = (AManagedTask*)pParams;
  pExecuteStepFunc_t pExecuteStepFunc = NULL;

  /* At this point all system has been initialized.
     Execute task specific delayed one time initialization. */
  xRes = AMTOnEnterTaskControlLoop(_this);
  if (SYS_IS_ERROR_CODE(xRes)) {
    /* stop the system execution */
    sys_error_handler();
  }

  for (;;) {
    if(_this->m_pfPMState2FuncMap == NULL) {
      sys_error_handler();
    }

    /* check if there is a pending power mode switch request */
    if (_this->m_xStatus.nPowerModeSwitchPending == 1U) {
      // clear the power mode switch delay because the task is ready to switch.
      taskENTER_CRITICAL();
      _this->m_xStatus.nDelayPowerModeSwitch = 0;
      taskEXIT_CRITICAL();
      vTaskSuspend(NULL);
    }
    else {
      /* find the execute step function  */
      uint8_t nPMState = (uint8_t)AMTGetTaskPowerMode(_this);
      pExecuteStepFunc = _this->m_pfPMState2FuncMap[nPMState];

      if (pExecuteStepFunc != NULL) {
        taskENTER_CRITICAL();
        _this->m_xStatus.nDelayPowerModeSwitch = 1;
        taskEXIT_CRITICAL();
        xRes = pExecuteStepFunc(_this);
        taskENTER_CRITICAL();
        _this->m_xStatus.nDelayPowerModeSwitch = 0;
        taskEXIT_CRITICAL();
      }
      else {
        /* there is no function so, because this is a AManagedTask simply suspend it for a while */
        vTaskDelay(pdMS_TO_TICKS(50));
      }

      /* notify the system that the task is working fine. */
      (void)AMTNotifyIsStillRunning((AManagedTask*)_this, xRes);

#if (SYS_TRACE > 1)
      if (res != SYS_NO_ERROR_CODE) {
        sys_check_error_code(xRes);
        sys_error_handler();
      }
#endif

    }
  }
}

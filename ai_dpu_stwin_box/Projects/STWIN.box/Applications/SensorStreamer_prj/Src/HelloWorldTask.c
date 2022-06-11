/**
 ******************************************************************************
 * @file    HelloWorldTask.c
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
 * <h2><center>COPYRIGHT &copy; 2021 STMicroelectronics</center></h2>
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file in
 * the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *********************************************************************************
 */

#include "HelloWorldTask.h"
#include "HelloWorldTask_vtbl.h"
#include "drivers/PushButtonDrv.h"
#include "drivers/PushButtonDrv_vtbl.h"
#include "services/sysdebug.h"


#ifndef HW_TASK_CFG_STACK_DEPTH
#define HW_TASK_CFG_STACK_DEPTH         TX_MINIMUM_STACK
#endif

#ifndef HW_TASK_CFG_PRIORITY
#define HW_TASK_CFG_PRIORITY            (TX_MAX_PRIORITIES-2)
#endif

#define HW_TASK_ANTI_DEBOUNCH_PERIOD_TICK        7U

#define SYS_DEBUGF(level, message)      SYS_DEBUGF3(SYS_DBG_HW, level, message)


/**
 * The only instance of the task object.
 */
static HelloWorldTask s_xTaskObj;


// Private member function declaration
// ***********************************

/**
 * Execute one step of the task control loop while the system is in RUN mode.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return SYS_NO_EROR_CODE if success, a task specific error code otherwise.
 */
static sys_error_code_t HelloWorldTaskExecuteStepRun(AManagedTask *_this);

/**
 * Class object declaration
 */
typedef struct _HelloWorldClass {
  /**
   * HelloWorldTask class virtual table.
   */
  AManagedTask_vtbl m_xVTBL;

  /**
   * HelloWorldTask (PM_STATE, ExecuteStepFunc) map.
   */
  pExecuteStepFunc_t m_pfPMState2FuncMap[];
} HelloWorldClass;

/**
 * The class object.
 */
static const HelloWorldClass s_xTheClass = {
    /* Class virtual table */
    {
        HelloWorldTask_vtblHardwareInit,
        HelloWorldTask_vtblOnCreateTask,
        HelloWorldTask_vtblDoEnterPowerMode,
        HelloWorldTask_vtblHandleError,
        HelloWorldTask_vtblOnEnterTaskControlLoop
    },

    /* class (PM_STATE, ExecuteStepFunc) map */
    {
        HelloWorldTaskExecuteStepRun,
        NULL,
        NULL,
        NULL
    }
};


// Inline function forward declaration
// ***********************************

#if defined (__GNUC__)
#endif


// Public API definition
// *********************

AManagedTask *HelloWorldTaskAlloc(const void *p_mx_drv_cfg) {
  // In this application there is only one Keyboard task,
  // so this allocator implement the singleton design pattern.

  // Initialize the super class
  AMTInit(&s_xTaskObj.super);

  s_xTaskObj.super.vptr = &s_xTheClass.m_xVTBL;

  s_xTaskObj.p_mx_drv_cfg = p_mx_drv_cfg;

  return (AManagedTask*)&s_xTaskObj;
}

// AManagedTask virtual functions definition
// ***********************************************

sys_error_code_t HelloWorldTask_vtblHardwareInit(AManagedTask *_this, void *pParams)
{
  assert_param(_this != NULL);
  sys_error_code_t xRes = SYS_NO_ERROR_CODE;
  HelloWorldTask *pObj = (HelloWorldTask*) _this;

  if (pObj->p_mx_drv_cfg != NULL)
  {
    pObj->m_pxDriver = PushButtonDrvAlloc();
    if (pObj->m_pxDriver == NULL)
    {
      SYS_DEBUGF(SYS_DBG_LEVEL_SEVERE, ("HW: unable to alloc driver object.\r\n"));
      xRes = SYS_GET_LAST_LOW_LEVEL_ERROR_CODE();
    }
    else {
      PushButtonDrvParams_t driver_cfg = {
          .p_mx_gpio_cfg = (void*)pObj->p_mx_drv_cfg
      };
      xRes = IDrvInit((IDriver*)pObj->m_pxDriver, &driver_cfg);
      if (SYS_IS_ERROR_CODE(xRes)) {
        SYS_DEBUGF(SYS_DBG_LEVEL_SEVERE, ("HW: error during driver initialization\r\n"));
      }
    }
  }

  return xRes;

}

sys_error_code_t HelloWorldTask_vtblOnCreateTask(AManagedTask *_this, tx_entry_function_t *pvTaskCode, CHAR **pcName,
    VOID **pvStackStart, ULONG *pnStackSize,
    UINT *pnPriority, UINT *pnPreemptThreshold,
    ULONG *pnTimeSlice, ULONG *pnAutoStart,
    ULONG *pnParams)
{
  assert_param(_this != NULL);
  sys_error_code_t xRes = SYS_NO_ERROR_CODE;
//  HelloWorldTask *pObj = (HelloWorldTask*)_this;

  _this->m_pfPMState2FuncMap = s_xTheClass.m_pfPMState2FuncMap;

  *pvTaskCode = AMTRun;
  *pcName = "HW";
  *pvStackStart = NULL; // allocate the task stack in the system memory pool.
  *pnStackSize = HW_TASK_CFG_STACK_DEPTH;
  *pnParams = (ULONG)_this;
  *pnPriority = HW_TASK_CFG_PRIORITY;
  *pnPreemptThreshold = HW_TASK_CFG_PRIORITY;
  *pnTimeSlice = TX_NO_TIME_SLICE;
  *pnAutoStart = TX_AUTO_START;

  return xRes;
}

sys_error_code_t HelloWorldTask_vtblDoEnterPowerMode(AManagedTask *_this, const EPowerMode eActivePowerMode, const EPowerMode eNewPowerMode)
{
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  HelloWorldTask *p_obj = (HelloWorldTask*)_this;

  /* first propagate the event to the driver. */
  if (p_obj->m_pxDriver != NULL)
  {
  	res = IDrvDoEnterPowerMode(p_obj->m_pxDriver, eActivePowerMode, eNewPowerMode);
  }

  if(eNewPowerMode == E_POWER_MODE_STATE1)
  {
    __NOP();
    SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("HW: -> STATE1\r\n"));
  }
  else if(eNewPowerMode == E_POWER_MODE_SLEEP_1)
  {
    __NOP();
    SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("HW: -> SLEEP_1\r\n"));
  }
  else if(eNewPowerMode == E_POWER_MODE_TEST)
  {
    __NOP();
    SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("HW: -> TEST\r\n"));
  }

  return res;
}

sys_error_code_t HelloWorldTask_vtblHandleError(AManagedTask *_this, SysEvent xError) {
  assert_param(_this);
  sys_error_code_t xRes = SYS_NO_ERROR_CODE;
//  HelloWorldTask *pObj = (HelloWorldTask*)_this;

  return xRes;
}

sys_error_code_t HelloWorldTask_vtblOnEnterTaskControlLoop(AManagedTask *_this)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  HelloWorldTask *p_obj = (HelloWorldTask*)_this;

  SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("HW: start.\r\n"));

  if (p_obj->m_pxDriver != NULL)
  {
  	IDrvStart(p_obj->m_pxDriver);
  }

  return res;
}


// Private function definition
// ***************************

static sys_error_code_t HelloWorldTaskExecuteStepRun(AManagedTask *_this) {
  assert_param(_this != NULL);
  sys_error_code_t xRes = SYS_NO_ERROR_CODE;

  tx_thread_sleep(1000);
//  NucleoDriverToggleLed((NucleoDriver*)_this->m_pxDriver);
  SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("Hello STWIN.box!!\r\n"));
  __NOP();
  __NOP();

  return xRes;
}

void HW_PB_EXTI_Callback(uint16_t pin)
{
  /* anti debounch */
  static uint32_t t_start = 0;
  if(HAL_GetTick() - t_start > 10*HW_TASK_ANTI_DEBOUNCH_PERIOD_TICK)
  {
    if(pin == USER_BUTTON_Pin)
    {
      /* generate the system event to change the PM state*/
      SysEvent evt = {
          .nRawEvent = SYS_PM_MAKE_EVENT(SYS_PM_EVT_SRC_PB, SYS_PM_EVT_PARAM_SHORT_PRESS)
      };
      SysPostPowerModeEvent(evt);

      t_start = HAL_GetTick();
    }
  }
}

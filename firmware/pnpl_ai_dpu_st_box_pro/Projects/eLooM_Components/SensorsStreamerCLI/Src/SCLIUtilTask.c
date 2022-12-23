/**
 ******************************************************************************
 * @file    SCLIUtilTask.c
 * @author  STMicroelectronics - AIS - MCD Team
 * @version $Version$
 * @date    $Date$
 *
 * @brief  SCLIUtilTask_t definition.
 *
 *
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

#include "SCLIUtilTask.h"
#include "SCLIUtilTask_vtbl.h"
#include "app_messages_parser.h"
#include "mx.h"
#include "services/sysdebug.h"

/* TODO: change XXX with a short id for the task */

#ifndef SCLIUTIL_TASK_CFG_STACK_DEPTH
#define SCLIUTIL_TASK_CFG_STACK_DEPTH              (120)
#endif

#ifndef SCLIUTIL_TASK_CFG_PRIORITY
#define SCLIUTIL_TASK_CFG_PRIORITY                 (tskIDLE_PRIORITY)
#endif

#ifndef SCLIUTIL_TASK_CFG_IN_QUEUE_ITEM_SIZE
#define SCLIUTIL_TASK_CFG_IN_QUEUE_ITEM_SIZE       sizeof(struct utilMessage_t)
#endif

#ifndef SCLIUTIL_TASK_CFG_IN_QUEUE_ITEM_COUNT
#define SCLIUTIL_TASK_CFG_IN_QUEUE_ITEM_COUNT      10
#endif

#ifndef SCLIUTIL_TASK_CFG_LP_TIMER_DEF_PERIOD_MS
#define SCLIUTIL_TASK_CFG_LP_TIMER_DEF_PERIOD_MS   10000
#endif

/* TODO: define the symbol SYS_DBG_UTIL in the file sysdebug_config.h */
#define SYS_DEBUGF(level, message)             SYS_DEBUGF3(SYS_DBG_UTIL, level, message)

#if defined(DEBUG) || defined (SYS_DEBUG)
#define sTaskObj                               sSCLIUtilTaskObj
#endif


/**
 * Class object declaration. The class object encapsulates members that are shared between
 * all instance of the class.
 */
typedef struct _SCLIUtilTaskClass_t {
  /**
   * SCLIUtilTask class virtual table.
   */
  AManagedTaskEx_vtbl vtbl;

  /**
   * SCLIUtilTask class (PM_STATE, ExecuteStepFunc) map. The map is implemente with an array and
   * the key is the index. Number of items of this array must be equal to the number of PM state
   * of the application. If the managed task does nothing in a PM state, then set to NULL the
   * relative entry in the map.
   */
  pExecuteStepFunc_t p_pm_state2func_map[];
} SCLIUtilTaskClass_t;


/* Private member function declaration */
/***************************************/

/**
 * Execute one step of the task control loop while the system is in STATE1.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return SYS_NO_EROR_CODE if success, a task specific error code otherwise.
 */
static sys_error_code_t SCLIUtilTaskExecuteStepState1(AManagedTask *_this);

/**
 * Execute one step of the task control loop while the system is in SENSORS_ACTIVE.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return SYS_NO_EROR_CODE if success, a task specific error code otherwise.
 */
static sys_error_code_t SCLIUtilTaskExecuteStepSensorsActive(AManagedTask *_this);

/**
 * Callback function called when the software timer expires.
 *
 * @param xTimer [IN] specifies the handle of the expired timer.
 */
static void SCLIUtilTaskSwTimerCallbackFunction(TimerHandle_t timer);


/* Inline function forward declaration */
/***************************************/

#if defined (__GNUC__) || defined (__ICCARM__)
/* Inline function defined inline in the header file SCLIUtilTask.h must be declared here as extern function. */
#endif


/**
 * The only instance of the task object.
 */
static SCLIUtilTask_t sTaskObj;

/**
 * The class object.
 */
static const SCLIUtilTaskClass_t sTheClass = {
    /* Class virtual table */
    {
        SCLIUtilTask_vtblHardwareInit,
        SCLIUtilTask_vtblOnCreateTask,
        SCLIUtilTask_vtblDoEnterPowerMode,
        SCLIUtilTask_vtblHandleError,
        SCLIUtilTask_vtblOnEnterTaskControlLoop,
        SCLIUtilTask_vtblForceExecuteStep,
        SCLIUtilTask_vtblOnEnterPowerMode
    },

    /* class (PM_STATE, ExecuteStepFunc) map */
    {
        SCLIUtilTaskExecuteStepState1,
        NULL,
        SCLIUtilTaskExecuteStepSensorsActive,
    }
};

/* Public API definition */
/*************************/

AManagedTaskEx *SCLIUtilTaskAlloc(const void *p_mx_led_drv_cfg, const void *p_mx_button_drv_cfg)
{
  /* In this application there is only one Keyboard task,
   * so this allocator implement the singleton design pattern.
   */

  /* Initialize the super class */
  AMTInitEx(&sTaskObj.super);

  sTaskObj.super.vptr = &sTheClass.vtbl;
  sTaskObj.p_mx_led_cfg = p_mx_led_drv_cfg;
  sTaskObj.p_mx_button_cfg = p_mx_button_drv_cfg;

  return (AManagedTaskEx*)&sTaskObj;
}

sys_error_code_t SCLIUtilTaskSetAutoLowPowerModePeriod(SCLIUtilTask_t *_this, uint32_t timeout_ms)
{
  assert_param(_this != NULL);

  _this->timeout_ms = timeout_ms;

  return SYS_NO_ERROR_CODE;
}

/* AManagedTask virtual functions definition */
/*********************************************/

sys_error_code_t SCLIUtilTask_vtblHardwareInit(AManagedTask *_this, void *p_params)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  SCLIUtilTask_t *p_obj = (SCLIUtilTask_t*)_this;

  if (!SYS_IS_ERROR_CODE(res))
  {
    /* Initialize the LED and User Button */
    // configure LED_1
    if (p_obj->p_mx_led_cfg != NULL)
    {
      ((MX_GPIOParams_t*)p_obj->p_mx_led_cfg)->p_mx_init_f();
    }

    if (p_obj->p_mx_button_cfg != NULL)
    {
      ((MX_GPIOParams_t*)p_obj->p_mx_button_cfg)->p_mx_init_f();
    }
  }

  return res;
}

sys_error_code_t SCLIUtilTask_vtblOnCreateTask(AManagedTask *_this, TaskFunction_t *p_task_code, const char **p_name, unsigned short *p_stack_depth, void **p_params, UBaseType_t *p_priority)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  SCLIUtilTask_t *p_obj = (SCLIUtilTask_t*)_this;


  /* initialize the object software resource here. */

  /* create the input queue */
  p_obj->in_queue = xQueueCreate(SCLIUTIL_TASK_CFG_IN_QUEUE_ITEM_COUNT, SCLIUTIL_TASK_CFG_IN_QUEUE_ITEM_SIZE);
  if(p_obj->in_queue == NULL)
  {
    res = SYS_SCLIUTIL_TASK_INIT_ERROR_CODE;
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_SCLIUTIL_TASK_INIT_ERROR_CODE);
    return res;
  }

#ifdef DEBUG
  vQueueAddToRegistry(p_obj->in_queue, "SCLIUTIL_Q");
#endif

  /* create the software timer*/
  p_obj->timeout_ms = SCLIUTIL_TASK_CFG_LP_TIMER_DEF_PERIOD_MS;
  p_obj->auto_low_power_timer = xTimerCreate("UtilT", pdMS_TO_TICKS(p_obj->timeout_ms), pdFALSE, p_obj, SCLIUtilTaskSwTimerCallbackFunction);
  if(p_obj->auto_low_power_timer == NULL)
  {
    res = SYS_SCLIUTIL_TASK_INIT_ERROR_CODE;
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_SCLIUTIL_TASK_INIT_ERROR_CODE);
    return res;
  }

  /* set the (PM_STATE, ExecuteStepFunc) map from the class object.  */
  _this->m_pfPMState2FuncMap = sTheClass.p_pm_state2func_map;

  *p_task_code = AMTExRun;
  *p_name = "UTIL";
  *p_stack_depth = SCLIUTIL_TASK_CFG_STACK_DEPTH;
  *p_params = _this;
  *p_priority = SCLIUTIL_TASK_CFG_PRIORITY;

  return res;
}

sys_error_code_t SCLIUtilTask_vtblDoEnterPowerMode(AManagedTask *_this, const EPowerMode active_power_mode, const EPowerMode new_power_mode)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  SCLIUtilTask_t *p_obj = (SCLIUtilTask_t*)_this;
  MX_GPIOParams_t *p_led = NULL;

  if(p_obj->p_mx_led_cfg != NULL)
  {
    p_led = (MX_GPIOParams_t*)p_obj->p_mx_led_cfg;
  }

  if(new_power_mode == E_POWER_MODE_STATE1)
  {
    /* turn on the LED_1 */
    if(p_obj->p_mx_led_cfg != NULL)
    {
      HAL_GPIO_WritePin(p_led->port, p_led->pin, GPIO_PIN_SET);
    }

    if (p_obj->timeout_ms != 0)
    {
      struct utilMessage_t msg = {
          .msgId = APP_MESSAGE_ID_UTIL,
          .cmd_id = SCLIUTIL_CMD_ID_START_LP_TIMER
      };

      if(pdTRUE != xQueueSendToFront(p_obj->in_queue, &msg, pdMS_TO_TICKS(150)))
      {
        res = SYS_TASK_QUEUE_FULL_ERROR_CODE;
      }
    }

  }
  else if(new_power_mode == E_POWER_MODE_SENSORS_ACTIVE)
  {
    if(pdPASS != xTimerStop(p_obj->auto_low_power_timer, pdMS_TO_TICKS(100)))
    {
      SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_SCLIUTIL_TASK_LP_TIMER_ERROR_CODE);
      res = SYS_SCLIUTIL_TASK_LP_TIMER_ERROR_CODE;
    }

    /* turn off the LED_1 */
    if(p_obj->p_mx_led_cfg != NULL)
    {
      HAL_GPIO_WritePin(p_led->port, p_led->pin, GPIO_PIN_RESET);
    }
  }
  else if(new_power_mode == E_POWER_MODE_SLEEP_1)
  {
    /* turn off the LED_1 */
    if(p_obj->p_mx_led_cfg != NULL)
    {
      HAL_GPIO_WritePin(p_led->port, p_led->pin, GPIO_PIN_RESET);
    }

    if(pdPASS != xTimerStop(p_obj->auto_low_power_timer, pdMS_TO_TICKS(100)))
    {
      SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_SCLIUTIL_TASK_LP_TIMER_ERROR_CODE);
      res = SYS_SCLIUTIL_TASK_LP_TIMER_ERROR_CODE;
    }
  }

  SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("UTIL: -> %d\r\n", (uint8_t)new_power_mode));


  return res;
}

sys_error_code_t SCLIUtilTask_vtblHandleError(AManagedTask *_this, SysEvent error)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
/*  SCLIUtilTask_t *p_obj = (SCLIUtilTask_t*)_this; */

  return res;
}

sys_error_code_t SCLIUtilTask_vtblOnEnterTaskControlLoop(AManagedTask *_this)
{
  assert_param(_this);
  sys_error_code_t xRes = SYS_NO_ERROR_CODE;
  SCLIUtilTask_t *p_obj = (SCLIUtilTask_t*)_this;

  SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("UTIL: start.\r\n"));

  /* enable the user button */
  if(p_obj->p_mx_button_cfg != NULL)
  {
    MX_GPIOParams_t *p_button = (MX_GPIOParams_t*)p_obj->p_mx_button_cfg;
    HAL_NVIC_EnableIRQ(p_button->irq_n);
  }

  /* turn on the LED_1 */
  if(p_obj->p_mx_led_cfg != NULL)
  {
    MX_GPIOParams_t *p_led = (MX_GPIOParams_t*)p_obj->p_mx_led_cfg;
    HAL_GPIO_WritePin(p_led->port, p_led->pin, GPIO_PIN_SET);
  }

  return xRes;
}


/* AManagedTaskEx virtual functions definition */
/***********************************************/

sys_error_code_t SCLIUtilTask_vtblForceExecuteStep(AManagedTaskEx *_this, EPowerMode active_power_mode)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  SCLIUtilTask_t *p_obj = (SCLIUtilTask_t*)_this;

  struct utilMessage_t msg = {
      .msgId = APP_REPORT_ID_FORCE_STEP
  };

  if(active_power_mode == E_POWER_MODE_STATE1)
  {
    xQueueSendToFront(p_obj->in_queue, &msg, pdMS_TO_TICKS(100));
  }
  else if(active_power_mode == E_POWER_MODE_SENSORS_ACTIVE)
  {
    xTaskAbortDelay(_this->m_xThaskHandle);
  }
  else
  {
    vTaskResume(_this->m_xThaskHandle);
  }

  return res;
}

sys_error_code_t SCLIUtilTask_vtblOnEnterPowerMode(AManagedTaskEx *_this, const EPowerMode active_power_mode, const EPowerMode new_power_mode)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
/*  SCLIUtilTask_t *p_obj = (SCLIUtilTask_t*)_this; */

//  LED_RED_GPIO_Port->BSRR = LED_RED_Pin;

  return res;
}


/* Private function definition */
/*******************************/

static sys_error_code_t SCLIUtilTaskExecuteStepState1(AManagedTask *_this)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  SCLIUtilTask_t *p_obj = (SCLIUtilTask_t*)_this;

  struct utilMessage_t msg = {0};

  AMTExSetInactiveState((AManagedTaskEx*) _this, TRUE);
  if(xQueueReceive(p_obj->in_queue, &msg, portMAX_DELAY) == pdTRUE)
  {
    AMTExSetInactiveState((AManagedTaskEx*) _this, FALSE);
    if(msg.msgId == APP_REPORT_ID_FORCE_STEP)
    {
      __NOP();
    }
    else if(msg.msgId == APP_MESSAGE_ID_UTIL)
    {
      if(msg.cmd_id == SCLIUTIL_CMD_ID_START_LP_TIMER)
      {
        if(pdPASS != xTimerChangePeriod(p_obj->auto_low_power_timer, p_obj->timeout_ms, pdMS_TO_TICKS(100)))
        {
          SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_SCLIUTIL_TASK_LP_TIMER_ERROR_CODE);
          res = SYS_SCLIUTIL_TASK_LP_TIMER_ERROR_CODE;
        }
        else if(pdPASS != xTimerReset(p_obj->auto_low_power_timer, pdMS_TO_TICKS(100)))
        {
          SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_SCLIUTIL_TASK_LP_TIMER_ERROR_CODE);
          res = SYS_SCLIUTIL_TASK_LP_TIMER_ERROR_CODE;
        }
      }
    }
  }

  return res;
}

static sys_error_code_t SCLIUtilTaskExecuteStepSensorsActive(AManagedTask *_this)
{
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  SCLIUtilTask_t *p_obj = (SCLIUtilTask_t*)_this;

  AMTExSetInactiveState((AManagedTaskEx*) _this, TRUE);
  vTaskDelay(pdMS_TO_TICKS(1000));
  AMTExSetInactiveState((AManagedTaskEx*) _this, FALSE);

  if(p_obj->p_mx_led_cfg != NULL)
  {
    MX_GPIOParams_t *p_led = (MX_GPIOParams_t*)p_obj->p_mx_led_cfg;
    HAL_GPIO_TogglePin(p_led->port, p_led->pin);
  }

  return res;
}

static void SCLIUtilTaskSwTimerCallbackFunction(TimerHandle_t timer)
{
  SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("UTIL: lp timer expired.\r\n"));

  SysEvent event;
  event.nRawEvent = SYS_PM_MAKE_EVENT(SYS_PM_EVT_SRC_LP_TIMER, SYS_PM_EVT_PARAM_ENTER_LP);
  SysPostPowerModeEvent(event);
}


/* CubeMX integration */
/**********************/

void Util_UB_EXTI_Callback(uint16_t pin)
{
  /* anti debounch */
  static uint32_t t_start = 0;
  if(HAL_GetTick() - t_start > 1000)
  {
    MX_GPIOParams_t *p_button = (MX_GPIOParams_t*)sTaskObj.p_mx_button_cfg;
    if(pin == p_button->pin)
    {
      /* generate the system event.*/
      SysEvent evt = {
          .nRawEvent = SYS_PM_MAKE_EVENT(SYS_PM_EVT_SRC_PB, SYS_PM_EVT_PARAM_SHORT_PRESS)
      };
      SysPostPowerModeEvent(evt);
      /* don't check the error code. For the moment we assume that we can loose a USER BUTTON PRessed event.*/
    }
  }

  t_start = HAL_GetTick();
}



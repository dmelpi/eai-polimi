/**
 ******************************************************************************
 * @file    UtilTask.c
 * @author  STMicroelectronics - AIS - MCD Team
 * @version $Version$
 * @date    $Date$
 *
 * @brief  UtilTask_t definition.
 *
 *
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

#include "UtilTask.h"
#include "UtilTask_vtbl.h"
#include "app_messages_parser.h"
#include "mx.h"
#include "services/sysdebug.h"

/* TODO: change XXX with a short id for the task */

#ifndef UTIL_TASK_CFG_STACK_DEPTH
#define UTIL_TASK_CFG_STACK_DEPTH              (120)
#endif

#ifndef UTIL_TASK_CFG_PRIORITY
#define UTIL_TASK_CFG_PRIORITY                 (tskIDLE_PRIORITY)
#endif

#ifndef UTIL_TASK_CFG_IN_QUEUE_ITEM_SIZE
#define UTIL_TASK_CFG_IN_QUEUE_ITEM_SIZE       sizeof(struct utilMessage_t)
#endif

#ifndef UTIL_TASK_CFG_IN_QUEUE_ITEM_COUNT
#define UTIL_TASK_CFG_IN_QUEUE_ITEM_COUNT      10
#endif

#ifndef UTIL_TASK_CFG_LP_TIMER_DEF_PERIOD_MS
#define UTIL_TASK_CFG_LP_TIMER_DEF_PERIOD_MS       10000
#endif

/* TODO: define the symbol SYS_DBG_UTIL in the file sysdebug_config.h */
#define SYS_DEBUGF(level, message)             SYS_DEBUGF3(SYS_DBG_UTIL, level, message)

#if defined(DEBUG) || defined (SYS_DEBUG)
#define sTaskObj                               sUtilTaskObj
#endif


/**
 * Class object declaration. The class object encapsulates members that are shared between
 * all instance of the class.
 */
typedef struct _UtilTaskClass_t {
  /**
   * UtilTask class virtual table.
   */
  AManagedTaskEx_vtbl vtbl;

  /**
   * UtilTask class (PM_STATE, ExecuteStepFunc) map. The map is implemente with an array and
   * the key is the index. Number of items of this array must be equal to the number of PM state
   * of the application. If the managed task does nothing in a PM state, then set to NULL the
   * relative entry in the map.
   */
  pExecuteStepFunc_t p_pm_state2func_map[];
} UtilTaskClass_t;


/* Private member function declaration */
/***************************************/

/**
 * Execute one step of the task control loop while the system is in STATE1.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return SYS_NO_EROR_CODE if success, a task specific error code otherwise.
 */
static sys_error_code_t UtilTaskExecuteStepState1(AManagedTask *_this);

/**
 * Execute one step of the task control loop while the system is in SENSORS_ACTIVE.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return SYS_NO_EROR_CODE if success, a task specific error code otherwise.
 */
static sys_error_code_t UtilTaskExecuteStepSensorsActive(AManagedTask *_this);

/**
 * Callback function called when the software timer expires.
 *
 * @param xTimer [IN] specifies the handle of the expired timer.
 */
static void UtilTaskSwTimerCallbackFunction(TimerHandle_t timer);

/**
 * Turn the user LED on/off.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @param on specifies [IN] the new status of the user LED.
 * @return SYS_NO_ERROR_CODE
 */
static inline sys_error_code_t UtilTaskSetLED(UtilTask_t *_this, boolean_t on);


/* Inline function forward declaration */
/***************************************/

#if defined (__GNUC__) || defined (__ICCARM__)
/* Inline function defined inline in the header file UtilTask.h must be declared here as extern function. */
#endif


/**
 * The only instance of the task object.
 */
static UtilTask_t sTaskObj;

/**
 * The class object.
 */
static const UtilTaskClass_t sTheClass = {
    /* Class virtual table */
    {
        UtilTask_vtblHardwareInit,
        UtilTask_vtblOnCreateTask,
        UtilTask_vtblDoEnterPowerMode,
        UtilTask_vtblHandleError,
        UtilTask_vtblOnEnterTaskControlLoop,
        UtilTask_vtblForceExecuteStep,
        UtilTask_vtblOnEnterPowerMode
    },

    /* class (PM_STATE, ExecuteStepFunc) map */
    {
        UtilTaskExecuteStepState1,
        NULL,
        UtilTaskExecuteStepSensorsActive,
    }
};

/* Public API definition */
/*************************/

AManagedTaskEx *UtilTaskAlloc(const void *p_mx_drv_cfg)
{
  /* In this application there is only one Keyboard task,
   * so this allocator implement the singleton design pattern.
   */

  /* Initialize the super class */
  AMTInitEx(&sTaskObj.super);

  sTaskObj.super.vptr = &sTheClass.vtbl;
  sTaskObj.p_mx_drv_cfg = p_mx_drv_cfg;

  return (AManagedTaskEx*)&sTaskObj;
}

sys_error_code_t UtilTaskSetAutoLowPowerModePeriod(UtilTask_t *_this, uint32_t timeout_ms)
{
  assert_param(_this != NULL);

  _this->timeout_ms = timeout_ms;

  return SYS_NO_ERROR_CODE;
}

SMUtilityDriver_t *GetSMUtilityDriver(void)
{
  return (SMUtilityDriver_t*)sTaskObj.p_sm_driver;
}

/* AManagedTask virtual functions definition */
/*********************************************/

sys_error_code_t UtilTask_vtblHardwareInit(AManagedTask *_this, void *p_params)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  UtilTask_t *p_obj = (UtilTask_t*)_this;
  GPIO_InitTypeDef GPIO_InitStruct = { 0 };

  p_obj->p_sm_driver = SMUtilityDriverAlloc();
  if(p_obj->p_sm_driver == NULL)
  {
    SYS_DEBUGF(SYS_DBG_LEVEL_SEVERE, ("UTIL task: unable to alloc driver object.\r\n"));
    res = SYS_GET_LAST_LOW_LEVEL_ERROR_CODE();
  }
  else
  {
    SMUtilityDriverParams_t cfg_params = {
        .p_mx_tim_cfg = (void*)p_obj->p_mx_drv_cfg
    };
    res = IDrvInit(p_obj->p_sm_driver, &cfg_params);
    if(SYS_IS_ERROR_CODE(res))
    {
      SYS_DEBUGF(SYS_DBG_LEVEL_SEVERE, ("UTIL task: error during driver initialization\r\n"));
    }
  }

  if (!SYS_IS_ERROR_CODE(res))
  {
    /* Initialize the LED and User Button */
    // configure LED2
    __HAL_RCC_GPIOD_CLK_ENABLE();
    HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
    GPIO_InitStruct.Pin = LED_GREEN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LED_GREEN_GPIO_Port, &GPIO_InitStruct);

    // configure the push button
    __HAL_RCC_GPIOC_CLK_ENABLE();
    GPIO_InitStruct.Pin = USER_Button_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(USER_Button_GPIO_Port, &GPIO_InitStruct);

    HAL_NVIC_SetPriority(USER_Button_EXTI_IRQn, 15, 0);
  }

  return res;
}

sys_error_code_t UtilTask_vtblOnCreateTask(AManagedTask *_this, TaskFunction_t *p_task_code, const char **p_name, unsigned short *p_stack_depth, void **p_params, UBaseType_t *p_priority)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  UtilTask_t *p_obj = (UtilTask_t*)_this;


  /* initialize the object software resource here. */

  /* create the input queue */
  p_obj->in_queue = xQueueCreate(UTIL_TASK_CFG_IN_QUEUE_ITEM_COUNT, UTIL_TASK_CFG_IN_QUEUE_ITEM_SIZE);
  if(p_obj->in_queue == NULL)
  {
    res = SYS_UTIL_TASK_INIT_ERROR_CODE;
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_UTIL_TASK_INIT_ERROR_CODE);
    return res;
  }

#ifdef DEBUG
  vQueueAddToRegistry(p_obj->in_queue, "UTIL_Q");
#endif

  /* create the software timer*/
  p_obj->timeout_ms = UTIL_TASK_CFG_LP_TIMER_DEF_PERIOD_MS;
  p_obj->auto_low_power_timer = xTimerCreate("UtilT", pdMS_TO_TICKS(p_obj->timeout_ms), pdFALSE, p_obj, UtilTaskSwTimerCallbackFunction);
  if(p_obj->auto_low_power_timer == NULL)
  {
    res = SYS_UTIL_TASK_INIT_ERROR_CODE;
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_UTIL_TASK_INIT_ERROR_CODE);
    return res;
  }

  /* set the (PM_STATE, ExecuteStepFunc) map from the class object.  */
  _this->m_pfPMState2FuncMap = sTheClass.p_pm_state2func_map;

  *p_task_code = AMTExRun;
  *p_name = "UTIL";
  *p_stack_depth = UTIL_TASK_CFG_STACK_DEPTH;
  *p_params = _this;
  *p_priority = UTIL_TASK_CFG_PRIORITY;

  return res;
}

sys_error_code_t UtilTask_vtblDoEnterPowerMode(AManagedTask *_this, const EPowerMode active_power_mode, const EPowerMode new_power_mode)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  UtilTask_t *p_obj = (UtilTask_t*)_this;

  /* propagate the call to the driver object. */
  IDrvDoEnterPowerMode(p_obj->p_sm_driver, active_power_mode, new_power_mode);

  if(new_power_mode == E_POWER_MODE_STATE1)
  {
    /* turn on the USER LED */
    HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);

    if (p_obj->timeout_ms != 0)
    {
      struct utilMessage_t msg = {
          .msgId = APP_MESSAGE_ID_UTIL,
          .cmd_id = UTIL_CMD_ID_START_LP_TIMER
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
      SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_UTIL_TASK_LP_TIMER_ERROR_CODE);
      res = SYS_UTIL_TASK_LP_TIMER_ERROR_CODE;
    }
  }
  else if(new_power_mode == E_POWER_MODE_SLEEP_1)
  {
    /* turn off the USER LED */
    HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);

    if(pdPASS != xTimerStop(p_obj->auto_low_power_timer, pdMS_TO_TICKS(100)))
    {
      SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_UTIL_TASK_LP_TIMER_ERROR_CODE);
      res = SYS_UTIL_TASK_LP_TIMER_ERROR_CODE;
    }
  }

  SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("UTIL: -> %d\r\n", (uint8_t)new_power_mode));


  return res;
}

sys_error_code_t UtilTask_vtblHandleError(AManagedTask *_this, SysEvent error)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
/*  UtilTask_t *p_obj = (UtilTask_t*)_this; */

  return res;
}

sys_error_code_t UtilTask_vtblOnEnterTaskControlLoop(AManagedTask *_this)
{
  assert_param(_this);
  sys_error_code_t xRes = SYS_NO_ERROR_CODE;
/*  UtilTask_t *p_obj = (UtilTask_t*)_this; */

  SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("UTIL: start.\r\n"));

  /* enable the user button */
  HAL_NVIC_EnableIRQ(USER_Button_EXTI_IRQn);
  UtilTaskSetLED(NULL, TRUE);

  return xRes;
}


/* AManagedTaskEx virtual functions definition */
/***********************************************/

sys_error_code_t UtilTask_vtblForceExecuteStep(AManagedTaskEx *_this, EPowerMode active_power_mode)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  UtilTask_t *p_obj = (UtilTask_t*)_this;

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

sys_error_code_t UtilTask_vtblOnEnterPowerMode(AManagedTaskEx *_this, const EPowerMode active_power_mode, const EPowerMode new_power_mode)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
/*  UtilTask_t *p_obj = (UtilTask_t*)_this; */

  return res;
}


/* Private function definition */
/*******************************/

static sys_error_code_t UtilTaskExecuteStepState1(AManagedTask *_this)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  UtilTask_t *p_obj = (UtilTask_t*)_this;

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
      if(msg.cmd_id == UTIL_CMD_ID_START_LP_TIMER)
      {
        if(pdPASS != xTimerChangePeriod(p_obj->auto_low_power_timer, p_obj->timeout_ms, pdMS_TO_TICKS(100)))
        {
          SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_UTIL_TASK_LP_TIMER_ERROR_CODE);
          res = SYS_UTIL_TASK_LP_TIMER_ERROR_CODE;
        }
        else if(pdPASS != xTimerReset(p_obj->auto_low_power_timer, pdMS_TO_TICKS(100)))
        {
          SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_UTIL_TASK_LP_TIMER_ERROR_CODE);
          res = SYS_UTIL_TASK_LP_TIMER_ERROR_CODE;
        }
      }
    }
  }

  return res;
}

static sys_error_code_t UtilTaskExecuteStepSensorsActive(AManagedTask *_this)
{
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  /*UtilTask_t *p_obj = (UtilTask_t*)_this;*/

  AMTExSetInactiveState((AManagedTaskEx*) _this, TRUE);
  vTaskDelay(pdMS_TO_TICKS(1000));
  AMTExSetInactiveState((AManagedTaskEx*) _this, FALSE);
  HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);

  return res;
}

static void UtilTaskSwTimerCallbackFunction(TimerHandle_t timer)
{
  SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("UTIL: lp timer expired.\r\n"));

  SysEvent event;
  event.nRawEvent = SYS_PM_MAKE_EVENT(SYS_PM_EVT_SRC_LP_TIMER, SYS_PM_EVT_PARAM_ENTER_LP);
  SysPostPowerModeEvent(event);
}

static inline
sys_error_code_t UtilTaskSetLED(UtilTask_t *_this, boolean_t on)
{
  UNUSED(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  if (on) {
	  LED_GREEN_GPIO_Port->BSRR = LED_GREEN_Pin;
  }
  else {
    LED_GREEN_GPIO_Port->BRR = LED_GREEN_Pin;
  }

  return res;
}


/* CubeMX integration */
/**********************/

void Util_UB_EXTI_Callback(uint16_t pin)
{
  /* anti debounch */
  static uint32_t t_start = 0;
  if(HAL_GetTick() - t_start > 1000)
  {
    if(pin == USER_Button_Pin)
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



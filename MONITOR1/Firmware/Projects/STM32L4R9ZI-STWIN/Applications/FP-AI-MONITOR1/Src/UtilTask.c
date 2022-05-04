/**
 ******************************************************************************
 * @file    UtilTask.c
 * @author  STMicroelectronics - AIS - MCD Team
 * @version V1.0.0
 * @date    15-September-2021
 *
 * @brief  UtilTask_t definition.
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

#include "UtilTask.h"
#include "UtilTask_vtbl.h"
#include "app_messages_parser.h"
#include "AppControllerMessagesDef.h"
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
#define UTIL_TASK_CFG_LP_TIMER_DEF_PERIOD_MS  10000
#endif

#define UTIL_ANTI_DEBOUNCH_PERIOD_TICK        7U
#define UTIL_LED_BLINK_SHORT_PERIOD_MS        60U
#define UTIL_LED_BLINK_PERIOD_MS              400U
#define UTIL_LED_BLINK_LONG_PERIOD_MS         800U
#define UTIL_LED_1_2_NOT_ACTIVE               0
#define UTIL_LED_1_ACTIVE                     1
#define UTIL_LED_2_ACTIVE                     2
#define UTIL_LED_1_2_ACTIVE                   3
#define UTIL_LED_2_BLINKING_TIME              2

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
 * Callback function called when the software timer for the auto low power mode expires.
 *
 * @param xTimer [IN] specifies the handle of the expired timer.
 */
static void UtilTaskSwTimerAutoLowPowerCallbackFunction(TimerHandle_t timer);

/**
 * Callback function called when the software timer for the double/single click expires.
 *
 * @param timer [IN] specifies the handle of the expired timer.
 */
static void UtilTaskSwTimerDoubleClickCallbackFunction(TimerHandle_t timer);

/**
 * Callback function called when the the software timer used to blink the LED expires.
 * @param timer [IN] specifies the handle of the expired timer.
 */
static void UtilTaskSwTimerLEDCallbackFunction(TimerHandle_t timer);

/**
 * Turn the user LED on/off.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @param on specifies [IN] the new status of the user LED.
 * @return SYS_NO_ERROR_CODE
 */
static inline sys_error_code_t UtilTaskSetLED(UtilTask_t *_this, bool on);


/* Inline function forward declaration */
/***************************************/

#if defined (__GNUC__) || defined (__ICCARM__)
/* Inline function defined inline in the header file UtilTask.h must be declared here as extern function. */
extern sys_error_code_t UtilTaskSetCtrlInQueue(UtilTask_t *_this, QueueHandle_t ctrl_queue);
extern sys_error_code_t UtilTaskSetLED2Enable(UtilTask_t *_this, bool enable);
extern QueueHandle_t UtilTaskGetInQueue(UtilTask_t *_this);
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

AManagedTaskEx *UtilTaskAlloc(const void *p_mx_sm_drv_cfg, const void *p_mx_bc_drv_cfg, const void *p_mx_bc_gpio_drv_cfg)
{
  /* In this application there is only one util task,
   * so this allocator implement the singleton design pattern.
   */

  /* Initialize the super class */
  AMTInitEx(&sTaskObj.super);

  sTaskObj.super.vptr = &sTheClass.vtbl;
  sTaskObj.p_mx_sm_drv_cfg = p_mx_sm_drv_cfg;
  sTaskObj.p_mx_bc_tim_drv_cfg = p_mx_bc_drv_cfg;
  sTaskObj.p_mx_bc_gpio_drv_cfg = p_mx_bc_gpio_drv_cfg;

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
    SYS_DEBUGF(SYS_DBG_LEVEL_SEVERE, ("UTIL task: unable to alloc SMUtilityDriver_t object.\r\n"));
    res = SYS_GET_LAST_LOW_LEVEL_ERROR_CODE();
  }
  else
  {
    SMUtilityDriverParams_t cfg_params = {
        .p_mx_tim_cfg = (void*)p_obj->p_mx_sm_drv_cfg
    };
    res = IDrvInit(p_obj->p_sm_driver, &cfg_params);
    if(SYS_IS_ERROR_CODE(res))
    {
      SYS_DEBUGF(SYS_DBG_LEVEL_SEVERE, ("UTIL task: error during SM driver initialization\r\n"));
    }
  }

  if (!SYS_IS_ERROR_CODE(res))
  {
    p_obj->p_bc_driver = BCDriverAlloc();
    if(p_obj->p_bc_driver == NULL)
    {
      SYS_DEBUGF(SYS_DBG_LEVEL_SEVERE, ("UTIL task: unable to alloc BCDriver_t object.\r\n"));
      res = SYS_GET_LAST_LOW_LEVEL_ERROR_CODE();
    }
    else
    {
      BCDriverParams_t cfg_params = {
          .p_mx_tim_cfg = (void*)p_obj->p_mx_bc_tim_drv_cfg,
          .p_mx_gpio_cfg = (void*)p_obj->p_mx_bc_gpio_drv_cfg
      };
      res = IDrvInit(p_obj->p_bc_driver, &cfg_params);
      if(SYS_IS_ERROR_CODE(res))
      {
        SYS_DEBUGF(SYS_DBG_LEVEL_SEVERE, ("UTIL task: error BC during driver initialization\r\n"));
      }
    }
  }

  if (!SYS_IS_ERROR_CODE(res))
  {
    /* Initialize the LED and User Button and Power Button */
    /* configure LED2 */
    __HAL_RCC_GPIOD_CLK_ENABLE();
    HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
    GPIO_InitStruct.Pin = LED2_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LED2_GPIO_Port, &GPIO_InitStruct);

    /* configure LED1 */
    __HAL_RCC_GPIOE_CLK_ENABLE();
    HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
    GPIO_InitStruct.Pin = LED1_Pin;
    HAL_GPIO_Init(LED1_GPIO_Port, &GPIO_InitStruct);

    /* configure the push button */
    __HAL_RCC_GPIOE_CLK_ENABLE();
    GPIO_InitStruct.Pin = USER_BUTTON_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(USER_BUTTON_GPIO_Port, &GPIO_InitStruct);

    HAL_NVIC_SetPriority(USER_BUTTON_EXTI_IRQn, 15, 0);

    /* configure the POWER BUTTON*/
    __HAL_RCC_GPIOD_CLK_ENABLE();

    /* Configure Button pin as input with External interrupt */
    GPIO_InitStruct.Pin = POWER_BUTTON_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(POWER_BUTTON_GPIO_Port, &GPIO_InitStruct);

    /* set the PB IRQ priority to 5 because it share the same IRQ line of IIS3DWB_INT1*/
    HAL_NVIC_SetPriority((IRQn_Type) POWER_BUTTON_EXTI_IRQn, 5, 0);
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
  p_obj->auto_low_power_timer = xTimerCreate("UtilT1", pdMS_TO_TICKS(p_obj->timeout_ms), pdFALSE, p_obj, UtilTaskSwTimerAutoLowPowerCallbackFunction);
  if(p_obj->auto_low_power_timer == NULL)
  {
    res = SYS_UTIL_TASK_INIT_ERROR_CODE;
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_UTIL_TASK_INIT_ERROR_CODE);
    return res;
  }

  p_obj->pb_timer = xTimerCreate("UtilT2", pdMS_TO_TICKS(UTIL_DOUBLE_PRESS_PERIOD_MS), pdFALSE, p_obj, UtilTaskSwTimerDoubleClickCallbackFunction);
  if(p_obj->pb_timer == NULL)
  {
    res = SYS_UTIL_TASK_INIT_ERROR_CODE;
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_UTIL_TASK_INIT_ERROR_CODE);
    return res;
  }

  p_obj->led_timer = xTimerCreate("UtilT3", pdMS_TO_TICKS(UTIL_LED_BLINK_PERIOD_MS), pdTRUE, p_obj, UtilTaskSwTimerLEDCallbackFunction);
  if(p_obj->pb_timer == NULL)
  {
    res = SYS_UTIL_TASK_INIT_ERROR_CODE;
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_UTIL_TASK_INIT_ERROR_CODE);
    return res;
  }

  /* initialize the protocol object */
  res = BCPInit(&p_obj->bc_protocol, p_obj->p_bc_driver);
  if(SYS_IS_ERROR_CODE(res))
  {
    return res;
  }

  /* set the (PM_STATE, ExecuteStepFunc) map from the class object.  */
  _this->m_pfPMState2FuncMap = sTheClass.p_pm_state2func_map;

  p_obj->button_down_ts_tick = 0;
  p_obj->button_pressed_count = 0;
  p_obj->ctrl_queue = NULL;
  p_obj->led_blinking_period = UTIL_LED_BLINK_PERIOD_MS;
  p_obj->led_mask = UTIL_LED_1_ACTIVE;
  p_obj->led_2_neai_blinking_count = 0;
  p_obj->pb_evt_skip_count = 1;
  p_obj->led_2_ai_blinking_count = 0;
  p_obj->led_2_ai_blinking_period = 0;

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
    /*stop the LED timer to stop the blinking*/
    if(pdPASS != xTimerStop(p_obj->led_timer, pdMS_TO_TICKS(100)))
    {
      SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_UTIL_TASK_LED_TIMER_ERROR_CODE);
      res = SYS_UTIL_TASK_LED_TIMER_ERROR_CODE;
    }

    /* turn on the USER LED GREEN */
    HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);

    /* turn off the USER LED ORANGE*/
    HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);

    p_obj->led_mask = UTIL_LED_1_ACTIVE;

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

    SysPowerStatus sys_pm_status = SysGetPowerStatus();
    if (sys_pm_status.active_ai_lib_id == SYS_X_CUBE_AI_ID)
    {
      p_obj->led_blinking_period =  UTIL_LED_BLINK_SHORT_PERIOD_MS;
      p_obj->led_mask |= UTIL_LED_2_ACTIVE;
      p_obj->led_2_ai_blinking_count = 0;
      p_obj->led_2_ai_blinking_period = 0;
    }
    else if (sys_pm_status.active_ai_lib_id == SYS_NEAI_DETECT_ID)
    {
      p_obj->led_blinking_period = UTIL_LED_BLINK_PERIOD_MS;
    }
    else if (sys_pm_status.active_ai_lib_id == SYS_NEAI_LEARN_ID)
    {
      p_obj->led_blinking_period = UTIL_LED_BLINK_LONG_PERIOD_MS;
    }

    p_obj->led_2_neai_blinking_count = 0;

    if (pdPASS != xTimerChangePeriod(p_obj->led_timer, p_obj->led_blinking_period, pdMS_TO_TICKS(100)))
    {
      SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_UTIL_TASK_LED_TIMER_ERROR_CODE);
      res = SYS_UTIL_TASK_LED_TIMER_ERROR_CODE;
    }
  }
  else if(new_power_mode == E_POWER_MODE_SLEEP_1)
  {
    /*stop the LED timer to stop the blinking*/
    if(pdPASS != xTimerStop(p_obj->led_timer, pdMS_TO_TICKS(100)))
    {
      SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_UTIL_TASK_LED_TIMER_ERROR_CODE);
      res = SYS_UTIL_TASK_LED_TIMER_ERROR_CODE;
    }

    /* turn off the USER LED GREEN */
    HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);

    /* turn off the USER LED ORANGE*/
    HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);

    if(pdPASS != xTimerStop(p_obj->auto_low_power_timer, pdMS_TO_TICKS(100)))
    {
      SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_UTIL_TASK_LP_TIMER_ERROR_CODE);
      res = SYS_UTIL_TASK_LP_TIMER_ERROR_CODE;
    }

    p_obj->led_mask = UTIL_LED_1_ACTIVE;
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
  HAL_NVIC_EnableIRQ(USER_BUTTON_EXTI_IRQn);
  HAL_NVIC_EnableIRQ(POWER_BUTTON_EXTI_IRQn);
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

  if((active_power_mode == E_POWER_MODE_STATE1) || (active_power_mode == E_POWER_MODE_SENSORS_ACTIVE))
  {
    xQueueSendToFront(p_obj->in_queue, &msg, pdMS_TO_TICKS(100));
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
  struct CtrlMessage_t out_msg = {
      .msgId = APP_MESSAGE_ID_CTRL,
      .cmd_id = CTRL_CMD_STRAT,
      .sparam = CTRL_CMD_PARAM_FROM_AUTOMODE
  };

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

      else if (msg.cmd_id == UTIL_CMD_ID_BUTTON_EVT)
      {
        SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("UTIL: button %c ts=%u\r\n",
                   msg.sparam == UTIL_PARAM_BUTTON_EVT_DOWN ? 'd' : 'u',
                   msg.param));

        if (msg.sparam == UTIL_PARAM_BUTTON_EVT_DOWN)
        {
          p_obj->button_down_ts_tick = msg.param;
        }
        else if (msg.sparam == UTIL_PARAM_BUTTON_EVT_UP)
        {
          /* this is a UTIL_PARAM_BUTTON_EVT_UP*/

          /* first check if it is a long press*/
          uint32_t period = msg.param - p_obj->button_down_ts_tick;
          if (period > UTIL_SHORT_PRESS_PERIOD_THRESHOLD_T)
          {
            SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("UTIL: long press evt\r\n"));

            /* reset the state the PB related members */
            if (p_obj->button_pressed_count != 0)
            {
              xTimerStop(p_obj->pb_timer, pdMS_TO_TICKS(100));
            }
            p_obj->button_pressed_count = 0;

            /*send the event to the AppController to start the execution phase*/
            if (p_obj->ctrl_queue != NULL)
            {
              out_msg.param  = CTRL_CMD_PARAM_NEAI_LEARN;
              xQueueSendToBack(p_obj->ctrl_queue, &out_msg, portMAX_DELAY);
            }
          }
          else
          {
            /* short press */
            if (++p_obj->button_pressed_count < 2)
            {
              /* first button pressed -> start the timer */
              xTimerReset(p_obj->pb_timer, pdMS_TO_TICKS(100));
            }
            else
            {
              /* double press event */
              SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("UTIL: double press evt\r\n"));

              /* reset the state the PB related members*/
              xTimerStop(p_obj->pb_timer, pdMS_TO_TICKS(100));
              p_obj->button_pressed_count = 0;

              /*send the event to the AppController to start the execution phase*/
              if (p_obj->ctrl_queue != NULL)
              {
                out_msg.param  = CTRL_CMD_PARAM_NEAI_DETECT;
                xQueueSendToBack(p_obj->ctrl_queue, &out_msg, portMAX_DELAY);
              }
            }
          }
        }
        else if (msg.sparam == UTIL_PARAM_BUTTON_EVT_POWER_PRESSED)
        {
          /* turn-off the battery charger */
          if (p_obj->pb_evt_skip_count-- == 0)
          {
            res = BCPPowerOff(&p_obj->bc_protocol);
          }
        }
      }

      else if (msg.cmd_id == UTIL_CMD_ID_UB_TIMER_ELAPSED)
      {
        /* double check that this is a a single short press */
        if (p_obj->button_pressed_count < 2)
        {
          SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("UTIL: short press\r\n"));

          /* reset the state of the PB related members*/
          p_obj->button_pressed_count = 0;

          /*send the event to the AppController to start the execution phase*/
          if (p_obj->ctrl_queue != NULL)
          {
            out_msg.param  = CTRL_CMD_PARAM_AI;
            xQueueSendToBack(p_obj->ctrl_queue, &out_msg, portMAX_DELAY);
          }
        }
      }
      else {
        SYS_DEBUGF(SYS_DBG_LEVEL_WARNING, ("UTIL: unsupported cmd ID:%x\r\n", msg.cmd_id));
      }
    }
  }

  return res;
}

static sys_error_code_t UtilTaskExecuteStepSensorsActive(AManagedTask *_this)
{
  assert_param(_this);
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
      if(msg.cmd_id == UTIL_CMD_ID_BUTTON_EVT)
      {
        if (msg.sparam == UTIL_PARAM_BUTTON_EVT_UP)
        {
          /* generate the system event to stop the execution phases*/
          SysEvent evt = {
              .nRawEvent = SYS_PM_MAKE_EVENT(SYS_PM_EVT_SRC_PB, SYS_PM_EVT_PARAM_SHORT_PRESS)
          };
          SysPostPowerModeEvent(evt);
        }
      }
      else if ((msg.cmd_id == UTIL_CMD_ID_LED2_SET_ENABLE) && (p_obj->led_2_neai_blinking_count == 0))
      {
        if (msg.sparam == 1)
        {
          p_obj->led_mask |= UTIL_LED_2_ACTIVE;
        }
        else
        {
          p_obj->led_mask &= ~(UTIL_LED_2_ACTIVE);
        }
      }
      else if (msg.cmd_id == UTIL_CMD_ID_LED2_SET_BLK_PERIOD)
      {
        p_obj->led_2_ai_blinking_period = msg.sparam;
      }
      else
      {
        SYS_DEBUGF(SYS_DBG_LEVEL_WARNING, ("UTIL: unsupported cmd ID:%x\r\n", msg.cmd_id));
      }
    }
  }

  return res;
}

static void UtilTaskSwTimerAutoLowPowerCallbackFunction(TimerHandle_t timer)
{
  SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("UTIL: lp timer expired.\r\n"));

  SysEvent event;
  event.nRawEvent = SYS_PM_MAKE_EVENT(SYS_PM_EVT_SRC_LP_TIMER, SYS_PM_EVT_PARAM_ENTER_LP);
  SysPostPowerModeEvent(event);
}

static void UtilTaskSwTimerDoubleClickCallbackFunction(TimerHandle_t timer)
{
  UtilTask_t *p_obj = (UtilTask_t*)pvTimerGetTimerID(timer);
  struct utilMessage_t msg = {
      .msgId = APP_MESSAGE_ID_UTIL,
      .cmd_id = UTIL_CMD_ID_UB_TIMER_ELAPSED
  };

  xQueueSendToBack(p_obj->in_queue, &msg, pdMS_TO_TICKS(100));
}

static void UtilTaskSwTimerLEDCallbackFunction(TimerHandle_t timer)
{
  UtilTask_t *p_obj = (UtilTask_t*)pvTimerGetTimerID(timer);

  if (p_obj->led_mask & UTIL_LED_1_ACTIVE)
  {
    HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
  }

  if (p_obj->led_mask & UTIL_LED_2_ACTIVE)
  {
    /* we manage the ORANGE LED in different way depending on the active AI engine */
    SysPowerStatus sys_status = SysGetPowerStatus();
    if (sys_status.active_ai_lib_id == SYS_X_CUBE_AI_ID)
    {
      if (p_obj->led_2_ai_blinking_period == 0)
      {
          HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
          p_obj->led_2_ai_blinking_count = 0;
      }
      else if (p_obj->led_2_ai_blinking_period == 0xFF)
      {
        HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
        p_obj->led_2_ai_blinking_count = 0;
      }
      else
      {
        if (p_obj->led_2_ai_blinking_count++ >= p_obj->led_2_ai_blinking_period)
        {
          HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
          p_obj->led_2_ai_blinking_count = 0;
        }
      }
    }
    else
    {
      HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
      if (p_obj->led_2_neai_blinking_count++ >= 2 * UTIL_LED_2_BLINKING_TIME)
      {
        /*stop the LED2 blinking*/
        HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
        p_obj->led_mask &= ~(UTIL_LED_2_ACTIVE);
        p_obj->led_2_neai_blinking_count = 0;
      }
    }
  }
}

static inline
sys_error_code_t UtilTaskSetLED(UtilTask_t *_this, bool on)
{
  UNUSED(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  if (on) {
    LED1_GPIO_Port->BSRR = LED1_Pin;
  }
  else {
    LED1_GPIO_Port->BRR = LED1_Pin;
  }

  return res;
}


/* CubeMX integration */
/**********************/

void Util_UB_EXTI_Callback(uint16_t pin)
{
  /* anti debounch */
  static uint32_t t_start = 0;
  uint32_t gpio_in_reg = 0;
  if(HAL_GetTick() - t_start > UTIL_ANTI_DEBOUNCH_PERIOD_TICK)
  {
    if(pin == USER_BUTTON_Pin)
    {
      /* we are in an ISR so we read directly the GPIO register instead of calling the HAL function*/
      gpio_in_reg = USER_BUTTON_GPIO_Port->IDR;
      /* prepare the button event message */
      struct utilMessage_t msg = {
          .msgId = APP_MESSAGE_ID_UTIL,
          .param = (uint32_t)xTaskGetTickCountFromISR(),
          .cmd_id = UTIL_CMD_ID_BUTTON_EVT,
      };
      msg.sparam = gpio_in_reg & USER_BUTTON_Pin ? UTIL_PARAM_BUTTON_EVT_DOWN : UTIL_PARAM_BUTTON_EVT_UP;
      xQueueSendToBackFromISR(sTaskObj.in_queue, &msg, NULL);

      t_start = HAL_GetTick();
    }
  }
}

void Util_PB_EXTI_Callback(uint16_t pin)
{
  /* anti debounch */
  static uint32_t t_start = 0;
  if(HAL_GetTick() - t_start > 10*UTIL_ANTI_DEBOUNCH_PERIOD_TICK)
  {
    if(pin == POWER_BUTTON_Pin)
    {
      /* generate the power-off message*/
      struct utilMessage_t msg = {
          .msgId = APP_MESSAGE_ID_UTIL,
          .sparam = UTIL_PARAM_BUTTON_EVT_POWER_PRESSED,
          .cmd_id = UTIL_CMD_ID_BUTTON_EVT,
      };
      xQueueSendToBackFromISR(sTaskObj.in_queue, &msg, NULL);
      t_start = HAL_GetTick();
    }
  }

}



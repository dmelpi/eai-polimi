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
#include "mx.h"
#include "services/SIterator.h"
#include "services/sysdebug.h"


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
#define UTIL_TASK_CFG_LP_TIMER_DEF_PERIOD_MS   10000
#endif

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
 * Set the default sensor configuration.
 *
 * @sa SLSensorsConfigF
 */
static sys_error_code_t UtilTaskDefSensorsConfig(SensorManager_t *p_sm, uint16_t *p_active_sensor_id);


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

AManagedTaskEx *UtilTaskAlloc(const void *p_mx_sm_drv_cfg, const void *p_mx_sm_ld_cfg)
{
  /* In this application there is only one Keyboard task,
   * so this allocator implement the singleton design pattern.
   */

  /* Initialize the super class */
  AMTInitEx(&sTaskObj.super);

  sTaskObj.super.vptr = &sTheClass.vtbl;
  sTaskObj.p_mx_sm_drv_cfg = p_mx_sm_drv_cfg;
  sTaskObj.p_mx_ld_drv_cfg = p_mx_sm_ld_cfg;

  return (AManagedTaskEx*)&sTaskObj;
}

sys_error_code_t UtilTaskSetAutoLowPowerModePeriod(UtilTask_t *_this, uint32_t timeout_ms)
{
  assert_param(_this != NULL);

  _this->timeout_ms = timeout_ms;

  return SYS_NO_ERROR_CODE;
}

sys_error_code_t UtilTaskSetSensorListener(UtilTask_t *_this, SensorListener_t *p_sensor_listener)
{
  assert_param(_this != NULL);

  if (_this->p_sensor_listener == NULL)
  {
    _this->p_sensor_listener = p_sensor_listener;
  }

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
//  GPIO_InitTypeDef GPIO_InitStruct = { 0 };

  p_obj->p_sm_driver = SMUtilityDriverAlloc();
  if(p_obj->p_sm_driver == NULL)
  {
    SYS_DEBUGF(SYS_DBG_LEVEL_SEVERE, ("UTIL task: unable to alloc driver object.\r\n"));
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
      SYS_DEBUGF(SYS_DBG_LEVEL_SEVERE, ("UTIL task: error during driver initialization\r\n"));
    }
  }

  if (!SYS_IS_ERROR_CODE(res))
  {
    /* Initialize the LED and User Button */
    // configure LED
    if (p_obj->p_mx_ld_drv_cfg != NULL) {
      ((MX_GPIOParams_t*)p_obj->p_mx_ld_drv_cfg)->p_mx_init_f();
    }
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
    if (p_obj->p_mx_ld_drv_cfg != NULL)
    {
      MX_GPIOParams_t *p_ld_params = (MX_GPIOParams_t*)p_obj->p_mx_ld_drv_cfg;
      HAL_GPIO_WritePin(p_ld_params->port, p_ld_params->pin, GPIO_PIN_SET);
    }

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
    if (p_obj->p_mx_ld_drv_cfg != NULL)
    {
      MX_GPIOParams_t *p_ld_params = (MX_GPIOParams_t*)p_obj->p_mx_ld_drv_cfg;
      HAL_GPIO_WritePin(p_ld_params->port, p_ld_params->pin, GPIO_PIN_RESET);
    }

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
  UtilTask_t *p_obj = (UtilTask_t*)_this;

  /* turn on the USER LED */
  if (p_obj->p_mx_ld_drv_cfg != NULL)
  {
    MX_GPIOParams_t *p_ld_params = (MX_GPIOParams_t*)p_obj->p_mx_ld_drv_cfg;
    HAL_GPIO_WritePin(p_ld_params->port, p_ld_params->pin, GPIO_PIN_SET);
  }

  /* initialize the sensor listener and set the sensors default configuration */
  if (p_obj->p_sensor_listener != NULL)
  {
    SensorListenerInit(p_obj->p_sensor_listener);
    SensorListenerSetDefSensorConfifF(p_obj->p_sensor_listener, UtilTaskDefSensorsConfig);
    SensorListenerUpdateSnsorConfiguration(p_obj->p_sensor_listener);
  }

  SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("UTIL: start.\r\n"));

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
    /* the task is waiting for a message */
    xQueueSendToFront(p_obj->in_queue, &msg, pdMS_TO_TICKS(100));
  }
  else if(active_power_mode == E_POWER_MODE_SENSORS_ACTIVE)
  {
    /* the task is suspended for 1s */
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
  UtilTask_t *p_obj = (UtilTask_t*)_this;

  AMTExSetInactiveState((AManagedTaskEx*) _this, TRUE);
  vTaskDelay(pdMS_TO_TICKS(1000));
  AMTExSetInactiveState((AManagedTaskEx*) _this, FALSE);
  if (p_obj->p_mx_ld_drv_cfg != NULL)
  {
    MX_GPIOParams_t *p_ld_params = (MX_GPIOParams_t*)p_obj->p_mx_ld_drv_cfg;
    HAL_GPIO_TogglePin(p_ld_params->port, p_ld_params->pin);
  }

  return res;
}

static void UtilTaskSwTimerCallbackFunction(TimerHandle_t timer)
{
  SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("UTIL: lp timer expired.\r\n"));

  SysEvent event;
  event.nRawEvent = SYS_PM_MAKE_EVENT(SYS_PM_EVT_SRC_LP_TIMER, SYS_PM_EVT_PARAM_ENTER_LP);
  SysPostPowerModeEvent(event);
}

static sys_error_code_t UtilTaskDefSensorsConfig(SensorManager_t *p_sm, uint16_t *p_active_sensor_id)
{
  assert_param(p_sm != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  /* Set the default configuration for the sensors */
  /* First disable all sensors and register the CTRL as listener. */
  SIterator_t itarator;
  uint16_t sensor_id = SI_NULL_SENSOR_ID;
  SIInit(&itarator, p_sm);
  while (SIHasNext(&itarator))
  {
    sensor_id = SINext(&itarator);
    SMSensorDisable(sensor_id);
  }
  /* Enable only one sensor */
  SIInit(&itarator, p_sm);
  if (SIHasNext(&itarator))
  {
    sensor_id = SINext(&itarator);
    SMSensorEnable(sensor_id);
    SMSensorSetODR(sensor_id, 1.0f);
    *p_active_sensor_id = sensor_id;
  }
  else
  {
    SYS_DEBUGF(SYS_DBG_LEVEL_WARNING, ("UTIL: no sensors found in SM!\r\n"));
  }

  return res;
}


/* CubeMX integration */
/**********************/



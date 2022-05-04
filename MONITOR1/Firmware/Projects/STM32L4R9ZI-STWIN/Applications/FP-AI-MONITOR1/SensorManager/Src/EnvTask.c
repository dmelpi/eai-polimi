/**
 ******************************************************************************
 * @file    EnvTask.c
 * @author  SRA - MCD
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


#include "EnvTask.h"
#include "EnvTask_vtbl.h"
#include "SMMessageParser.h"
#include "SensorCommands.h"
#include "SensorDef.h"
#include "SensorRegister.h"
#include "events/ISensorEventListener.h"
#include "events/ISensorEventListener_vtbl.h"
#include "ISensor.h"
#include "ISensor_vtbl.h"
#include "SMUtilTask.h"
#include <string.h>
#include "services/sysdebug.h"


#ifndef ENV_TASK_CFG_STACK_DEPTH
#define ENV_TASK_CFG_STACK_DEPTH        (180)
#endif

#ifndef ENV_TASK_CFG_PRIORITY
#define ENV_TASK_CFG_PRIORITY           (tskIDLE_PRIORITY)
#endif

#ifndef ENV_TASK_CFG_IN_QUEUE_LENGTH
#define ENV_TASK_CFG_IN_QUEUE_LENGTH          20
#endif

#define ENV_TASK_CFG_IN_QUEUE_ITEM_SIZE       sizeof(SMMessage)

#define SYS_DEBUGF(level, message)      SYS_DEBUGF3(SYS_DBG_ENV, level, message)

#if defined(DEBUG) || defined (SYS_DEBUG)
#define sTaskObj                        sEnvTaskObj
#endif

#ifndef HSD_USE_DUMMY_DATA
#define HSD_USE_DUMMY_DATA 0
#endif


static const ISensor_vtbl sEnvTask_HTS221TempSensorIF_vtbl = {
    EnvTask_vtblHTS221TempGetId,
    EnvTask_vtblHTS221TempGetEventSourceIF,
    EnvTask_vtblHTS221TempGetODR,
    EnvTask_vtblHTS221TempGetFS,
    EnvTask_vtblHTS221TempGetSensitivity,
    EnvTask_vtblSensorStart,
    EnvTask_vtblSensorStop,
    EnvTask_vtblSensorSetODR,
    EnvTask_vtblSensorSetFS,
    EnvTask_vtblSensorEnable,
    EnvTask_vtblSensorDisable,
    EnvTask_vtblSensorIsEnabled,
    EnvTask_vtblSensorGetDescription
};

static const ISensor_vtbl sEnvTask_HTS221HumSensorIF_vtbl = {
    EnvTask_vtblHTS221HumGetId,
    EnvTask_vtblHTS221HumGetEventSourceIF,
    EnvTask_vtblHTS221HumGetODR,
    EnvTask_vtblHTS221HumGetFS,
    EnvTask_vtblHTS221HumGetSensitivity,
    EnvTask_vtblSensorStart,
    EnvTask_vtblSensorStop,
    EnvTask_vtblSensorSetODR,
    EnvTask_vtblSensorSetFS,
    EnvTask_vtblSensorEnable,
    EnvTask_vtblSensorDisable,
    EnvTask_vtblSensorIsEnabled,
    EnvTask_vtblSensorGetDescription
};

static const ISensor_vtbl sEnvTask_LPS22HHPressSensorIF_vtbl = {
    EnvTask_vtblLPS22HHPressGetId,
    EnvTask_vtblLPS22HHPressGetEventSourceIF,
    EnvTask_vtblLPS22HHPressGetODR,
    EnvTask_vtblLPS22HHPressGetFS,
    EnvTask_vtblLPS22HHPressGetSensitivity,
    EnvTask_vtblSensorStart,
    EnvTask_vtblSensorStop,
    EnvTask_vtblSensorSetODR,
    EnvTask_vtblSensorSetFS,
    EnvTask_vtblSensorEnable,
    EnvTask_vtblSensorDisable,
    EnvTask_vtblSensorIsEnabled,
    EnvTask_vtblSensorGetDescription
};

static const ISensor_vtbl sEnvTask_LPS22HHTempSensorIF_vtbl = {
    EnvTask_vtblLPS22HHTempGetId,
    EnvTask_vtblLPS22HHTempGetEventSourceIF,
    EnvTask_vtblLPS22HHTempGetODR,
    EnvTask_vtblLPS22HHTempGetFS,
    EnvTask_vtblLPS22HHTempGetSensitivity,
    EnvTask_vtblSensorStart,
    EnvTask_vtblSensorStop,
    EnvTask_vtblSensorSetODR,
    EnvTask_vtblSensorSetFS,
    EnvTask_vtblSensorEnable,
    EnvTask_vtblSensorDisable,
    EnvTask_vtblSensorIsEnabled,
    EnvTask_vtblSensorGetDescription
};

/**
 *  EnvTask_t internal structure.
 */
struct _EnvTask {
  /**
   * Base class object.
   */
  AManagedTaskEx super;

  /**
   * Synchronization object used to send command to the task.
   */
  QueueHandle_t in_queue;

  /**
   * HTS221 class
   */
  HTS221Class HTS221_class_obj;

  /**
   * LPS22HH class
   */
  LPS22HHClass LPS22HH_class_obj;

};

/**
 * Class object declaration
 */
typedef struct _EnvTaskClass {
  /**
   * EnvTask class virtual table.
   */
  AManagedTaskEx_vtbl vtbl;

  /**
   * EnvTask (PM_STATE, ExecuteStepFunc) map.
   */
  pExecuteStepFunc_t p_pm_state2func_map[];
} EnvTaskClass_t;


/* Private member function declaration */
/***************************************/

/**
 * Obtain the task object.
 *
 * @return EnvTask task object.
 */
static EnvTask * EnvTaskGetInstance(void);

/**
 * Execute one step of the task control loop while the system is in RUN mode.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return SYS_NO_EROR_CODE if success, a task specific error code otherwise.
 */
static sys_error_code_t EnvTaskExecuteStepState1(AManagedTask *_this);

/**
 * Execute one step of the task control loop while the system is in SENSORS_ACTIVE mode.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return SYS_NO_EROR_CODE if success, a task specific error code otherwise.
 */
static sys_error_code_t EnvTaskExecuteStepDatalog(AManagedTask *_this);

/**
 * Register the sensor with the global DB and initialize the default parameters.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return SYS_NO_ERROR_CODE if success, an error code otherwise
 */
static sys_error_code_t EnvTaskSensorRegister(EnvTask *_this);

/**
 * Initialize the default parameters.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return SYS_NO_ERROR_CODE if success, an error code otherwise
 */
static sys_error_code_t EnvTaskSensorInitTaskParams(EnvTask *_this);

/**
 * Dispatch commands for HTS221 sensor
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @param pReport [IN] specifies a report to send.
 * @return SYS_NO_ERROR_CODE if success, an error code otherwise
 */
static sys_error_code_t HTS221ExecuteCommand(EnvTask *_this, SMMessage report);

/**
 * Dispatch commands for LPS22HH sensor
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @param pReport [IN] specifies a report to send.
 * @return SYS_NO_ERROR_CODE if success, an error code otherwise
 */
static sys_error_code_t LPS22HHExecuteCommand(EnvTask *_this, SMMessage report);

/**
 * SPI CS Pin interrupt callback
 */
void HTS221Class_EXTI_Callback(uint16_t nPin);

/**
 * Callback function called when the software timer expires.
 *
 * @param xTimer [IN] specifies the handle of the expired timer.
 */
void LPS22HHClassTimerCallbackFunction(TimerHandle_t timer);


/* Inline function forward declaration */
/***************************************/

/**
 * Private function used to post a report into the front of the task queue.
 * Used to resume the task when the required by the INIT task.
 *
 * @param this [IN] specifies a pointer to the task object.
 * @param pReport [IN] specifies a report to send.
 * @return SYS_NO_EROR_CODE if success, SYS_APP_TASK_REPORT_LOST_ERROR_CODE.
 */
static inline sys_error_code_t EnvTaskPostReportToFront(EnvTask *_this, SMMessage *pReport);

/**
 * Private function used to post a report into the back of the task queue.
 * Used to resume the task when the required by the INIT task.
 *
 * @param this [IN] specifies a pointer to the task object.
 * @param pReport [IN] specifies a report to send.
 * @return SYS_NO_EROR_CODE if success, SYS_APP_TASK_REPORT_LOST_ERROR_CODE.
 */
static inline sys_error_code_t EnvTaskPostReportToBack(EnvTask *_this, SMMessage *pReport);


#if defined (__GNUC__) || defined (__ICCARM__)
/* Inline function defined inline in the header file EnvTask.h must be declared here as extern function. */
#endif


/* Objects instance */
/********************/

/**
 * The only instance of the task object.
 */
static EnvTask sTaskObj;

/**
 * The class object.
 */
static const EnvTaskClass_t sTheClass = {
    /* Class virtual table */
    {
        EnvTask_vtblHardwareInit,
        EnvTask_vtblOnCreateTask,
        EnvTask_vtblDoEnterPowerMode,
        EnvTask_vtblHandleError,
        EnvTask_vtblOnEnterTaskControlLoop,
        EnvTask_vtblForceExecuteStep,
        EnvTask_vtblOnEnterPowerMode
    },

    /* class (PM_STATE, ExecuteStepFunc) map */
    {
        EnvTaskExecuteStepState1,
        NULL,
        EnvTaskExecuteStepDatalog,
    }
};


/* Public API definition */
/*************************/

ISourceObservable *EnvTaskGetHTS221TempSensorIF(EnvTask *_this){
  return (ISourceObservable *)&(_this->HTS221_class_obj.temp_sensor_if);
}

ISourceObservable *EnvTaskGetHTS221HumSensorIF(EnvTask *_this){
  return (ISourceObservable *)&(_this->HTS221_class_obj.hum_sensor_if);
}

ISourceObservable *EnvTaskGetLPS22HHPressSensorIF(EnvTask *_this){
  return (ISourceObservable *)&(_this->LPS22HH_class_obj.press_sensor_if);
}

ISourceObservable *EnvTaskGetLPS22HHTempSensorIF(EnvTask *_this){
  return (ISourceObservable *)&(_this->LPS22HH_class_obj.temp_sensor_if);
}

AManagedTaskEx *EnvTaskAlloc(void)
{
  /* In this application there is only one Keyboard task,
   * so this allocator implement the singleton design pattern.
   */

  /* Initialize the super class */
  AMTInitEx(&sTaskObj.super);

  sTaskObj.super.vptr = &sTheClass.vtbl;
  sTaskObj.HTS221_class_obj.temp_sensor_if.vptr = &sEnvTask_HTS221TempSensorIF_vtbl;
  sTaskObj.HTS221_class_obj.hum_sensor_if.vptr = &sEnvTask_HTS221HumSensorIF_vtbl;
  sTaskObj.LPS22HH_class_obj.press_sensor_if.vptr = &sEnvTask_LPS22HHPressSensorIF_vtbl;
  sTaskObj.LPS22HH_class_obj.temp_sensor_if.vptr = &sEnvTask_LPS22HHTempSensorIF_vtbl;

  return (AManagedTaskEx*)&sTaskObj;
}


I2CBusIF *EnvTaskGetHTS221SensorIF(EnvTask *_this) {
  assert_param(_this);

  return &_this->HTS221_class_obj.sensor_if;
}

I2CBusIF *EnvTaskGetLPS22HHSensorIF(EnvTask *_this) {
  assert_param(_this);

  return &_this->LPS22HH_class_obj.sensor_if;
}

IEventSrc *EnvTaskGetHTS221TempEventSrcIF(EnvTask *_this) {
  assert_param(_this);

  return _this->HTS221_class_obj.p_temp_event_src;
}

IEventSrc *EnvTaskGetHTS221HumEventSrcIF(EnvTask *_this) {
  assert_param(_this);

  return _this->HTS221_class_obj.p_hum_event_src;
}

IEventSrc *EnvTaskGetLPS22HHPressEventSrcIF(EnvTask *_this) {
  assert_param(_this);

  return _this->LPS22HH_class_obj.p_press_event_src;
}

IEventSrc *EnvTaskGetLPS22HHTempEventSrcIF(EnvTask *_this) {
  assert_param(_this);

  return _this->LPS22HH_class_obj.p_temp_event_src;
}

/* AManagedTask virtual functions definition */
/*********************************************/

sys_error_code_t EnvTask_vtblHardwareInit(AManagedTask *_this, void *p_params)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  EnvTask *p_obj = (EnvTask*)_this;

  HTS221ClassConfigureIrqPin(&p_obj->HTS221_class_obj, FALSE);

  return res;
}

sys_error_code_t EnvTask_vtblOnCreateTask(AManagedTask *_this, TaskFunction_t *p_task_code, const char **p_name, unsigned short *p_stack_depth, void **p_params, UBaseType_t *p_priority)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  EnvTask *p_obj = (EnvTask*)_this;

  // Create task specific sw resources.
  p_obj->in_queue = xQueueCreate(ENV_TASK_CFG_IN_QUEUE_LENGTH, ENV_TASK_CFG_IN_QUEUE_ITEM_SIZE);
  if (p_obj->in_queue == NULL) {
    res = SYS_TASK_HEAP_OUT_OF_MEMORY_ERROR_CODE;
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(res);
    return res;
  }

#ifdef DEBUG
  vQueueAddToRegistry(p_obj->in_queue, "ENVTASK_Q");
#endif

  /* create the software timer used to read LPS22HH data */
  p_obj->LPS22HH_class_obj.read_fifo_timer = xTimerCreate("LPS22HH_T", pdMS_TO_TICKS(LPS22HH_CLASS_CFG_TIMER_PERIOD_MS), pdFALSE, p_obj, LPS22HHClassTimerCallbackFunction);
  if(p_obj->LPS22HH_class_obj.read_fifo_timer == NULL)
  {
    res = SYS_TASK_HEAP_OUT_OF_MEMORY_ERROR_CODE;
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(res);
    return res;
  }

  res = I2CBusIFInit(&p_obj->HTS221_class_obj.sensor_if, HTS221_ID, HTS221_I2C_ADDRESS);
  if (SYS_IS_ERROR_CODE(res)) {
    return res;
  }
  ABusIFSetHandle(&p_obj->HTS221_class_obj.sensor_if.super, &p_obj->HTS221_class_obj.sensor_if);

  res = I2CBusIFInit(&p_obj->LPS22HH_class_obj.sensor_if, LPS22HH_ID, LPS22HH_I2C_ADD_H);
  if (SYS_IS_ERROR_CODE(res)) {
    return res;
  }
  ABusIFSetHandle(&p_obj->LPS22HH_class_obj.sensor_if.super, &p_obj->LPS22HH_class_obj.sensor_if);

  // Initialize the EventSrc interface.
  p_obj->HTS221_class_obj.p_temp_event_src = SensorEventSrcAlloc();
  if (p_obj->HTS221_class_obj.p_temp_event_src == NULL) {
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_OUT_OF_MEMORY_ERROR_CODE);
    res = SYS_OUT_OF_MEMORY_ERROR_CODE;
    return res;
  }
  IEventSrcInit(p_obj->HTS221_class_obj.p_temp_event_src);

  p_obj->HTS221_class_obj.p_hum_event_src = SensorEventSrcAlloc();
  if (p_obj->HTS221_class_obj.p_hum_event_src == NULL) {
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_OUT_OF_MEMORY_ERROR_CODE);
    res = SYS_OUT_OF_MEMORY_ERROR_CODE;
    return res;
  }
  IEventSrcInit(p_obj->HTS221_class_obj.p_hum_event_src);

  p_obj->LPS22HH_class_obj.p_press_event_src = SensorEventSrcAlloc();
  if (p_obj->LPS22HH_class_obj.p_press_event_src == NULL) {
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_OUT_OF_MEMORY_ERROR_CODE);
    res = SYS_OUT_OF_MEMORY_ERROR_CODE;
    return res;
  }
  IEventSrcInit(p_obj->LPS22HH_class_obj.p_press_event_src);

  p_obj->LPS22HH_class_obj.p_temp_event_src = SensorEventSrcAlloc();
  if (p_obj->LPS22HH_class_obj.p_temp_event_src == NULL) {
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_OUT_OF_MEMORY_ERROR_CODE);
    res = SYS_OUT_OF_MEMORY_ERROR_CODE;
    return res;
  }
  IEventSrcInit(p_obj->LPS22HH_class_obj.p_temp_event_src);

  memset(p_obj->HTS221_class_obj.p_sensor_data_buff, 0, sizeof(p_obj->HTS221_class_obj.p_sensor_data_buff));
  p_obj->HTS221_class_obj.temp_id = 0;
  p_obj->HTS221_class_obj.hum_id = 1;
  p_obj->HTS221_class_obj.timestamp_tick = 0;
  p_obj->HTS221_class_obj.old_timestamp_tick = 0;
  p_obj->HTS221_class_obj.timestamp = 0;
  p_obj->HTS221_class_obj.x0_t = p_obj->HTS221_class_obj.y0_t = p_obj->HTS221_class_obj.x1_t = p_obj->HTS221_class_obj.y1_t = 0;
  p_obj->HTS221_class_obj.x0_h = p_obj->HTS221_class_obj.y0_h = p_obj->HTS221_class_obj.x1_h = p_obj->HTS221_class_obj.y1_h = 0;

  memset(p_obj->LPS22HH_class_obj.p_sensor_data_buff, 0, sizeof(p_obj->LPS22HH_class_obj.p_sensor_data_buff));
  memset(p_obj->LPS22HH_class_obj.p_temp_data_buff, 0, sizeof(p_obj->LPS22HH_class_obj.p_temp_data_buff));
  memset(p_obj->LPS22HH_class_obj.p_press_data_buff, 0, sizeof(p_obj->LPS22HH_class_obj.p_press_data_buff));
  p_obj->LPS22HH_class_obj.press_id = 2;
  p_obj->LPS22HH_class_obj.temp_id = 3;
  p_obj->LPS22HH_class_obj.timestamp_tick = 0;
  p_obj->LPS22HH_class_obj.old_timestamp_tick = 0;
  p_obj->LPS22HH_class_obj.timestamp = 0;
  p_obj->LPS22HH_class_obj.fifo_level = 0;
  p_obj->LPS22HH_class_obj.task_delay = 0;

  _this->m_pfPMState2FuncMap = sTheClass.p_pm_state2func_map;

  *p_task_code = AMTExRun;
  *p_name = "ENV";
  *p_stack_depth = ENV_TASK_CFG_STACK_DEPTH;
  *p_params = _this;
  *p_priority = ENV_TASK_CFG_PRIORITY;

  res = EnvTaskSensorInitTaskParams(p_obj);
  if (SYS_IS_ERROR_CODE(res)) {
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_OUT_OF_MEMORY_ERROR_CODE);
    res = SYS_OUT_OF_MEMORY_ERROR_CODE;
    return res;
  }

  res = EnvTaskSensorRegister(p_obj);
  if (SYS_IS_ERROR_CODE(res)) {
    SYS_DEBUGF(SYS_DBG_LEVEL_SEVERE, ("ENVTASK: unable to register with DB\r\n"));
    sys_error_handler();
  }

  return res;
}

sys_error_code_t EnvTask_vtblDoEnterPowerMode(AManagedTask *_this, const EPowerMode active_power_mode, const EPowerMode new_power_mode)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  EnvTask *p_obj = (EnvTask*)_this;
  stmdev_ctx_t *HTS221_sensor_drv = (stmdev_ctx_t*) &p_obj->HTS221_class_obj.sensor_if.super.m_xConnector;
  stmdev_ctx_t *LPS22HH_sensor_drv = (stmdev_ctx_t*) &p_obj->LPS22HH_class_obj.sensor_if.super.m_xConnector;

  if (new_power_mode == E_POWER_MODE_SENSORS_ACTIVE) {
    if (HTS221ClassSensorIsActive(&p_obj->HTS221_class_obj)) {
      SMMessage report = {
          .sensorMessage.messageId = SM_MESSAGE_ID_SENSOR_CMD,
          .sensorMessage.nSensorId = p_obj->HTS221_class_obj.temp_id,
          .sensorMessage.nCmdID = SENSOR_CMD_ID_START
      };

      if (xQueueSendToBack(p_obj->in_queue, &report, pdMS_TO_TICKS(100)) != pdTRUE) {
        res = SYS_APP_TASK_REPORT_LOST_ERROR_CODE;
        SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_APP_TASK_REPORT_LOST_ERROR_CODE);
      }
      // reset the variables for the time stamp computation.
      p_obj->HTS221_class_obj.timestamp_tick = 0;
      p_obj->HTS221_class_obj.old_timestamp_tick = 0;
      p_obj->HTS221_class_obj.timestamp = 0;
    }

    SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("HTS221: -> SENSORS_ACTIVE\r\n"));
    SYS_DEBUGF3(SYS_DBG_APP, SYS_DBG_LEVEL_VERBOSE, ("HTS221: -> SENSORS_ACTIVE\r\n"));

    if (LPS22HHClassSensorIsActive(&p_obj->LPS22HH_class_obj)) {
      SMMessage report = {
          .sensorMessage.messageId = SM_MESSAGE_ID_SENSOR_CMD,
          .sensorMessage.nSensorId = p_obj->LPS22HH_class_obj.press_id,
          .sensorMessage.nCmdID = SENSOR_CMD_ID_START
      };
      if (xQueueSendToBack(p_obj->in_queue, &report, pdMS_TO_TICKS(100)) != pdTRUE) {
        res = SYS_APP_TASK_REPORT_LOST_ERROR_CODE;
        SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_APP_TASK_REPORT_LOST_ERROR_CODE);
      }

      // reset the variables for the time stamp computation.
      p_obj->LPS22HH_class_obj.timestamp_tick = 0;
      p_obj->LPS22HH_class_obj.old_timestamp_tick = 0;
      p_obj->LPS22HH_class_obj.timestamp = 0;
    }

    SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("LPS22HH: -> SENSORS_ACTIVE\r\n"));
    SYS_DEBUGF3(SYS_DBG_APP, SYS_DBG_LEVEL_VERBOSE, ("LPS22HH: -> SENSORS_ACTIVE\r\n"));
  }
  else if (new_power_mode == E_POWER_MODE_STATE1) {
    if (active_power_mode == E_POWER_MODE_SENSORS_ACTIVE) {
      /* SM_SENSOR_STATE_SUSPENDING: HTS221 */
      hts221_power_on_set(HTS221_sensor_drv, PROPERTY_DISABLE);
      SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("HTS221: -> RUN\r\n"));
      SYS_DEBUGF3(SYS_DBG_APP, SYS_DBG_LEVEL_VERBOSE, ("HTS221: -> RUN\r\n"));

      /* SM_SENSOR_STATE_SUSPENDING: LPS22HH */
      lps22hh_data_rate_set(LPS22HH_sensor_drv, (lps22hh_odr_t)(LPS22HH_POWER_DOWN | 0x10));
      SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("LPS22HH: -> RUN\r\n"));
      SYS_DEBUGF3(SYS_DBG_APP, SYS_DBG_LEVEL_VERBOSE, ("LPS22HH: -> RUN\r\n"));

      xTimerStop(p_obj->LPS22HH_class_obj.read_fifo_timer, pdMS_TO_TICKS(100));
      xQueueReset(p_obj->in_queue);
    }
  }
  else if (new_power_mode == E_POWER_MODE_SLEEP_1) {
    // the MCU is going in stop so I put the sensor in low power
    // from the INIT task
    res = HTS221ClassEnterLowPowerMode(&p_obj->HTS221_class_obj);
    if (SYS_IS_ERROR_CODE(res)) {
      sys_error_handler();
    }
    HTS221ClassConfigureIrqPin(&p_obj->HTS221_class_obj, TRUE);
    // notify the bus
    if (p_obj->HTS221_class_obj.sensor_if.super.m_pfBusCtrl != NULL) {
      p_obj->HTS221_class_obj.sensor_if.super.m_pfBusCtrl(&p_obj->HTS221_class_obj.sensor_if.super, E_BUS_CTRL_DEV_NOTIFY_POWER_MODE, 0);
    }

    SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("HTS221: -> SLEEP_1\r\n"));
    SYS_DEBUGF3(SYS_DBG_APP, SYS_DBG_LEVEL_VERBOSE, ("HTS221: -> SLEEP_1\r\n"));

    // the MCU is going in stop so I put the sensor in low power
    // from the INIT task
    res = LPS22HHClassEnterLowPowerMode(&p_obj->LPS22HH_class_obj);
    if (SYS_IS_ERROR_CODE(res)) {
      sys_error_handler();
    }
    // notify the bus
    if (p_obj->LPS22HH_class_obj.sensor_if.super.m_pfBusCtrl != NULL) {
      p_obj->LPS22HH_class_obj.sensor_if.super.m_pfBusCtrl(&p_obj->LPS22HH_class_obj.sensor_if.super, E_BUS_CTRL_DEV_NOTIFY_POWER_MODE, 0);
    }

    xTimerStop(p_obj->LPS22HH_class_obj.read_fifo_timer, pdMS_TO_TICKS(100));

    SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("LPS22HH: -> SLEEP_1\r\n"));
    SYS_DEBUGF3(SYS_DBG_APP, SYS_DBG_LEVEL_VERBOSE, ("LPS22HH: -> SLEEP_1\r\n"));
  }

  return res;
}

sys_error_code_t EnvTask_vtblHandleError(AManagedTask *_this, SysEvent error)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  /*  EnvTask *p_obj = (EnvTask*)_this; */

  return res;
}

sys_error_code_t EnvTask_vtblOnEnterTaskControlLoop(AManagedTask *_this) {
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("EnvTask: start.\r\n"));

  // At this point all system has been initialized.
  // Execute task specific delayed one time initialization.

  return res;
}

sys_error_code_t EnvTask_vtblOnEnterPowerMode(AManagedTaskEx *_this, const EPowerMode active_power_mode, const EPowerMode new_power_mode) {
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  /*  EnvTask *p_obj = (EnvTask*)_this; */

  return res;
}

/* AManagedTaskEx virtual functions definition */
/***********************************************/

sys_error_code_t EnvTask_vtblForceExecuteStep(AManagedTaskEx *_this, EPowerMode active_power_mode)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  EnvTask *p_obj = (EnvTask*)_this;

  SMMessage report = {
      .internalMessageFE.messageId = SM_MESSAGE_ID_FORCE_STEP,
      .internalMessageFE.nData = 0
  };

  if ((active_power_mode == E_POWER_MODE_STATE1) || (active_power_mode == E_POWER_MODE_SENSORS_ACTIVE)) {
    if(AMTExIsTaskInactive(_this)) {
      res = EnvTaskPostReportToFront(p_obj, (SMMessage*)&report);
    }
    else {
      // do nothing and wait for the step to complete.
      //      _this->m_xStatus.nDelayPowerModeSwitch = 0;
    }
  }
  else {
    if(eTaskGetState(_this->m_xThaskHandle) == eSuspended) {
      vTaskResume(_this->m_xThaskHandle);
    }
  }

  return res;
}


// ISensor virtual functions definition
// *******************************************

uint8_t EnvTask_vtblHTS221TempGetId(ISourceObservable *_this){
  assert_param(_this);
  EnvTask *p_obj = EnvTaskGetInstance();
  uint8_t res = p_obj->HTS221_class_obj.temp_id;

  return res;
}

uint8_t EnvTask_vtblHTS221HumGetId(ISourceObservable *_this){
  assert_param(_this);
  EnvTask *p_obj = EnvTaskGetInstance();
  uint8_t res = p_obj->HTS221_class_obj.hum_id;

  return res;
}

IEventSrc *EnvTask_vtblHTS221TempGetEventSourceIF(ISourceObservable *_this){
  assert_param(_this);
  EnvTask *p_obj = EnvTaskGetInstance();
  return p_obj->HTS221_class_obj.p_temp_event_src;
}

IEventSrc *EnvTask_vtblHTS221HumGetEventSourceIF(ISourceObservable *_this){
  assert_param(_this);
  EnvTask *p_obj = EnvTaskGetInstance();
  return p_obj->HTS221_class_obj.p_hum_event_src;
}

sys_error_code_t EnvTask_vtblHTS221TempGetODR(ISourceObservable *_this, float *p_measured, float *p_nominal){
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  /* parameter validation */
  if ((p_measured) == NULL || (p_nominal == NULL))
  {
    res = SYS_INVALID_PARAMETER_ERROR_CODE;
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_INVALID_PARAMETER_ERROR_CODE);
  }
  else
  {
  	/*get the object implementing the ISourceObservable IF */
  	HTS221Class *p_if_owner = (HTS221Class*)((uint32_t)_this - offsetof(HTS221Class, temp_sensor_if));
  	*p_measured =  p_if_owner->sensor_status.pSubSensorStatus[0].MeasuredODR;
  	*p_nominal =  p_if_owner->sensor_status.pSubSensorStatus[0].ODR;
  }

  return res;
}

float EnvTask_vtblHTS221TempGetFS(ISourceObservable *_this){
  assert_param(_this);
  EnvTask *p_obj = EnvTaskGetInstance();
  float res = p_obj->HTS221_class_obj.sensor_status.pSubSensorStatus[0].FS;

  return res;
}

float EnvTask_vtblHTS221TempGetSensitivity(ISourceObservable *_this){
  assert_param(_this);
  EnvTask *p_obj = EnvTaskGetInstance();
  float res = p_obj->HTS221_class_obj.sensor_status.pSubSensorStatus[0].Sensitivity;

  return res;
}

sys_error_code_t EnvTask_vtblHTS221HumGetODR(ISourceObservable *_this, float *p_measured, float *p_nominal){
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  /* parameter validation */
  if ((p_measured) == NULL || (p_nominal == NULL))
  {
    res = SYS_INVALID_PARAMETER_ERROR_CODE;
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_INVALID_PARAMETER_ERROR_CODE);
  }
  else
  {
    /*get the object implementing the ISourceObservable IF */
    HTS221Class *p_if_owner = (HTS221Class*)((uint32_t)_this - offsetof(HTS221Class, hum_sensor_if));
    *p_measured =  p_if_owner->sensor_status.pSubSensorStatus[1].MeasuredODR;
    *p_nominal =  p_if_owner->sensor_status.pSubSensorStatus[1].ODR;
  }

  return res;
}

float EnvTask_vtblHTS221HumGetFS(ISourceObservable *_this){
  assert_param(_this);
  EnvTask *p_obj = EnvTaskGetInstance();
  float res = p_obj->HTS221_class_obj.sensor_status.pSubSensorStatus[1].FS;

  return res;
}

float EnvTask_vtblHTS221HumGetSensitivity(ISourceObservable *_this){
  assert_param(_this);
  EnvTask *p_obj = EnvTaskGetInstance();
  float res = p_obj->HTS221_class_obj.sensor_status.pSubSensorStatus[1].Sensitivity;

  return res;
}

uint8_t EnvTask_vtblLPS22HHTempGetId(ISourceObservable *_this){
  assert_param(_this);
  EnvTask *p_obj = EnvTaskGetInstance();
  uint8_t res = p_obj->LPS22HH_class_obj.temp_id;

  return res;
}

uint8_t EnvTask_vtblLPS22HHPressGetId(ISourceObservable *_this){
  assert_param(_this);
  EnvTask *p_obj = EnvTaskGetInstance();
  uint8_t res = p_obj->LPS22HH_class_obj.press_id;

  return res;
}

IEventSrc *EnvTask_vtblLPS22HHTempGetEventSourceIF(ISourceObservable *_this){
  assert_param(_this);
  EnvTask *p_obj = EnvTaskGetInstance();
  return p_obj->LPS22HH_class_obj.p_temp_event_src;
}

IEventSrc *EnvTask_vtblLPS22HHPressGetEventSourceIF(ISourceObservable *_this){
  assert_param(_this);
  EnvTask *p_obj = EnvTaskGetInstance();
  return p_obj->LPS22HH_class_obj.p_press_event_src;
}

sys_error_code_t EnvTask_vtblLPS22HHPressGetODR(ISourceObservable *_this, float *p_measured, float *p_nominal){
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  /* parameter validation */
  if ((p_measured) == NULL || (p_nominal == NULL))
  {
    res = SYS_INVALID_PARAMETER_ERROR_CODE;
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_INVALID_PARAMETER_ERROR_CODE);
  }
  else
  {
    /*get the object implementing the ISourceObservable IF */
  	LPS22HHClass *p_if_owner = (LPS22HHClass*)((uint32_t)_this - offsetof(LPS22HHClass, press_sensor_if));
    *p_measured =  p_if_owner->sensor_status.pSubSensorStatus[0].MeasuredODR;
    *p_nominal =  p_if_owner->sensor_status.pSubSensorStatus[0].ODR;
  }

  return res;
}

float EnvTask_vtblLPS22HHPressGetFS(ISourceObservable *_this){
  assert_param(_this);
  EnvTask *p_obj = EnvTaskGetInstance();
  float res =  p_obj->LPS22HH_class_obj.sensor_status.pSubSensorStatus[0].FS;

  return res;
}

float EnvTask_vtblLPS22HHPressGetSensitivity(ISourceObservable *_this){
  assert_param(_this);
  EnvTask *p_obj = EnvTaskGetInstance();
  float res =  p_obj->LPS22HH_class_obj.sensor_status.pSubSensorStatus[0].Sensitivity;

  return res;
}

sys_error_code_t EnvTask_vtblLPS22HHTempGetODR(ISourceObservable *_this, float *p_measured, float *p_nominal){
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  /* parameter validation */
  if ((p_measured) == NULL || (p_nominal == NULL))
  {
    res = SYS_INVALID_PARAMETER_ERROR_CODE;
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_INVALID_PARAMETER_ERROR_CODE);
  }
  else
  {
    /*get the object implementing the ISourceObservable IF */
  	LPS22HHClass *p_if_owner = (LPS22HHClass*)((uint32_t)_this - offsetof(LPS22HHClass, temp_sensor_if));
    *p_measured =  p_if_owner->sensor_status.pSubSensorStatus[1].MeasuredODR;
    *p_nominal =  p_if_owner->sensor_status.pSubSensorStatus[1].ODR;
  }

  return res;
}

float EnvTask_vtblLPS22HHTempGetFS(ISourceObservable *_this){
  assert_param(_this);
  EnvTask *p_obj = EnvTaskGetInstance();
  float res =  p_obj->LPS22HH_class_obj.sensor_status.pSubSensorStatus[1].FS;

  return res;
}

float EnvTask_vtblLPS22HHTempGetSensitivity(ISourceObservable *_this){
  assert_param(_this);
  EnvTask *p_obj = EnvTaskGetInstance();
  float res =  p_obj->LPS22HH_class_obj.sensor_status.pSubSensorStatus[1].Sensitivity;

  return res;
}

sys_error_code_t EnvTask_vtblSensorStart(ISensor_t *_this){
  assert_param(_this);
  sys_error_code_t res = SYS_NOT_IMPLEMENTED_ERROR_CODE;

  return res;
}

sys_error_code_t EnvTask_vtblSensorStop(ISensor_t *_this){
  assert_param(_this);
  sys_error_code_t res = SYS_NOT_IMPLEMENTED_ERROR_CODE;

  return res;
}

sys_error_code_t EnvTask_vtblSensorSetODR(ISensor_t *_this, float ODR){
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  EnvTask *p_obj = EnvTaskGetInstance();
  EPowerMode log_status = AMTGetTaskPowerMode((AManagedTask *)p_obj);
  uint8_t sensor_id = ISourceGetId((ISourceObservable *)_this);

  if ((log_status == E_POWER_MODE_SENSORS_ACTIVE) && ISensorIsEnabled(_this))
  {
    res = SYS_INVALID_FUNC_CALL_ERROR_CODE;
  }
  else
  {
    /* Set a new command message in the queue */
    SMMessage report = {
        .sensorMessage.messageId = SM_MESSAGE_ID_SENSOR_CMD,
        .sensorMessage.nCmdID = SENSOR_CMD_ID_SET_ODR,
        .sensorMessage.nSensorId = sensor_id,
        .sensorMessage.nParam = (uint32_t)ODR
    };
    res = EnvTaskPostReportToBack(p_obj, (SMMessage*)&report);
  }

  return res;
}

sys_error_code_t EnvTask_vtblSensorSetFS(ISensor_t *_this, float FS){
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  EnvTask *p_obj = EnvTaskGetInstance();
  EPowerMode log_status = AMTGetTaskPowerMode((AManagedTask *)p_obj);
  uint8_t sensor_id = ISourceGetId((ISourceObservable *)_this);

  if ((log_status == E_POWER_MODE_SENSORS_ACTIVE) && ISensorIsEnabled(_this))
  {
    res = SYS_INVALID_FUNC_CALL_ERROR_CODE;
  }
  else
  {
    /* Set a new command message in the queue */
    SMMessage report = {
        .sensorMessage.messageId = SM_MESSAGE_ID_SENSOR_CMD,
        .sensorMessage.nCmdID = SENSOR_CMD_ID_SET_FS,
        .sensorMessage.nSensorId = sensor_id,
        .sensorMessage.nParam = (uint32_t)FS
    };
    res = EnvTaskPostReportToBack(p_obj, (SMMessage*)&report);
  }

  return res;

}

sys_error_code_t EnvTask_vtblSensorEnable(ISensor_t *_this){
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  EnvTask *p_obj = EnvTaskGetInstance();
  EPowerMode log_status = AMTGetTaskPowerMode((AManagedTask *)p_obj);
  uint8_t sensor_id = ISourceGetId((ISourceObservable *)_this);

  if ((log_status == E_POWER_MODE_SENSORS_ACTIVE) && ISensorIsEnabled(_this))
  {
    res = SYS_INVALID_FUNC_CALL_ERROR_CODE;
  }
  else
  {
    /* Set a new command message in the queue */
    SMMessage report = {
        .sensorMessage.messageId = SM_MESSAGE_ID_SENSOR_CMD,
        .sensorMessage.nCmdID = SENSOR_CMD_ID_ENABLE,
        .sensorMessage.nSensorId = sensor_id,
    };
    res = EnvTaskPostReportToBack(p_obj, (SMMessage*)&report);
  }

  return res;
}

sys_error_code_t EnvTask_vtblSensorDisable(ISensor_t *_this){
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  EnvTask *p_obj = EnvTaskGetInstance();
  EPowerMode log_status = AMTGetTaskPowerMode((AManagedTask *)p_obj);
  uint8_t sensor_id = ISourceGetId((ISourceObservable *)_this);

  if ((log_status == E_POWER_MODE_SENSORS_ACTIVE) && ISensorIsEnabled(_this))
  {
    res = SYS_INVALID_FUNC_CALL_ERROR_CODE;
  }
  else
  {
    /* Set a new command message in the queue */
    SMMessage report = {
        .sensorMessage.messageId = SM_MESSAGE_ID_SENSOR_CMD,
        .sensorMessage.nCmdID = SENSOR_CMD_ID_DISABLE,
        .sensorMessage.nSensorId = sensor_id,
    };
    res = EnvTaskPostReportToBack(p_obj, (SMMessage*)&report);
  }

  return res;
}

boolean_t EnvTask_vtblSensorIsEnabled(ISensor_t *_this){
  boolean_t res = FALSE;

  EnvTask *p_obj = EnvTaskGetInstance();
  uint8_t id = ISourceGetId((ISourceObservable *)_this);

  if(id == p_obj->HTS221_class_obj.temp_id)
    res = p_obj->HTS221_class_obj.sensor_status.pSubSensorStatus[0].IsActive;
  else if(id == p_obj->HTS221_class_obj.hum_id)
    res = p_obj->HTS221_class_obj.sensor_status.pSubSensorStatus[1].IsActive;
  else if(id == p_obj->LPS22HH_class_obj.press_id)
    res = p_obj->LPS22HH_class_obj.sensor_status.pSubSensorStatus[0].IsActive;
  else if(id == p_obj->LPS22HH_class_obj.temp_id)
    res = p_obj->LPS22HH_class_obj.sensor_status.pSubSensorStatus[1].IsActive;

  return res;
}

SensorDescriptor_t EnvTask_vtblSensorGetDescription(ISensor_t *_this){
  SensorDescriptor_t res;

  EnvTask *p_obj = EnvTaskGetInstance();
  uint8_t id = ISourceGetId((ISourceObservable *)_this);

  if(id == p_obj->HTS221_class_obj.temp_id || id == p_obj->HTS221_class_obj.hum_id)
    res = p_obj->HTS221_class_obj.sensor_descriptor;
  else if(id == p_obj->LPS22HH_class_obj.temp_id || id == p_obj->LPS22HH_class_obj.press_id)
    res = p_obj->LPS22HH_class_obj.sensor_descriptor;

  return res;
}


// Private function definition
// ***************************

static EnvTask* EnvTaskGetInstance(){
  return (EnvTask*)&sTaskObj;
}

static sys_error_code_t EnvTaskExecuteStepState1(AManagedTask *_this)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  EnvTask *p_obj = (EnvTask*)_this;
  SMMessage report = {0};

  AMTExSetInactiveState((AManagedTaskEx*)_this, TRUE);
  if (pdTRUE == xQueueReceive(p_obj->in_queue, &report, portMAX_DELAY)) {
    AMTExSetInactiveState((AManagedTaskEx*)_this, FALSE);

    switch (report.messageID)
    {
      case SM_MESSAGE_ID_FORCE_STEP:
      {
        // do nothing. I need only to resume.
        __NOP();
        break;
      }
      case SM_MESSAGE_ID_SENSOR_CMD:
      {
        if (report.sensorMessage.nSensorId == p_obj->HTS221_class_obj.hum_id || report.sensorMessage.nSensorId == p_obj->HTS221_class_obj.temp_id)
        {
          res = HTS221ExecuteCommand(p_obj, report);
        }
        else if (report.sensorMessage.nSensorId == p_obj->LPS22HH_class_obj.press_id || report.sensorMessage.nSensorId == p_obj->LPS22HH_class_obj.temp_id)
        {
          res = LPS22HHExecuteCommand(p_obj, report);
        }
        else
        {
          // unwanted report
          res = SYS_APP_TASK_UNKNOWN_REPORT_ERROR_CODE;
          SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_APP_TASK_UNKNOWN_REPORT_ERROR_CODE);

          SYS_DEBUGF(SYS_DBG_LEVEL_WARNING, ("ENVTASK: unexpected report in Run: %i\r\n", report.messageID));
          SYS_DEBUGF3(SYS_DBG_APP, SYS_DBG_LEVEL_WARNING, ("ENVTASK: unexpected report in Run: %i\r\n", report.messageID));
        }
        break;
      }
      default:
      {
        // unwanted report
        res = SYS_APP_TASK_UNKNOWN_REPORT_ERROR_CODE;
        SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_APP_TASK_UNKNOWN_REPORT_ERROR_CODE);

        SYS_DEBUGF(SYS_DBG_LEVEL_WARNING, ("ENVTASK: unexpected report in Run: %i\r\n", report.messageID));
        SYS_DEBUGF3(SYS_DBG_APP, SYS_DBG_LEVEL_WARNING, ("ENVTASK: unexpected report in Run: %i\r\n", report.messageID));
        break;
      }
    }
  }

  return res;
}

static sys_error_code_t EnvTaskExecuteStepDatalog(AManagedTask *_this) {
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  EnvTask *p_obj = (EnvTask*)_this;
  SMMessage report = {0};

  AMTExSetInactiveState((AManagedTaskEx*)_this, TRUE);
  if (pdTRUE == xQueueReceive(p_obj->in_queue, &report, portMAX_DELAY)){
    AMTExSetInactiveState((AManagedTaskEx*)_this, FALSE);

    switch (report.messageID) {
      case SM_MESSAGE_ID_FORCE_STEP:
      {
        // do nothing. I need only to resume.
        __NOP();
        break;
      }
      case SM_MESSAGE_ID_HTS221:
      {
//        SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("HTS221: new data.\r\n"));

        res = HTS221ClassSensorReadData(&p_obj->HTS221_class_obj);
        if (!SYS_IS_ERROR_CODE(res)) {
          // update the time stamp
          uint32_t period = 0;
          if (p_obj->HTS221_class_obj.timestamp_tick >= p_obj->HTS221_class_obj.old_timestamp_tick) {
            period = p_obj->HTS221_class_obj.timestamp_tick - p_obj->HTS221_class_obj.old_timestamp_tick;
          }
          else {
            // overflow of the hw timer
            period = p_obj->HTS221_class_obj.timestamp_tick + (0xFFFFFFFF -p_obj->HTS221_class_obj.old_timestamp_tick);
          }
          p_obj->HTS221_class_obj.old_timestamp_tick = p_obj->HTS221_class_obj.timestamp_tick;
          p_obj->HTS221_class_obj.timestamp += period;

          // notify the listeners...
          double timestamp = (double)p_obj->HTS221_class_obj.timestamp/(double)(SystemCoreClock);
          double delta_timestamp = (double)period/(double)(SystemCoreClock);

          if(p_obj->HTS221_class_obj.sensor_status.pSubSensorStatus[0].IsActive)
          {
            SensorEvent evt;

            AI_SP_Stream_t stream = {
                .packet.payload = (uint8_t *) p_obj->HTS221_class_obj.p_sensor_data_buff,
                .packet.payload_fmt = AI_SP_FMT_FLOAT32_RESET(),
                .mode = AI_SP_MODE_FULL //TODO: STF - this means that data are interleaved?!?
                // bonus question: what is AI_LOGGING_SHAPES_DEPTH ??
                // (can I represent anomogeneous matrix [4*4] with this data format ?
            };
            ai_logging_create_shape_0d(&stream.packet.shape);

            SensorEventInit((IEvent*)&evt, p_obj->HTS221_class_obj.p_temp_event_src, (ai_logging_packet_t*)&stream, timestamp, p_obj->HTS221_class_obj.temp_id);
            IEventSrcSendEvent(p_obj->HTS221_class_obj.p_temp_event_src, (IEvent*)&evt, NULL);
          }
          if(p_obj->HTS221_class_obj.sensor_status.pSubSensorStatus[1].IsActive)
          {
            SensorEvent evt;

            AI_SP_Stream_t stream = {
                .packet.payload = (uint8_t *) &p_obj->HTS221_class_obj.p_sensor_data_buff[1],
                .packet.payload_fmt = AI_SP_FMT_FLOAT32_RESET(),
                .mode = AI_SP_MODE_FULL //TODO: STF - this means that data are interleaved?!?
                // bonus question: what is AI_LOGGING_SHAPES_DEPTH ??
                // (can I represent anomogeneous matrix [4*4] with this data format ?
            };
            ai_logging_create_shape_0d(&stream.packet.shape);
            SensorEventInit((IEvent*)&evt, p_obj->HTS221_class_obj.p_hum_event_src, (ai_logging_packet_t*)&stream, timestamp, p_obj->HTS221_class_obj.hum_id);
            IEventSrcSendEvent(p_obj->HTS221_class_obj.p_hum_event_src, (IEvent*)&evt, NULL);
          }

          /* update measuredODR */
          p_obj->HTS221_class_obj.sensor_status.pSubSensorStatus[0].MeasuredODR = 1/delta_timestamp;
          p_obj->HTS221_class_obj.sensor_status.pSubSensorStatus[1].MeasuredODR = 1/delta_timestamp;

//          SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("HTS221: ts = %f\r\n", (float)timestamp));
        }
        break;
      }
      case SM_MESSAGE_ID_LPS22HH:
      {
//        SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("LPS22HH: new data.\r\n"));
        if (pdPASS != xTimerReset(p_obj->LPS22HH_class_obj.read_fifo_timer, pdMS_TO_TICKS(100)))
          res = SYS_UNDEFINED_ERROR_CODE;

        res = LPS22HHClassSensorReadData(&p_obj->LPS22HH_class_obj);
        if (!SYS_IS_ERROR_CODE(res)) {
          // update the time stamp
          uint32_t period = 0;
          if (p_obj->LPS22HH_class_obj.timestamp_tick >= p_obj->LPS22HH_class_obj.old_timestamp_tick) {
            period = p_obj->LPS22HH_class_obj.timestamp_tick - p_obj->LPS22HH_class_obj.old_timestamp_tick;
          }
          else {
            // overflow of the hw timer
            period = p_obj->LPS22HH_class_obj.timestamp_tick + (0xFFFFFFFF -p_obj->LPS22HH_class_obj.old_timestamp_tick);
          }
          p_obj->LPS22HH_class_obj.old_timestamp_tick = p_obj->LPS22HH_class_obj.timestamp_tick;
          p_obj->LPS22HH_class_obj.timestamp += period;
          // notify the listeners...
          double timestamp = (double)p_obj->LPS22HH_class_obj.timestamp/(double)(SystemCoreClock);
          if(p_obj->LPS22HH_class_obj.sensor_status.pSubSensorStatus[0].IsActive)
          {
            SensorEvent evt;

            AI_SP_Stream_t stream = {
                .packet.payload = (uint8_t *)p_obj->LPS22HH_class_obj.p_press_data_buff,
                .packet.payload_fmt = AI_SP_FMT_FLOAT32_RESET(),
                .mode = AI_SP_MODE_FULL //TODO: STF - this means that data are interleaved?!?
                // bonus question: what is AI_LOGGING_SHAPES_DEPTH ??
                // (can I represent anomogeneous matrix [4*4] with this data format ?
            };
            ai_logging_create_shape_1d(&stream.packet.shape, p_obj->LPS22HH_class_obj.fifo_level );

            SensorEventInit((IEvent*)&evt, p_obj->LPS22HH_class_obj.p_press_event_src, (ai_logging_packet_t*)&stream, timestamp, p_obj->LPS22HH_class_obj.press_id);
            IEventSrcSendEvent(p_obj->LPS22HH_class_obj.p_press_event_src, (IEvent*)&evt, NULL);
          }
          if(p_obj->LPS22HH_class_obj.sensor_status.pSubSensorStatus[1].IsActive)
          {
            SensorEvent evt;
            AI_SP_Stream_t stream = {
                .packet.payload = (uint8_t *)p_obj->LPS22HH_class_obj.p_temp_data_buff,
                .packet.payload_fmt = AI_SP_FMT_FLOAT32_RESET(),
                .mode = AI_SP_MODE_FULL //TODO: STF - this means that data are interleaved?!?
                // bonus question: what is AI_LOGGING_SHAPES_DEPTH ??
                // (can I represent anomogeneous matrix [4*4] with this data format ?
            };
            ai_logging_create_shape_1d(&stream.packet.shape, p_obj->LPS22HH_class_obj.fifo_level );

            SensorEventInit((IEvent*)&evt, p_obj->LPS22HH_class_obj.p_temp_event_src, (ai_logging_packet_t*)&stream, timestamp, p_obj->LPS22HH_class_obj.temp_id);
            IEventSrcSendEvent(p_obj->LPS22HH_class_obj.p_temp_event_src, (IEvent*)&evt, NULL);
          }

//          SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("LPS22HH: ts = %f\r\n", (float)timestamp));

          if (pdPASS != xTimerStart(p_obj->LPS22HH_class_obj.read_fifo_timer, pdMS_TO_TICKS(100)))
            res = SYS_UNDEFINED_ERROR_CODE;
        }
        break;
      }
      case SM_MESSAGE_ID_SENSOR_CMD:
      {
        if (report.sensorMessage.nSensorId == p_obj->HTS221_class_obj.hum_id || report.sensorMessage.nSensorId == p_obj->HTS221_class_obj.temp_id)
        {
          res = HTS221ExecuteCommand(p_obj, report);
        }
        else if (report.sensorMessage.nSensorId == p_obj->LPS22HH_class_obj.press_id || report.sensorMessage.nSensorId == p_obj->LPS22HH_class_obj.temp_id)
        {
          res = LPS22HHExecuteCommand(p_obj, report);
        }
        else
        {
          // unwanted report
          res = SYS_APP_TASK_UNKNOWN_REPORT_ERROR_CODE;
          SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_APP_TASK_UNKNOWN_REPORT_ERROR_CODE);

          SYS_DEBUGF(SYS_DBG_LEVEL_WARNING, ("ENVTASK: unexpected report in Datalog: %i\r\n", report.messageID));
          SYS_DEBUGF3(SYS_DBG_APP, SYS_DBG_LEVEL_WARNING, ("ENVTASK: unexpected report in Datalog: %i\r\n", report.messageID));
        }
        break;
      }
      default:
        // unwanted report
        res = SYS_APP_TASK_UNKNOWN_REPORT_ERROR_CODE;
        SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_APP_TASK_UNKNOWN_REPORT_ERROR_CODE);
        SYS_DEBUGF(SYS_DBG_LEVEL_WARNING, ("ENVTASK: unexpected report in Datalog: %i\r\n", report.messageID));
        break;
    }
  }

  return res;
}

static inline sys_error_code_t EnvTaskPostReportToFront(EnvTask *_this, SMMessage *pReport) {
  assert_param(_this);
  assert_param(pReport);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  if (SYS_IS_CALLED_FROM_ISR()) {
    if  (pdTRUE != xQueueSendToFrontFromISR(_this->in_queue, pReport, NULL)) {
      res = SYS_APP_TASK_REPORT_LOST_ERROR_CODE;
      // this function is private and the caller will ignore this return code.
    }
  }
  else {
    if (pdTRUE != xQueueSendToFront(_this->in_queue, pReport, pdMS_TO_TICKS(100))) {
      res = SYS_APP_TASK_REPORT_LOST_ERROR_CODE;
      // this function is private and the caller will ignore this return code.
    }
  }


  return res;
}

static inline sys_error_code_t EnvTaskPostReportToBack(EnvTask *_this, SMMessage *pReport) {
  assert_param(_this);
  assert_param(pReport);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  if (SYS_IS_CALLED_FROM_ISR()) {
    if  (pdTRUE != xQueueSendToBackFromISR(_this->in_queue, pReport, NULL)) {
      res = SYS_APP_TASK_REPORT_LOST_ERROR_CODE;
      // this function is private and the caller will ignore this return code.
    }
  }
  else {
    if (pdTRUE != xQueueSendToBack(_this->in_queue, pReport, pdMS_TO_TICKS(100))) {
      res = SYS_APP_TASK_REPORT_LOST_ERROR_CODE;
      // this function is private and the caller will ignore this return code.
    }
  }


  return res;
}

static sys_error_code_t EnvTaskSensorRegister(EnvTask *_this) {
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  ISensor_t *HTS221_temp_if = (ISensor_t *)EnvTaskGetHTS221TempSensorIF(_this);
  ISensor_t *HTS221_hum_if = (ISensor_t *)EnvTaskGetHTS221HumSensorIF(_this);
  ISensor_t *LPS22HH_press_if = (ISensor_t *)EnvTaskGetLPS22HHPressSensorIF(_this);
  ISensor_t *LPS22HH_temp_if = (ISensor_t *)EnvTaskGetLPS22HHTempSensorIF(_this);

  _this->HTS221_class_obj.temp_id = SMAddSensor(HTS221_temp_if);
  _this->HTS221_class_obj.hum_id = SMAddSensor(HTS221_hum_if);
  _this->LPS22HH_class_obj.press_id = SMAddSensor(LPS22HH_press_if);
  _this->LPS22HH_class_obj.temp_id = SMAddSensor(LPS22HH_temp_if);

  return res;
}

static sys_error_code_t EnvTaskSensorInitTaskParams(EnvTask *_this) {
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;


  /* SENSOR DESCRIPTOR */
  strcpy(_this->HTS221_class_obj.sensor_descriptor.Name, "HTS221");
  _this->HTS221_class_obj.sensor_descriptor.NumberSubSensors = 2;

  /* SUBSENSOR 0 DESCRIPTOR */
  _this->HTS221_class_obj.sensor_descriptor.pSubSensorDescriptor[0].IdSub = _this->HTS221_class_obj.temp_id;
  _this->HTS221_class_obj.sensor_descriptor.pSubSensorDescriptor[0].SensorType = COM_TYPE_TEMP;
  _this->HTS221_class_obj.sensor_descriptor.pSubSensorDescriptor[0].Dimensions = 1;
  strcpy(_this->HTS221_class_obj.sensor_descriptor.pSubSensorDescriptor[0].DimensionsLabel[0], "tem");
  _this->HTS221_class_obj.sensor_descriptor.pSubSensorDescriptor[0].DataType = DATA_TYPE_FLOAT;
  _this->HTS221_class_obj.sensor_descriptor.pSubSensorDescriptor[0].pODR[0] = 1.0f;
  _this->HTS221_class_obj.sensor_descriptor.pSubSensorDescriptor[0].pODR[1] = COM_END_OF_LIST_FLOAT;
  _this->HTS221_class_obj.sensor_descriptor.pSubSensorDescriptor[0].pSamplesPerTimestamp[0] = 0;
  _this->HTS221_class_obj.sensor_descriptor.pSubSensorDescriptor[0].pSamplesPerTimestamp[1] = 1000;
  strcpy(_this->HTS221_class_obj.sensor_descriptor.pSubSensorDescriptor[0].unit, "Celsius");
  _this->HTS221_class_obj.sensor_descriptor.pSubSensorDescriptor[0].pFS[0] = 120.0f;
  _this->HTS221_class_obj.sensor_descriptor.pSubSensorDescriptor[0].pFS[1] = COM_END_OF_LIST_FLOAT;

  /* SUBSENSOR 0 STATUS */
  _this->HTS221_class_obj.sensor_status.pSubSensorStatus[0].IsActive = TRUE;
  _this->HTS221_class_obj.sensor_status.pSubSensorStatus[0].FS = 120.0f;
  _this->HTS221_class_obj.sensor_status.pSubSensorStatus[0].Sensitivity = 1.0f;
  _this->HTS221_class_obj.sensor_status.pSubSensorStatus[0].ODR = 12.5f;
  _this->HTS221_class_obj.sensor_status.pSubSensorStatus[0].MeasuredODR = 0.0f;
  _this->HTS221_class_obj.sensor_status.pSubSensorStatus[0].InitialOffset = 0.0f;
#if (HSD_USE_DUMMY_DATA == 1)
  _this->HTS221_class_obj.sensor_status.pSubSensorStatus[0].SamplesPerTimestamp = 0;
#else
  _this->HTS221_class_obj.sensor_status.pSubSensorStatus[0].SamplesPerTimestamp = 50;
#endif

  /* SUBSENSOR 1 DESCRIPTOR */
  _this->HTS221_class_obj.sensor_descriptor.pSubSensorDescriptor[1].IdSub = _this->HTS221_class_obj.hum_id;
  _this->HTS221_class_obj.sensor_descriptor.pSubSensorDescriptor[1].SensorType = COM_TYPE_HUM;
  _this->HTS221_class_obj.sensor_descriptor.pSubSensorDescriptor[1].Dimensions = 1;
  strcpy(_this->HTS221_class_obj.sensor_descriptor.pSubSensorDescriptor[1].DimensionsLabel[0], "hum");
  _this->HTS221_class_obj.sensor_descriptor.pSubSensorDescriptor[1].DataType = DATA_TYPE_FLOAT;
  _this->HTS221_class_obj.sensor_descriptor.pSubSensorDescriptor[1].pODR[0] = 1.0f;
  _this->HTS221_class_obj.sensor_descriptor.pSubSensorDescriptor[1].pODR[1] = 7.0f;
  _this->HTS221_class_obj.sensor_descriptor.pSubSensorDescriptor[1].pODR[2] = 12.5f;
  _this->HTS221_class_obj.sensor_descriptor.pSubSensorDescriptor[1].pODR[3] = COM_END_OF_LIST_FLOAT;
  _this->HTS221_class_obj.sensor_descriptor.pSubSensorDescriptor[1].pSamplesPerTimestamp[0] = 0;
  _this->HTS221_class_obj.sensor_descriptor.pSubSensorDescriptor[1].pSamplesPerTimestamp[1] = 1000;
  strcpy(_this->HTS221_class_obj.sensor_descriptor.pSubSensorDescriptor[1].unit, "%");
  _this->HTS221_class_obj.sensor_descriptor.pSubSensorDescriptor[1].pFS[0] = 100.0f;
  _this->HTS221_class_obj.sensor_descriptor.pSubSensorDescriptor[1].pFS[1] = COM_END_OF_LIST_FLOAT;

  /* SUBSENSOR 1 STATUS */
  _this->HTS221_class_obj.sensor_status.pSubSensorStatus[1].IsActive = TRUE;
  _this->HTS221_class_obj.sensor_status.pSubSensorStatus[1].FS = 100.0f;
  _this->HTS221_class_obj.sensor_status.pSubSensorStatus[1].Sensitivity = 1.0f;
  _this->HTS221_class_obj.sensor_status.pSubSensorStatus[1].ODR = 12.5f;
  _this->HTS221_class_obj.sensor_status.pSubSensorStatus[1].MeasuredODR = 0.0f;
  _this->HTS221_class_obj.sensor_status.pSubSensorStatus[1].InitialOffset = 0.0f;
#if (HSD_USE_DUMMY_DATA == 1)
  _this->HTS221_class_obj.sensor_status.pSubSensorStatus[1].SamplesPerTimestamp = 0;
#else
  _this->HTS221_class_obj.sensor_status.pSubSensorStatus[1].SamplesPerTimestamp = 50;
#endif


  /* SENSOR DESCRIPTOR */
  strcpy(_this->LPS22HH_class_obj.sensor_descriptor.Name, "LPS22HH");
  _this->LPS22HH_class_obj.sensor_descriptor.NumberSubSensors = 2;

  /* SUBSENSOR 0 DESCRIPTOR */
  _this->LPS22HH_class_obj.sensor_descriptor.pSubSensorDescriptor[0].IdSub = _this->LPS22HH_class_obj.press_id;
  _this->LPS22HH_class_obj.sensor_descriptor.pSubSensorDescriptor[0].SensorType = COM_TYPE_PRESS;
  _this->LPS22HH_class_obj.sensor_descriptor.pSubSensorDescriptor[0].Dimensions = 1;
  strcpy(_this->LPS22HH_class_obj.sensor_descriptor.pSubSensorDescriptor[0].DimensionsLabel[0], "prs");
  _this->LPS22HH_class_obj.sensor_descriptor.pSubSensorDescriptor[0].DataType = DATA_TYPE_FLOAT;
  _this->LPS22HH_class_obj.sensor_descriptor.pSubSensorDescriptor[0].pODR[0] = 1.0f;
  _this->LPS22HH_class_obj.sensor_descriptor.pSubSensorDescriptor[0].pODR[1] = 10.0f;
  _this->LPS22HH_class_obj.sensor_descriptor.pSubSensorDescriptor[0].pODR[2] = 25.0f;
  _this->LPS22HH_class_obj.sensor_descriptor.pSubSensorDescriptor[0].pODR[3] = 50.0f;
  _this->LPS22HH_class_obj.sensor_descriptor.pSubSensorDescriptor[0].pODR[4] = 75.0f;
  _this->LPS22HH_class_obj.sensor_descriptor.pSubSensorDescriptor[0].pODR[5] = 100.0f;
  _this->LPS22HH_class_obj.sensor_descriptor.pSubSensorDescriptor[0].pODR[6] = 200.0f;
  _this->LPS22HH_class_obj.sensor_descriptor.pSubSensorDescriptor[0].pODR[7] = COM_END_OF_LIST_FLOAT;
  _this->LPS22HH_class_obj.sensor_descriptor.pSubSensorDescriptor[0].pSamplesPerTimestamp[0] = 0;
  _this->LPS22HH_class_obj.sensor_descriptor.pSubSensorDescriptor[0].pSamplesPerTimestamp[1] = 1000;
  strcpy(_this->LPS22HH_class_obj.sensor_descriptor.pSubSensorDescriptor[0].unit, "hPa");
  _this->LPS22HH_class_obj.sensor_descriptor.pSubSensorDescriptor[0].pFS[0] = 1260.0f;
  _this->LPS22HH_class_obj.sensor_descriptor.pSubSensorDescriptor[0].pFS[1] = COM_END_OF_LIST_FLOAT;

  /* SUBSENSOR 0 STATUS */
  _this->LPS22HH_class_obj.sensor_status.pSubSensorStatus[0].IsActive = TRUE;
  _this->LPS22HH_class_obj.sensor_status.pSubSensorStatus[0].FS = 1260.0f;
  _this->LPS22HH_class_obj.sensor_status.pSubSensorStatus[0].Sensitivity = 1.0f;
  _this->LPS22HH_class_obj.sensor_status.pSubSensorStatus[0].ODR = 10.0f;
  _this->LPS22HH_class_obj.sensor_status.pSubSensorStatus[0].MeasuredODR = 0.0f;
  _this->LPS22HH_class_obj.sensor_status.pSubSensorStatus[0].InitialOffset = 0.0f;
#if (HSD_USE_DUMMY_DATA == 1)
  _this->LPS22HH_class_obj.sensor_status.pSubSensorStatus[0].SamplesPerTimestamp = 0;
#else
  _this->LPS22HH_class_obj.sensor_status.pSubSensorStatus[0].SamplesPerTimestamp = 200;
#endif

  /* SUBSENSOR 1 DESCRIPTOR */
  _this->LPS22HH_class_obj.sensor_descriptor.pSubSensorDescriptor[1].IdSub = _this->LPS22HH_class_obj.temp_id;
  _this->LPS22HH_class_obj.sensor_descriptor.pSubSensorDescriptor[1].SensorType = COM_TYPE_TEMP;
  _this->LPS22HH_class_obj.sensor_descriptor.pSubSensorDescriptor[1].Dimensions = 1;
  strcpy(_this->LPS22HH_class_obj.sensor_descriptor.pSubSensorDescriptor[1].DimensionsLabel[0], "tem");
  _this->LPS22HH_class_obj.sensor_descriptor.pSubSensorDescriptor[1].DataType = DATA_TYPE_FLOAT;
  _this->LPS22HH_class_obj.sensor_descriptor.pSubSensorDescriptor[1].pODR[0] = 1.0f;
  _this->LPS22HH_class_obj.sensor_descriptor.pSubSensorDescriptor[1].pODR[1] = 10.0f;
  _this->LPS22HH_class_obj.sensor_descriptor.pSubSensorDescriptor[1].pODR[2] = 25.0f;
  _this->LPS22HH_class_obj.sensor_descriptor.pSubSensorDescriptor[1].pODR[3] = 50.0f;
  _this->LPS22HH_class_obj.sensor_descriptor.pSubSensorDescriptor[1].pODR[4] = 75.0f;
  _this->LPS22HH_class_obj.sensor_descriptor.pSubSensorDescriptor[1].pODR[5] = 100.0f;
  _this->LPS22HH_class_obj.sensor_descriptor.pSubSensorDescriptor[1].pODR[6] = 200.0f;
  _this->LPS22HH_class_obj.sensor_descriptor.pSubSensorDescriptor[1].pODR[7] = COM_END_OF_LIST_FLOAT;
  _this->LPS22HH_class_obj.sensor_descriptor.pSubSensorDescriptor[1].pSamplesPerTimestamp[0] = 0;
  _this->LPS22HH_class_obj.sensor_descriptor.pSubSensorDescriptor[1].pSamplesPerTimestamp[1] = 1000;
  strcpy(_this->LPS22HH_class_obj.sensor_descriptor.pSubSensorDescriptor[1].unit, "Celsius");
  _this->LPS22HH_class_obj.sensor_descriptor.pSubSensorDescriptor[1].pFS[0] = 85.0f;
  _this->LPS22HH_class_obj.sensor_descriptor.pSubSensorDescriptor[1].pFS[1] = COM_END_OF_LIST_FLOAT;

  /* SUBSENSOR 1 STATUS */
  _this->LPS22HH_class_obj.sensor_status.pSubSensorStatus[1].IsActive = TRUE;
  _this->LPS22HH_class_obj.sensor_status.pSubSensorStatus[1].FS = 85.0f;
  _this->LPS22HH_class_obj.sensor_status.pSubSensorStatus[1].Sensitivity = 1.0f;
  _this->LPS22HH_class_obj.sensor_status.pSubSensorStatus[1].ODR = 10.0f;
  _this->LPS22HH_class_obj.sensor_status.pSubSensorStatus[1].MeasuredODR = 0.0f;
  _this->LPS22HH_class_obj.sensor_status.pSubSensorStatus[1].InitialOffset = 0.0f;
#if (HSD_USE_DUMMY_DATA == 1)
  _this->LPS22HH_class_obj.sensor_status.pSubSensorStatus[1].SamplesPerTimestamp = 0;
#else
  _this->LPS22HH_class_obj.sensor_status.pSubSensorStatus[1].SamplesPerTimestamp = 200;
#endif

  return res;
}

static sys_error_code_t HTS221ExecuteCommand(EnvTask *_this, SMMessage report){
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  switch (report.sensorMessage.nCmdID)
  {
    case SENSOR_CMD_ID_START:
      res = HTS221ClassSensorInit(&_this->HTS221_class_obj);
      if (!SYS_IS_ERROR_CODE(res)) {
        HTS221ClassConfigureIrqPin(&_this->HTS221_class_obj, FALSE);
        // enable the IRQs
        HAL_NVIC_EnableIRQ(HTS221_INT_EXTI_IRQn);
      }
      break;
    case SENSOR_CMD_ID_STOP:
      res = HTS221ClassSensorStop(&_this->HTS221_class_obj);
      break;
    case SENSOR_CMD_ID_SET_ODR:
      res = HTS221ClassSensorSetODR(&_this->HTS221_class_obj, (float)report.sensorMessage.nParam, report.sensorMessage.nSensorId);
      break;
    case SENSOR_CMD_ID_SET_FS:
      res = HTS221ClassSensorSetFS(&_this->HTS221_class_obj, (float)report.sensorMessage.nParam, report.sensorMessage.nSensorId);
      break;
    case SENSOR_CMD_ID_ENABLE:
      res = HTS221ClassSensorEnable(&_this->HTS221_class_obj, report.sensorMessage.nSensorId);
      break;
    case SENSOR_CMD_ID_DISABLE:
      res = HTS221ClassSensorDisable(&_this->HTS221_class_obj, report.sensorMessage.nSensorId);
      break;
    default:
      // unwanted report
      res = SYS_APP_TASK_UNKNOWN_REPORT_ERROR_CODE;
      break;
  }
  return res;
}


static sys_error_code_t LPS22HHExecuteCommand(EnvTask *_this, SMMessage report){
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  switch (report.sensorMessage.nCmdID)
  {
    case SENSOR_CMD_ID_START:
      res = LPS22HHClassSensorInit(&_this->LPS22HH_class_obj);
      if (pdPASS != xTimerStart(_this->LPS22HH_class_obj.read_fifo_timer, pdMS_TO_TICKS(100)))
        res = SYS_UNDEFINED_ERROR_CODE;
      break;
    case SENSOR_CMD_ID_STOP:
      res = LPS22HHClassSensorStop(&_this->LPS22HH_class_obj);
      break;
    case SENSOR_CMD_ID_SET_ODR:
      res = LPS22HHClassSensorSetODR(&_this->LPS22HH_class_obj, (float)report.sensorMessage.nParam, report.sensorMessage.nSensorId);
      break;
    case SENSOR_CMD_ID_SET_FS:
      res = LPS22HHClassSensorSetFS(&_this->LPS22HH_class_obj, (float)report.sensorMessage.nParam, report.sensorMessage.nSensorId);
      break;
    case SENSOR_CMD_ID_ENABLE:
      res = LPS22HHClassSensorEnable(&_this->LPS22HH_class_obj, report.sensorMessage.nSensorId);
      break;
    case SENSOR_CMD_ID_DISABLE:
      res = LPS22HHClassSensorDisable(&_this->LPS22HH_class_obj, report.sensorMessage.nSensorId);
      break;
    default:
      // unwanted report
      res = SYS_APP_TASK_UNKNOWN_REPORT_ERROR_CODE;
      SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_APP_TASK_UNKNOWN_REPORT_ERROR_CODE);
      break;
  }
  return res;
}


// CubeMX integration
// ******************

void HTS221Class_EXTI_Callback(uint16_t nPin) {
  struct hts221Message_t report = {
      .messageId = SM_MESSAGE_ID_HTS221,
      .bDataReady = 1
  };

  if (sTaskObj.in_queue != NULL ) {
    if (pdTRUE != xQueueSendToBackFromISR(sTaskObj.in_queue, &report, NULL)) {
      // unable to send the report. Signal the error
      sys_error_handler();
    }
    sTaskObj.HTS221_class_obj.timestamp_tick = SMUtilGetTimeStamp();
  }
}

void LPS22HHClassTimerCallbackFunction(TimerHandle_t timer)
{
  struct lps22hhMessage_t report = {
      .messageId = SM_MESSAGE_ID_LPS22HH,
      .bDataReady = 1
  };

  if (sTaskObj.in_queue != NULL ) {
    if (pdTRUE != xQueueSendToBackFromISR(sTaskObj.in_queue, &report, NULL)) {
      // unable to send the report. Signal the error
      sys_error_handler();
    }
    sTaskObj.LPS22HH_class_obj.timestamp_tick = SMUtilGetTimeStamp();
  }
}



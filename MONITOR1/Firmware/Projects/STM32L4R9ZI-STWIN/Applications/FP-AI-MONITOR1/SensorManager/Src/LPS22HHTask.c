/**
 ******************************************************************************
 * @file    LPS22HHTask.c
 * @author  SRA - MCD
 * @version 1.0.0
 * @date    6-Sep-2021
 * @brief   This file provides a set of functions to handle lps22hh sensor
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

/* Includes ------------------------------------------------------------------*/
#include "LPS22HHTask.h"
#include "LPS22HHTask_vtbl.h"
#include "SMMessageParser.h"
#include "SensorCommands.h"
#include "SensorDef.h"
#include "SensorRegister.h"
#include "events/ISensorEventListener.h"
#include "events/ISensorEventListener_vtbl.h"
#include "ISensor.h"
#include "ISensor_vtbl.h"
#include "timers.h"
#include "SMUtilTask.h"
#include <string.h>
#include "services/sysdebug.h"

/* Private includes ----------------------------------------------------------*/

#ifndef LPS22HH_TASK_CFG_STACK_DEPTH
#define LPS22HH_TASK_CFG_STACK_DEPTH              200
#endif

#ifndef LPS22HH_TASK_CFG_PRIORITY
#define LPS22HH_TASK_CFG_PRIORITY                 (tskIDLE_PRIORITY)
#endif

#ifndef LPS22HH_TASK_CFG_IN_QUEUE_LENGTH
#define LPS22HH_TASK_CFG_IN_QUEUE_LENGTH          20
#endif

#ifndef LPS22HH_TASK_CFG_TIMER_PERIOD_MS
#define LPS22HH_TASK_CFG_TIMER_PERIOD_MS          500
#endif

#define LPS22HH_TASK_CFG_IN_QUEUE_ITEM_SIZE       sizeof(SMMessage)

#define SYS_DEBUGF(level, message)                SYS_DEBUGF3(SYS_DBG_LPS22HH, level, message)

#if defined(DEBUG) || defined (SYS_DEBUG)
#define sTaskObj                                  sLPS22HHTaskObj
#endif

#ifndef HSD_USE_DUMMY_DATA
#define HSD_USE_DUMMY_DATA 0
#endif

#if (HSD_USE_DUMMY_DATA == 1)
static uint16_t dummyDataCounter_press = 0;
static uint16_t dummyDataCounter_temp = 0;
#endif


static const ISensor_vtbl sLPS22HHTask_PressSensorIF_vtbl = {
    LPS22HHTask_vtblPressGetId,
    LPS22HHTask_vtblPressGetEventSourceIF,
    LPS22HHTask_vtblPressGetODR,
    LPS22HHTask_vtblPressGetFS,
    LPS22HHTask_vtblPressGetSensitivity,
    LPS22HHTask_vtblSensorStart,
    LPS22HHTask_vtblSensorStop,
    LPS22HHTask_vtblSensorSetODR,
    LPS22HHTask_vtblSensorSetFS,
    LPS22HHTask_vtblSensorEnable,
    LPS22HHTask_vtblSensorDisable,
    LPS22HHTask_vtblSensorIsEnabled,
    LPS22HHTask_vtblSensorGetDescription
};

static const ISensor_vtbl sLPS22HHTask_TempSensorIF_vtbl = {
    LPS22HHTask_vtblTempGetId,
    LPS22HHTask_vtblTempGetEventSourceIF,
    LPS22HHTask_vtblTempGetODR,
    LPS22HHTask_vtblTempGetFS,
    LPS22HHTask_vtblTempGetSensitivity,
    LPS22HHTask_vtblSensorStart,
    LPS22HHTask_vtblSensorStop,
    LPS22HHTask_vtblSensorSetODR,
    LPS22HHTask_vtblSensorSetFS,
    LPS22HHTask_vtblSensorEnable,
    LPS22HHTask_vtblSensorDisable,
    LPS22HHTask_vtblSensorIsEnabled,
    LPS22HHTask_vtblSensorGetDescription
};


/**
 *  LPS22HHTask internal structure.
 */
struct _LPS22HHTask {
  /**
   * Base class object.
   */
  AManagedTaskEx super;

  // Task variables should be added here.

  /**
   * I2C IF object used to connect the sensor task to the SPI bus.
   */
  I2CBusIF sensor_if;

  /**
   * Implements the temperature ISensor interface.
   */
  ISensor_t temp_sensor_if;

  /**
   * Implements the pressure ISensor interface.
   */
  ISensor_t press_sensor_if;

  /**
   * Specifies sensor capabilities.
   */
  SensorDescriptor_t sensor_descriptor;

  /**
   * Specifies sensor configuration.
   */
  SensorStatus_t sensor_status;

  /**
   * Specifies the sensor ID for the temperature subsensor.
   */
  uint8_t temp_id;

  /**
   * Specifies the sensor ID for the pressure subsensor.
   */
  uint8_t press_id;

  /**
   * Synchronization object used to send command to the task.
   */
  QueueHandle_t in_queue;

  /**
   * Buffer to store the data read from the sensor FIFO
   */
  uint8_t p_sensor_data_buff[256*5];

  /**
   * Temperautre data
   */
  float p_temp_data_buff[128 * 2];

  /**
   * Pressure data
   */
  float p_press_data_buff[128 * 2];

  /**
   * ::IEventSrc interface implementation for this class.
   */
  IEventSrc *p_temp_event_src;

  /**
   * ::IEventSrc interface implementation for this class.
   */
  IEventSrc *p_press_event_src;

  /**
   * Specifies the FIFO watermark level (it depends from ODR)
   */
  uint8_t fifo_level;

  /**
   * Specifies the ms delay between 2 consecutive read (it depends from ODR)
   */
  uint16_t task_delay;

  /**
   * Software timer used to generate the read command
   */
  TimerHandle_t read_fifo_timer;

  /**
   * Specifies the time stamp in tick.
   */
  uint32_t timestamp_tick;

  /**
   * Used during the time stamp computation to manage the overflow of the hardware timer.
   */
  uint32_t old_timestamp_tick;

  /**
   * Specifies the time stamp linked with the sensor data.
   */
  uint64_t timestamp;
};

/**
 * Class object declaration
 */
typedef struct _LPS22HHTaskClass {
  /**
   * LPS22HHTask class virtual table.
   */
  AManagedTaskEx_vtbl vtbl;

  /**
   * LPS22HHTask (PM_STATE, ExecuteStepFunc) map.
   */
  pExecuteStepFunc_t p_pm_state2func_map[];
} LPS22HHTaskClass_t;

// Private member function declaration
// ***********************************

/**
 * Obtain the task object.
 *
 * @return LPS22HHTask task object.
 */
static LPS22HHTask * LPS22HHTaskGetInstance(void);

/**
 * Execute one step of the task control loop while the system is in STATE1 mode.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return SYS_NO_EROR_CODE if success, a task specific error code otherwise.
 */
static sys_error_code_t LPS22HHTaskExecuteStepState1(AManagedTask *_this);

/**
 * Execute one step of the task control loop while the system is in DATALOG mode.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return SYS_NO_EROR_CODE if success, a task specific error code otherwise.
 */
static sys_error_code_t LPS22HHTaskExecuteStepDatalog(AManagedTask *_this);


/**
 * Initialize the sensor according to the actual parameters.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return SYS_NO_EROR_CODE if success, a task specific error code otherwise.
 */
static sys_error_code_t LPS22HHTaskSensorInit(LPS22HHTask *_this);

/**
 * Read the data from the sensor.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return SYS_NO_EROR_CODE if success, a task specific error code otherwise.
 */
static sys_error_code_t LPS22HHTaskSensorReadData(LPS22HHTask *_this);

/**
 * Register the sensor with the global DB and initialize the default parameters.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return SYS_NO_ERROR_CODE if success, an error code otherwise
 */
static sys_error_code_t LPS22HHTaskSensorRegister(LPS22HHTask *_this);

/**
 * Initialize the default parameters.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return SYS_NO_ERROR_CODE if success, an error code otherwise
 */
static sys_error_code_t LPS22HHTaskSensorInitTaskParams(LPS22HHTask *_this);

/**
 * Private implementation of sensor interface methods for LPS22HH sensor
 */
static sys_error_code_t LPS22HHTaskSensorStart(LPS22HHTask *_this, SMMessage message);
static sys_error_code_t LPS22HHTaskSensorStop(LPS22HHTask *_this, SMMessage message);
static sys_error_code_t LPS22HHTaskSensorSetODR(LPS22HHTask *_this, SMMessage message);
static sys_error_code_t LPS22HHTaskSensorSetFS(LPS22HHTask *_this, SMMessage message);
static sys_error_code_t LPS22HHTaskSensorEnable(LPS22HHTask *_this, SMMessage message);
static sys_error_code_t LPS22HHTaskSensorDisable(LPS22HHTask *_this, SMMessage message);

/**
 * Check if the sensor is active. The sensor is active if at least one of the sub sensor is active.
 * @param _this [IN] specifies a pointer to a task object.
 * @return TRUE if the sensor is active, FALSE otherwise.
 */
static boolean_t LPS22HHTaskSensorIsActive(const LPS22HHTask *_this);

static sys_error_code_t LPS22HHTaskEnterLowPowerMode(const LPS22HHTask *_this);

/**
 * Callback function called when the software timer expires.
 *
 * @param xTimer [IN] specifies the handle of the expired timer.
 */
static void LPS22HHTaskTimerCallbackFunction(TimerHandle_t timer);




// Inline function forward declaration
// ***********************************

/**
 * Private function used to post a message into the front of the task queue.
 * Used to resume the task when the required by the INIT task.
 *
 * @param this [IN] specifies a pointer to the task object.
 * @param pMessage [IN] specifies a message to send.
 * @return SYS_NO_EROR_CODE if success, SYS_APP_TASK_REPORT_LOST_ERROR_CODE.
 */
static inline sys_error_code_t LPS22HHTaskPostMessageToFront(LPS22HHTask *_this, SMMessage *pMessage);

/**
 * Private function used to post a message into the back of the task queue.
 * Used to resume the task when the required by the INIT task.
 *
 * @param this [IN] specifies a pointer to the task object.
 * @param pMessage [IN] specifies a message to send.
 * @return SYS_NO_EROR_CODE if success, SYS_APP_TASK_REPORT_LOST_ERROR_CODE.
 */
static inline sys_error_code_t LPS22HHTaskPostMessageToBack(LPS22HHTask *_this, SMMessage *pMessage);

#if defined (__GNUC__)
// Inline function defined inline in the header file LPS22HHTask.h must be declared here as extern function.
#endif


/* Objects instance */
/********************/

/**
 * The only instance of the task object.
 */
static LPS22HHTask sTaskObj;

/**
 * The class object.
 */
static const LPS22HHTaskClass_t sTheClass = {
    /* Class virtual table */
    {
        LPS22HHTask_vtblHardwareInit,
        LPS22HHTask_vtblOnCreateTask,
        LPS22HHTask_vtblDoEnterPowerMode,
        LPS22HHTask_vtblHandleError,
        LPS22HHTask_vtblOnEnterTaskControlLoop,
        LPS22HHTask_vtblForceExecuteStep,
        LPS22HHTask_vtblOnEnterPowerMode
    },

    /* class (PM_STATE, ExecuteStepFunc) map */
    {
        LPS22HHTaskExecuteStepState1,
        NULL,
        LPS22HHTaskExecuteStepDatalog,
    }
};


// Public API definition
// *********************

ISourceObservable *LPS22HHTaskGetTempSensorIF(LPS22HHTask *_this){
  return (ISourceObservable *)&(_this->temp_sensor_if);
}

ISourceObservable *LPS22HHTaskGetPressSensorIF(LPS22HHTask *_this){
  return (ISourceObservable *)&(_this->press_sensor_if);
}

AManagedTaskEx *LPS22HHTaskAlloc() {
  // In this application there is only one Keyboard task,
  // so this allocator implement the singleton design pattern.

  // Initialize the super class
  AMTInitEx(&sTaskObj.super);

  sTaskObj.super.vptr = &sTheClass.vtbl;
  sTaskObj.temp_sensor_if.vptr = &sLPS22HHTask_TempSensorIF_vtbl;
  sTaskObj.press_sensor_if.vptr = &sLPS22HHTask_PressSensorIF_vtbl;

  return (AManagedTaskEx*)&sTaskObj;
}

I2CBusIF *LPS22HHTaskGetSensorIF(LPS22HHTask *_this) {
  assert_param(_this);

  return &_this->sensor_if;
}

IEventSrc *LPS22HHTaskGetTempEventSrcIF(LPS22HHTask *_this) {
  assert_param(_this);

  return _this->p_temp_event_src;
}

IEventSrc *LPS22HHTaskGetPressEventSrcIF(LPS22HHTask *_this) {
  assert_param(_this);

  return _this->p_press_event_src;
}

// AManagedTask virtual functions definition
// ***********************************************

sys_error_code_t LPS22HHTask_vtblHardwareInit(AManagedTask *_this, void *pParams) {
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  return res;
}

sys_error_code_t LPS22HHTask_vtblOnCreateTask(AManagedTask *_this, TaskFunction_t *pTaskCode, const char **pName, unsigned short *pStackDepth, void **pParams, UBaseType_t *pPriority) {
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  LPS22HHTask *p_obj = (LPS22HHTask*)_this;

  // Create task specific sw resources.

  p_obj->in_queue = xQueueCreate(LPS22HH_TASK_CFG_IN_QUEUE_LENGTH, LPS22HH_TASK_CFG_IN_QUEUE_ITEM_SIZE);
  if (p_obj->in_queue == NULL) {
    res = SYS_TASK_HEAP_OUT_OF_MEMORY_ERROR_CODE;
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(res);
    return res;
  }

#ifdef DEBUG
  vQueueAddToRegistry(p_obj->in_queue, "LPS22HH_Q");
#endif

  /* create the software timer*/
  p_obj->read_fifo_timer = xTimerCreate("LPS22HH_T", pdMS_TO_TICKS(LPS22HH_TASK_CFG_TIMER_PERIOD_MS), pdFALSE, p_obj, LPS22HHTaskTimerCallbackFunction);
  if(p_obj->read_fifo_timer == NULL)
  {
    res = SYS_TASK_HEAP_OUT_OF_MEMORY_ERROR_CODE;
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(res);
    return res;
  }

  res = I2CBusIFInit(&p_obj->sensor_if, LPS22HH_ID, LPS22HH_I2C_ADD_H);
  if (SYS_IS_ERROR_CODE(res)) {
    return res;
  }
  // set the SPIBusIF object as handle the IF connector because the SPIBus task
  // will use the handle to access the SPIBusIF.
  ABusIFSetHandle(&p_obj->sensor_if.super, &p_obj->sensor_if);

  // Initialize the EventSrc interface.
  p_obj->p_temp_event_src = SensorEventSrcAlloc();
  if (p_obj->p_temp_event_src == NULL) {
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_OUT_OF_MEMORY_ERROR_CODE);
    res = SYS_OUT_OF_MEMORY_ERROR_CODE;
    return res;
  }
  IEventSrcInit(p_obj->p_temp_event_src);

  p_obj->p_press_event_src = SensorEventSrcAlloc();
  if (p_obj->p_press_event_src == NULL) {
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_OUT_OF_MEMORY_ERROR_CODE);
    res = SYS_OUT_OF_MEMORY_ERROR_CODE;
    return res;
  }
  IEventSrcInit(p_obj->p_press_event_src);

  memset(p_obj->p_sensor_data_buff, 0, sizeof(p_obj->p_sensor_data_buff));
  memset(p_obj->p_temp_data_buff, 0, sizeof(p_obj->p_temp_data_buff));
  memset(p_obj->p_press_data_buff, 0, sizeof(p_obj->p_press_data_buff));
  p_obj->press_id = 0;
  p_obj->temp_id = 1;
  p_obj->timestamp_tick = 0;
  p_obj->old_timestamp_tick = 0;
  p_obj->timestamp = 0;
  p_obj->fifo_level = 0;
  p_obj->task_delay = 0;
  _this->m_pfPMState2FuncMap = sTheClass.p_pm_state2func_map;

  *pTaskCode = AMTExRun;
  *pName = "LPS22HH";
  *pStackDepth = LPS22HH_TASK_CFG_STACK_DEPTH;
  *pParams = _this;
  *pPriority = LPS22HH_TASK_CFG_PRIORITY;

  res = LPS22HHTaskSensorInitTaskParams(p_obj);
  if (SYS_IS_ERROR_CODE(res)) {
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_OUT_OF_MEMORY_ERROR_CODE);
    res = SYS_OUT_OF_MEMORY_ERROR_CODE;
    return res;
  }

  res = LPS22HHTaskSensorRegister(p_obj);
  if (SYS_IS_ERROR_CODE(res)) {
    SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("LPS22HH: unable to register with DB\r\n"));
    sys_error_handler();
  }

  return res;
}


sys_error_code_t LPS22HHTask_vtblDoEnterPowerMode(AManagedTask *_this, const EPowerMode ActivePowerMode, const EPowerMode NewPowerMode) {
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  LPS22HHTask *p_obj = (LPS22HHTask*)_this;
  stmdev_ctx_t *p_sensor_drv = (stmdev_ctx_t*) &p_obj->sensor_if.super.m_xConnector;

  if (NewPowerMode == E_POWER_MODE_SENSORS_ACTIVE) {
    if (LPS22HHTaskSensorIsActive(p_obj)) {
      SMMessage message = {
          .sensorMessage.messageId = SM_MESSAGE_ID_SENSOR_CMD,
          .sensorMessage.nCmdID = SENSOR_CMD_ID_START
      };

      if (xQueueSendToBack(p_obj->in_queue, &message, pdMS_TO_TICKS(100)) != pdTRUE) {
        res = SYS_APP_TASK_REPORT_LOST_ERROR_CODE;
        SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_APP_TASK_REPORT_LOST_ERROR_CODE);
      }


      // reset the variables for the time stamp computation.
      p_obj->timestamp_tick = 0;
      p_obj->old_timestamp_tick = 0;
      p_obj->timestamp = 0;
    }

    SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("LPS22HH: -> DATALOG\r\n"));
    SYS_DEBUGF3(SYS_DBG_APP, SYS_DBG_LEVEL_VERBOSE, ("LPS22HH: -> DATALOG\r\n"));
  }
  else if (NewPowerMode == E_POWER_MODE_STATE1) {
    if (ActivePowerMode == E_POWER_MODE_SENSORS_ACTIVE) {
      /* SM_SENSOR_STATE_SUSPENDING */
      lps22hh_data_rate_set(p_sensor_drv, (lps22hh_odr_t)(LPS22HH_POWER_DOWN | 0x10));
      xQueueReset(p_obj->in_queue);
      xTimerStop(p_obj->read_fifo_timer, pdMS_TO_TICKS(100));
    }

    SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("LPS22HH: -> STATE1\r\n"));
    SYS_DEBUGF3(SYS_DBG_APP, SYS_DBG_LEVEL_VERBOSE, ("LPS22HH: -> STATE1\r\n"));
  }
  else if (NewPowerMode == E_POWER_MODE_SLEEP_1) {
    // the MCU is going in stop so I put the sensor in low power
    // from the INIT task
    res = LPS22HHTaskEnterLowPowerMode(p_obj);
    if (SYS_IS_ERROR_CODE(res)) {
      sys_error_handler();
    }
    // notify the bus
    if (p_obj->sensor_if.super.m_pfBusCtrl != NULL) {
      p_obj->sensor_if.super.m_pfBusCtrl(&p_obj->sensor_if.super, E_BUS_CTRL_DEV_NOTIFY_POWER_MODE, 0);
    }

    xTimerStop(p_obj->read_fifo_timer, pdMS_TO_TICKS(100));

    SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("LPS22HH: -> SLEEP_1\r\n"));
    SYS_DEBUGF3(SYS_DBG_APP, SYS_DBG_LEVEL_VERBOSE, ("LPS22HH: -> SLEEP_1\r\n"));
  }

  return res;
}

sys_error_code_t LPS22HHTask_vtblHandleError(AManagedTask *_this, SysEvent xError) {
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  //  LPS22HHTask *p_obj = (LPS22HHTask*)_this;

  return res;
}

sys_error_code_t LPS22HHTask_vtblOnEnterTaskControlLoop(AManagedTask *_this) {
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("LPS22HH: start.\r\n"));

  // At this point all system has been initialized.
  // Execute task specific delayed one time initialization.

  return res;
}

sys_error_code_t LPS22HHTask_vtblForceExecuteStep(AManagedTaskEx *_this, EPowerMode ActivePowerMode) {
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  LPS22HHTask *p_obj = (LPS22HHTask*)_this;

  SMMessage message = {
      .internalMessageFE.messageId = SM_MESSAGE_ID_FORCE_STEP,
      .internalMessageFE.nData = 0
  };

  if ((ActivePowerMode == E_POWER_MODE_STATE1) || (ActivePowerMode == E_POWER_MODE_SENSORS_ACTIVE)) {
    if(AMTExIsTaskInactive(_this)) {
      res = LPS22HHTaskPostMessageToFront(p_obj, (SMMessage*)&message);
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

sys_error_code_t LPS22HHTask_vtblOnEnterPowerMode(AManagedTaskEx *_this, const EPowerMode ActivePowerMode, const EPowerMode NewPowerMode) {
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  //  LPS22HHTask *p_obj = (LPS22HHTask*)_this;

  return res;
}


// ISensor virtual functions definition
// *******************************************

uint8_t LPS22HHTask_vtblTempGetId(ISourceObservable *_this){
  assert_param(_this);
  LPS22HHTask *p_lps22hh_instance = LPS22HHTaskGetInstance();
  uint8_t res = p_lps22hh_instance->temp_id;

  return res;
}

uint8_t LPS22HHTask_vtblPressGetId(ISourceObservable *_this){
  assert_param(_this);
  LPS22HHTask *p_lps22hh_instance = LPS22HHTaskGetInstance();
  uint8_t res = p_lps22hh_instance->press_id;

  return res;
}

IEventSrc *LPS22HHTask_vtblTempGetEventSourceIF(ISourceObservable *_this){
  assert_param(_this);
  LPS22HHTask *p_lps22hh_instance = LPS22HHTaskGetInstance();
  return p_lps22hh_instance->p_temp_event_src;
}

IEventSrc *LPS22HHTask_vtblPressGetEventSourceIF(ISourceObservable *_this){
  assert_param(_this);
  LPS22HHTask *p_lps22hh_instance = LPS22HHTaskGetInstance();
  return p_lps22hh_instance->p_press_event_src;
}

sys_error_code_t LPS22HHTask_vtblPressGetODR(ISourceObservable *_this, float *p_measured, float *p_nominal){
  assert_param(_this != NULL);
  /*get the object implementing the ISourceObservable IF */
  LPS22HHTask *p_if_owner = (LPS22HHTask*)((uint32_t)_this - offsetof(LPS22HHTask, press_sensor_if));
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  /* parameter validation */
  if ((p_measured) == NULL || (p_nominal == NULL))
  {
    res = SYS_INVALID_PARAMETER_ERROR_CODE;
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_INVALID_PARAMETER_ERROR_CODE);
  }
  else
  {
    *p_measured =  p_if_owner->sensor_status.pSubSensorStatus[0].MeasuredODR;
    *p_nominal =  p_if_owner->sensor_status.pSubSensorStatus[0].ODR;
  }

  return res;
}

float LPS22HHTask_vtblPressGetFS(ISourceObservable *_this){
  assert_param(_this);
  LPS22HHTask *p_lps22hh_instance = LPS22HHTaskGetInstance();
  float res = p_lps22hh_instance->sensor_status.pSubSensorStatus[0].FS;

  return res;
}

float LPS22HHTask_vtblPressGetSensitivity(ISourceObservable *_this){
  assert_param(_this);
  LPS22HHTask *p_lps22hh_instance = LPS22HHTaskGetInstance();
  float res = p_lps22hh_instance->sensor_status.pSubSensorStatus[0].Sensitivity;

  return res;
}

sys_error_code_t LPS22HHTask_vtblTempGetODR(ISourceObservable *_this, float *p_measured, float *p_nominal){
  assert_param(_this != NULL);
  /*get the object implementing the ISourceObservable IF */
  LPS22HHTask *p_if_owner = (LPS22HHTask*)((uint32_t)_this - offsetof(LPS22HHTask, temp_sensor_if));
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  /* parameter validation */
  if ((p_measured) == NULL || (p_nominal == NULL))
  {
    res = SYS_INVALID_PARAMETER_ERROR_CODE;
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_INVALID_PARAMETER_ERROR_CODE);
  }
  else
  {
    *p_measured =  p_if_owner->sensor_status.pSubSensorStatus[1].MeasuredODR;
    *p_nominal =  p_if_owner->sensor_status.pSubSensorStatus[1].ODR;
  }

  return res;
}

float LPS22HHTask_vtblTempGetFS(ISourceObservable *_this){
  assert_param(_this);
  LPS22HHTask *p_lps22hh_instance = LPS22HHTaskGetInstance();
  float res = p_lps22hh_instance->sensor_status.pSubSensorStatus[1].FS;

  return res;
}

float LPS22HHTask_vtblTempGetSensitivity(ISourceObservable *_this){
  assert_param(_this);
  LPS22HHTask *p_lps22hh_instance = LPS22HHTaskGetInstance();
  float res = p_lps22hh_instance->sensor_status.pSubSensorStatus[1].Sensitivity;

  return res;
}

sys_error_code_t LPS22HHTask_vtblSensorStart(ISensor_t *_this){
  assert_param(_this);
  sys_error_code_t res = SYS_NOT_IMPLEMENTED_ERROR_CODE;

  return res;
}

sys_error_code_t LPS22HHTask_vtblSensorStop(ISensor_t *_this){
  assert_param(_this);
  sys_error_code_t res = SYS_NOT_IMPLEMENTED_ERROR_CODE;

  return res;
}

sys_error_code_t LPS22HHTask_vtblSensorSetODR(ISensor_t *_this, float ODR){
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  LPS22HHTask *p_lps22hh_instance = LPS22HHTaskGetInstance();
  EPowerMode log_status = AMTGetTaskPowerMode((AManagedTask *)p_lps22hh_instance);
  uint8_t sensor_id = ISourceGetId((ISourceObservable *)_this);

  if ((log_status == E_POWER_MODE_SENSORS_ACTIVE) && ISensorIsEnabled(_this))
  {
    res = SYS_INVALID_PARAMETER_ERROR_CODE;
  }
  else
  {
    /* Set a new command message in the queue */
    SMMessage message = {
        .sensorMessage.messageId = SM_MESSAGE_ID_SENSOR_CMD,
        .sensorMessage.nCmdID = SENSOR_CMD_ID_SET_ODR,
        .sensorMessage.nSensorId = sensor_id,
        .sensorMessage.nParam = (uint32_t)ODR
    };
    res = LPS22HHTaskPostMessageToBack(p_lps22hh_instance, (SMMessage*)&message);
  }

  return res;
}

sys_error_code_t LPS22HHTask_vtblSensorSetFS(ISensor_t *_this, float FS){
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  LPS22HHTask *p_lps22hh_instance = LPS22HHTaskGetInstance();
  EPowerMode log_status = AMTGetTaskPowerMode((AManagedTask *)p_lps22hh_instance);
  uint8_t sensor_id = ISourceGetId((ISourceObservable *)_this);

  if ((log_status == E_POWER_MODE_SENSORS_ACTIVE) && ISensorIsEnabled(_this))
  {
    res = SYS_INVALID_PARAMETER_ERROR_CODE;
  }
  else
  {
    /* Set a new command message in the queue */
    SMMessage message = {
        .sensorMessage.messageId = SM_MESSAGE_ID_SENSOR_CMD,
        .sensorMessage.nCmdID = SENSOR_CMD_ID_SET_FS,
        .sensorMessage.nSensorId = sensor_id,
        .sensorMessage.nParam = (uint32_t)FS
    };
    res = LPS22HHTaskPostMessageToBack(p_lps22hh_instance, (SMMessage*)&message);
  }

  return res;

}

sys_error_code_t LPS22HHTask_vtblSensorEnable(ISensor_t *_this){
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  LPS22HHTask *p_lps22hh_instance = LPS22HHTaskGetInstance();
  EPowerMode log_status = AMTGetTaskPowerMode((AManagedTask *)p_lps22hh_instance);
  uint8_t sensor_id = ISourceGetId((ISourceObservable *)_this);

  if ((log_status == E_POWER_MODE_SENSORS_ACTIVE) && ISensorIsEnabled(_this))
  {
    res = SYS_INVALID_PARAMETER_ERROR_CODE;
  }
  else
  {
    /* Set a new command message in the queue */
    SMMessage message = {
        .sensorMessage.messageId = SM_MESSAGE_ID_SENSOR_CMD,
        .sensorMessage.nCmdID = SENSOR_CMD_ID_ENABLE,
        .sensorMessage.nSensorId = sensor_id,
    };
    res = LPS22HHTaskPostMessageToBack(p_lps22hh_instance, (SMMessage*)&message);
  }

  return res;
}

sys_error_code_t LPS22HHTask_vtblSensorDisable(ISensor_t *_this){
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  LPS22HHTask *p_lps22hh_instance = LPS22HHTaskGetInstance();
  EPowerMode log_status = AMTGetTaskPowerMode((AManagedTask *)p_lps22hh_instance);
  uint8_t sensor_id = ISourceGetId((ISourceObservable *)_this);

  if ((log_status == E_POWER_MODE_SENSORS_ACTIVE) && ISensorIsEnabled(_this))
  {
    res = SYS_INVALID_PARAMETER_ERROR_CODE;
  }
  else
  {
    /* Set a new command message in the queue */
    SMMessage message = {
        .sensorMessage.messageId = SM_MESSAGE_ID_SENSOR_CMD,
        .sensorMessage.nCmdID = SENSOR_CMD_ID_DISABLE,
        .sensorMessage.nSensorId = sensor_id,
    };
    res = LPS22HHTaskPostMessageToBack(p_lps22hh_instance, (SMMessage*)&message);
  }

  return res;
}

boolean_t LPS22HHTask_vtblSensorIsEnabled(ISensor_t *_this){
  boolean_t res = FALSE;

  LPS22HHTask *p_lps22hh_instance = LPS22HHTaskGetInstance();

  if(ISourceGetId((ISourceObservable *)_this) == p_lps22hh_instance->press_id)
    res = p_lps22hh_instance->sensor_status.pSubSensorStatus[0].IsActive;
  else if(ISourceGetId((ISourceObservable *)_this) == p_lps22hh_instance->temp_id)
    res = p_lps22hh_instance->sensor_status.pSubSensorStatus[1].IsActive;

  return res;
}

SensorDescriptor_t LPS22HHTask_vtblSensorGetDescription(ISensor_t *_this){

  LPS22HHTask *p_lps22hh_instance = LPS22HHTaskGetInstance();
  return p_lps22hh_instance->sensor_descriptor;
}


// Private function definition
// ***************************

static LPS22HHTask* LPS22HHTaskGetInstance(){
  return (LPS22HHTask*)&sTaskObj;
}

static sys_error_code_t LPS22HHTaskExecuteStepState1(AManagedTask *_this) {
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  LPS22HHTask *p_obj = (LPS22HHTask*)_this;
  SMMessage message = {0};

  AMTExSetInactiveState((AManagedTaskEx*)_this, TRUE);
  if (pdTRUE == xQueueReceive(p_obj->in_queue, &message, portMAX_DELAY)) {
    AMTExSetInactiveState((AManagedTaskEx*)_this, FALSE);

    switch (message.messageID)
    {
      case SM_MESSAGE_ID_FORCE_STEP:
      {
        // do nothing. I need only to resume.
        __NOP();
        break;
      }
      case SM_MESSAGE_ID_SENSOR_CMD:
      {
        switch (message.sensorMessage.nCmdID)
        {
          case SENSOR_CMD_ID_START:
            res = LPS22HHTaskSensorStart(p_obj, message);
            break;
          case SENSOR_CMD_ID_STOP:
            res = LPS22HHTaskSensorStop(p_obj, message);
            break;
          case SENSOR_CMD_ID_SET_ODR:
            res = LPS22HHTaskSensorSetODR(p_obj, message);
            break;
          case SENSOR_CMD_ID_SET_FS:
            res = LPS22HHTaskSensorSetFS(p_obj, message);
            break;
          case SENSOR_CMD_ID_ENABLE:
            res = LPS22HHTaskSensorEnable(p_obj, message);
            break;
          case SENSOR_CMD_ID_DISABLE:
            res = LPS22HHTaskSensorDisable(p_obj, message);
            break;
          default:
            // unwanted message
            res = SYS_APP_TASK_UNKNOWN_REPORT_ERROR_CODE;
            SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_APP_TASK_UNKNOWN_REPORT_ERROR_CODE);

            SYS_DEBUGF(SYS_DBG_LEVEL_WARNING, ("LPS22HH: unexpected message in State1: %i\r\n", message.messageID));
            SYS_DEBUGF3(SYS_DBG_APP, SYS_DBG_LEVEL_WARNING, ("LPS22HH: unexpected message in State1: %i\r\n", message.messageID));
            break;
        }
        break;
      }
      default:
      {
        // unwanted message
        res = SYS_APP_TASK_UNKNOWN_REPORT_ERROR_CODE;
        SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_APP_TASK_UNKNOWN_REPORT_ERROR_CODE);

        SYS_DEBUGF(SYS_DBG_LEVEL_WARNING, ("LPS22HH: unexpected message in State1: %i\r\n", message.messageID));
        SYS_DEBUGF3(SYS_DBG_APP, SYS_DBG_LEVEL_WARNING, ("LPS22HH: unexpected message in State1: %i\r\n", message.messageID));
        break;
      }
    }
  }

  return res;
}

static sys_error_code_t LPS22HHTaskExecuteStepDatalog(AManagedTask *_this) {
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  LPS22HHTask *p_obj = (LPS22HHTask*)_this;
  SMMessage message = {0};

  AMTExSetInactiveState((AManagedTaskEx*)_this, TRUE);
  if (pdTRUE == xQueueReceive(p_obj->in_queue, &message, portMAX_DELAY)) {
    AMTExSetInactiveState((AManagedTaskEx*)_this, FALSE);

    switch (message.messageID) {
      case SM_MESSAGE_ID_FORCE_STEP:
      {
        // do nothing. I need only to resume.
        __NOP();
        break;
      }
      case SM_MESSAGE_ID_LPS22HH:
      {
//        SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("LPS22HH: new data.\r\n"));
        if (pdPASS != xTimerReset(p_obj->read_fifo_timer, pdMS_TO_TICKS(100)))
          res = SYS_UNDEFINED_ERROR_CODE;

        res = LPS22HHTaskSensorReadData(p_obj);
        if (!SYS_IS_ERROR_CODE(res)) {
          // update the time stamp
          uint32_t period = 0;
          if (p_obj->timestamp_tick >= p_obj->old_timestamp_tick) {
            period = p_obj->timestamp_tick - p_obj->old_timestamp_tick;
          }
          else {
            // overflow of the hw timer
            period = p_obj->timestamp_tick + (0xFFFFFFFF -p_obj->old_timestamp_tick);
          }
          p_obj->old_timestamp_tick = p_obj->timestamp_tick;
          p_obj->timestamp += period;
          // notify the listeners...
          double timestamp = (double)p_obj->timestamp/(double)(SystemCoreClock);
          double delta_timestamp = (double)period/(double)(SystemCoreClock);

          if(p_obj->sensor_status.pSubSensorStatus[0].IsActive)
          {
            SensorEvent evt;

            AI_SP_Stream_t stream = {
                .packet.payload = (uint8_t *)p_obj->p_press_data_buff,
                .packet.payload_fmt = AI_SP_FMT_FLOAT32_RESET(),
                .mode = AI_SP_MODE_COLUMN //TODO: STF - this means that data are interleaved?!?
                // bonus question: what is AI_LOGGING_SHAPES_DEPTH ??
                // (can I represent anomogeneous matrix [4*4] with this data format ?
            };
            ai_logging_create_shape_1d(&stream.packet.shape, p_obj->fifo_level );

            SensorEventInit((IEvent*)&evt, p_obj->p_press_event_src, (ai_logging_packet_t*)&stream, timestamp, p_obj->press_id);
            IEventSrcSendEvent(p_obj->p_press_event_src, (IEvent*)&evt, NULL);
          }
          if(p_obj->sensor_status.pSubSensorStatus[1].IsActive)
          {
            SensorEvent evt;

            AI_SP_Stream_t stream = {
                .packet.payload = (uint8_t *)p_obj->p_temp_data_buff,
                .packet.payload_fmt = AI_SP_FMT_FLOAT32_RESET(),
                .mode = AI_SP_MODE_COLUMN //TODO: STF - this means that data are interleaved?!?
                // bonus question: what is AI_LOGGING_SHAPES_DEPTH ??
                // (can I represent anomogeneous matrix [4*4] with this data format ?
            };
            ai_logging_create_shape_1d(&stream.packet.shape, p_obj->fifo_level );

            SensorEventInit((IEvent*)&evt, p_obj->p_temp_event_src, (ai_logging_packet_t*)&stream, timestamp, p_obj->temp_id);
            IEventSrcSendEvent(p_obj->p_temp_event_src, (IEvent*)&evt, NULL);
          }

          /* update measuredODR */
          p_obj->sensor_status.pSubSensorStatus[0].MeasuredODR = p_obj->fifo_level/delta_timestamp;
          p_obj->sensor_status.pSubSensorStatus[1].MeasuredODR = p_obj->fifo_level/delta_timestamp;

//          SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("LPS22HH: ts = %f\r\n", (float)timestamp));

          if (pdPASS != xTimerStart(p_obj->read_fifo_timer, pdMS_TO_TICKS(100)))
            res = SYS_UNDEFINED_ERROR_CODE;
        }
        break;
      }
      case SM_MESSAGE_ID_SENSOR_CMD:
      {
        switch (message.sensorMessage.nCmdID)
        {
          case SENSOR_CMD_ID_START:
            res = LPS22HHTaskSensorInit(p_obj);
            if (pdPASS != xTimerStart(p_obj->read_fifo_timer, pdMS_TO_TICKS(100)))
              res = SYS_UNDEFINED_ERROR_CODE;
            break;
          case SENSOR_CMD_ID_STOP:
            res = LPS22HHTaskSensorStop(p_obj, message);
            break;
          case SENSOR_CMD_ID_SET_ODR:
            res = LPS22HHTaskSensorSetODR(p_obj, message);
            break;
          case SENSOR_CMD_ID_SET_FS:
            res = LPS22HHTaskSensorSetFS(p_obj, message);
            break;
          case SENSOR_CMD_ID_ENABLE:
            res = LPS22HHTaskSensorEnable(p_obj, message);
            break;
          case SENSOR_CMD_ID_DISABLE:
            res = LPS22HHTaskSensorDisable(p_obj, message);
            break;
          default:
            // unwanted message
            res = SYS_APP_TASK_UNKNOWN_REPORT_ERROR_CODE;
            SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_APP_TASK_UNKNOWN_REPORT_ERROR_CODE);

            SYS_DEBUGF(SYS_DBG_LEVEL_WARNING, ("LPS22HH: unexpected message in Datalog: %i\r\n", message.messageID));
            SYS_DEBUGF3(SYS_DBG_APP, SYS_DBG_LEVEL_WARNING, ("LPS22HH: unexpected message in Datalog: %i\r\n", message.messageID));
            break;
        }
        break;
      }
      default:
        // unwanted message
        res = SYS_APP_TASK_UNKNOWN_REPORT_ERROR_CODE;
        SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_APP_TASK_UNKNOWN_REPORT_ERROR_CODE);

        SYS_DEBUGF(SYS_DBG_LEVEL_WARNING, ("LPS22HH: unexpected message in Datalog: %i\r\n", message.messageID));
        break;
    }
  }

  return res;
}

static inline sys_error_code_t LPS22HHTaskPostMessageToFront(LPS22HHTask *_this, SMMessage *pMessage) {
  assert_param(_this);
  assert_param(pMessage);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  if (SYS_IS_CALLED_FROM_ISR()) {
    if (pdTRUE != xQueueSendToFrontFromISR(_this->in_queue, pMessage, NULL)) {
      res = SYS_APP_TASK_REPORT_LOST_ERROR_CODE;
      // this function is private and the caller will ignore this return code.
    }
  }
  else {
    if (pdTRUE != xQueueSendToFront(_this->in_queue, pMessage, pdMS_TO_TICKS(100))) {
      res = SYS_APP_TASK_REPORT_LOST_ERROR_CODE;
      // this function is private and the caller will ignore this return code.
    }
  }

  return res;
}

static inline sys_error_code_t LPS22HHTaskPostMessageToBack(LPS22HHTask *_this, SMMessage *pMessage) {
  assert_param(_this);
  assert_param(pMessage);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  if (SYS_IS_CALLED_FROM_ISR()) {
    if (pdTRUE != xQueueSendToBackFromISR(_this->in_queue, pMessage, NULL)) {
      res = SYS_APP_TASK_REPORT_LOST_ERROR_CODE;
      // this function is private and the caller will ignore this return code.
    }
  }
  else {
    if (pdTRUE != xQueueSendToBack(_this->in_queue, pMessage, pdMS_TO_TICKS(100))) {
      res = SYS_APP_TASK_REPORT_LOST_ERROR_CODE;
      // this function is private and the caller will ignore this return code.
    }
  }

  return res;
}

static sys_error_code_t LPS22HHTaskSensorInit(LPS22HHTask *_this) {
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  stmdev_ctx_t *p_sensor_drv = (stmdev_ctx_t*) &_this->sensor_if.super.m_xConnector;

  uint8_t reg0 = 0;
  int32_t ret_val = 0;
  float lps22hh_odr = 0.0f;

  /* Power Down */
  ret_val = lps22hh_device_id_get( p_sensor_drv, (uint8_t *)&reg0);
  if (!ret_val) {
    I2CBusIFSetWhoAmI(&_this->sensor_if, reg0);
  }
  SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("LPS22HH: sensor - I am 0x%x.\r\n", reg0));

  /* Disable MIPI I3C(SM) interface */
  lps22hh_i3c_interface_set(p_sensor_drv, LPS22HH_I3C_DISABLE);

  /* Power down the device, set Low Noise Enable (bit 5), clear One Shot (bit 4) */
  lps22hh_data_rate_set(p_sensor_drv, (lps22hh_odr_t)(LPS22HH_POWER_DOWN | 0x10));

  /* Disable low-pass filter on LPS22HH pressure data */
  lps22hh_lp_bandwidth_set(p_sensor_drv, LPS22HH_LPF_ODR_DIV_2);

  /* Set block data update mode */
  lps22hh_block_data_update_set(p_sensor_drv, PROPERTY_ENABLE);

  /* Set autoincrement for multi-byte read/write */
  lps22hh_auto_increment_set(p_sensor_drv, PROPERTY_ENABLE);

  lps22hh_reset_set(p_sensor_drv,1);

  /* Set fifo mode */
  lps22hh_fifo_mode_set(p_sensor_drv, LPS22HH_STREAM_MODE);

  if (_this->sensor_status.pSubSensorStatus[1].IsActive == TRUE)
  {
    lps22hh_odr = _this->sensor_status.pSubSensorStatus[1].ODR;
    _this->sensor_status.pSubSensorStatus[0].ODR = _this->sensor_status.pSubSensorStatus[1].ODR;
  }
  else
  {
    lps22hh_odr = _this->sensor_status.pSubSensorStatus[0].ODR;
    _this->sensor_status.pSubSensorStatus[1].ODR = _this->sensor_status.pSubSensorStatus[0].ODR;
  }

  if(lps22hh_odr < 2.0f)
  {
    lps22hh_data_rate_set(p_sensor_drv, LPS22HH_1_Hz);
    _this->task_delay = 1000;
  }
  else if(lps22hh_odr < 11.0f)
  {
    lps22hh_data_rate_set(p_sensor_drv, LPS22HH_10_Hz);
    _this->task_delay = 1000;
  }
  else if(lps22hh_odr < 26.0f)
  {
    lps22hh_data_rate_set(p_sensor_drv, LPS22HH_25_Hz);
    _this->task_delay = 1000;
  }
  else if(lps22hh_odr < 51.0f)
  {
    lps22hh_data_rate_set(p_sensor_drv, LPS22HH_50_Hz);
    _this->task_delay = 1000;
  }
  else if(lps22hh_odr < 76.0f)
  {
    lps22hh_data_rate_set(p_sensor_drv, LPS22HH_75_Hz);
    _this->task_delay = 1000;
  }
  else if(lps22hh_odr < 101.0f)
  {
    lps22hh_data_rate_set(p_sensor_drv, LPS22HH_100_Hz);
    _this->task_delay = 1000;
  }
  else
  {
    lps22hh_data_rate_set(p_sensor_drv, LPS22HH_200_Hz);
    _this->task_delay = 500;
  }

  return res;
}

static sys_error_code_t LPS22HHTaskSensorReadData(LPS22HHTask *_this) {
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  stmdev_ctx_t *p_sensor_drv = (stmdev_ctx_t*) &_this->sensor_if.super.m_xConnector;

  lps22hh_fifo_data_level_get(p_sensor_drv, (uint8_t *)&_this->fifo_level);
  lps22hh_read_reg(p_sensor_drv, LPS22HH_FIFO_DATA_OUT_PRESS_XL, (uint8_t *) _this->p_sensor_data_buff, 5*_this->fifo_level);
  uint16_t i = 0;

  for(i = 0; i < _this->fifo_level; i++)
  {
    uint32_t press = (((uint32_t)_this->p_sensor_data_buff[5 * i + 0])) | (((uint32_t)_this->p_sensor_data_buff[5 * i + 1]) << (8 * 1)) | (((uint32_t)_this->p_sensor_data_buff[5 * i + 2]) << (8 * 2));

    /* convert the 2's complement 24 bit to 2's complement 32 bit */
    if(press & 0x00800000)
      press |= 0xFF000000;

    uint16_t temp = *((uint16_t *)(&_this->p_sensor_data_buff[5 * i + 3]));

    if(_this->sensor_status.pSubSensorStatus[0].IsActive && !_this->sensor_status.pSubSensorStatus[1].IsActive) /* Only Pressure */
    {
      _this->p_press_data_buff[i] = (float)press/4096.0f; /* Pressure */
    }
    else if(!_this->sensor_status.pSubSensorStatus[0].IsActive && _this->sensor_status.pSubSensorStatus[1].IsActive) /* Only Temperature */
    {
      _this->p_temp_data_buff[i] = (float)temp/100.0f; /* Temperature */
    }
    else if(_this->sensor_status.pSubSensorStatus[0].IsActive && _this->sensor_status.pSubSensorStatus[1].IsActive) /* Both Sub Sensors */
    {
      _this->p_press_data_buff[i] = (float)press/4096.0f; /* Pressure */
      _this->p_temp_data_buff[i] = (float)temp/100.0f; /* Temperature */
    }
  }

#if (HSD_USE_DUMMY_DATA == 1)
  for (i = 0; i < _this->fifo_level ; i++)
  {
    _this->p_press_data_buff[i]  = (float)dummyDataCounter_press++;
    _this->p_temp_data_buff[i] =(float)dummyDataCounter_temp++;
  }
#endif

  return res;
}

static sys_error_code_t LPS22HHTaskSensorRegister(LPS22HHTask *_this) {
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  ISensor_t *press_if = (ISensor_t *)LPS22HHTaskGetPressSensorIF(_this);
  ISensor_t *temp_if = (ISensor_t *)LPS22HHTaskGetTempSensorIF(_this);

  _this->press_id = SMAddSensor(press_if);
  _this->temp_id = SMAddSensor(temp_if);

  return res;
}


static sys_error_code_t LPS22HHTaskSensorInitTaskParams(LPS22HHTask *_this) {
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  /* SENSOR DESCRIPTOR */
  strcpy(_this->sensor_descriptor.Name, "LPS22HH");
  _this->sensor_descriptor.NumberSubSensors = 2;

  /* SUBSENSOR 0 DESCRIPTOR */
  _this->sensor_descriptor.pSubSensorDescriptor[0].IdSub = _this->press_id;
  _this->sensor_descriptor.pSubSensorDescriptor[0].SensorType = COM_TYPE_PRESS;
  _this->sensor_descriptor.pSubSensorDescriptor[0].Dimensions = 1;
  strcpy(_this->sensor_descriptor.pSubSensorDescriptor[0].DimensionsLabel[0], "prs");
  _this->sensor_descriptor.pSubSensorDescriptor[0].DataType = DATA_TYPE_FLOAT;
  _this->sensor_descriptor.pSubSensorDescriptor[0].pODR[0] = 1.0f;
  _this->sensor_descriptor.pSubSensorDescriptor[0].pODR[1] = 10.0f;
  _this->sensor_descriptor.pSubSensorDescriptor[0].pODR[2] = 25.0f;
  _this->sensor_descriptor.pSubSensorDescriptor[0].pODR[3] = 50.0f;
  _this->sensor_descriptor.pSubSensorDescriptor[0].pODR[4] = 75.0f;
  _this->sensor_descriptor.pSubSensorDescriptor[0].pODR[5] = 100.0f;
  _this->sensor_descriptor.pSubSensorDescriptor[0].pODR[6] = 200.0f;
  _this->sensor_descriptor.pSubSensorDescriptor[0].pODR[7] = COM_END_OF_LIST_FLOAT;
  _this->sensor_descriptor.pSubSensorDescriptor[0].pSamplesPerTimestamp[0] = 0;
  _this->sensor_descriptor.pSubSensorDescriptor[0].pSamplesPerTimestamp[1] = 1000;
  strcpy(_this->sensor_descriptor.pSubSensorDescriptor[0].unit, "hPa");
  _this->sensor_descriptor.pSubSensorDescriptor[0].pFS[0] = 1260.0f;
  _this->sensor_descriptor.pSubSensorDescriptor[0].pFS[1] = COM_END_OF_LIST_FLOAT;

  /* SUBSENSOR 0 STATUS */
  _this->sensor_status.pSubSensorStatus[0].IsActive = TRUE;
  _this->sensor_status.pSubSensorStatus[0].FS = 1260.0f;
  _this->sensor_status.pSubSensorStatus[0].Sensitivity = 1.0f;
  _this->sensor_status.pSubSensorStatus[0].ODR = 10.0f;
  _this->sensor_status.pSubSensorStatus[0].MeasuredODR = _this->sensor_status.pSubSensorStatus[0].ODR;
  _this->sensor_status.pSubSensorStatus[0].InitialOffset = 0.0f;
#if (HSD_USE_DUMMY_DATA == 1)
  _this->sensor_status.pSubSensorStatus[0].SamplesPerTimestamp = 0;
#else
  _this->sensor_status.pSubSensorStatus[0].SamplesPerTimestamp = 200;
#endif

  /* SUBSENSOR 1 DESCRIPTOR */
  _this->sensor_descriptor.pSubSensorDescriptor[1].IdSub = _this->temp_id;
  _this->sensor_descriptor.pSubSensorDescriptor[1].SensorType = COM_TYPE_TEMP;
  _this->sensor_descriptor.pSubSensorDescriptor[1].Dimensions = 1;
  strcpy(_this->sensor_descriptor.pSubSensorDescriptor[1].DimensionsLabel[0], "tem");
  _this->sensor_descriptor.pSubSensorDescriptor[1].DataType = DATA_TYPE_FLOAT;
  _this->sensor_descriptor.pSubSensorDescriptor[1].pODR[0] = 1.0f;
  _this->sensor_descriptor.pSubSensorDescriptor[1].pODR[1] = 10.0f;
  _this->sensor_descriptor.pSubSensorDescriptor[1].pODR[2] = 25.0f;
  _this->sensor_descriptor.pSubSensorDescriptor[1].pODR[3] = 50.0f;
  _this->sensor_descriptor.pSubSensorDescriptor[1].pODR[4] = 75.0f;
  _this->sensor_descriptor.pSubSensorDescriptor[1].pODR[5] = 100.0f;
  _this->sensor_descriptor.pSubSensorDescriptor[1].pODR[6] = 200.0f;
  _this->sensor_descriptor.pSubSensorDescriptor[1].pODR[7] = COM_END_OF_LIST_FLOAT;
  _this->sensor_descriptor.pSubSensorDescriptor[1].pSamplesPerTimestamp[0] = 0;
  _this->sensor_descriptor.pSubSensorDescriptor[1].pSamplesPerTimestamp[1] = 1000;
  strcpy(_this->sensor_descriptor.pSubSensorDescriptor[1].unit, "Celsius");
  _this->sensor_descriptor.pSubSensorDescriptor[1].pFS[0] = 85.0f;
  _this->sensor_descriptor.pSubSensorDescriptor[1].pFS[1] = COM_END_OF_LIST_FLOAT;

  /* SUBSENSOR 1 STATUS */
  _this->sensor_status.pSubSensorStatus[1].IsActive = TRUE;
  _this->sensor_status.pSubSensorStatus[1].FS = 85.0f;
  _this->sensor_status.pSubSensorStatus[1].Sensitivity = 1.0f;
  _this->sensor_status.pSubSensorStatus[1].ODR = 10.0f;
  _this->sensor_status.pSubSensorStatus[1].MeasuredODR = _this->sensor_status.pSubSensorStatus[1].ODR;
  _this->sensor_status.pSubSensorStatus[1].InitialOffset = 0.0f;
#if (HSD_USE_DUMMY_DATA == 1)
  _this->sensor_status.pSubSensorStatus[1].SamplesPerTimestamp = 0;
#else
  _this->sensor_status.pSubSensorStatus[1].SamplesPerTimestamp = 200;
#endif

  return res;
}

static sys_error_code_t LPS22HHTaskSensorStart(LPS22HHTask *_this, SMMessage message){
  assert_param(_this);
  sys_error_code_t res = SYS_NOT_IMPLEMENTED_ERROR_CODE;

  return res;
}

static sys_error_code_t LPS22HHTaskSensorStop(LPS22HHTask *_this, SMMessage message){
  assert_param(_this);
  sys_error_code_t res = SYS_NOT_IMPLEMENTED_ERROR_CODE;

  return res;
}

static sys_error_code_t LPS22HHTaskSensorSetODR(LPS22HHTask *_this, SMMessage message){
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  stmdev_ctx_t *p_sensor_drv = (stmdev_ctx_t*) &_this->sensor_if.super.m_xConnector;
  float ODR = (float)message.sensorMessage.nParam;
  uint8_t id = message.sensorMessage.nSensorId;

  if(id == _this->temp_id || id == _this->press_id)
  {
    if(ODR < 2.0f)
    {
      lps22hh_data_rate_set(p_sensor_drv, LPS22HH_1_Hz);
      ODR = 1.0f;
      _this->task_delay = 1000;
    }
    else if(ODR < 11.0f)
    {
      lps22hh_data_rate_set(p_sensor_drv, LPS22HH_10_Hz);
      ODR = 10.0f;
      _this->task_delay = 1000;
    }
    else if(ODR < 26.0f)
    {
      lps22hh_data_rate_set(p_sensor_drv, LPS22HH_25_Hz);
      ODR = 25.0f;
      _this->task_delay = 1000;
    }
    else if(ODR < 51.0f)
    {
      lps22hh_data_rate_set(p_sensor_drv, LPS22HH_50_Hz);
      ODR = 50.0f;
      _this->task_delay = 1000;
    }
    else if(ODR < 76.0f)
    {
      lps22hh_data_rate_set(p_sensor_drv, LPS22HH_75_Hz);
      ODR = 75.0f;
      _this->task_delay = 1000;
    }
    else if(ODR < 101.0f)
    {
      lps22hh_data_rate_set(p_sensor_drv, LPS22HH_100_Hz);
      ODR = 100.0f;
      _this->task_delay = 1000;
    }
    else
    {
      lps22hh_data_rate_set(p_sensor_drv, LPS22HH_200_Hz);
      ODR = 200.0f;
      _this->task_delay = 500;
    }

    if (!SYS_IS_ERROR_CODE(res))
    {
      if(id == _this->press_id)
      {
        _this->sensor_status.pSubSensorStatus[0].ODR = ODR;
        _this->sensor_status.pSubSensorStatus[0].MeasuredODR = 0.0f;
      }
      else
      {
        _this->sensor_status.pSubSensorStatus[1].ODR = ODR;
        _this->sensor_status.pSubSensorStatus[0].MeasuredODR = 0.0f;
      }
    }
  }
  else
  {
    res = SYS_INVALID_PARAMETER_ERROR_CODE;
  }

  return res;
}

static sys_error_code_t LPS22HHTaskSensorSetFS(LPS22HHTask *_this, SMMessage message){
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  float FS = (float)message.sensorMessage.nParam;
  uint8_t id = message.sensorMessage.nSensorId;

  if(id == _this->temp_id)
  {
    if(FS != 85.0f)
      res = SYS_INVALID_PARAMETER_ERROR_CODE;

    if (!SYS_IS_ERROR_CODE(res))
    {
      _this->sensor_status.pSubSensorStatus[1].FS = FS;
    }
  }
  else if(id == _this->press_id)
  {
    if(FS != 1260.0f)
      res = SYS_INVALID_PARAMETER_ERROR_CODE;

    if (!SYS_IS_ERROR_CODE(res))
    {
      _this->sensor_status.pSubSensorStatus[0].FS = FS;
    }
  }
  else
  {
    res = SYS_INVALID_PARAMETER_ERROR_CODE;
  }

  return res;
}

static sys_error_code_t LPS22HHTaskSensorEnable(LPS22HHTask *_this, SMMessage message){
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  uint8_t id = message.sensorMessage.nSensorId;

  if(id == _this->temp_id)
    _this->sensor_status.pSubSensorStatus[1].IsActive = TRUE;
  else if(id == _this->press_id)
    _this->sensor_status.pSubSensorStatus[0].IsActive = TRUE;
  else
    res = SYS_INVALID_PARAMETER_ERROR_CODE;

  return res;
}

static sys_error_code_t LPS22HHTaskSensorDisable(LPS22HHTask *_this, SMMessage message){
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  uint8_t id = message.sensorMessage.nSensorId;

  if(id == _this->temp_id)
    _this->sensor_status.pSubSensorStatus[1].IsActive = FALSE;
  else if(id == _this->press_id)
    _this->sensor_status.pSubSensorStatus[0].IsActive = FALSE;
  else
    res = SYS_INVALID_PARAMETER_ERROR_CODE;

  return res;
}

static boolean_t LPS22HHTaskSensorIsActive(const LPS22HHTask *_this) {
  assert_param(_this);
  boolean_t res = FALSE;

  // check if at least one sub sensor is active
  for (uint8_t i=0; i<_this->sensor_descriptor.NumberSubSensors; ++i) {
    if (_this->sensor_status.pSubSensorStatus[i].IsActive) {
      res = TRUE;
      break;
    }
  }

  return res;
}

static sys_error_code_t LPS22HHTaskEnterLowPowerMode(const LPS22HHTask *_this) {
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  stmdev_ctx_t *p_sensor_drv = (stmdev_ctx_t*) &_this->sensor_if.super.m_xConnector;

  if (lps22hh_data_rate_set(p_sensor_drv, (lps22hh_odr_t)(LPS22HH_POWER_DOWN | 0x10))) {
    res = SYS_SENSOR_TASK_OP_ERROR_CODE;
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_SENSOR_TASK_OP_ERROR_CODE);
  }

  return res;
}

static void LPS22HHTaskTimerCallbackFunction(TimerHandle_t timer)
{
  struct lps22hhMessage_t message = {
      .messageId = SM_MESSAGE_ID_LPS22HH,
      .bDataReady = 1
  };

  if (sTaskObj.in_queue != NULL ) {
    if (pdTRUE != xQueueSendToBackFromISR(sTaskObj.in_queue, &message, NULL)) {
      // unable to send the message. Signal the error
      sys_error_handler();
    }
    sTaskObj.timestamp_tick = SMUtilGetTimeStamp();
  }
}


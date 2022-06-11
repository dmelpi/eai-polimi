/**
 ******************************************************************************
 * @file    ILPS22QSTask.c
 * @author  SRA - MCD
 * @version 1.0.0
 * @date    6-Sep-2021
 * @brief   This file provides a set of functions to handle ilps22qs sensor
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
#include "ILPS22QSTask.h"
#include "ILPS22QSTask_vtbl.h"
#include "SMMessageParser.h"
#include "SensorCommands.h"
#include "SensorDef.h"
#include "SensorRegister.h"
#include "events/ISensorEventListener.h"
#include "events/ISensorEventListener_vtbl.h"
//#include "services/SysTimestamp.h"
#include "ilps22qs_reg.h"
#include "SMUtilTask.h"
#include <string.h>
#include "services/sysdebug.h"

/* Private includes ----------------------------------------------------------*/

#ifndef ILPS22QS_TASK_CFG_STACK_DEPTH
#define ILPS22QS_TASK_CFG_STACK_DEPTH              (TX_MINIMUM_STACK*10)
#endif

#ifndef ILPS22QS_TASK_CFG_PRIORITY
#define ILPS22QS_TASK_CFG_PRIORITY                 4
#endif

#ifndef ILPS22QS_TASK_CFG_IN_QUEUE_LENGTH
#define ILPS22QS_TASK_CFG_IN_QUEUE_LENGTH          20
#endif

#ifndef ILPS22QS_TASK_CFG_TIMER_PERIOD_MS
#define ILPS22QS_TASK_CFG_TIMER_PERIOD_MS          500
#endif

#define ILPS22QS_TASK_CFG_IN_QUEUE_ITEM_SIZE       sizeof(SMMessage)

#define SYS_DEBUGF(level, message)                SYS_DEBUGF3(SYS_DBG_ILPS22QS, level, message)

#if defined(DEBUG) || defined (SYS_DEBUG)
#define sTaskObj                                  sILPS22QSTaskObj
#endif

#ifndef HSD_USE_DUMMY_DATA
#define HSD_USE_DUMMY_DATA 0
#endif

#if (HSD_USE_DUMMY_DATA == 1)
static uint16_t dummyDataCounter_press = 0;
#endif

/**
 *  ILPS22QSTask internal structure.
 */
struct _ILPS22QSTask
{
  /**
   * Base class object.
   */
  AManagedTaskEx super;

  // Task variables should be added here.

  /**
   * IRQ GPIO configuration parameters.
   */
  const MX_GPIOParams_t *pIRQConfig;

  /**
   * SPI CS GPIO configuration parameters.
   */
  const MX_GPIOParams_t *pCSConfig;

  /**
   * Bus IF object used to connect the sensor task to the specific bus.
   */
  ABusIF *p_sensor_bus_if;

  /**
   * Specifies the bus type used by the sensor task.
   */
  EBusType eBusType;

  /**
   * Implements the pressure ISensor interface.
   */
  ISensor_t press_sensor_if;

  /**
   * Specifies pressure sensor capabilities.
   */
  const SensorDescriptor_t *press_sensor_descriptor;

  /**
   * Specifies pressure sensor configuration.
   */
  SensorStatus_t press_sensor_status;

  /**
   * Specifies the sensor ID for the pressure subsensor.
   */
  uint8_t press_id;

  /**
   * Synchronization object used to send command to the task.
   */
  TX_QUEUE in_queue;

  /**
   * Pressure data
   */
  float p_press_data_buff[ILPS22QS_MAX_WTM_LEVEL];

  /**
   * Sensor data from FIFO
   */
  uint8_t p_fifo_data_buff[ILPS22QS_MAX_WTM_LEVEL * 3];

  /**
   * ::IEventSrc interface implementation for this class.
   */
  IEventSrc *p_press_event_src;

  /**
   * Specifies the FIFO watermark level (it depends from ODR)
   */
  uint8_t samples_per_it;

  /**
   * Specifies the ms delay between 2 consecutive read (it depends from ODR)
   */
  uint16_t task_delay;

  /**
   * Software timer used to generate the read command
   */
  TX_TIMER read_fifo_timer;

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
typedef struct _ILPS22QSTaskClass
{
  /**
   * ILPS22QSTask class virtual table.
   */
  AManagedTaskEx_vtbl vtbl;

  /**
   * Pressure IF virtual table.
   */
  ISensor_vtbl press_sensor_if_vtbl;

  /**
   * Specifies pressure sensor capabilities.
   */
  SensorDescriptor_t press_class_descriptor;

  /**
   * ILPS22QSTask (PM_STATE, ExecuteStepFunc) map.
   */
  pExecuteStepFunc_t p_pm_state2func_map[];
} ILPS22QSTaskClass_t;

// Private member function declaration
// ***********************************

/**
 * Execute one step of the task control loop while the system is in RUN mode.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return SYS_NO_EROR_CODE if success, a task specific error code otherwise.
 */
static sys_error_code_t ILPS22QSTaskExecuteStepState1(AManagedTask *_this);

/**
 * Execute one step of the task control loop while the system is in SENSORS_ACTIVE mode.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return SYS_NO_EROR_CODE if success, a task specific error code otherwise.
 */
static sys_error_code_t ILPS22QSTaskExecuteStepDatalog(AManagedTask *_this);

/**
 * Initialize the sensor according to the actual parameters.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return SYS_NO_EROR_CODE if success, a task specific error code otherwise.
 */
static sys_error_code_t ILPS22QSTaskSensorInit(ILPS22QSTask *_this);

/**
 * Read the data from the sensor.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return SYS_NO_EROR_CODE if success, a task specific error code otherwise.
 */
static sys_error_code_t ILPS22QSTaskSensorReadData(ILPS22QSTask *_this);

/**
 * Register the sensor with the global DB and initialize the default parameters.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return SYS_NO_ERROR_CODE if success, an error code otherwise
 */
static sys_error_code_t ILPS22QSTaskSensorRegister(ILPS22QSTask *_this);

/**
 * Initialize the default parameters.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return SYS_NO_ERROR_CODE if success, an error code otherwise
 */
static sys_error_code_t ILPS22QSTaskSensorInitTaskParams(ILPS22QSTask *_this);

/**
 * Private implementation of sensor interface methods for ILPS22QS sensor
 */
static sys_error_code_t ILPS22QSTaskSensorSetODR(ILPS22QSTask *_this, SMMessage message);
static sys_error_code_t ILPS22QSTaskSensorSetFS(ILPS22QSTask *_this, SMMessage message);
static sys_error_code_t ILPS22QSTaskSensorEnable(ILPS22QSTask *_this, SMMessage message);
static sys_error_code_t ILPS22QSTaskSensorDisable(ILPS22QSTask *_this, SMMessage message);

/**
 * Check if the sensor is active. The sensor is active if at least one of the sub sensor is active.
 * @param _this [IN] specifies a pointer to a task object.
 * @return TRUE if the sensor is active, FALSE otherwise.
 */
static boolean_t ILPS22QSTaskSensorIsActive(const ILPS22QSTask *_this);

static sys_error_code_t ILPS22QSTaskEnterLowPowerMode(const ILPS22QSTask *_this);

static sys_error_code_t ILPS22QSTaskConfigureIrqPin(const ILPS22QSTask *_this, boolean_t LowPower);

/**
 * Callback function called when the software timer expires.
 *
 * @param xTimer [IN] specifies the handle of the expired timer.
 */
static void ILPS22QSTaskTimerCallbackFunction(ULONG timer);

/**
 * Given a interface pointer it return the instance of the object that implement the interface.
 *
 * @param p_if [IN] specifies a sensor interface implemented by the task object.
 * @return the instance of the task object that implements the given interface.
 */
static inline ILPS22QSTask* ILPS22QSTaskGetOwnerFromISensorIF(ISensor_t *p_if);

/**
 * IRQ callback
 */
void ILPS22QSTask_EXTI_Callback(uint16_t nPin);

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
static inline sys_error_code_t ILPS22QSTaskPostMessageToFront(ILPS22QSTask *_this, SMMessage *pMessage);

/**
 * Private function used to post a message into the back of the task queue.
 * Used to resume the task when the required by the INIT task.
 *
 * @param this [IN] specifies a pointer to the task object.
 * @param pMessage [IN] specifies a message to send.
 * @return SYS_NO_EROR_CODE if success, SYS_APP_TASK_REPORT_LOST_ERROR_CODE.
 */
static inline sys_error_code_t ILPS22QSTaskPostMessageToBack(ILPS22QSTask *_this, SMMessage *pMessage);

#if defined (__GNUC__)
// Inline function defined inline in the header file ILPS22QSTask.h must be declared here as extern function.
#endif

/* Objects instance */
/********************/

/**
 * The only instance of the task object.
 */
static ILPS22QSTask sTaskObj;

/**
 * The class object.
 */
static const ILPS22QSTaskClass_t sTheClass =
{
    /* Class virtual table */
    {
        ILPS22QSTask_vtblHardwareInit,
        ILPS22QSTask_vtblOnCreateTask,
        ILPS22QSTask_vtblDoEnterPowerMode,
        ILPS22QSTask_vtblHandleError,
        ILPS22QSTask_vtblOnEnterTaskControlLoop,
        ILPS22QSTask_vtblForceExecuteStep,
        ILPS22QSTask_vtblOnEnterPowerMode },

    /* class::press_sensor_if_vtbl virtual table */
    {
        ILPS22QSTask_vtblPressGetId,
        ILPS22QSTask_vtblPressGetEventSourceIF,
        ILPS22QSTask_vtblPressGetODR,
        ILPS22QSTask_vtblPressGetFS,
        ILPS22QSTask_vtblPressGetSensitivity,
        ILPS22QSTask_vtblPressGetDataType,
        ILPS22QSTask_vtblPressGetDimensions,
        ILPS22QSTask_vtblSensorSetODR,
        ILPS22QSTask_vtblSensorSetFS,
        ILPS22QSTask_vtblSensorEnable,
        ILPS22QSTask_vtblSensorDisable,
        ILPS22QSTask_vtblSensorIsEnabled,
        ILPS22QSTask_vtblPressGetDescription,
        ILPS22QSTask_vtblPressGetStatus },

    /* PRESSURE DESCRIPTOR */
    {
        "ilps22qs",
        COM_TYPE_PRESS,
        {
            1.0f,
            4.0f,
            10.0f,
            25.0f,
            50.0f,
            75.0f,
            100.0f,
            200.0f,
            COM_END_OF_LIST_FLOAT, },
        {
            1260.0f,
            4060.0f,
            COM_END_OF_LIST_FLOAT, },
        {
            "prs", },
        "hPa",
        {
            0,
            1000, } },

    /* class (PM_STATE, ExecuteStepFunc) map */
    {
        ILPS22QSTaskExecuteStepState1,
        NULL,
        ILPS22QSTaskExecuteStepDatalog, } };

// Public API definition
// *********************

ISourceObservable* ILPS22QSTaskGetPressSensorIF(ILPS22QSTask *_this)
{
  assert_param(_this != NULL);
  return (ISourceObservable*) &(_this->press_sensor_if);
}

AManagedTaskEx* ILPS22QSTaskAlloc(const void *pIRQConfig, const void *pCSConfig, EBusType eBusType)
{
  // In this application there is only one Keyboard task,
  // so this allocator implement the singleton design pattern.

  // Initialize the super class
  AMTInitEx(&sTaskObj.super);

  sTaskObj.super.vptr = &sTheClass.vtbl;
  sTaskObj.press_sensor_if.vptr = &sTheClass.press_sensor_if_vtbl;
  sTaskObj.press_sensor_descriptor = &sTheClass.press_class_descriptor;

  sTaskObj.pIRQConfig = (MX_GPIOParams_t*)pIRQConfig;
  sTaskObj.pCSConfig = (MX_GPIOParams_t*)pCSConfig;

  sTaskObj.eBusType = eBusType;

  return (AManagedTaskEx*) &sTaskObj;
}

ABusIF* ILPS22QSTaskGetSensorIF(ILPS22QSTask *_this)
{
  assert_param(_this != NULL);

  return _this->p_sensor_bus_if;
}

IEventSrc* ILPS22QSTaskGetPressEventSrcIF(ILPS22QSTask *_this)
{
  assert_param(_this != NULL);

  return _this->p_press_event_src;
}

// AManagedTask virtual functions definition
// ***********************************************

sys_error_code_t ILPS22QSTask_vtblHardwareInit(AManagedTask *_this, void *pParams)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  ILPS22QSTask *p_obj = (ILPS22QSTask*) _this;

  /* Configure CS Pin */
  if(p_obj->pCSConfig != NULL)
  {
    p_obj->pCSConfig->p_mx_init_f();
  }

  return res;
}

sys_error_code_t ILPS22QSTask_vtblOnCreateTask(
		AManagedTask *_this,
		tx_entry_function_t *pTaskCode,
		CHAR **pName,
		VOID **pvStackStart,
		ULONG *pStackDepth,
		UINT *pPriority,
		UINT *pPreemptThreshold,
		ULONG *pTimeSlice,
		ULONG *pAutoStart,
		ULONG *pParams)
{
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  ILPS22QSTask *p_obj = (ILPS22QSTask*) _this;

  // Create task specific sw resources.

  uint16_t nItemSize = ILPS22QS_TASK_CFG_IN_QUEUE_ITEM_SIZE;
  VOID *pvQueueItemsBuff = SysAlloc(ILPS22QS_TASK_CFG_IN_QUEUE_LENGTH * nItemSize);
  if(pvQueueItemsBuff == NULL)
  {
    res = SYS_TASK_HEAP_OUT_OF_MEMORY_ERROR_CODE;
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(res);
    return res;
  }

  if(TX_SUCCESS != tx_queue_create(&p_obj->in_queue, "ILPS22QS_Q", nItemSize / 4, pvQueueItemsBuff, ILPS22QS_TASK_CFG_IN_QUEUE_LENGTH * nItemSize))
  {
    res = SYS_TASK_HEAP_OUT_OF_MEMORY_ERROR_CODE;
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(res);
    return res;
  }

  /* create the software timer*/
  if(TX_SUCCESS != tx_timer_create(
		  &p_obj->read_fifo_timer,
		  "ILPS22WS_T",
		  ILPS22QSTaskTimerCallbackFunction,
		  (ULONG)TX_NULL,
		  AMT_MS_TO_TICKS(ILPS22QS_TASK_CFG_TIMER_PERIOD_MS),
		  0,
		  TX_NO_ACTIVATE))
  {
    res = SYS_TASK_HEAP_OUT_OF_MEMORY_ERROR_CODE;
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(res);
    return res;
  }

  switch (p_obj->eBusType)
  {
    case SPI_IF:
#if (SM_SPI_BUS_IF == 1)
      assert(p_obj->pCSConfig != NULL);
      p_obj->p_sensor_bus_if = SPIBusIFAlloc(ILPS22QS_ID, p_obj->pCSConfig->port, p_obj->pCSConfig->pin, 0);
      if (p_obj->p_sensor_bus_if == NULL)
      {
        res = SYS_TASK_HEAP_OUT_OF_MEMORY_ERROR_CODE;
        SYS_SET_SERVICE_LEVEL_ERROR_CODE(res);
      }
#endif
      break;
    case I2C_IF:
#if (SM_I2C_BUS_IF == 1)
      p_obj->p_sensor_bus_if = I2CBusIFAlloc(ILPS22QS_ID, ILPS22QS_I2C_ADD, 0);
      if (p_obj->p_sensor_bus_if == NULL)
      {
        res = SYS_TASK_HEAP_OUT_OF_MEMORY_ERROR_CODE;
        SYS_SET_SERVICE_LEVEL_ERROR_CODE(res);
      }
#endif
      break;
    default:
      // Unknown bus type
      res = SYS_TASK_INVALID_PARAM_ERROR_CODE;
      SYS_SET_SERVICE_LEVEL_ERROR_CODE(res);
      break;
  }

  if (SYS_IS_ERROR_CODE(res))
  {
    return res;
  }

  p_obj->p_press_event_src = SensorEventSrcAlloc();
  if(p_obj->p_press_event_src == NULL)
  {
    res = SYS_TASK_HEAP_OUT_OF_MEMORY_ERROR_CODE;
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(res);
    return res;
  }
  IEventSrcInit(p_obj->p_press_event_src);

  memset(p_obj->p_press_data_buff, 0, sizeof(p_obj->p_press_data_buff));
  p_obj->press_id = 0;
  p_obj->timestamp_tick = 0;
  p_obj->old_timestamp_tick = 0;
  p_obj->timestamp = 0;
  p_obj->samples_per_it = 0;
  p_obj->task_delay = 0;
  _this->m_pfPMState2FuncMap = sTheClass.p_pm_state2func_map;

  *pTaskCode = AMTExRun;
  *pName = "ILPS22QS";
  *pvStackStart = NULL; // allocate the task stack in the system memory pool.
  *pStackDepth = ILPS22QS_TASK_CFG_STACK_DEPTH;
  *pParams = (ULONG) _this;
  *pPriority = ILPS22QS_TASK_CFG_PRIORITY;
  *pPreemptThreshold = ILPS22QS_TASK_CFG_PRIORITY;
  *pTimeSlice = TX_NO_TIME_SLICE;
  *pAutoStart = TX_AUTO_START;

  res = ILPS22QSTaskSensorInitTaskParams(p_obj);
  if(SYS_IS_ERROR_CODE(res))
  {
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_OUT_OF_MEMORY_ERROR_CODE);
    res = SYS_OUT_OF_MEMORY_ERROR_CODE;
    return res;
  }

  res = ILPS22QSTaskSensorRegister(p_obj);
  if(SYS_IS_ERROR_CODE(res))
  {
    SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("ILPS22QS: unable to register with DB\r\n"));
    sys_error_handler();
  }

  return res;
}

sys_error_code_t ILPS22QSTask_vtblDoEnterPowerMode(AManagedTask *_this, const EPowerMode ActivePowerMode, const EPowerMode NewPowerMode)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  ILPS22QSTask *p_obj = (ILPS22QSTask*) _this;
  stmdev_ctx_t *p_sensor_drv = (stmdev_ctx_t*) &p_obj->p_sensor_bus_if->m_xConnector;

  if(NewPowerMode == E_POWER_MODE_SENSORS_ACTIVE)
  {
    if(ILPS22QSTaskSensorIsActive(p_obj))
    {
      SMMessage message =
      {
          .sensorMessage.messageId = SM_MESSAGE_ID_SENSOR_CMD,
          .sensorMessage.nCmdID = SENSOR_CMD_ID_INIT
      };

      if(tx_queue_send(&p_obj->in_queue, &message, AMT_MS_TO_TICKS(100)) != TX_SUCCESS)
      {
        res = SYS_APP_TASK_REPORT_LOST_ERROR_CODE;
        SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_APP_TASK_REPORT_LOST_ERROR_CODE);
      }

      // reset the variables for the time stamp computation.
      p_obj->timestamp_tick = 0;
      p_obj->old_timestamp_tick = 0;
      p_obj->timestamp = 0;
    }

    SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("ILPS22QS: -> SENSORS_ACTIVE\r\n"));
  }
  else if(NewPowerMode == E_POWER_MODE_STATE1)
  {
    if(ActivePowerMode == E_POWER_MODE_SENSORS_ACTIVE)
    {
      ilps22qs_md_t val;
      ilps22qs_mode_get(p_sensor_drv, &val);
      val.odr = ILPS22QS_ONE_SHOT;
      /* SM_SENSOR_STATE_SUSPENDING */
      ilps22qs_mode_set(p_sensor_drv, &val);
      tx_queue_flush(&p_obj->in_queue);
      if(p_obj->pIRQConfig == NULL)
      {
        tx_timer_deactivate(&p_obj->read_fifo_timer);
      }
    }

    SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("ILPS22QS: -> RUN\r\n"));
  }
  else if(NewPowerMode == E_POWER_MODE_SLEEP_1)
  {
    // the MCU is going in stop so I put the sensor in low power
    // from the INIT task
    res = ILPS22QSTaskEnterLowPowerMode(p_obj);
    if(SYS_IS_ERROR_CODE(res))
    {
      sys_error_handler();
    }
    if(p_obj->pIRQConfig != NULL)
    {
      ILPS22QSTaskConfigureIrqPin(p_obj, TRUE);
    }
    // notify the bus
    if(p_obj->p_sensor_bus_if->m_pfBusCtrl != NULL)
    {
      p_obj->p_sensor_bus_if->m_pfBusCtrl(p_obj->p_sensor_bus_if, E_BUS_CTRL_DEV_NOTIFY_POWER_MODE, 0);
    }
    if(p_obj->pIRQConfig == NULL)
    {
      tx_timer_deactivate(&p_obj->read_fifo_timer);
    }

    SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("ILPS22QS: -> SLEEP_1\r\n"));
  }

  return res;
}

sys_error_code_t ILPS22QSTask_vtblHandleError(AManagedTask *_this, SysEvent xError)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  //  ILPS22QSTask *p_obj = (ILPS22QSTask*)_this;

  return res;
}

sys_error_code_t ILPS22QSTask_vtblOnEnterTaskControlLoop(AManagedTask *_this)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("ILPS22QS: start.\r\n"));

  // At this point all system has been initialized.
  // Execute task specific delayed one time initialization.

  return res;
}

sys_error_code_t ILPS22QSTask_vtblForceExecuteStep(AManagedTaskEx *_this, EPowerMode ActivePowerMode)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  ILPS22QSTask *p_obj = (ILPS22QSTask*) _this;

  SMMessage message =
  {
      .internalMessageFE.messageId = SM_MESSAGE_ID_FORCE_STEP,
      .internalMessageFE.nData = 0
  };

  if((ActivePowerMode == E_POWER_MODE_STATE1) || (ActivePowerMode == E_POWER_MODE_SENSORS_ACTIVE))
  {
    if(AMTExIsTaskInactive(_this))
    {
      res = ILPS22QSTaskPostMessageToFront(p_obj, (SMMessage*) &message);
    }
    else
    {
      // do nothing and wait for the step to complete.
      //      _this->m_xStatus.nDelayPowerModeSwitch = 0;
    }
  }
  else
  {
    UINT state;
    if(TX_SUCCESS == tx_thread_info_get(&_this->m_xThaskHandle, TX_NULL, &state, TX_NULL, TX_NULL, TX_NULL, TX_NULL, TX_NULL, TX_NULL))
    {
      if(state == TX_SUSPENDED)
      {
        tx_thread_resume(&_this->m_xThaskHandle);
      }
    }
  }

  return res;
}

sys_error_code_t ILPS22QSTask_vtblOnEnterPowerMode(AManagedTaskEx *_this, const EPowerMode ActivePowerMode, const EPowerMode NewPowerMode)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  //  ILPS22QSTask *p_obj = (ILPS22QSTask*)_this;

  return res;
}

// ISensor virtual functions definition
// *******************************************

uint8_t ILPS22QSTask_vtblPressGetId(ISourceObservable *_this)
{
  assert_param(_this != NULL);
  ILPS22QSTask *p_if_owner = (ILPS22QSTask*) ((uint32_t) _this - offsetof(ILPS22QSTask, press_sensor_if));
  uint8_t res = p_if_owner->press_id;

  return res;
}

IEventSrc* ILPS22QSTask_vtblPressGetEventSourceIF(ISourceObservable *_this)
{
  assert_param(_this != NULL);
  ILPS22QSTask *p_if_owner = (ILPS22QSTask*) ((uint32_t) _this - offsetof(ILPS22QSTask, press_sensor_if));

  return p_if_owner->p_press_event_src;
}

sys_error_code_t ILPS22QSTask_vtblPressGetODR(ISourceObservable *_this, float *p_measured, float *p_nominal)
{
  assert_param(_this != NULL);
  /*get the object implementing the ISourceObservable IF */
  ILPS22QSTask *p_if_owner = (ILPS22QSTask*) ((uint32_t) _this - offsetof(ILPS22QSTask, press_sensor_if));
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  /* parameter validation */
  if((p_measured) == NULL || (p_nominal == NULL))
  {
    res = SYS_INVALID_PARAMETER_ERROR_CODE;
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_INVALID_PARAMETER_ERROR_CODE);
  }
  else
  {
    *p_measured = p_if_owner->press_sensor_status.MeasuredODR;
    *p_nominal = p_if_owner->press_sensor_status.ODR;
  }

  return res;
}

float ILPS22QSTask_vtblPressGetFS(ISourceObservable *_this)
{
  assert_param(_this != NULL);
  ILPS22QSTask *p_if_owner = (ILPS22QSTask*) ((uint32_t) _this - offsetof(ILPS22QSTask, press_sensor_if));
  float res = p_if_owner->press_sensor_status.FS;

  return res;
}

float ILPS22QSTask_vtblPressGetSensitivity(ISourceObservable *_this)
{
  assert_param(_this != NULL);
  ILPS22QSTask *p_if_owner = (ILPS22QSTask*) ((uint32_t) _this - offsetof(ILPS22QSTask, press_sensor_if));
  float res = p_if_owner->press_sensor_status.Sensitivity;

  return res;
}

uint8_t ILPS22QSTask_vtblPressGetDataType(ISourceObservable *_this)
{
  assert_param(_this != NULL);
  ILPS22QSTask *p_if_owner = (ILPS22QSTask*)((uint32_t)_this - offsetof(ILPS22QSTask, press_sensor_if));
  uint8_t res = p_if_owner->press_sensor_status.DataType;

  return res;
}

uint16_t ILPS22QSTask_vtblPressGetDimensions(ISourceObservable *_this)
{
  assert_param(_this != NULL);
  ILPS22QSTask *p_if_owner = (ILPS22QSTask*)((uint32_t)_this - offsetof(ILPS22QSTask, press_sensor_if));
  uint16_t res = p_if_owner->press_sensor_status.Dimensions;

  return res;
}

sys_error_code_t ILPS22QSTask_vtblSensorSetODR(ISensor_t *_this, float ODR)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  ILPS22QSTask *p_if_owner = ILPS22QSTaskGetOwnerFromISensorIF(_this);

  EPowerMode log_status = AMTGetTaskPowerMode((AManagedTask*) p_if_owner);
  uint8_t sensor_id = ISourceGetId((ISourceObservable*) _this);

  if((log_status == E_POWER_MODE_SENSORS_ACTIVE) && ISensorIsEnabled(_this))
  {
    res = SYS_INVALID_PARAMETER_ERROR_CODE;
  }
  else
  {
    /* Set a new command message in the queue */
    SMMessage message =
    {
        .sensorMessage.messageId = SM_MESSAGE_ID_SENSOR_CMD,
        .sensorMessage.nCmdID = SENSOR_CMD_ID_SET_ODR,
        .sensorMessage.nSensorId = sensor_id,
        .sensorMessage.nParam = (uint32_t) ODR
    };
    res = ILPS22QSTaskPostMessageToBack(p_if_owner, (SMMessage*) &message);
  }

  return res;
}

sys_error_code_t ILPS22QSTask_vtblSensorSetFS(ISensor_t *_this, float FS)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  ILPS22QSTask *p_if_owner = ILPS22QSTaskGetOwnerFromISensorIF(_this);

  EPowerMode log_status = AMTGetTaskPowerMode((AManagedTask*) p_if_owner);
  uint8_t sensor_id = ISourceGetId((ISourceObservable*) _this);

  if((log_status == E_POWER_MODE_SENSORS_ACTIVE) && ISensorIsEnabled(_this))
  {
    res = SYS_INVALID_PARAMETER_ERROR_CODE;
  }
  else
  {
    /* Set a new command message in the queue */
    SMMessage message =
    {
        .sensorMessage.messageId = SM_MESSAGE_ID_SENSOR_CMD,
        .sensorMessage.nCmdID = SENSOR_CMD_ID_SET_FS,
        .sensorMessage.nSensorId = sensor_id,
        .sensorMessage.nParam = (uint32_t) FS
    };
    res = ILPS22QSTaskPostMessageToBack(p_if_owner, (SMMessage*) &message);
  }

  return res;

}

sys_error_code_t ILPS22QSTask_vtblSensorEnable(ISensor_t *_this)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  ILPS22QSTask *p_if_owner = ILPS22QSTaskGetOwnerFromISensorIF(_this);

  EPowerMode log_status = AMTGetTaskPowerMode((AManagedTask*) p_if_owner);
  uint8_t sensor_id = ISourceGetId((ISourceObservable*) _this);

  if((log_status == E_POWER_MODE_SENSORS_ACTIVE) && ISensorIsEnabled(_this))
  {
    res = SYS_INVALID_PARAMETER_ERROR_CODE;
  }
  else
  {
    /* Set a new command message in the queue */
    SMMessage message =
    {
        .sensorMessage.messageId = SM_MESSAGE_ID_SENSOR_CMD,
        .sensorMessage.nCmdID = SENSOR_CMD_ID_ENABLE,
        .sensorMessage.nSensorId = sensor_id
    };
    res = ILPS22QSTaskPostMessageToBack(p_if_owner, (SMMessage*) &message);
  }

  return res;
}

sys_error_code_t ILPS22QSTask_vtblSensorDisable(ISensor_t *_this)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  ILPS22QSTask *p_if_owner = ILPS22QSTaskGetOwnerFromISensorIF(_this);

  EPowerMode log_status = AMTGetTaskPowerMode((AManagedTask*) p_if_owner);
  uint8_t sensor_id = ISourceGetId((ISourceObservable*) _this);

  if((log_status == E_POWER_MODE_SENSORS_ACTIVE) && ISensorIsEnabled(_this))
  {
    res = SYS_INVALID_PARAMETER_ERROR_CODE;
  }
  else
  {
    /* Set a new command message in the queue */
    SMMessage message =
    {
        .sensorMessage.messageId = SM_MESSAGE_ID_SENSOR_CMD,
        .sensorMessage.nCmdID = SENSOR_CMD_ID_DISABLE,
        .sensorMessage.nSensorId = sensor_id
    };
    res = ILPS22QSTaskPostMessageToBack(p_if_owner, (SMMessage*) &message);
  }

  return res;
}

boolean_t ILPS22QSTask_vtblSensorIsEnabled(ISensor_t *_this)
{
  assert_param(_this != NULL);
  boolean_t res = FALSE;
  ILPS22QSTask *p_if_owner = ILPS22QSTaskGetOwnerFromISensorIF(_this);

  if(ISourceGetId((ISourceObservable*) _this) == p_if_owner->press_id)
    res = p_if_owner->press_sensor_status.IsActive;

  return res;
}

SensorDescriptor_t ILPS22QSTask_vtblPressGetDescription(ISensor_t *_this)
{
  assert_param(_this != NULL);
  ILPS22QSTask *p_if_owner = ILPS22QSTaskGetOwnerFromISensorIF(_this);
  return *p_if_owner->press_sensor_descriptor;
}

SensorStatus_t ILPS22QSTask_vtblPressGetStatus(ISensor_t *_this)
{
  assert_param(_this != NULL);
  ILPS22QSTask *p_if_owner = ILPS22QSTaskGetOwnerFromISensorIF(_this);
  return p_if_owner->press_sensor_status;
}

// Private function definition
// ***************************

static sys_error_code_t ILPS22QSTaskExecuteStepState1(AManagedTask *_this)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  ILPS22QSTask *p_obj = (ILPS22QSTask*) _this;
  SMMessage message =
  {
      0
  };

  AMTExSetInactiveState((AManagedTaskEx*) _this, TRUE);
  if(TX_SUCCESS == tx_queue_receive(&p_obj->in_queue, &message, TX_WAIT_FOREVER))
  {
    AMTExSetInactiveState((AManagedTaskEx*) _this, FALSE);

    switch(message.messageID)
    {
      case SM_MESSAGE_ID_FORCE_STEP:
        {
          // do nothing. I need only to resume.
          __NOP();
          break;
        }
      case SM_MESSAGE_ID_SENSOR_CMD:
        {
          switch(message.sensorMessage.nCmdID)
          {
            case SENSOR_CMD_ID_SET_ODR:
              res = ILPS22QSTaskSensorSetODR(p_obj, message);
              break;
            case SENSOR_CMD_ID_SET_FS:
              res = ILPS22QSTaskSensorSetFS(p_obj, message);
              break;
            case SENSOR_CMD_ID_ENABLE:
              res = ILPS22QSTaskSensorEnable(p_obj, message);
              break;
            case SENSOR_CMD_ID_DISABLE:
              res = ILPS22QSTaskSensorDisable(p_obj, message);
              break;
            default:
              // unwanted message
              res = SYS_APP_TASK_UNKNOWN_REPORT_ERROR_CODE;
              SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_APP_TASK_UNKNOWN_REPORT_ERROR_CODE);

              SYS_DEBUGF(SYS_DBG_LEVEL_WARNING, ("ILPS22QS: unexpected message in State1: %i\r\n", message.messageID));
              break;
          }
          break;
        }
      default:
        {
          // unwanted message
          res = SYS_APP_TASK_UNKNOWN_REPORT_ERROR_CODE;
          SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_APP_TASK_UNKNOWN_REPORT_ERROR_CODE);

          SYS_DEBUGF(SYS_DBG_LEVEL_WARNING, ("ILPS22QS: unexpected message in State1: %i\r\n", message.messageID));
          break;
        }
    }
  }

  return res;
}

static sys_error_code_t ILPS22QSTaskExecuteStepDatalog(AManagedTask *_this)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  ILPS22QSTask *p_obj = (ILPS22QSTask*) _this;
  SMMessage message =
  {
      0
  };

  AMTExSetInactiveState((AManagedTaskEx*) _this, TRUE);
  if(TX_SUCCESS == tx_queue_receive(&p_obj->in_queue, &message, TX_WAIT_FOREVER))
  {
    AMTExSetInactiveState((AManagedTaskEx*) _this, FALSE);

    switch(message.messageID)
    {
      case SM_MESSAGE_ID_FORCE_STEP:
        {
          // do nothing. I need only to resume.
          __NOP();
          break;
        }
      case SM_MESSAGE_ID_DATA_READY:
        {
//          SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("ILPS22QS: new data.\r\n"));
          if(p_obj->pIRQConfig == NULL)
          {
            if(TX_SUCCESS != tx_timer_change(&p_obj->read_fifo_timer, AMT_MS_TO_TICKS(p_obj->task_delay), AMT_MS_TO_TICKS(p_obj->task_delay)))
            {
              return SYS_UNDEFINED_ERROR_CODE;
            }
          }

          res = ILPS22QSTaskSensorReadData(p_obj);
          if(!SYS_IS_ERROR_CODE(res))
          {
            // update the time stamp
            uint32_t period = 0;
            if(p_obj->timestamp_tick >= p_obj->old_timestamp_tick)
            {
              period = p_obj->timestamp_tick - p_obj->old_timestamp_tick;
            }
            else
            {
              // overflow of the hw timer
              period = p_obj->timestamp_tick + (0xFFFFFFFF - p_obj->old_timestamp_tick);
            }
            p_obj->old_timestamp_tick = p_obj->timestamp_tick;
            p_obj->timestamp += period;
            // notify the listeners...
            double timestamp = (double) p_obj->timestamp / (double) (SystemCoreClock);
            double delta_timestamp = (double) period / (double) (SystemCoreClock);

            if(p_obj->press_sensor_status.IsActive)
            {
              SensorEvent evt;

              AI_SP_Stream_t stream =
              {
                  .packet.payload = (uint8_t*) p_obj->p_press_data_buff,
                  .packet.payload_fmt = AI_SP_FMT_FLOAT32_RESET(),
                  .mode = AI_SP_MODE_COLUMN //TODO: STF - this means that data are interleaved?!?
                  // bonus question: what is AI_LOGGING_SHAPES_DEPTH ??
                  // (can I represent anomogeneous matrix [4*4] with this data format ?
                  };
              ai_logging_create_shape_1d(&stream.packet.shape, p_obj->samples_per_it);
              stream.packet.payload_size = 4 * stream.packet.shape.shapes[0] * stream.packet.shape.shapes[1];

              SensorEventInit((IEvent*) &evt, p_obj->p_press_event_src, (ai_logging_packet_t*) &stream, timestamp, p_obj->press_id);
              IEventSrcSendEvent(p_obj->p_press_event_src, (IEvent*) &evt, NULL);
            }

            /* update measuredODR */
            p_obj->press_sensor_status.MeasuredODR = p_obj->samples_per_it / delta_timestamp;

//          SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("ILPS22QS: ts = %f\r\n", (float)timestamp));
            if(p_obj->pIRQConfig == NULL)
            {
              if (TX_SUCCESS != tx_timer_activate(&p_obj->read_fifo_timer))
              {
            	  res = SYS_UNDEFINED_ERROR_CODE;
              }
            }
          }
          break;
        }
      case SM_MESSAGE_ID_SENSOR_CMD:
        {
          switch(message.sensorMessage.nCmdID)
          {
            case SENSOR_CMD_ID_INIT:
              res = ILPS22QSTaskSensorInit(p_obj);
              if(!SYS_IS_ERROR_CODE(res))
              {
                if(p_obj->pIRQConfig == NULL)
                {
                  if (TX_SUCCESS != tx_timer_activate(&p_obj->read_fifo_timer))
                  {
                    res = SYS_UNDEFINED_ERROR_CODE;
                  }
        	    }
                else
                {
        		  ILPS22QSTaskConfigureIrqPin(p_obj, FALSE);
        	    }
              }
              break;
            case SENSOR_CMD_ID_SET_ODR:
              res = ILPS22QSTaskSensorSetODR(p_obj, message);
              break;
            case SENSOR_CMD_ID_SET_FS:
              res = ILPS22QSTaskSensorSetFS(p_obj, message);
              break;
            case SENSOR_CMD_ID_ENABLE:
              res = ILPS22QSTaskSensorEnable(p_obj, message);
              break;
            case SENSOR_CMD_ID_DISABLE:
              res = ILPS22QSTaskSensorDisable(p_obj, message);
              break;
            default:
              // unwanted message
              res = SYS_APP_TASK_UNKNOWN_REPORT_ERROR_CODE;
              SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_APP_TASK_UNKNOWN_REPORT_ERROR_CODE)
              ;

              SYS_DEBUGF(SYS_DBG_LEVEL_WARNING, ("ILPS22QS: unexpected message in Datalog: %i\r\n", message.messageID));
              break;
          }
          break;
        }
      default:
        // unwanted message
        res = SYS_APP_TASK_UNKNOWN_REPORT_ERROR_CODE;
        SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_APP_TASK_UNKNOWN_REPORT_ERROR_CODE)
        ;

        SYS_DEBUGF(SYS_DBG_LEVEL_WARNING, ("ILPS22QS: unexpected message in Datalog: %i\r\n", message.messageID));
        break;
    }
  }

  return res;
}

static inline sys_error_code_t ILPS22QSTaskPostMessageToFront(ILPS22QSTask *_this, SMMessage *pMessage)
{
  assert_param(_this != NULL);
  assert_param(pMessage);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  if(SYS_IS_CALLED_FROM_ISR())
  {
    if(TX_SUCCESS != tx_queue_send(&_this->in_queue, pMessage, TX_NO_WAIT))
    {
      res = SYS_APP_TASK_REPORT_LOST_ERROR_CODE;
      // this function is private and the caller will ignore this return code.
    }
  }
  else
  {
    if(TX_SUCCESS != tx_queue_send(&_this->in_queue, pMessage, AMT_MS_TO_TICKS(100)))
    {
      res = SYS_APP_TASK_REPORT_LOST_ERROR_CODE;
      // this function is private and the caller will ignore this return code.
    }
  }

  return res;
}

static inline sys_error_code_t ILPS22QSTaskPostMessageToBack(ILPS22QSTask *_this, SMMessage *pMessage)
{
  assert_param(_this != NULL);
  assert_param(pMessage);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  if(SYS_IS_CALLED_FROM_ISR())
  {
    if(TX_SUCCESS != tx_queue_front_send(&_this->in_queue, pMessage, TX_NO_WAIT))
    {
      res = SYS_APP_TASK_REPORT_LOST_ERROR_CODE;
      // this function is private and the caller will ignore this return code.
    }
  }
  else
  {
    if(TX_SUCCESS != tx_queue_front_send(&_this->in_queue, pMessage, AMT_MS_TO_TICKS(100)))
    {
      res = SYS_APP_TASK_REPORT_LOST_ERROR_CODE;
      // this function is private and the caller will ignore this return code.
    }
  }

  return res;
}

static sys_error_code_t ILPS22QSTaskSensorInit(ILPS22QSTask *_this)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  stmdev_ctx_t *p_sensor_drv = (stmdev_ctx_t*) &_this->p_sensor_bus_if->m_xConnector;

  int32_t ret_val = 0;

  /* Set bdu and if_inc recommended for driver usage */
  ilps22qs_init_set(p_sensor_drv, ILPS22QS_DRV_RDY);

  /* Select bus interface */
  ilps22qs_bus_mode_t bus_mode;
  bus_mode.interface = ILPS22QS_SEL_BY_HW;
  bus_mode.filter = ILPS22QS_AUTO;
  ilps22qs_bus_mode_set(p_sensor_drv, &bus_mode);

  /* Set Output Data Rate in Power Down */
  /* Configure basic parameters */
  ilps22qs_md_t md;
  ilps22qs_mode_get(p_sensor_drv, &md);
  md.odr = ILPS22QS_ONE_SHOT;
  md.avg = ILPS22QS_4_AVG;
  md.lpf = ILPS22QS_LPF_ODR_DIV_4;
  md.fs = ILPS22QS_1260hPa;
  ilps22qs_mode_set(p_sensor_drv, &md);

  /* Read sensor id */
  ilps22qs_id_t reg0;

  ret_val = ilps22qs_id_get(p_sensor_drv, &reg0);
  if(!ret_val)
  {
    ABusIFSetWhoAmI(_this->p_sensor_bus_if, reg0.whoami);
  }
  SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("ILPS22QS: sensor - I am 0x%x.\r\n", reg0.whoami));

  /* Set fifo mode */
  ilps22qs_fifo_md_t fifo_md;
  fifo_md.operation = ILPS22QS_STREAM;
  ilps22qs_fifo_mode_set(p_sensor_drv, &fifo_md);

  /* Set ODR and FS */
  ilps22qs_mode_get(p_sensor_drv, &md);

  /* Currently, the INT pin is not exposed */
  if(_this->pIRQConfig != NULL)
  {
    /* you should enable the INT here */
  }

  float ilps22qs_odr;
  ilps22qs_odr = _this->press_sensor_status.ODR;
  if(ilps22qs_odr < 2.0f)
  {
    md.odr = ILPS22QS_1Hz;
    _this->task_delay = 2000;
  }
  else if(ilps22qs_odr < 5.0f)
  {
    md.odr = ILPS22QS_4Hz;
    _this->task_delay = 1000;
  }
  else if(ilps22qs_odr < 11.0f)
  {
    md.odr = ILPS22QS_10Hz;
    _this->task_delay = 1000;
  }
  else if(ilps22qs_odr < 26.0f)
  {
    md.odr = ILPS22QS_25Hz;
    _this->task_delay = 1000;
  }
  else if(ilps22qs_odr < 51.0f)
  {
    md.odr = ILPS22QS_50Hz;
    _this->task_delay = 1000;
  }
  else if(ilps22qs_odr < 76.0f)
  {
    md.odr = ILPS22QS_75Hz;
    _this->task_delay = 1000;
  }
  else if(ilps22qs_odr < 101.0f)
  {
    md.odr = ILPS22QS_100Hz;
    _this->task_delay = 1000;
  }
  else
  {
    md.odr = ILPS22QS_200Hz;
    _this->task_delay = 500;
  }

  float ilps22qs_fs;
  ilps22qs_fs = _this->press_sensor_status.FS;
  if(ilps22qs_fs < 1261.0f)
  {
    md.fs = ILPS22QS_1260hPa;
  }
  else
  {
    md.fs = ILPS22QS_4000hPa;
  }
  ilps22qs_mode_set(p_sensor_drv, &md);

  return res;
}

static sys_error_code_t ILPS22QSTaskSensorReadData(ILPS22QSTask *_this)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  stmdev_ctx_t *p_sensor_drv = (stmdev_ctx_t*) &_this->p_sensor_bus_if->m_xConnector;

  uint16_t i = 0;

  /* Check FIFO_WTM_IA and fifo level */
  ilps22qs_fifo_level_get(p_sensor_drv, (uint8_t*) &_this->samples_per_it);

  ilps22qs_read_reg(p_sensor_drv, ILPS22QS_FIFO_DATA_OUT_PRESS_XL, (uint8_t*) _this->p_fifo_data_buff, _this->samples_per_it * 3);

#if (HSD_USE_DUMMY_DATA == 1)
  for (i = 0; i < _this->samples_per_it ; i++)
  {
    _this->p_press_data_buff[i]  = (float)(dummyDataCounter_press++);
  }
#else
  /* Arrange Data */
  int32_t data;
  uint8_t *p8_src = _this->p_fifo_data_buff;
  uint8_t *p8_dest = _this->p_fifo_data_buff;

  for(i = 0; i < _this->samples_per_it; i++)
  {
    data = (int16_t) (p8_src[2]);
    data = (data * 256) + p8_src[1];
    data = (data * 256) + p8_src[0];
    data = (data * 256);
    *p8_dest++ = *p8_src++;
    *p8_dest++ = *p8_src++;
    *p8_dest++ = *p8_src++;

    _this->p_press_data_buff[i] = ilps22qs_from_fs4000_to_hPa(data);
  }
#endif

  return res;
}

static sys_error_code_t ILPS22QSTaskSensorRegister(ILPS22QSTask *_this)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  ISensor_t *press_if = (ISensor_t*) ILPS22QSTaskGetPressSensorIF(_this);

  _this->press_id = SMAddSensor(press_if);

  return res;
}

static sys_error_code_t ILPS22QSTaskSensorInitTaskParams(ILPS22QSTask *_this)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  /* PRESSURE STATUS */
  _this->press_sensor_status.DataType = DATA_TYPE_FLOAT;
  _this->press_sensor_status.Dimensions = 1;
  _this->press_sensor_status.IsActive = TRUE;
  _this->press_sensor_status.FS = 4060.0f;
  _this->press_sensor_status.Sensitivity = 1.0f;
  _this->press_sensor_status.ODR = 10.0f;
  _this->press_sensor_status.MeasuredODR = 0.0f;
  _this->press_sensor_status.InitialOffset = 0.0f;
  _this->press_sensor_status.DataPacketSize = 16;
#if (HSD_USE_DUMMY_DATA == 1)
  _this->press_sensor_status.SamplesPerTimestamp = 0;
#else
  _this->press_sensor_status.SamplesPerTimestamp = 200;
#endif

  return res;
}

static sys_error_code_t ILPS22QSTaskSensorSetODR(ILPS22QSTask *_this, SMMessage message)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  stmdev_ctx_t *p_sensor_drv = (stmdev_ctx_t*) &_this->p_sensor_bus_if->m_xConnector;
  float ODR = (float) message.sensorMessage.nParam;
  uint8_t id = message.sensorMessage.nSensorId;

  ilps22qs_md_t md;
  ilps22qs_mode_get(p_sensor_drv, &md);

  if(id == _this->press_id)
  {
    if(ODR < 2.0f)
    {
      md.odr = ILPS22QS_1Hz;
      ODR = 1;
      _this->task_delay = 2000;
    }
    else if(ODR < 5.0f)
    {
      md.odr = ILPS22QS_4Hz;
      ODR = 4;
      _this->task_delay = 1000;
    }
    else if(ODR < 11.0f)
    {
      md.odr = ILPS22QS_10Hz;
      ODR = 10;
      _this->task_delay = 1000;
    }
    else if(ODR < 26.0f)
    {
      md.odr = ILPS22QS_25Hz;
      ODR = 25;
      _this->task_delay = 1000;
    }
    else if(ODR < 51.0f)
    {
      md.odr = ILPS22QS_50Hz;
      ODR = 50;
      _this->task_delay = 1000;
    }
    else if(ODR < 76.0f)
    {
      md.odr = ILPS22QS_75Hz;
      ODR = 75;
      _this->task_delay = 1000;
    }
    else if(ODR < 101.0f)
    {
      md.odr = ILPS22QS_100Hz;
      ODR = 100;
      _this->task_delay = 1000;
    }
    else
    {
      md.odr = ILPS22QS_200Hz;
      ODR = 100;
      _this->task_delay = 500;
    }
    ilps22qs_mode_set(p_sensor_drv, &md);

    if(!SYS_IS_ERROR_CODE(res))
    {
      if(id == _this->press_id)
      {
        _this->press_sensor_status.ODR = ODR;
        _this->press_sensor_status.MeasuredODR = 0.0f;
      }
    }
  }
  else
  {
    res = SYS_INVALID_PARAMETER_ERROR_CODE;
  }

  return res;
}

static sys_error_code_t ILPS22QSTaskSensorSetFS(ILPS22QSTask *_this, SMMessage message)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  stmdev_ctx_t *p_sensor_drv = (stmdev_ctx_t*) &_this->p_sensor_bus_if->m_xConnector;
  float FS = (float) message.sensorMessage.nParam;
  uint8_t id = message.sensorMessage.nSensorId;

  ilps22qs_md_t md;
  ilps22qs_mode_get(p_sensor_drv, &md);

  if(id == _this->press_id)
  {
    if(FS <= 1261.0f)
    {
      md.fs = ILPS22QS_1260hPa;
      FS = 1260.0f;
    }
    else
    {
      md.fs = ILPS22QS_4000hPa;
      FS = 4060.0f;
    }
    ilps22qs_mode_set(p_sensor_drv, &md);

    if(!SYS_IS_ERROR_CODE(res))
    {
      _this->press_sensor_status.FS = FS;
    }
  }
  else
  {
    res = SYS_INVALID_PARAMETER_ERROR_CODE;
  }

  return res;
}

static sys_error_code_t ILPS22QSTaskSensorEnable(ILPS22QSTask *_this, SMMessage message)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  uint8_t id = message.sensorMessage.nSensorId;

  if(id == _this->press_id)
  {
    _this->press_sensor_status.IsActive = TRUE;
  }
  else
  {
    res = SYS_INVALID_PARAMETER_ERROR_CODE;
  }

  return res;
}

static sys_error_code_t ILPS22QSTaskSensorDisable(ILPS22QSTask *_this, SMMessage message)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  uint8_t id = message.sensorMessage.nSensorId;

  if(id == _this->press_id)
  {
    _this->press_sensor_status.IsActive = FALSE;
  }
  else
  {
    res = SYS_INVALID_PARAMETER_ERROR_CODE;
  }

  return res;
}

static boolean_t ILPS22QSTaskSensorIsActive(const ILPS22QSTask *_this)
{
  assert_param(_this != NULL);
  return (_this->press_sensor_status.IsActive);
}

static sys_error_code_t ILPS22QSTaskEnterLowPowerMode(const ILPS22QSTask *_this)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  stmdev_ctx_t *p_sensor_drv = (stmdev_ctx_t*) &_this->p_sensor_bus_if->m_xConnector;

  ilps22qs_md_t val;
  ilps22qs_mode_get(p_sensor_drv, &val);
  val.odr = ILPS22QS_ONE_SHOT;

  if(ilps22qs_mode_set(p_sensor_drv, &val))
  {
    res = SYS_SENSOR_TASK_OP_ERROR_CODE;
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_SENSOR_TASK_OP_ERROR_CODE);
  }

  return res;
}

static sys_error_code_t ILPS22QSTaskConfigureIrqPin(const ILPS22QSTask *_this, boolean_t LowPower)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  if (!LowPower)
  {
    /* Configure INT Pin */
    _this->pIRQConfig->p_mx_init_f();
  }
  else
  {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // first disable the IRQ to avoid spurious interrupt to wake the MCU up.
    HAL_NVIC_DisableIRQ(_this->pIRQConfig->irq_n);
    HAL_NVIC_ClearPendingIRQ(_this->pIRQConfig->irq_n);
    // then reconfigure the PIN in analog high impedance to reduce the power consumption.
    GPIO_InitStruct.Pin =  _this->pIRQConfig->pin;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(_this->pIRQConfig->port, &GPIO_InitStruct);
  }

  return res;
}

static void ILPS22QSTaskTimerCallbackFunction(ULONG timer)
{
  SMMessage report =
  {
    .sensorDataReadyMessage.messageId = SM_MESSAGE_ID_DATA_READY,
    //.sensorDataReadyMessage.fTimestamp = SysTsGetTimestampF(SysGetTimestampSrv())
  };

  // if (sTaskObj.in_queue != NULL ) {//TODO: STF.Port - how to check if the queue has been initialized ??
  if(TX_SUCCESS != tx_queue_send(&sTaskObj.in_queue, &report, TX_NO_WAIT))
  {
    // unable to send the message. Signal the error
    sys_error_handler();
  }
  sTaskObj.timestamp_tick = SMUtilGetTimeStamp();
  //}
}

void ILPS22QSTask_EXTI_Callback(uint16_t nPin)
{
  SMMessage report =
  {
    .sensorDataReadyMessage.messageId = SM_MESSAGE_ID_DATA_READY,
    //.sensorDataReadyMessage.fTimestamp = SysTsGetTimestampF(SysGetTimestampSrv())
  };

  //  if (sTaskObj.in_queue != NULL) { //TODO: STF.Port - how to check if the queue has been initialized ??
  if(TX_SUCCESS != tx_queue_send(&sTaskObj.in_queue, &report, TX_NO_WAIT))
  {
    // unable to send the report. Signal the error
    sys_error_handler();
  }
  sTaskObj.timestamp_tick = SMUtilGetTimeStamp();
//  }
}

static inline ILPS22QSTask* ILPS22QSTaskGetOwnerFromISensorIF(ISensor_t *p_if)
{
  assert_param(p_if != NULL);
  ILPS22QSTask *p_if_owner = NULL;

  /* check if the virtual function has been called from the pressure IF */
  p_if_owner = (ILPS22QSTask*) ((uint32_t) p_if - offsetof(ILPS22QSTask, press_sensor_if));

  return p_if_owner;
}

/**
 ******************************************************************************
 * @file    IIS2MDCTask.c
 * @author  SRA - MCD
 * @version 1.0.0
 * @date    30-Jul-2021
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

#include "IIS2MDCTask.h"
#include "IIS2MDCTask_vtbl.h"
#include "SMMessageParser.h"
#include "SensorCommands.h"
#include "SensorDef.h"
#include "SensorRegister.h"
#include "events/ISensorEventListener.h"
#include "events/ISensorEventListener_vtbl.h"
//#include "services/SysTimestamp.h"
#include "iis2mdc_reg.h"
#include "SMUtilTask.h"
#include <string.h>
#include "services/sysdebug.h"

#ifndef IIS2MDC_TASK_CFG_STACK_DEPTH
#define IIS2MDC_TASK_CFG_STACK_DEPTH        (TX_MINIMUM_STACK*8)
#endif

#ifndef IIS2MDC_TASK_CFG_PRIORITY
#define IIS2MDC_TASK_CFG_PRIORITY           (4)
#endif

#ifndef IIS2MDC_TASK_CFG_IN_QUEUE_LENGTH
#define IIS2MDC_TASK_CFG_IN_QUEUE_LENGTH          20
#endif

#define IIS2MDC_TASK_CFG_IN_QUEUE_ITEM_SIZE       sizeof(SMMessage)

#ifndef IIS2MDC_TASK_CFG_TIMER_PERIOD_MS
#define IIS2MDC_TASK_CFG_TIMER_PERIOD_MS          500
#endif

#define SYS_DEBUGF(level, message)      SYS_DEBUGF3(SYS_DBG_IIS2MDC, level, message)

#if defined(DEBUG) || defined (SYS_DEBUG)
#define sTaskObj                                  sIIS2MDTaskObj
#endif

#ifndef HSD_USE_DUMMY_DATA
#define HSD_USE_DUMMY_DATA 0
#endif

#if (HSD_USE_DUMMY_DATA == 1)
static uint16_t dummyDataCounter = 0;
#endif

/**
 *  IIS2MDCTask internal structure.
 */
struct _IIS2MDCTask
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
   * Implements the magnetometer ISensor interface.
   */
  ISensor_t mag_sensor_if;

  /**
   * Specifies sensor capabilities.
   */
  const SensorDescriptor_t *mag_sensor_descriptor;

  /**
   * Specifies sensor configuration.
   */
  SensorStatus_t mag_sensor_status;

  /**
   * Specifies the sensor ID for the magnetometer.
   */
  uint8_t mag_id;

  /**
   * Synchronization object used to send command to the task.
   */
  TX_QUEUE in_queue;

  /**
   * Buffer to store the data read from the sensor FIFO.
   */
  uint8_t p_sensor_data_buff[6];

  /**
   * Specifies the FIFO watermark level (it depends from ODR)
   */
  uint16_t samples_per_it;

  /**
   * ::IEventSrc interface implementation for this class.
   */
  IEventSrc *p_mag_event_src;

  /**
   * Software timer used to generate the read command
   */
  TX_TIMER read_timer;

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
typedef struct _IIS2MDCTaskClass
{
  /**
   * IIS2MDCTask class virtual table.
   */
  AManagedTaskEx_vtbl vtbl;

  /**
   * Accelerometer IF virtual table.
   */
  ISensor_vtbl mag_sensor_if_vtbl;

  /**
   * Specifies accelerometer sensor capabilities.
   */
  SensorDescriptor_t mag_class_descriptor;

  /**
   * IIS2MDCTask (PM_STATE, ExecuteStepFunc) map.
   */
  pExecuteStepFunc_t p_pm_state2func_map[];
} IIS2MDCTaskClass_t;

// Private member function declaration
// ***********************************

/**
 * Execute one step of the task control loop while the system is in RUN mode.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return SYS_NO_EROR_CODE if success, a task specific error code otherwise.
 */
static sys_error_code_t IIS2MDCTaskExecuteStepState1(AManagedTask *_this);

/**
 * Execute one step of the task control loop while the system is in SENSORS_ACTIVE mode.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return SYS_NO_EROR_CODE if success, a task specific error code otherwise.
 */
static sys_error_code_t IIS2MDCTaskExecuteStepDatalog(AManagedTask *_this);

/**
 * Initialize the sensor according to the actual parameters.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return SYS_NO_EROR_CODE if success, a task specific error code otherwise.
 */
static sys_error_code_t IIS2MDCTaskSensorInit(IIS2MDCTask *_this);

/**
 * Read the data from the sensor.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return SYS_NO_EROR_CODE if success, a task specific error code otherwise.
 */
static sys_error_code_t IIS2MDCTaskSensorReadData(IIS2MDCTask *_this);

/**
 * Register the sensor with the global DB and initialize the default parameters.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return SYS_NO_ERROR_CODE if success, an error code otherwise
 */
static sys_error_code_t IIS2MDCTaskSensorRegister(IIS2MDCTask *_this);

/**
 * Initialize the default parameters.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return SYS_NO_ERROR_CODE if success, an error code otherwise
 */
static sys_error_code_t IIS2MDCTaskSensorInitTaskParams(IIS2MDCTask *_this);

/**
 * Private implementation of sensor interface methods for IIS2MDC sensor
 */
static sys_error_code_t IIS2MDCTaskSensorSetODR(IIS2MDCTask *_this, SMMessage report);
static sys_error_code_t IIS2MDCTaskSensorSetFS(IIS2MDCTask *_this, SMMessage report);
static sys_error_code_t IIS2MDCTaskSensorEnable(IIS2MDCTask *_this, SMMessage report);
static sys_error_code_t IIS2MDCTaskSensorDisable(IIS2MDCTask *_this, SMMessage report);

/**
 * Check if the sensor is active. The sensor is active if at least one of the sub sensor is active.
 * @param _this [IN] specifies a pointer to a task object.
 * @return TRUE if the sensor is active, FALSE otherwise.
 */
static boolean_t IIS2MDCTaskSensorIsActive(const IIS2MDCTask *_this);

static sys_error_code_t IIS2MDCTaskEnterLowPowerMode(const IIS2MDCTask *_this);

static sys_error_code_t IIS2MDCTaskConfigureIrqPin(const IIS2MDCTask *_this, boolean_t LowPower);

/**
 * Callback function called when the software timer expires.
 *
 * @param timer [IN] specifies the handle of the expired timer.
 */
static void IIS2MDCTaskTimerCallbackFunction(ULONG timer);

/**
 * IRQ callback
 */
void IIS2MDCTask_EXTI_Callback(uint16_t Pin);

// Inline function forward declaration
// ***********************************

/**
 * Private function used to post a report into the front of the task queue.
 * Used to resume the task when the required by the INIT task.
 *
 * @param this [IN] specifies a pointer to the task object.
 * @param pReport [IN] specifies a report to send.
 * @return SYS_NO_EROR_CODE if success, SYS_APP_TASK_REPORT_LOST_ERROR_CODE.
 */
static inline sys_error_code_t IIS2MDCTaskPostReportToFront(IIS2MDCTask *_this, SMMessage *pReport);

/**
 * Private function used to post a report into the back of the task queue.
 * Used to resume the task when the required by the INIT task.
 *
 * @param this [IN] specifies a pointer to the task object.
 * @param pReport [IN] specifies a report to send.
 * @return SYS_NO_EROR_CODE if success, SYS_APP_TASK_REPORT_LOST_ERROR_CODE.
 */
static inline sys_error_code_t IIS2MDCTaskPostReportToBack(IIS2MDCTask *_this, SMMessage *pReport);

#if defined (__GNUC__)
// Inline function defined inline in the header file IIS2MDCTask.h must be declared here as extern function.
#endif

/* Objects instance */
/********************/

/**
 * The only instance of the task object.
 */
static IIS2MDCTask sTaskObj;

/**
 * The class object.
 */
static const IIS2MDCTaskClass_t sTheClass =
{
    /* Class virtual table */
    {
        IIS2MDCTask_vtblHardwareInit,
        IIS2MDCTask_vtblOnCreateTask,
        IIS2MDCTask_vtblDoEnterPowerMode,
        IIS2MDCTask_vtblHandleError,
        IIS2MDCTask_vtblOnEnterTaskControlLoop,
        IIS2MDCTask_vtblForceExecuteStep,
        IIS2MDCTask_vtblOnEnterPowerMode },

    /* class::mag_sensor_if_vtbl virtual table */
    {
        IIS2MDCTask_vtblMagGetId,
        IIS2MDCTask_vtblMagGetEventSourceIF,
        IIS2MDCTask_vtblMagGetODR,
        IIS2MDCTask_vtblMagGetFS,
        IIS2MDCTask_vtblMagGetSensitivity,
        IIS2MDCTask_vtblMagGetDataType,
        IIS2MDCTask_vtblMagGetDimensions,
        IIS2MDCTask_vtblSensorSetODR,
        IIS2MDCTask_vtblSensorSetFS,
        IIS2MDCTask_vtblSensorEnable,
        IIS2MDCTask_vtblSensorDisable,
        IIS2MDCTask_vtblSensorIsEnabled,
        IIS2MDCTask_vtblSensorGetDescription,
        IIS2MDCTask_vtblSensorGetStatus },

    /* MAGNETOMETER DESCRIPTOR */
    {
        "iis2mdc",
        COM_TYPE_MAG,
        {
            10.0,
            20.0,
            50.0,
            100.0,
            COM_END_OF_LIST_FLOAT, },
        {
            50.0f,
            COM_END_OF_LIST_FLOAT, },
        {
            "x",
            "y",
            "z", },
        "gauss",
        {
            0,
            1000, } },

    /* class (PM_STATE, ExecuteStepFunc) map */
    {
        IIS2MDCTaskExecuteStepState1,
        NULL,
        IIS2MDCTaskExecuteStepDatalog, } };

// Public API definition
// *********************

ISourceObservable* IIS2MDCTaskGetMagSensorIF(IIS2MDCTask *_this)
{
  return (ISourceObservable*) &(_this->mag_sensor_if);
}

AManagedTaskEx* IIS2MDCTaskAlloc(const void *pIRQConfig, const void *pCSConfig, EBusType eBusType)
{
  // In this application there is only one Keyboard task,
  // so this allocator implement the singleton design pattern.

  // Initialize the super class
  AMTInitEx(&sTaskObj.super);

  sTaskObj.super.vptr = &sTheClass.vtbl;
  sTaskObj.mag_sensor_if.vptr = &sTheClass.mag_sensor_if_vtbl;
  sTaskObj.mag_sensor_descriptor = &sTheClass.mag_class_descriptor;

  sTaskObj.pIRQConfig = (MX_GPIOParams_t*)pIRQConfig;
  sTaskObj.pCSConfig = (MX_GPIOParams_t*)pCSConfig;

  sTaskObj.eBusType = eBusType;

  return (AManagedTaskEx*) &sTaskObj;
}

ABusIF* IIS2MDCTaskGetSensorIF(IIS2MDCTask *_this)
{
  assert_param(_this);

  return _this->p_sensor_bus_if;
}

IEventSrc* IIS2MDCTaskGetMagEventSrcIF(IIS2MDCTask *_this)
{
  assert_param(_this);

  return (IEventSrc*) _this->p_mag_event_src;
}

// AManagedTask virtual functions definition
// *******************************************

sys_error_code_t IIS2MDCTask_vtblHardwareInit(AManagedTask *_this, void *pParams)
{
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  IIS2MDCTask *p_obj = (IIS2MDCTask*) _this;

  /* Configure CS Pin */
  if(p_obj->pCSConfig != NULL)
  {
    p_obj->pCSConfig->p_mx_init_f();
  }

  return res;
}

sys_error_code_t IIS2MDCTask_vtblOnCreateTask(
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
  IIS2MDCTask *p_obj = (IIS2MDCTask*) _this;

  // Create task specific sw resources.

  uint16_t nItemSize = IIS2MDC_TASK_CFG_IN_QUEUE_ITEM_SIZE;
  VOID *pvQueueItemsBuff = SysAlloc(IIS2MDC_TASK_CFG_IN_QUEUE_LENGTH * nItemSize);
  if(pvQueueItemsBuff == NULL)
  {
    res = SYS_TASK_HEAP_OUT_OF_MEMORY_ERROR_CODE;
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(res);
    return res;
  }

  if(TX_SUCCESS != tx_queue_create(&p_obj->in_queue, "IIS2MDC_Q", nItemSize / 4, pvQueueItemsBuff, IIS2MDC_TASK_CFG_IN_QUEUE_LENGTH * nItemSize))
  {
    res = SYS_TASK_HEAP_OUT_OF_MEMORY_ERROR_CODE;
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(res);
    return res;
  }

  /* create the software timer*/
  if(TX_SUCCESS != tx_timer_create(
		  &p_obj->read_timer,
		  "IIS2MDC_T",
		  IIS2MDCTaskTimerCallbackFunction,
		  (ULONG)TX_NULL,
		  AMT_MS_TO_TICKS(IIS2MDC_TASK_CFG_TIMER_PERIOD_MS),
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
      p_obj->p_sensor_bus_if = SPIBusIFAlloc(IIS2MDC_ID, p_obj->pCSConfig->port, p_obj->pCSConfig->pin, 0);
      if (p_obj->p_sensor_bus_if == NULL)
      {
        res = SYS_TASK_HEAP_OUT_OF_MEMORY_ERROR_CODE;
        SYS_SET_SERVICE_LEVEL_ERROR_CODE(res);
      }
#endif
      break;
    case I2C_IF:
#if (SM_I2C_BUS_IF == 1)
      p_obj->p_sensor_bus_if = I2CBusIFAlloc(IIS2MDC_ID, IIS2MDC_I2C_ADD, 0);
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
  // Initialize the EventSrc interface.
  p_obj->p_mag_event_src = SensorEventSrcAlloc();
  if(p_obj->p_mag_event_src == NULL)
  {
    res = SYS_TASK_HEAP_OUT_OF_MEMORY_ERROR_CODE;
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(res);
    return res;
  }
  IEventSrcInit(p_obj->p_mag_event_src);

  memset(p_obj->p_sensor_data_buff, 0, sizeof(p_obj->p_sensor_data_buff));
  p_obj->mag_id = 0;
  p_obj->timestamp_tick = 0;
  p_obj->old_timestamp_tick = 0;
  p_obj->timestamp = 0;
  p_obj->samples_per_it = 0;
  _this->m_pfPMState2FuncMap = sTheClass.p_pm_state2func_map;

  *pTaskCode = AMTExRun;
  *pName = "IIS2MDC";
  *pvStackStart = NULL; // allocate the task stack in the system memory pool.
  *pStackDepth = IIS2MDC_TASK_CFG_STACK_DEPTH;
  *pParams = (ULONG) _this;
  *pPriority = IIS2MDC_TASK_CFG_PRIORITY;
  *pPreemptThreshold = IIS2MDC_TASK_CFG_PRIORITY;
  *pTimeSlice = TX_NO_TIME_SLICE;
  *pAutoStart = TX_AUTO_START;

  res = IIS2MDCTaskSensorInitTaskParams(p_obj);
  if(SYS_IS_ERROR_CODE(res))
  {
    res = SYS_TASK_HEAP_OUT_OF_MEMORY_ERROR_CODE;
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(res);
    return res;
  }

  res = IIS2MDCTaskSensorRegister(p_obj);
  if(SYS_IS_ERROR_CODE(res))
  {
    SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("IIS2MDC: unable to register with DB\r\n"));
    sys_error_handler();
  }

  return res;
}

sys_error_code_t IIS2MDCTask_vtblDoEnterPowerMode(AManagedTask *_this, const EPowerMode ActivePowerMode, const EPowerMode NewPowerMode)
{
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  IIS2MDCTask *p_obj = (IIS2MDCTask*) _this;
  stmdev_ctx_t *p_sensor_drv = (stmdev_ctx_t*) &p_obj->p_sensor_bus_if->m_xConnector;

  if(NewPowerMode == E_POWER_MODE_SENSORS_ACTIVE)
  {
    if(IIS2MDCTaskSensorIsActive(p_obj))
    {
      SMMessage report =
      {
          .sensorMessage.messageId = SM_MESSAGE_ID_SENSOR_CMD,
          .sensorMessage.nCmdID = SENSOR_CMD_ID_INIT
      };

      if(tx_queue_send(&p_obj->in_queue, &report, AMT_MS_TO_TICKS(100)) != TX_SUCCESS)
      {
        res = SYS_APP_TASK_REPORT_LOST_ERROR_CODE;
        SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_APP_TASK_REPORT_LOST_ERROR_CODE);
      }

      // reset the variables for the time stamp computation.
      p_obj->timestamp_tick = 0;
      p_obj->old_timestamp_tick = 0;
      p_obj->timestamp = 0;
    }

    SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("IIS2MDC: -> SENSORS_ACTIVE\r\n"));
  }
  else if(NewPowerMode == E_POWER_MODE_STATE1)
  {
    if(ActivePowerMode == E_POWER_MODE_SENSORS_ACTIVE)
    {
      /* SM_SENSOR_STATE_SUSPENDED */
      iis2mdc_power_mode_set(p_sensor_drv, PROPERTY_DISABLE); //TODO: SO: Disable the low power
      tx_queue_flush(&p_obj->in_queue);
      if(p_obj->pIRQConfig == NULL)
      {
        tx_timer_deactivate(&p_obj->read_timer);
      }
    }

    SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("IIS2MDC: -> RUN\r\n"));
  }
  else if(NewPowerMode == E_POWER_MODE_SLEEP_1)
  {
    // the MCU is going in stop so I put the sensor in low power
    // from the INIT task
    res = IIS2MDCTaskEnterLowPowerMode(p_obj);
    if(SYS_IS_ERROR_CODE(res))
    {
      SYS_DEBUGF(SYS_DBG_LEVEL_WARNING, ("IIS2MDC - Enter Low Power Mode failed.\r\n"));
    }
    if(p_obj->pIRQConfig != NULL)
    {
      IIS2MDCTaskConfigureIrqPin(p_obj, TRUE);
    }
    // notify the bus
    if(p_obj->p_sensor_bus_if->m_pfBusCtrl != NULL)
    {
      p_obj->p_sensor_bus_if->m_pfBusCtrl(p_obj->p_sensor_bus_if, E_BUS_CTRL_DEV_NOTIFY_POWER_MODE, 0);
    }
    if(p_obj->pIRQConfig == NULL)
    {
      tx_timer_deactivate(&p_obj->read_timer);
    }

    SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("IIS2MDC: -> SLEEP_1\r\n"));
  }

  return res;
}

sys_error_code_t IIS2MDCTask_vtblHandleError(AManagedTask *_this, SysEvent xError)
{
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  //IIS2MDCTask *p_obj = (IIS2MDCTask*)_this;

  return res;
}

sys_error_code_t IIS2MDCTask_vtblOnEnterTaskControlLoop(AManagedTask *_this)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("IIS2MDC: start.\r\n"));

  // At this point all system has been initialized.
  // Execute task specific delayed one time initialization.

  return res;
}

sys_error_code_t IIS2MDCTask_vtblForceExecuteStep(AManagedTaskEx *_this, EPowerMode ActivePowerMode)
{
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  IIS2MDCTask *p_obj = (IIS2MDCTask*) _this;

  SMMessage report =
  {
      .internalMessageFE.messageId = SM_MESSAGE_ID_FORCE_STEP,
      .internalMessageFE.nData = 0
  };

  if((ActivePowerMode == E_POWER_MODE_STATE1) || (ActivePowerMode == E_POWER_MODE_SENSORS_ACTIVE))
  {
    if(AMTExIsTaskInactive(_this))
    {
      res = IIS2MDCTaskPostReportToFront(p_obj, (SMMessage*) &report);
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

sys_error_code_t IIS2MDCTask_vtblOnEnterPowerMode(AManagedTaskEx *_this, const EPowerMode ActivePowerMode, const EPowerMode NewPowerMode)
{
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  //  IIS2MDCTask *p_obj = (IIS2MDCTask*)_this;

  return res;
}

// ISensor virtual functions definition
// *******************************************

uint8_t IIS2MDCTask_vtblMagGetId(ISourceObservable *_this)
{
  assert_param(_this);
  IIS2MDCTask *p_if_owner = (IIS2MDCTask*) ((uint32_t) _this - offsetof(IIS2MDCTask, mag_sensor_if));
  uint8_t res = p_if_owner->mag_id;

  return res;
}

IEventSrc* IIS2MDCTask_vtblMagGetEventSourceIF(ISourceObservable *_this)
{
  assert_param(_this);
  IIS2MDCTask *p_if_owner = (IIS2MDCTask*) ((uint32_t) _this - offsetof(IIS2MDCTask, mag_sensor_if));

  return p_if_owner->p_mag_event_src;
}

sys_error_code_t IIS2MDCTask_vtblMagGetODR(ISourceObservable *_this, float *p_measured, float *p_nominal)
{
  assert_param(_this != NULL);
  /*get the object implementing the ISourceObservable IF */
  IIS2MDCTask *p_if_owner = (IIS2MDCTask*) ((uint32_t) _this - offsetof(IIS2MDCTask, mag_sensor_if));
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  /* parameter validation */
  if((p_measured) == NULL || (p_nominal == NULL))
  {
    res = SYS_INVALID_PARAMETER_ERROR_CODE;
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_INVALID_PARAMETER_ERROR_CODE);
  }
  else
  {
    *p_measured = p_if_owner->mag_sensor_status.MeasuredODR;
    *p_nominal = p_if_owner->mag_sensor_status.ODR;
  }

  return res;
}

float IIS2MDCTask_vtblMagGetFS(ISourceObservable *_this)
{
  assert_param(_this);
  IIS2MDCTask *p_if_owner = (IIS2MDCTask*) ((uint32_t) _this - offsetof(IIS2MDCTask, mag_sensor_if));
  float res = p_if_owner->mag_sensor_status.FS;

  return res;
}

float IIS2MDCTask_vtblMagGetSensitivity(ISourceObservable *_this)
{
  assert_param(_this);
  IIS2MDCTask *p_if_owner = (IIS2MDCTask*) ((uint32_t) _this - offsetof(IIS2MDCTask, mag_sensor_if));
  float res = p_if_owner->mag_sensor_status.Sensitivity;

  return res;
}

uint8_t IIS2MDCTask_vtblMagGetDataType(ISourceObservable *_this)
{
  assert_param(_this != NULL);
  IIS2MDCTask *p_if_owner = (IIS2MDCTask*)((uint32_t)_this - offsetof(IIS2MDCTask, mag_sensor_if));
  uint8_t res = p_if_owner->mag_sensor_status.DataType;

  return res;
}

uint16_t IIS2MDCTask_vtblMagGetDimensions(ISourceObservable *_this)
{
  assert_param(_this != NULL);
  IIS2MDCTask *p_if_owner = (IIS2MDCTask*)((uint32_t)_this - offsetof(IIS2MDCTask, mag_sensor_if));
  uint16_t res = p_if_owner->mag_sensor_status.Dimensions;

  return res;
}

sys_error_code_t IIS2MDCTask_vtblSensorSetODR(ISensor_t *_this, float ODR)
{
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  IIS2MDCTask *p_if_owner = (IIS2MDCTask*) ((uint32_t) _this - offsetof(IIS2MDCTask, mag_sensor_if));

  EPowerMode log_status = AMTGetTaskPowerMode((AManagedTask*) p_if_owner);
  uint8_t sensor_id = ISourceGetId((ISourceObservable*) _this);

  if((log_status == E_POWER_MODE_SENSORS_ACTIVE) && ISensorIsEnabled(_this))
  {
    res = SYS_INVALID_FUNC_CALL_ERROR_CODE;
  }
  else
  {
    /* Set a new command message in the queue */
    SMMessage report =
    {
        .sensorMessage.messageId = SM_MESSAGE_ID_SENSOR_CMD,
        .sensorMessage.nCmdID = SENSOR_CMD_ID_SET_ODR,
        .sensorMessage.nSensorId = sensor_id,
        .sensorMessage.nParam = (uint32_t) ODR
    };
    res = IIS2MDCTaskPostReportToBack(p_if_owner, (SMMessage*) &report);
  }

  return res;
}

sys_error_code_t IIS2MDCTask_vtblSensorSetFS(ISensor_t *_this, float FS)
{
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  IIS2MDCTask *p_if_owner = (IIS2MDCTask*) ((uint32_t) _this - offsetof(IIS2MDCTask, mag_sensor_if));

  EPowerMode log_status = AMTGetTaskPowerMode((AManagedTask*) p_if_owner);
  uint8_t sensor_id = ISourceGetId((ISourceObservable*) _this);

  if((log_status == E_POWER_MODE_SENSORS_ACTIVE) && ISensorIsEnabled(_this))
  {
    res = SYS_INVALID_FUNC_CALL_ERROR_CODE;
  }
  else
  {
    /* Set a new command message in the queue */
    SMMessage report =
    {
        .sensorMessage.messageId = SM_MESSAGE_ID_SENSOR_CMD,
        .sensorMessage.nCmdID = SENSOR_CMD_ID_SET_FS,
        .sensorMessage.nSensorId = sensor_id,
        .sensorMessage.nParam = (uint32_t) FS
    };
    res = IIS2MDCTaskPostReportToBack(p_if_owner, (SMMessage*) &report);
  }

  return res;

}

sys_error_code_t IIS2MDCTask_vtblSensorEnable(ISensor_t *_this)
{
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  IIS2MDCTask *p_if_owner = (IIS2MDCTask*) ((uint32_t) _this - offsetof(IIS2MDCTask, mag_sensor_if));

  EPowerMode log_status = AMTGetTaskPowerMode((AManagedTask*) p_if_owner);
  uint8_t sensor_id = ISourceGetId((ISourceObservable*) _this);

  if((log_status == E_POWER_MODE_SENSORS_ACTIVE) && ISensorIsEnabled(_this))
  {
    res = SYS_INVALID_FUNC_CALL_ERROR_CODE;
  }
  else
  {
    /* Set a new command message in the queue */
    SMMessage report =
    {
        .sensorMessage.messageId = SM_MESSAGE_ID_SENSOR_CMD,
        .sensorMessage.nCmdID = SENSOR_CMD_ID_ENABLE,
        .sensorMessage.nSensorId = sensor_id
    };
    res = IIS2MDCTaskPostReportToBack(p_if_owner, (SMMessage*) &report);
  }

  return res;
}

sys_error_code_t IIS2MDCTask_vtblSensorDisable(ISensor_t *_this)
{
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  IIS2MDCTask *p_if_owner = (IIS2MDCTask*) ((uint32_t) _this - offsetof(IIS2MDCTask, mag_sensor_if));

  EPowerMode log_status = AMTGetTaskPowerMode((AManagedTask*) p_if_owner);
  uint8_t sensor_id = ISourceGetId((ISourceObservable*) _this);

  if((log_status == E_POWER_MODE_SENSORS_ACTIVE) && ISensorIsEnabled(_this))
  {
    res = SYS_INVALID_FUNC_CALL_ERROR_CODE;
  }
  else
  {
    /* Set a new command message in the queue */
    SMMessage report =
    {
        .sensorMessage.messageId = SM_MESSAGE_ID_SENSOR_CMD,
        .sensorMessage.nCmdID = SENSOR_CMD_ID_DISABLE,
        .sensorMessage.nSensorId = sensor_id
    };
    res = IIS2MDCTaskPostReportToBack(p_if_owner, (SMMessage*) &report);
  }

  return res;
}

boolean_t IIS2MDCTask_vtblSensorIsEnabled(ISensor_t *_this)
{
  assert_param(_this != NULL);
  boolean_t res = FALSE;
  IIS2MDCTask *p_if_owner = (IIS2MDCTask*) ((uint32_t) _this - offsetof(IIS2MDCTask, mag_sensor_if));

  if(ISourceGetId((ISourceObservable*) _this) == p_if_owner->mag_id)
    res = p_if_owner->mag_sensor_status.IsActive;

  return res;
}

SensorDescriptor_t IIS2MDCTask_vtblSensorGetDescription(ISensor_t *_this)
{
  assert_param(_this != NULL);
  IIS2MDCTask *p_if_owner = (IIS2MDCTask*) ((uint32_t) _this - offsetof(IIS2MDCTask, mag_sensor_if));

  return *p_if_owner->mag_sensor_descriptor;
}

SensorStatus_t IIS2MDCTask_vtblSensorGetStatus(ISensor_t *_this)
{
  assert_param(_this != NULL);
  IIS2MDCTask *p_if_owner = (IIS2MDCTask*) ((uint32_t) _this - offsetof(IIS2MDCTask, mag_sensor_if));

  return p_if_owner->mag_sensor_status;
}

// Private function definition
// ***************************

static sys_error_code_t IIS2MDCTaskExecuteStepState1(AManagedTask *_this)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  IIS2MDCTask *p_obj = (IIS2MDCTask*) _this;
  SMMessage report =
  {
      0
  };

  AMTExSetInactiveState((AManagedTaskEx*) _this, TRUE);
  if(TX_SUCCESS == tx_queue_receive(&p_obj->in_queue, &report, TX_WAIT_FOREVER))
  {
    AMTExSetInactiveState((AManagedTaskEx*) _this, FALSE);

    switch(report.messageID)
    {
      case SM_MESSAGE_ID_FORCE_STEP:
        {
          // do nothing. I need only to resume.
          __NOP();
          break;
        }
      case SM_MESSAGE_ID_SENSOR_CMD:
        {
          switch(report.sensorMessage.nCmdID)
          {
            case SENSOR_CMD_ID_SET_ODR:
              res = IIS2MDCTaskSensorSetODR(p_obj, report);
              break;
            case SENSOR_CMD_ID_SET_FS:
              res = IIS2MDCTaskSensorSetFS(p_obj, report);
              break;
            case SENSOR_CMD_ID_ENABLE:
              res = IIS2MDCTaskSensorEnable(p_obj, report);
              break;
            case SENSOR_CMD_ID_DISABLE:
              res = IIS2MDCTaskSensorDisable(p_obj, report);
              break;
            default:
              // unwanted report
              res = SYS_APP_TASK_UNKNOWN_REPORT_ERROR_CODE;
              SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_APP_TASK_UNKNOWN_REPORT_ERROR_CODE);

              SYS_DEBUGF(SYS_DBG_LEVEL_WARNING, ("IIS2MDC: unexpected report in Run: %i\r\n", report.messageID));
              break;
          }
          break;
        }
      default:
        {
          // unwanted report
          res = SYS_APP_TASK_UNKNOWN_REPORT_ERROR_CODE;
          SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_APP_TASK_UNKNOWN_REPORT_ERROR_CODE);

          SYS_DEBUGF(SYS_DBG_LEVEL_WARNING, ("IIS2MDC: unexpected report in Run: %i\r\n", report.messageID));
          break;
        }
    }
  }

  return res;
}

static sys_error_code_t IIS2MDCTaskExecuteStepDatalog(AManagedTask *_this)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  IIS2MDCTask *p_obj = (IIS2MDCTask*) _this;
  SMMessage report =
  {
      0
  };

  AMTExSetInactiveState((AManagedTaskEx*) _this, TRUE);
  if(TX_SUCCESS == tx_queue_receive(&p_obj->in_queue, &report, TX_WAIT_FOREVER))
  {
    AMTExSetInactiveState((AManagedTaskEx*) _this, FALSE);

    switch(report.messageID)
    {
      case SM_MESSAGE_ID_FORCE_STEP:
        {
          // do nothing. I need only to resume.
          __NOP();
          break;
        }
      case SM_MESSAGE_ID_DATA_READY:
        {
          //       SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("IIS2MDC: new data.\r\n"));
          if(p_obj->pIRQConfig == NULL)
          {
            if(TX_SUCCESS != tx_timer_change(&p_obj->read_timer, AMT_MS_TO_TICKS(IIS2MDC_TASK_CFG_TIMER_PERIOD_MS), AMT_MS_TO_TICKS(IIS2MDC_TASK_CFG_TIMER_PERIOD_MS)))
            {
              return SYS_UNDEFINED_ERROR_CODE;
            }
          }

          res = IIS2MDCTaskSensorReadData(p_obj);
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

            AI_SP_Stream_t stream =
            {
                .packet.payload = p_obj->p_sensor_data_buff,
                .packet.payload_fmt = AI_SP_FMT_INT16_RESET(),
                .mode = AI_SP_MODE_COLUMN //TODO: STF - this means that data are interleaved?!?
                // bonus question: what is AI_LOGGING_SHAPES_DEPTH ??
                // (can I represent anomogeneous matrix [4*4] with this data format ?
                };
            ai_logging_create_shape_2d(&stream.packet.shape, 3, p_obj->samples_per_it);
            stream.packet.payload_size = 2 * stream.packet.shape.shapes[0] * stream.packet.shape.shapes[1];

            SensorEvent evt;
            SensorEventInit((IEvent*) &evt, p_obj->p_mag_event_src, (ai_logging_packet_t*) &stream, timestamp, p_obj->mag_id);
            IEventSrcSendEvent(p_obj->p_mag_event_src, (IEvent*) &evt, NULL);

            /* update measuredODR */
            p_obj->mag_sensor_status.MeasuredODR = p_obj->samples_per_it / delta_timestamp;

//          SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("IIS2MDC: ts = %f\r\n", (float)timestamp));
            if(p_obj->pIRQConfig == NULL)
            {
              if (TX_SUCCESS != tx_timer_activate(&p_obj->read_timer))
              {
                res = SYS_UNDEFINED_ERROR_CODE;
              }
            }
          }
          break;
        }
      case SM_MESSAGE_ID_SENSOR_CMD:
        {
          switch(report.sensorMessage.nCmdID)
          {
            case SENSOR_CMD_ID_INIT:
              res = IIS2MDCTaskSensorInit(p_obj);
              if(!SYS_IS_ERROR_CODE(res))
              {
                if(p_obj->pIRQConfig == NULL)
                {
                  if (TX_SUCCESS != tx_timer_activate(&p_obj->read_timer))
                  {
                    res = SYS_UNDEFINED_ERROR_CODE;
                  }
        	    }
                else
        	    {
        		  IIS2MDCTaskConfigureIrqPin(p_obj, FALSE);
        	    }
              }
              break;
            case SENSOR_CMD_ID_SET_ODR:
              res = IIS2MDCTaskSensorSetODR(p_obj, report);
              break;
            case SENSOR_CMD_ID_SET_FS:
              res = IIS2MDCTaskSensorSetFS(p_obj, report);
              break;
            case SENSOR_CMD_ID_ENABLE:
              res = IIS2MDCTaskSensorEnable(p_obj, report);
              break;
            case SENSOR_CMD_ID_DISABLE:
              res = IIS2MDCTaskSensorDisable(p_obj, report);
              break;
            default:
              // unwanted report
              res = SYS_APP_TASK_UNKNOWN_REPORT_ERROR_CODE;
              SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_APP_TASK_UNKNOWN_REPORT_ERROR_CODE);

              SYS_DEBUGF(SYS_DBG_LEVEL_WARNING, ("IIS2MDC: unexpected report in Datalog: %i\r\n", report.messageID));
              break;
          }
          break;
        }
      default:
        // unwanted report
        res = SYS_APP_TASK_UNKNOWN_REPORT_ERROR_CODE;
        SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_APP_TASK_UNKNOWN_REPORT_ERROR_CODE);

        SYS_DEBUGF(SYS_DBG_LEVEL_WARNING, ("IIS2MDC: unexpected report in Datalog: %i\r\n", report.messageID));
        break;
    }
  }

  return res;
}

static inline sys_error_code_t IIS2MDCTaskPostReportToFront(IIS2MDCTask *_this, SMMessage *pReport)
{
  assert_param(_this);
  assert_param(pReport);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  if(SYS_IS_CALLED_FROM_ISR())
  {
    if(TX_SUCCESS != tx_queue_front_send(&_this->in_queue, pReport, TX_NO_WAIT))
    {
      res = SYS_APP_TASK_REPORT_LOST_ERROR_CODE;
      // this function is private and the caller will ignore this return code.
    }
  }
  else
  {
    if(TX_SUCCESS != tx_queue_front_send(&_this->in_queue, pReport, AMT_MS_TO_TICKS(100)))
    {
      res = SYS_APP_TASK_REPORT_LOST_ERROR_CODE;
      // this function is private and the caller will ignore this return code.
    }
  }

  return res;
}

static inline sys_error_code_t IIS2MDCTaskPostReportToBack(IIS2MDCTask *_this, SMMessage *pReport)
{
  assert_param(_this);
  assert_param(pReport);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  if(SYS_IS_CALLED_FROM_ISR())
  {
    if(TX_SUCCESS != tx_queue_send(&_this->in_queue, pReport, TX_NO_WAIT))
    {
      res = SYS_APP_TASK_REPORT_LOST_ERROR_CODE;
      // this function is private and the caller will ignore this return code.
    }
  }
  else
  {
    if(TX_SUCCESS != tx_queue_send(&_this->in_queue, pReport, AMT_MS_TO_TICKS(100)))
    {
      res = SYS_APP_TASK_REPORT_LOST_ERROR_CODE;
      // this function is private and the caller will ignore this return code.
    }
  }

  return res;
}

static sys_error_code_t IIS2MDCTaskSensorInit(IIS2MDCTask *_this)
{
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  stmdev_ctx_t *p_sensor_drv = (stmdev_ctx_t*) &_this->p_sensor_bus_if->m_xConnector;

  uint8_t reg0 = 0;
  int32_t ret_val = 0;

  //TODO: SO Check if the following part is OK

  iis2mdc_reset_set(p_sensor_drv, 1);
  iis2mdc_block_data_update_set(p_sensor_drv, 1);
  iis2mdc_self_test_set(p_sensor_drv, 0);

  ret_val = iis2mdc_device_id_get(p_sensor_drv, (uint8_t*) &reg0);
  if(!ret_val)
  {
    ABusIFSetWhoAmI(_this->p_sensor_bus_if, reg0);
  }
  SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("IIS2MDC: sensor - I am 0x%x.\r\n", reg0));

  /* Enable Interrupt */
  if(_this->pIRQConfig != NULL)
  {
    iis2mdc_drdy_on_pin_set(p_sensor_drv, 1);
  }
  else
  {
    iis2mdc_drdy_on_pin_set(p_sensor_drv, 0);
  }

  _this->samples_per_it = 1;

  /*Read first data to start it generation */
  int16_t buff[] =
  {
      0,
      0,
      0
  };
  iis2mdc_mag_user_offset_set(p_sensor_drv, buff);
  iis2mdc_operating_mode_set(p_sensor_drv, IIS2MDC_CONTINUOUS_MODE);

  if(_this->mag_sensor_status.ODR < 11.0f)
  {
    iis2mdc_data_rate_set(p_sensor_drv, IIS2MDC_ODR_10Hz);
  }
  else if(_this->mag_sensor_status.ODR < 21.0f)
  {
    iis2mdc_data_rate_set(p_sensor_drv, IIS2MDC_ODR_20Hz);
  }
  else if(_this->mag_sensor_status.ODR < 51.0f)
  {
    iis2mdc_data_rate_set(p_sensor_drv, IIS2MDC_ODR_50Hz);
  }
  else if(_this->mag_sensor_status.ODR < 101.0f)
  {
    iis2mdc_data_rate_set(p_sensor_drv, IIS2MDC_ODR_100Hz);
  }

  iis2mdc_magnetic_raw_get(p_sensor_drv, (int16_t*) &_this->p_sensor_data_buff);

  return res;
}

static sys_error_code_t IIS2MDCTaskSensorReadData(IIS2MDCTask *_this)
{
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  stmdev_ctx_t *p_sensor_drv = (stmdev_ctx_t*) &_this->p_sensor_bus_if->m_xConnector;

  iis2mdc_magnetic_raw_get(p_sensor_drv, (int16_t*) &_this->p_sensor_data_buff);

#if (HSD_USE_DUMMY_DATA == 1)
    uint16_t i;
    int16_t * p16 = (int16_t *)_this->p_sensor_data_buff;

    for (i = 0; i < _this->samples_per_it; i++)
    {
      *p16++ = dummyDataCounter++;
      *p16++ = dummyDataCounter++;
      *p16++ = dummyDataCounter++;
    }
#endif
  //TODO: Other things to add?? --> Add a callback for the ready data?
  // IIS2MDC_Data_Ready(0, (uint8_t *)iis2mdc_mem, 6, TimeStamp_iis2mdc);

  return res;
}

static sys_error_code_t IIS2MDCTaskSensorRegister(IIS2MDCTask *_this)
{
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  ISensor_t *mag_if = (ISensor_t*) IIS2MDCTaskGetMagSensorIF(_this);
  _this->mag_id = SMAddSensor(mag_if);

  return res;
}

static sys_error_code_t IIS2MDCTaskSensorInitTaskParams(IIS2MDCTask *_this)
{
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  /* ACCELEROMETER SENSOR STATUS */
  _this->mag_sensor_status.DataType = DATA_TYPE_INT16;
  _this->mag_sensor_status.Dimensions = 3;
  _this->mag_sensor_status.IsActive = TRUE;
  _this->mag_sensor_status.FS = 50.0f;
  _this->mag_sensor_status.Sensitivity = 0.0015f;
  _this->mag_sensor_status.ODR = 100.0f;
  _this->mag_sensor_status.MeasuredODR = 0.0f;
  _this->mag_sensor_status.InitialOffset = 0.0f;
  _this->mag_sensor_status.DataPacketSize = 600;
#if (HSD_USE_DUMMY_DATA == 1)
  _this->mag_sensor_status.SamplesPerTimestamp = 0;
#else
  _this->mag_sensor_status.SamplesPerTimestamp = 1000;
#endif

  return res;
}

static sys_error_code_t IIS2MDCTaskSensorSetODR(IIS2MDCTask *_this, SMMessage report)
{
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  stmdev_ctx_t *p_sensor_drv = (stmdev_ctx_t*) &_this->p_sensor_bus_if->m_xConnector;
  float ODR = (float) report.sensorMessage.nParam;
  uint8_t id = report.sensorMessage.nSensorId;

  if(id == _this->mag_id)
  {
    if(ODR < 11.0f)
      iis2mdc_data_rate_set(p_sensor_drv, IIS2MDC_ODR_10Hz);
    else if(ODR < 21.0f)
      iis2mdc_data_rate_set(p_sensor_drv, IIS2MDC_ODR_20Hz);
    else if(ODR < 51.0f)
      iis2mdc_data_rate_set(p_sensor_drv, IIS2MDC_ODR_50Hz);
    else if(ODR < 101.0f)
      iis2mdc_data_rate_set(p_sensor_drv, IIS2MDC_ODR_100Hz);
    else
      res = SYS_INVALID_PARAMETER_ERROR_CODE;

    if(!SYS_IS_ERROR_CODE(res))
    {
      _this->mag_sensor_status.ODR = ODR;
    }
  }
  else
  {
    res = SYS_INVALID_PARAMETER_ERROR_CODE;
  }

  return res;
}

static sys_error_code_t IIS2MDCTaskSensorSetFS(IIS2MDCTask *_this, SMMessage report)
{
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  float FS = (float) report.sensorMessage.nParam;
  uint8_t id = report.sensorMessage.nSensorId;

  if(id == _this->mag_id)
  {
    if(FS != 50.0f)
      res = SYS_INVALID_PARAMETER_ERROR_CODE;

    if(!SYS_IS_ERROR_CODE(res))
    {
      _this->mag_sensor_status.FS = FS;
    }
  }
  else
  {
    res = SYS_INVALID_PARAMETER_ERROR_CODE;
  }

  return res;
}

static sys_error_code_t IIS2MDCTaskSensorEnable(IIS2MDCTask *_this, SMMessage report)
{
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  uint8_t id = report.sensorMessage.nSensorId;

  if(id == _this->mag_id)
    _this->mag_sensor_status.IsActive = TRUE;
  else
    res = SYS_INVALID_PARAMETER_ERROR_CODE;

  return res;
}

static sys_error_code_t IIS2MDCTaskSensorDisable(IIS2MDCTask *_this, SMMessage report)
{
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  uint8_t id = report.sensorMessage.nSensorId;

  if(id == _this->mag_id)
    _this->mag_sensor_status.IsActive = FALSE;
  else
    res = SYS_INVALID_PARAMETER_ERROR_CODE;

  return res;
}

static boolean_t IIS2MDCTaskSensorIsActive(const IIS2MDCTask *_this)
{
  assert_param(_this);
  return _this->mag_sensor_status.IsActive;
}

static sys_error_code_t IIS2MDCTaskEnterLowPowerMode(const IIS2MDCTask *_this)
{
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  stmdev_ctx_t *p_sensor_drv = (stmdev_ctx_t*) &_this->p_sensor_bus_if->m_xConnector;

  if(iis2mdc_power_mode_set(p_sensor_drv, IIS2MDC_LOW_POWER))
  {
    res = SYS_SENSOR_TASK_OP_ERROR_CODE;
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_SENSOR_TASK_OP_ERROR_CODE);
  }

  return res;
}

static sys_error_code_t IIS2MDCTaskConfigureIrqPin(const IIS2MDCTask *_this, boolean_t LowPower)
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

static void IIS2MDCTaskTimerCallbackFunction(ULONG timer)
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

// CubeMX integration
// ******************

void IIS2MDCTask_EXTI_Callback(uint16_t Pin)
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
//  }
  sTaskObj.timestamp_tick = SMUtilGetTimeStamp();
}

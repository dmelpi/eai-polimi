/**
 ******************************************************************************
 * @file    HTS221Task.c
 * @author  SRA - MCD
 * @version 1.0.0
 * @date    6-Sep-2021
 * @brief   This file provides a set of functions to handle hts221 sensor
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
#include "HTS221Task.h"
#include "HTS221Task_vtbl.h"
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

/* Private includes ----------------------------------------------------------*/

#ifndef HTS221_TASK_CFG_STACK_DEPTH
#define HTS221_TASK_CFG_STACK_DEPTH              120
#endif

#ifndef HTS221_TASK_CFG_PRIORITY
#define HTS221_TASK_CFG_PRIORITY                 (tskIDLE_PRIORITY)
#endif

#ifndef HTS221_TASK_CFG_IN_QUEUE_LENGTH
#define HTS221_TASK_CFG_IN_QUEUE_LENGTH          20
#endif

#define HTS221_TASK_CFG_IN_QUEUE_ITEM_SIZE       sizeof(SMMessage)

#ifndef HTS221_USER_PIN_CONFIG           /*!< to allow the definition of the hw configuration at app level */
#define HTS221_USER_PIN_CONFIG           1
#define HTS221_INT_Pin                  GPIO_PIN_6
#define HTS221_INT_GPIO_Port            GPIOG
#define HTS221_INT_EXTI_IRQn            EXTI9_5_IRQn
#define HTS221_INT_GPIO_ADDITIONAL()    HAL_PWREx_EnableVddIO2()
#define HTS221_INT_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOG_CLK_ENABLE()
#endif

#define SYS_DEBUGF(level, message)                SYS_DEBUGF3(SYS_DBG_HTS221, level, message)

#if defined(DEBUG) || defined (SYS_DEBUG)
#define sTaskObj                                  sHTS221TaskObj
#endif

#ifndef HSD_USE_DUMMY_DATA
#define HSD_USE_DUMMY_DATA 0
#endif

#if (HSD_USE_DUMMY_DATA == 1)
static uint16_t dummyDataCounter_temp = 0;
static uint16_t dummyDataCounter_hum = 0;
#endif


static const ISensor_vtbl sHTS221Task_TempSensorIF_vtbl = {
    HTS221Task_vtblTempGetId,
    HTS221Task_vtblTempGetEventSourceIF,
    HTS221Task_vtblTempGetODR,
    HTS221Task_vtblTempGetFS,
    HTS221Task_vtblTempGetSensitivity,
    HTS221Task_vtblSensorStart,
    HTS221Task_vtblSensorStop,
    HTS221Task_vtblSensorSetODR,
    HTS221Task_vtblSensorSetFS,
    HTS221Task_vtblSensorEnable,
    HTS221Task_vtblSensorDisable,
    HTS221Task_vtblSensorIsEnabled,
    HTS221Task_vtblSensorGetDescription
};

static const ISensor_vtbl sHTS221Task_HumSensorIF_vtbl = {
    HTS221Task_vtblHumGetId,
    HTS221Task_vtblHumGetEventSourceIF,
    HTS221Task_vtblHumGetODR,
    HTS221Task_vtblHumGetFS,
    HTS221Task_vtblHumGetSensitivity,
    HTS221Task_vtblSensorStart,
    HTS221Task_vtblSensorStop,
    HTS221Task_vtblSensorSetODR,
    HTS221Task_vtblSensorSetFS,
    HTS221Task_vtblSensorEnable,
    HTS221Task_vtblSensorDisable,
    HTS221Task_vtblSensorIsEnabled,
    HTS221Task_vtblSensorGetDescription
};


/**
 *  HTS221Task internal structure.
 */
struct _HTS221Task {
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
   * Implements the humidity ISensor interface.
   */
  ISensor_t hum_sensor_if;

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
   * Specifies the sensor ID for the humidity subsensor.
   */
  uint8_t hum_id;

  /**
   * Synchronization object used to send command to the task.
   */
  QueueHandle_t in_queue;

  /**
   * Buffer to store the data read from the sensor
   */
  float p_sensor_data_buff[2];

  /**
   * ::IEventSrc interface implementation for this class.
   */
  IEventSrc *p_temp_event_src;

  /**
   * ::IEventSrc interface implementation for this class.
   */
  IEventSrc *p_hum_event_src;

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

  /*
   * Calibration values
   */
  float x0_t, y0_t, x1_t, y1_t;
  float x0_h, y0_h, x1_h, y1_h;
};


/**
 * Class object declaration
 */
typedef struct _HTS221TaskClass {
  /**
   * HTS221Task class virtual table.
   */
  AManagedTaskEx_vtbl vtbl;

  /**
   * HTS221Task (PM_STATE, ExecuteStepFunc) map.
   */
  pExecuteStepFunc_t p_pm_state2func_map[];
} HTS221TaskClass_t;

// Private member function declaration
// ***********************************

/**
 * Obtain the task object.
 *
 * @return HTS221Task task object.
 */
static HTS221Task * HTS221TaskGetInstance(void);

/**
 * Execute one step of the task control loop while the system is in STATE1 mode.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return SYS_NO_EROR_CODE if success, a task specific error code otherwise.
 */
static sys_error_code_t HTS221TaskExecuteStepState1(AManagedTask *_this);

/**
 * Execute one step of the task control loop while the system is in DATALOG mode.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return SYS_NO_EROR_CODE if success, a task specific error code otherwise.
 */
static sys_error_code_t HTS221TaskExecuteStepDatalog(AManagedTask *_this);

/**
 * Initialize the sensor according to the actual parameters.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return SYS_NO_EROR_CODE if success, a task specific error code otherwise.
 */
static sys_error_code_t HTS221TaskSensorInit(HTS221Task *_this);

/**
 * Read the data from the sensor.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return SYS_NO_EROR_CODE if success, a task specific error code otherwise.
 */
static sys_error_code_t HTS221TaskSensorReadData(HTS221Task *_this);

/**
 * Register the sensor with the global DB and initialize the default parameters.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return SYS_NO_ERROR_CODE if success, an error code otherwise
 */
static sys_error_code_t HTS221TaskSensorRegister(HTS221Task *_this);

/**
 * Initialize the default parameters.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return SYS_NO_ERROR_CODE if success, an error code otherwise
 */
static sys_error_code_t HTS221TaskSensorInitTaskParams(HTS221Task *_this);

/**
 * Private implementation of sensor interface methods for HTS221 sensor
 */
static sys_error_code_t HTS221TaskSensorStart(HTS221Task *_this, SMMessage report);
static sys_error_code_t HTS221TaskSensorStop(HTS221Task *_this, SMMessage report);
static sys_error_code_t HTS221TaskSensorSetODR(HTS221Task *_this, SMMessage report);
static sys_error_code_t HTS221TaskSensorSetFS(HTS221Task *_this, SMMessage report);
static sys_error_code_t HTS221TaskSensorEnable(HTS221Task *_this, SMMessage report);
static sys_error_code_t HTS221TaskSensorDisable(HTS221Task *_this, SMMessage report);

/**
 * Check if the sensor is active. The sensor is active if at least one of the sub sensor is active.
 * @param _this [IN] specifies a pointer to a task object.
 * @return TRUE if the sensor is active, FALSE otherwise.
 */
static boolean_t HTS221TaskSensorIsActive(const HTS221Task *_this);

static sys_error_code_t HTS221TaskEnterLowPowerMode(const HTS221Task *_this);

static sys_error_code_t HTS221TaskConfigureIrqPin(const HTS221Task *_this, boolean_t LowPower);

/**
 * SPI CS Pin interrupt callback
 */
void HTS221Task_EXTI_Callback(uint16_t nPin);


// Inline function forward declaration
// ***********************************

/**
 * Private function used to post a report into the front of the task queue.
 * Used to resume the task when the required by the INIT task.
 *
 * @param this [IN] specifies a pointer to the task object.
 * @param pMessage [IN] specifies a report to send.
 * @return SYS_NO_EROR_CODE if success, SYS_APP_TASK_REPORT_LOST_ERROR_CODE.
 */
static inline sys_error_code_t HTS221TaskPostMessageToFront(HTS221Task *_this, SMMessage *pMessage);

/**
 * Private function used to post a report into the back of the task queue.
 * Used to resume the task when the required by the INIT task.
 *
 * @param this [IN] specifies a pointer to the task object.
 * @param pMessage [IN] specifies a report to send.
 * @return SYS_NO_EROR_CODE if success, SYS_APP_TASK_REPORT_LOST_ERROR_CODE.
 */
static inline sys_error_code_t HTS221TaskPostMessageToBack(HTS221Task *_this, SMMessage *pMessage);


#if defined (__GNUC__)
// Inline function defined inline in the header file HTS221Task.h must be declared here as extern function.
#endif


/* Objects instance */
/********************/

/**
 * The only instance of the task object.
 */
static HTS221Task sTaskObj;

/**
 * The class object.
 */
static const HTS221TaskClass_t sTheClass = {
    /* Class virtual table */
    {
        HTS221Task_vtblHardwareInit,
        HTS221Task_vtblOnCreateTask,
        HTS221Task_vtblDoEnterPowerMode,
        HTS221Task_vtblHandleError,
        HTS221Task_vtblOnEnterTaskControlLoop,
        HTS221Task_vtblForceExecuteStep,
        HTS221Task_vtblOnEnterPowerMode
    },

    /* class (PM_STATE, ExecuteStepFunc) map */
    {
        HTS221TaskExecuteStepState1,
        NULL,
        HTS221TaskExecuteStepDatalog,
    }
};


// Public API definition
// *********************

ISourceObservable *HTS221TaskGetTempSensorIF(HTS221Task *_this){
  return (ISourceObservable *)&(_this->temp_sensor_if);
}

ISourceObservable *HTS221TaskGetHumSensorIF(HTS221Task *_this){
  return (ISourceObservable *)&(_this->hum_sensor_if);
}

AManagedTaskEx *HTS221TaskAlloc() {
  // In this application there is only one Keyboard task,
  // so this allocator implement the singleton design pattern.

  // Initialize the super class
  AMTInitEx(&sTaskObj.super);

  sTaskObj.super.vptr = &sTheClass.vtbl;
  sTaskObj.temp_sensor_if.vptr = &sHTS221Task_TempSensorIF_vtbl;
  sTaskObj.hum_sensor_if.vptr = &sHTS221Task_HumSensorIF_vtbl;

  return (AManagedTaskEx*)&sTaskObj;
}

I2CBusIF *HTS221TaskGetSensorIF(HTS221Task *_this) {
  assert_param(_this);

  return &_this->sensor_if;
}

IEventSrc *HTS221TaskGetTempEventSrcIF(HTS221Task *_this) {
  assert_param(_this);

  return _this->p_temp_event_src;
}

IEventSrc *HTS221TaskGetHumEventSrcIF(HTS221Task *_this) {
  assert_param(_this);

  return _this->p_hum_event_src;
}

// AManagedTask virtual functions definition
// ***********************************************

sys_error_code_t HTS221Task_vtblHardwareInit(AManagedTask *_this, void *pParams) {
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  HTS221Task *p_obj = (HTS221Task*)_this;

  HTS221TaskConfigureIrqPin(p_obj, FALSE);

  return res;
}

sys_error_code_t HTS221Task_vtblOnCreateTask(AManagedTask *_this, TaskFunction_t *pTaskCode, const char **pName, unsigned short *pStackDepth, void **pParams, UBaseType_t *pPriority) {
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  HTS221Task *p_obj = (HTS221Task*)_this;

  // Create task specific sw resources.

  p_obj->in_queue = xQueueCreate(HTS221_TASK_CFG_IN_QUEUE_LENGTH, HTS221_TASK_CFG_IN_QUEUE_ITEM_SIZE);
  if (p_obj->in_queue == NULL) {
    res = SYS_TASK_HEAP_OUT_OF_MEMORY_ERROR_CODE;
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(res);
    return res;
  }

#ifdef DEBUG
  vQueueAddToRegistry(p_obj->in_queue, "HTS221_Q");
#endif

  res = I2CBusIFInit(&p_obj->sensor_if, HTS221_ID, HTS221_I2C_ADDRESS);
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

  p_obj->p_hum_event_src = SensorEventSrcAlloc();
  if (p_obj->p_hum_event_src == NULL) {
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_OUT_OF_MEMORY_ERROR_CODE);
    res = SYS_OUT_OF_MEMORY_ERROR_CODE;
    return res;
  }
  IEventSrcInit(p_obj->p_hum_event_src);

  memset(p_obj->p_sensor_data_buff, 0, sizeof(p_obj->p_sensor_data_buff));
  p_obj->temp_id = 0;
  p_obj->hum_id = 1;
  p_obj->timestamp_tick = 0;
  p_obj->old_timestamp_tick = 0;
  p_obj->timestamp = 0;
  p_obj->x0_t = p_obj->y0_t = p_obj->x1_t = p_obj->y1_t = 0;
  p_obj->x0_h = p_obj->y0_h = p_obj->x1_h = p_obj->y1_h = 0;
  _this->m_pfPMState2FuncMap = sTheClass.p_pm_state2func_map;

  *pTaskCode = AMTExRun;
  *pName = "HTS221";
  *pStackDepth = HTS221_TASK_CFG_STACK_DEPTH;
  *pParams = _this;
  *pPriority = HTS221_TASK_CFG_PRIORITY;

  res = HTS221TaskSensorInitTaskParams(p_obj);
  if (SYS_IS_ERROR_CODE(res)) {
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_OUT_OF_MEMORY_ERROR_CODE);
    res = SYS_OUT_OF_MEMORY_ERROR_CODE;
    return res;
  }

  res = HTS221TaskSensorRegister(p_obj);
  if (SYS_IS_ERROR_CODE(res)) {
    SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("HTS221: unable to register with DB\r\n"));
    sys_error_handler();
  }

  return res;
}


sys_error_code_t HTS221Task_vtblDoEnterPowerMode(AManagedTask *_this, const EPowerMode ActivePowerMode, const EPowerMode NewPowerMode) {
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  HTS221Task *p_obj = (HTS221Task*)_this;
  stmdev_ctx_t *p_sensor_drv = (stmdev_ctx_t*) &p_obj->sensor_if.super.m_xConnector;

  if (NewPowerMode == E_POWER_MODE_SENSORS_ACTIVE) {
    if (HTS221TaskSensorIsActive(p_obj)) {
      SMMessage report = {
          .sensorMessage.messageId = SM_MESSAGE_ID_SENSOR_CMD,
          .sensorMessage.nCmdID = SENSOR_CMD_ID_START
      };

      if (xQueueSendToBack(p_obj->in_queue, &report, pdMS_TO_TICKS(100)) != pdTRUE) {
        res = SYS_APP_TASK_REPORT_LOST_ERROR_CODE;
        SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_APP_TASK_REPORT_LOST_ERROR_CODE);
      }


      // reset the variables for the time stamp computation.
      p_obj->timestamp_tick = 0;
      p_obj->old_timestamp_tick = 0;
      p_obj->timestamp = 0;
    }

    SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("HTS221: -> DATALOG\r\n"));
    SYS_DEBUGF3(SYS_DBG_APP, SYS_DBG_LEVEL_VERBOSE, ("HTS221: -> DATALOG\r\n"));
  }
  else if (NewPowerMode == E_POWER_MODE_STATE1) {
    if (ActivePowerMode == E_POWER_MODE_SENSORS_ACTIVE) {
      /* SM_SENSOR_STATE_SUSPENDING */
      hts221_power_on_set(p_sensor_drv, PROPERTY_DISABLE);
      xQueueReset(p_obj->in_queue);
    }

    SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("HTS221: -> STATE1\r\n"));
    SYS_DEBUGF3(SYS_DBG_APP, SYS_DBG_LEVEL_VERBOSE, ("HTS221: -> STATE1\r\n"));
  }
  else if (NewPowerMode == E_POWER_MODE_SLEEP_1) {
    // the MCU is going in stop so I put the sensor in low power
    // from the INIT task
    res = HTS221TaskEnterLowPowerMode(p_obj);
    if (SYS_IS_ERROR_CODE(res)) {
      sys_error_handler();
    }
    HTS221TaskConfigureIrqPin(p_obj, TRUE);
    // notify the bus
    if (p_obj->sensor_if.super.m_pfBusCtrl != NULL) {
      p_obj->sensor_if.super.m_pfBusCtrl(&p_obj->sensor_if.super, E_BUS_CTRL_DEV_NOTIFY_POWER_MODE, 0);
    }

    SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("HTS221: -> SLEEP_1\r\n"));
    SYS_DEBUGF3(SYS_DBG_APP, SYS_DBG_LEVEL_VERBOSE, ("HTS221: -> SLEEP_1\r\n"));
  }

  return res;
}

sys_error_code_t HTS221Task_vtblHandleError(AManagedTask *_this, SysEvent xError) {
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  //  HTS221Task *p_obj = (HTS221Task*)_this;


  return res;
}

sys_error_code_t HTS221Task_vtblOnEnterTaskControlLoop(AManagedTask *_this) {
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("HTS221: start.\r\n"));

  // At this point all system has been initialized.
  // Execute task specific delayed one time initialization.

  return res;
}

sys_error_code_t HTS221Task_vtblForceExecuteStep(AManagedTaskEx *_this, EPowerMode ActivePowerMode) {
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  HTS221Task *p_obj = (HTS221Task*)_this;

  SMMessage report = {
      .internalMessageFE.messageId = SM_MESSAGE_ID_FORCE_STEP,
      .internalMessageFE.nData = 0
  };

  if ((ActivePowerMode == E_POWER_MODE_STATE1) || (ActivePowerMode == E_POWER_MODE_SENSORS_ACTIVE)) {
    if(AMTExIsTaskInactive(_this)) {
      res = HTS221TaskPostMessageToFront(p_obj, (SMMessage*)&report);
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

sys_error_code_t HTS221Task_vtblOnEnterPowerMode(AManagedTaskEx *_this, const EPowerMode ActivePowerMode, const EPowerMode NewPowerMode) {
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  //  HTS221Task *p_obj = (HTS221Task*)_this;

  return res;
}


// ISensor virtual functions definition
// *******************************************

uint8_t HTS221Task_vtblTempGetId(ISourceObservable *_this){
  assert_param(_this);
  HTS221Task *p_hts221_instance = HTS221TaskGetInstance();
  uint8_t res = p_hts221_instance->temp_id;

  return res;
}

uint8_t HTS221Task_vtblHumGetId(ISourceObservable *_this){
  assert_param(_this);
  HTS221Task *p_hts221_instance = HTS221TaskGetInstance();
  uint8_t res = p_hts221_instance->hum_id;

  return res;
}

IEventSrc *HTS221Task_vtblTempGetEventSourceIF(ISourceObservable *_this){
  assert_param(_this);
  HTS221Task *p_hts221_instance = HTS221TaskGetInstance();
  return p_hts221_instance->p_temp_event_src;
}

IEventSrc *HTS221Task_vtblHumGetEventSourceIF(ISourceObservable *_this){
  assert_param(_this);
  HTS221Task *p_hts221_instance = HTS221TaskGetInstance();
  return p_hts221_instance->p_hum_event_src;
}

sys_error_code_t HTS221Task_vtblTempGetODR(ISourceObservable *_this, float *p_measured, float *p_nominal){
  assert_param(_this != NULL);
  /*get the object implementing the ISourceObservable IF */
  HTS221Task *p_if_owner = (HTS221Task*)((uint32_t)_this - offsetof(HTS221Task, temp_sensor_if));
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

float HTS221Task_vtblTempGetFS(ISourceObservable *_this){
  assert_param(_this);
  HTS221Task *p_hts221_instance = HTS221TaskGetInstance();
  float res = p_hts221_instance->sensor_status.pSubSensorStatus[0].FS;

  return res;
}

float HTS221Task_vtblTempGetSensitivity(ISourceObservable *_this){
  assert_param(_this);
  HTS221Task *p_hts221_instance = HTS221TaskGetInstance();
  float res = p_hts221_instance->sensor_status.pSubSensorStatus[0].Sensitivity;

  return res;
}

sys_error_code_t HTS221Task_vtblHumGetODR(ISourceObservable *_this, float *p_measured, float *p_nominal){
  assert_param(_this != NULL);
  /*get the object implementing the ISourceObservable IF */
  HTS221Task *p_if_owner = (HTS221Task*)((uint32_t)_this - offsetof(HTS221Task, hum_sensor_if));
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

float HTS221Task_vtblHumGetFS(ISourceObservable *_this){
  assert_param(_this);
  HTS221Task *p_hts221_instance = HTS221TaskGetInstance();
  float res = p_hts221_instance->sensor_status.pSubSensorStatus[1].FS;

  return res;
}

float HTS221Task_vtblHumGetSensitivity(ISourceObservable *_this){
  assert_param(_this);
  HTS221Task *p_hts221_instance = HTS221TaskGetInstance();
  float res = p_hts221_instance->sensor_status.pSubSensorStatus[1].Sensitivity;

  return res;
}

sys_error_code_t HTS221Task_vtblSensorStart(ISensor_t *_this){
  assert_param(_this);
  sys_error_code_t res = SYS_NOT_IMPLEMENTED_ERROR_CODE;

  return res;
}

sys_error_code_t HTS221Task_vtblSensorStop(ISensor_t *_this){
  assert_param(_this);
  sys_error_code_t res = SYS_NOT_IMPLEMENTED_ERROR_CODE;

  return res;
}

sys_error_code_t HTS221Task_vtblSensorSetODR(ISensor_t *_this, float ODR){
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  HTS221Task *p_hts221_instance = HTS221TaskGetInstance();
  EPowerMode log_status = AMTGetTaskPowerMode((AManagedTask *)p_hts221_instance);
  uint8_t sensor_id = ISourceGetId((ISourceObservable *)_this);

  if ((log_status == E_POWER_MODE_SENSORS_ACTIVE) && ISensorIsEnabled(_this))
  {
    res = SYS_INVALID_PARAMETER_ERROR_CODE;
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
    res = HTS221TaskPostMessageToBack(p_hts221_instance, (SMMessage*)&report);
  }

  return res;
}

sys_error_code_t HTS221Task_vtblSensorSetFS(ISensor_t *_this, float FS){
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  HTS221Task *p_hts221_instance = HTS221TaskGetInstance();
  EPowerMode log_status = AMTGetTaskPowerMode((AManagedTask *)p_hts221_instance);
  uint8_t sensor_id = ISourceGetId((ISourceObservable *)_this);

  if ((log_status == E_POWER_MODE_SENSORS_ACTIVE) && ISensorIsEnabled(_this))
  {
    res = SYS_INVALID_PARAMETER_ERROR_CODE;
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
    res = HTS221TaskPostMessageToBack(p_hts221_instance, (SMMessage*)&report);
  }

  return res;

}

sys_error_code_t HTS221Task_vtblSensorEnable(ISensor_t *_this){
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  HTS221Task *p_hts221_instance = HTS221TaskGetInstance();
  EPowerMode log_status = AMTGetTaskPowerMode((AManagedTask *)p_hts221_instance);
  uint8_t sensor_id = ISourceGetId((ISourceObservable *)_this);

  if ((log_status == E_POWER_MODE_SENSORS_ACTIVE) && ISensorIsEnabled(_this))
  {
    res = SYS_INVALID_PARAMETER_ERROR_CODE;
  }
  else
  {
    /* Set a new command message in the queue */
    SMMessage report = {
        .sensorMessage.messageId = SM_MESSAGE_ID_SENSOR_CMD,
        .sensorMessage.nCmdID = SENSOR_CMD_ID_ENABLE,
        .sensorMessage.nSensorId = sensor_id,
    };
    res = HTS221TaskPostMessageToBack(p_hts221_instance, (SMMessage*)&report);
  }

  return res;
}

sys_error_code_t HTS221Task_vtblSensorDisable(ISensor_t *_this){
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  HTS221Task *p_hts221_instance = HTS221TaskGetInstance();
  EPowerMode log_status = AMTGetTaskPowerMode((AManagedTask *)p_hts221_instance);
  uint8_t sensor_id = ISourceGetId((ISourceObservable *)_this);

  if ((log_status == E_POWER_MODE_SENSORS_ACTIVE) && ISensorIsEnabled(_this))
  {
    res = SYS_INVALID_PARAMETER_ERROR_CODE;
  }
  else
  {
    /* Set a new command message in the queue */
    SMMessage report = {
        .sensorMessage.messageId = SM_MESSAGE_ID_SENSOR_CMD,
        .sensorMessage.nCmdID = SENSOR_CMD_ID_DISABLE,
        .sensorMessage.nSensorId = sensor_id,
    };
    res = HTS221TaskPostMessageToBack(p_hts221_instance, (SMMessage*)&report);
  }

  return res;
}

boolean_t HTS221Task_vtblSensorIsEnabled(ISensor_t *_this){
  boolean_t res = FALSE;

  HTS221Task *p_hts221_instance = HTS221TaskGetInstance();

  if(ISourceGetId((ISourceObservable *)_this) == p_hts221_instance->temp_id)
    res = p_hts221_instance->sensor_status.pSubSensorStatus[0].IsActive;
  else if(ISourceGetId((ISourceObservable *)_this) == p_hts221_instance->hum_id)
    res = p_hts221_instance->sensor_status.pSubSensorStatus[1].IsActive;

  return res;
}

SensorDescriptor_t HTS221Task_vtblSensorGetDescription(ISensor_t *_this){

  HTS221Task *p_hts221_instance = HTS221TaskGetInstance();
  return p_hts221_instance->sensor_descriptor;
}


// Private function definition
// ***************************

static HTS221Task* HTS221TaskGetInstance(){
  return (HTS221Task*)&sTaskObj;
}

static sys_error_code_t HTS221TaskExecuteStepState1(AManagedTask *_this) {
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  HTS221Task *p_obj = (HTS221Task*)_this;
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
        switch (report.sensorMessage.nCmdID)
        {
          case SENSOR_CMD_ID_START:
            res = HTS221TaskSensorStart(p_obj, report);
            break;
          case SENSOR_CMD_ID_STOP:
            res = HTS221TaskSensorStop(p_obj, report);
            break;
          case SENSOR_CMD_ID_SET_ODR:
            res = HTS221TaskSensorSetODR(p_obj, report);
            break;
          case SENSOR_CMD_ID_SET_FS:
            res = HTS221TaskSensorSetFS(p_obj, report);
            break;
          case SENSOR_CMD_ID_ENABLE:
            res = HTS221TaskSensorEnable(p_obj, report);
            break;
          case SENSOR_CMD_ID_DISABLE:
            res = HTS221TaskSensorDisable(p_obj, report);
            break;
          default:
            // unwanted report
            res = SYS_APP_TASK_UNKNOWN_REPORT_ERROR_CODE;
            SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_APP_TASK_UNKNOWN_REPORT_ERROR_CODE);

            SYS_DEBUGF(SYS_DBG_LEVEL_WARNING, ("HTS221: unexpected report in State1: %i\r\n", report.messageID));
            SYS_DEBUGF3(SYS_DBG_APP, SYS_DBG_LEVEL_WARNING, ("HTS221: unexpected report in State1: %i\r\n", report.messageID));
            break;
        }
        break;
      }
      default:
      {
        // unwanted report
        res = SYS_APP_TASK_UNKNOWN_REPORT_ERROR_CODE;
        SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_APP_TASK_UNKNOWN_REPORT_ERROR_CODE);

        SYS_DEBUGF(SYS_DBG_LEVEL_WARNING, ("HTS221: unexpected report in State1: %i\r\n", report.messageID));
        SYS_DEBUGF3(SYS_DBG_APP, SYS_DBG_LEVEL_WARNING, ("HTS221: unexpected report in State1: %i\r\n", report.messageID));
        break;
      }
    }
  }

  return res;
}

static sys_error_code_t HTS221TaskExecuteStepDatalog(AManagedTask *_this) {
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  HTS221Task *p_obj = (HTS221Task*)_this;
  SMMessage report = {0};

  AMTExSetInactiveState((AManagedTaskEx*)_this, TRUE);
  if (pdTRUE == xQueueReceive(p_obj->in_queue, &report, portMAX_DELAY)) {
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
//      SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("HTS221: new data.\r\n"));

        res = HTS221TaskSensorReadData(p_obj);
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
                .packet.payload = (uint8_t *) p_obj->p_sensor_data_buff,
                .packet.payload_fmt = AI_SP_FMT_FLOAT32_RESET(),
                .mode = AI_SP_MODE_FULL //TODO: STF - this means that data are interleaved?!?
                // bonus question: what is AI_LOGGING_SHAPES_DEPTH ??
                // (can I represent anomogeneous matrix [4*4] with this data format ?
            };
            ai_logging_create_shape_0d(&stream.packet.shape);

            SensorEventInit((IEvent*)&evt, p_obj->p_temp_event_src, (ai_logging_packet_t*)&stream, timestamp, p_obj->temp_id);
            IEventSrcSendEvent(p_obj->p_temp_event_src, (IEvent*)&evt, NULL);
          }
          if(p_obj->sensor_status.pSubSensorStatus[1].IsActive)
          {
            SensorEvent evt;

            AI_SP_Stream_t stream = {
                .packet.payload = (uint8_t *) &p_obj->p_sensor_data_buff[1],
                .packet.payload_fmt = AI_SP_FMT_FLOAT32_RESET(),
                .mode = AI_SP_MODE_FULL //TODO: STF - this means that data are interleaved?!?
                // bonus question: what is AI_LOGGING_SHAPES_DEPTH ??
                // (can I represent anomogeneous matrix [4*4] with this data format ?
            };
            ai_logging_create_shape_0d(&stream.packet.shape);
            SensorEventInit((IEvent*)&evt, p_obj->p_hum_event_src, (ai_logging_packet_t*)&stream, timestamp, p_obj->hum_id);
            IEventSrcSendEvent(p_obj->p_hum_event_src, (IEvent*)&evt, NULL);
          }

          /* update measuredODR */
          p_obj->sensor_status.pSubSensorStatus[0].MeasuredODR = 1/delta_timestamp;
          p_obj->sensor_status.pSubSensorStatus[1].MeasuredODR = 1/delta_timestamp;

//          SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("HTS221: ts = %f\r\n", (float)timestamp));
//          HTS221: ts = assertion "Balloc succeeded" failed: file "/build/gnu-tools-for-stm32_9-2020-q2-update.20201001-1621/src/newlib/newlib/libc/stdlib/dtoa.c", line 426
      }
      break;
    }
    case SM_MESSAGE_ID_SENSOR_CMD:
    {
      switch (report.sensorMessage.nCmdID)
      {
        case SENSOR_CMD_ID_START:
          res = HTS221TaskSensorInit(p_obj);
          if (!SYS_IS_ERROR_CODE(res)) {
//          HTS221TaskConfigureIrqPin(p_obj, FALSE);
          // enable the IRQs
          HAL_NVIC_EnableIRQ(HTS221_INT_EXTI_IRQn);
        }
      break;
        case SENSOR_CMD_ID_STOP:
          res = HTS221TaskSensorStop(p_obj, report);
          break;
        case SENSOR_CMD_ID_SET_ODR:
          res = HTS221TaskSensorSetODR(p_obj, report);
          break;
        case SENSOR_CMD_ID_SET_FS:
          res = HTS221TaskSensorSetFS(p_obj, report);
          break;
        case SENSOR_CMD_ID_ENABLE:
          res = HTS221TaskSensorEnable(p_obj, report);
          break;
        case SENSOR_CMD_ID_DISABLE:
          res = HTS221TaskSensorDisable(p_obj, report);
          break;
        default:
          // unwanted report
          res = SYS_APP_TASK_UNKNOWN_REPORT_ERROR_CODE;
          SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_APP_TASK_UNKNOWN_REPORT_ERROR_CODE);

          SYS_DEBUGF(SYS_DBG_LEVEL_WARNING, ("HTS221: unexpected report in Datalog: %i\r\n", report.messageID));
          SYS_DEBUGF3(SYS_DBG_APP, SYS_DBG_LEVEL_WARNING, ("HTS221: unexpected report in Datalog: %i\r\n", report.messageID));
          break;
      }
      break;
    }
    default:
      // unwanted report
        res = SYS_APP_TASK_UNKNOWN_REPORT_ERROR_CODE;
      SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_APP_TASK_UNKNOWN_REPORT_ERROR_CODE);

        SYS_DEBUGF(SYS_DBG_LEVEL_WARNING, ("HTS221: unexpected report in Datalog: %i\r\n", report.messageID));
        break;
    }
  }

  return res;
}

static inline sys_error_code_t HTS221TaskPostMessageToFront(HTS221Task *_this, SMMessage *pMessage) {
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

static inline sys_error_code_t HTS221TaskPostMessageToBack(HTS221Task *_this, SMMessage *pMessage) {
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

static sys_error_code_t HTS221TaskSensorInit(HTS221Task *_this) {
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  stmdev_ctx_t *p_sensor_drv = (stmdev_ctx_t*) &_this->sensor_if.super.m_xConnector;

  uint8_t reg0 = 0;
  int32_t ret_val = 0;
  float hts221_odr = 0.0f;

  typedef struct	/* From hts221.h - HL PID */
  {
    float x0;
    float y0;
    float x1;
    float y1;
  } lin_t;

  lin_t lin_temp;
  lin_t lin_hum;

  ret_val = hts221_device_id_get( p_sensor_drv, (uint8_t *)&reg0);
  if (!ret_val) {
    I2CBusIFSetWhoAmI(&_this->sensor_if, reg0);
  }
  SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("HTS221: sensor - I am 0x%x.\r\n", reg0));

  typedef union
  { /* From hts221.h - HL PID */
    int16_t i16bit;
    uint8_t u8bit[2];
  } hts221_axis1bit16_t;

  hts221_axis1bit16_t data_raw_humidity;
  hts221_axis1bit16_t data_raw_temperature;

  /* Read the output registers to reset the interrupt pin */
  /* Without these instructions, INT PIN remains HIGH and */
  /* it never swithces anymore */
  hts221_temperature_raw_get(p_sensor_drv, &data_raw_temperature.i16bit);
  hts221_humidity_raw_get(p_sensor_drv, &data_raw_humidity.i16bit);

  /* Power on and reset the component */
  hts221_power_on_set(p_sensor_drv, PROPERTY_DISABLE);
  hts221_boot_set(p_sensor_drv, PROPERTY_ENABLE);

  /* Set BDU */
  hts221_block_data_update_set(p_sensor_drv, PROPERTY_ENABLE);
  /* Enable Interrupt */
  hts221_drdy_on_int_set(p_sensor_drv, PROPERTY_ENABLE);

  /* Set Data Rate */
  if (_this->sensor_status.pSubSensorStatus[1].IsActive == TRUE)
  {
    hts221_odr = _this->sensor_status.pSubSensorStatus[1].ODR;
    _this->sensor_status.pSubSensorStatus[0].ODR = _this->sensor_status.pSubSensorStatus[1].ODR;
  }
  else
  {
    hts221_odr = _this->sensor_status.pSubSensorStatus[0].ODR;
    _this->sensor_status.pSubSensorStatus[1].ODR = _this->sensor_status.pSubSensorStatus[0].ODR;
  }

  if(hts221_odr < 2.0f)
  {
    hts221_data_rate_set(p_sensor_drv, HTS221_ODR_1Hz);
  }
  else if(hts221_odr < 8.0f)
  {
    hts221_data_rate_set(p_sensor_drv, HTS221_ODR_7Hz);
  }
  else
  {
    hts221_data_rate_set(p_sensor_drv, HTS221_ODR_12Hz5);
  }

  /* Get calibration values (only first time) */
  hts221_temp_adc_point_0_get(p_sensor_drv, &lin_temp.x0);
  _this->x0_t = lin_temp.x0;

  hts221_temp_deg_point_0_get(p_sensor_drv, &lin_temp.y0);
  _this->y0_t = lin_temp.y0;

  hts221_temp_adc_point_1_get(p_sensor_drv, &lin_temp.x1);
  _this->x1_t = lin_temp.x1;

  hts221_temp_deg_point_1_get(p_sensor_drv, &lin_temp.y1);
  _this->y1_t = lin_temp.y1;

  hts221_hum_adc_point_0_get(p_sensor_drv, &lin_hum.x0);
  _this->x0_h = lin_hum.x0;

  hts221_hum_rh_point_0_get(p_sensor_drv, &lin_hum.y0);
  _this->y0_h = lin_hum.y0;

  hts221_hum_adc_point_1_get(p_sensor_drv, &lin_hum.x1);
  _this->x1_h = lin_hum.x1;

  hts221_hum_rh_point_1_get(p_sensor_drv, &lin_hum.y1);
  _this->y1_h = lin_hum.y1;

  /* Power Up */
  hts221_power_on_set(p_sensor_drv, PROPERTY_ENABLE);

  return res;
}

static sys_error_code_t HTS221TaskSensorReadData(HTS221Task *_this) {
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  stmdev_ctx_t *p_sensor_drv = (stmdev_ctx_t*) &_this->sensor_if.super.m_xConnector;

  typedef union{			/* From hts221.h - HL PID */
    int16_t i16bit;
    uint8_t u8bit[2];
  } hts221_axis1bit16_t;

  hts221_axis1bit16_t data_raw_humidity;
  hts221_axis1bit16_t data_raw_temperature;

  hts221_temperature_raw_get(p_sensor_drv, &data_raw_temperature.i16bit);
  hts221_humidity_raw_get(p_sensor_drv, &data_raw_humidity.i16bit);

  /* Apply calibration */ /* To be optimized eventually */
  _this->p_sensor_data_buff[0] = (((_this->y1_t - _this->y0_t) * (float)(data_raw_temperature.i16bit)) + ((_this->x1_t * _this->y0_t) - (_this->x0_t * _this->y1_t))) / (_this->x1_t - _this->x0_t);

  /* Apply calibration */ /* To be optimized eventually */
  _this->p_sensor_data_buff[1] = (((_this->y1_h - _this->y0_h) * (float)(data_raw_humidity.i16bit)) + ((_this->x1_h * _this->y0_h) - (_this->x0_h * _this->y1_h))) / (_this->x1_h - _this->x0_h);

#if (HSD_USE_DUMMY_DATA == 1)
  _this->p_sensor_data_buff[0] = (float)(dummyDataCounter_temp++);
  _this->p_sensor_data_buff[1] = (float)(dummyDataCounter_hum++);
#endif

  return res;
}

static sys_error_code_t HTS221TaskSensorRegister(HTS221Task *_this) {
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  ISensor_t *temp_if = (ISensor_t *)HTS221TaskGetTempSensorIF(_this);
  ISensor_t *hum_if = (ISensor_t *)HTS221TaskGetHumSensorIF(_this);

  _this->temp_id = SMAddSensor(temp_if);
  _this->hum_id = SMAddSensor(hum_if);

  return res;
}


static sys_error_code_t HTS221TaskSensorInitTaskParams(HTS221Task *_this) {
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

    /* SENSOR DESCRIPTOR */
  strcpy(_this->sensor_descriptor.Name, "HTS221");
  _this->sensor_descriptor.NumberSubSensors = 2;

    /* SUBSENSOR 0 DESCRIPTOR */
  _this->sensor_descriptor.pSubSensorDescriptor[0].IdSub = _this->temp_id;
  _this->sensor_descriptor.pSubSensorDescriptor[0].SensorType = COM_TYPE_TEMP;
  _this->sensor_descriptor.pSubSensorDescriptor[0].Dimensions = 1;
  strcpy(_this->sensor_descriptor.pSubSensorDescriptor[0].DimensionsLabel[0], "tem");
  _this->sensor_descriptor.pSubSensorDescriptor[0].DataType = DATA_TYPE_FLOAT;
  _this->sensor_descriptor.pSubSensorDescriptor[0].pODR[0] = 1.0f;
  _this->sensor_descriptor.pSubSensorDescriptor[0].pODR[1] = 7.0f;
  _this->sensor_descriptor.pSubSensorDescriptor[0].pODR[2] = 12.5f;
  _this->sensor_descriptor.pSubSensorDescriptor[0].pODR[3] = COM_END_OF_LIST_FLOAT;
  _this->sensor_descriptor.pSubSensorDescriptor[0].pSamplesPerTimestamp[0] = 0;
  _this->sensor_descriptor.pSubSensorDescriptor[0].pSamplesPerTimestamp[1] = 1000;
  strcpy(_this->sensor_descriptor.pSubSensorDescriptor[0].unit, "Celsius");
  _this->sensor_descriptor.pSubSensorDescriptor[0].pFS[0] = 120.0f;
  _this->sensor_descriptor.pSubSensorDescriptor[0].pFS[1] = COM_END_OF_LIST_FLOAT;

    /* SUBSENSOR 0 STATUS */
  _this->sensor_status.pSubSensorStatus[0].IsActive = TRUE;
  _this->sensor_status.pSubSensorStatus[0].FS = 120.0f;
  _this->sensor_status.pSubSensorStatus[0].Sensitivity = 1.0f;
  _this->sensor_status.pSubSensorStatus[0].ODR = 12.5f;
  _this->sensor_status.pSubSensorStatus[0].MeasuredODR = _this->sensor_status.pSubSensorStatus[0].ODR;
  _this->sensor_status.pSubSensorStatus[0].InitialOffset = 0.0f;
#if (HSD_USE_DUMMY_DATA == 1)
  _this->sensor_status.pSubSensorStatus[0].SamplesPerTimestamp = 0;
#else
  _this->sensor_status.pSubSensorStatus[0].SamplesPerTimestamp = 50;
#endif

    /* SUBSENSOR 1 DESCRIPTOR */
  _this->sensor_descriptor.pSubSensorDescriptor[1].IdSub = _this->hum_id;
  _this->sensor_descriptor.pSubSensorDescriptor[1].SensorType = COM_TYPE_HUM;
  _this->sensor_descriptor.pSubSensorDescriptor[1].Dimensions = 1;
  strcpy(_this->sensor_descriptor.pSubSensorDescriptor[1].DimensionsLabel[0], "hum");
  _this->sensor_descriptor.pSubSensorDescriptor[1].DataType = DATA_TYPE_FLOAT;
  _this->sensor_descriptor.pSubSensorDescriptor[1].pODR[0] = 1.0f;
  _this->sensor_descriptor.pSubSensorDescriptor[1].pODR[1] = 7.0f;
  _this->sensor_descriptor.pSubSensorDescriptor[1].pODR[2] = 12.5f;
  _this->sensor_descriptor.pSubSensorDescriptor[1].pODR[3] = COM_END_OF_LIST_FLOAT;
  _this->sensor_descriptor.pSubSensorDescriptor[1].pSamplesPerTimestamp[0] = 0;
  _this->sensor_descriptor.pSubSensorDescriptor[1].pSamplesPerTimestamp[1] = 1000;
  strcpy(_this->sensor_descriptor.pSubSensorDescriptor[1].unit, "%");
  _this->sensor_descriptor.pSubSensorDescriptor[1].pFS[0] = 100.0f;
  _this->sensor_descriptor.pSubSensorDescriptor[1].pFS[1] = COM_END_OF_LIST_FLOAT;

    /* SUBSENSOR 1 STATUS */
  _this->sensor_status.pSubSensorStatus[1].IsActive = TRUE;
  _this->sensor_status.pSubSensorStatus[1].FS = 100.0f;
  _this->sensor_status.pSubSensorStatus[1].Sensitivity = 1.0f;
  _this->sensor_status.pSubSensorStatus[1].ODR = 12.5f;
  _this->sensor_status.pSubSensorStatus[1].MeasuredODR = _this->sensor_status.pSubSensorStatus[1].ODR;
  _this->sensor_status.pSubSensorStatus[1].InitialOffset = 0.0f;
#if (HSD_USE_DUMMY_DATA == 1)
  _this->sensor_status.pSubSensorStatus[1].SamplesPerTimestamp = 0;
#else
  _this->sensor_status.pSubSensorStatus[1].SamplesPerTimestamp = 50;
#endif

  return res;
}

static sys_error_code_t HTS221TaskSensorStart(HTS221Task *_this, SMMessage report){
  assert_param(_this);
  sys_error_code_t res = SYS_NOT_IMPLEMENTED_ERROR_CODE;

  return res;
}

static sys_error_code_t HTS221TaskSensorStop(HTS221Task *_this, SMMessage report){
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  stmdev_ctx_t *p_sensor_drv = (stmdev_ctx_t*) &_this->sensor_if.super.m_xConnector;
  hts221_power_on_set(p_sensor_drv, PROPERTY_DISABLE);
  return res;
}

static sys_error_code_t HTS221TaskSensorSetODR(HTS221Task *_this, SMMessage report){
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  stmdev_ctx_t *p_sensor_drv = (stmdev_ctx_t*) &_this->sensor_if.super.m_xConnector;
  float ODR = (float)report.sensorMessage.nParam;
  uint8_t id = report.sensorMessage.nSensorId;

  if(id == _this->temp_id || id == _this->hum_id)
  {
    if(ODR < 2.0f)
    {
      hts221_data_rate_set(p_sensor_drv, HTS221_ODR_1Hz);
      ODR = 1.0f;
    }
    else if(ODR < 8.0f)
    {
      hts221_data_rate_set(p_sensor_drv, HTS221_ODR_7Hz);
      ODR = 7.0f;
    }
    else
    {
      hts221_data_rate_set(p_sensor_drv, HTS221_ODR_12Hz5);
      ODR = 12.5f;
    }

    if (!SYS_IS_ERROR_CODE(res))
    {
      if(id == _this->temp_id)
      {
        _this->sensor_status.pSubSensorStatus[0].ODR = ODR;
        _this->sensor_status.pSubSensorStatus[0].MeasuredODR = 0.0f;
      }
      else
      {
        _this->sensor_status.pSubSensorStatus[1].ODR = ODR;
        _this->sensor_status.pSubSensorStatus[1].MeasuredODR = 0.0f;
      }
    }
  }
  else
  {
    res = SYS_INVALID_PARAMETER_ERROR_CODE;
  }

  return res;
}

static sys_error_code_t HTS221TaskSensorSetFS(HTS221Task *_this, SMMessage report){
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  float FS = (float)report.sensorMessage.nParam;
  uint8_t id = report.sensorMessage.nSensorId;

  if(id == _this->temp_id)
  {
    if(FS != 120.0f)
      res = SYS_INVALID_PARAMETER_ERROR_CODE;

    if (!SYS_IS_ERROR_CODE(res))
    {
      _this->sensor_status.pSubSensorStatus[0].FS = FS;
    }
  }
  else if(id == _this->hum_id)
  {
    if(FS != 100.0f)
      res = SYS_INVALID_PARAMETER_ERROR_CODE;

    if (!SYS_IS_ERROR_CODE(res))
    {
      _this->sensor_status.pSubSensorStatus[1].FS = FS;
    }
  }
  else
  {
    res = SYS_INVALID_PARAMETER_ERROR_CODE;
  }

  return res;
}

static sys_error_code_t HTS221TaskSensorEnable(HTS221Task *_this, SMMessage report){
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  uint8_t id = report.sensorMessage.nSensorId;

  if(id == _this->temp_id)
    _this->sensor_status.pSubSensorStatus[0].IsActive = TRUE;
  else if(id == _this->hum_id)
    _this->sensor_status.pSubSensorStatus[1].IsActive = TRUE;
  else
    res = SYS_INVALID_PARAMETER_ERROR_CODE;

  return res;
}

static sys_error_code_t HTS221TaskSensorDisable(HTS221Task *_this, SMMessage report){
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  uint8_t id = report.sensorMessage.nSensorId;

  if(id == _this->temp_id)
    _this->sensor_status.pSubSensorStatus[0].IsActive = FALSE;
  else if(id == _this->hum_id)
    _this->sensor_status.pSubSensorStatus[1].IsActive = FALSE;
  else
    res = SYS_INVALID_PARAMETER_ERROR_CODE;

  return res;
}

static boolean_t HTS221TaskSensorIsActive(const HTS221Task *_this) {
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

static sys_error_code_t HTS221TaskEnterLowPowerMode(const HTS221Task *_this) {
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  stmdev_ctx_t *p_sensor_drv = (stmdev_ctx_t*) &_this->sensor_if.super.m_xConnector;

  if (hts221_power_on_set(p_sensor_drv, PROPERTY_DISABLE)) {
    res = SYS_SENSOR_TASK_OP_ERROR_CODE;
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_SENSOR_TASK_OP_ERROR_CODE);
  }

  return res;
}

static sys_error_code_t HTS221TaskConfigureIrqPin(const HTS221Task *_this, boolean_t LowPower) {
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  if (!LowPower) {

	  HTS221_INT_GPIO_ADDITIONAL();
	  HTS221_INT_GPIO_CLK_ENABLE();
	  /*Configure GPIO pins : HTS221_INT_Pin hts221_INT1_Pin */
	  GPIO_InitStruct.Pin =  HTS221_INT_Pin ;
	  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  HAL_GPIO_Init(HTS221_INT_GPIO_Port, &GPIO_InitStruct);

	  /* EXTI interrupt init*/
	  HAL_NVIC_SetPriority(HTS221_INT_EXTI_IRQn, 5, 0);
  }
  else {
    // first disable the IRQ to avoid spurious interrupt to wake the MCU up.
    HAL_NVIC_DisableIRQ(HTS221_INT_EXTI_IRQn);
    HAL_NVIC_ClearPendingIRQ(HTS221_INT_EXTI_IRQn);
    // then reconfigure the PIN in analog high impedance to reduce the power consumption.
    GPIO_InitStruct.Pin =  HTS221_INT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(HTS221_INT_GPIO_Port, &GPIO_InitStruct);
  }

  return res;
}


// CubeMX integration
// ******************

void HTS221Task_EXTI_Callback(uint16_t nPin) {
  struct hts221Message_t report = {
      .messageId = SM_MESSAGE_ID_HTS221,
      .bDataReady = 1
  };

  if (sTaskObj.in_queue != NULL ) {
    if (pdTRUE != xQueueSendToBackFromISR(sTaskObj.in_queue, &report, NULL)) {
      // unable to send the report. Signal the error
      sys_error_handler();
    }
    sTaskObj.timestamp_tick = SMUtilGetTimeStamp();
  }
}

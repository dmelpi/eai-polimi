/**
 ******************************************************************************
 * @file    IMP23ABSUTask.c
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

#include "IMP23ABSUTask.h"
#include "IMP23ABSUTask_vtbl.h"
#include "drivers/AnalogMicDriver.h"
#include "drivers/AnalogMicDriver_vtbl.h"
#include "services/sysdebug.h"
#include <string.h>
#include "SensorCommands.h"
#include "SensorDef.h"
#include "SensorRegister.h"
#include "queue.h"
#include "events/ISensorEventListener.h"
#include "events/ISensorEventListener_vtbl.h"
#include "ISensor.h"
#include "ISensor_vtbl.h"
#include "SMMessageParser.h"
#include "SMUtilTask.h"
#include "events/SensorEventSrc.h"
#include "events/SensorEventSrc_vtbl.h"



#ifndef IMP23ABSU_TASK_CFG_STACK_DEPTH
#define IMP23ABSU_TASK_CFG_STACK_DEPTH           120
#endif

#ifndef IMP23ABSU_TASK_CFG_PRIORITY
#define IMP23ABSU_TASK_CFG_PRIORITY              (tskIDLE_PRIORITY)
#endif

#define SYS_DEBUGF(level, message)               SYS_DEBUGF3(SYS_DBG_IMP23ABSU, level, message)

#if defined(DEBUG) || defined (SYS_DEBUG)
#define sTaskObj                                 sIMP23ABSUTaskObj
#endif

#define WRITE_BUFFER_SIZE_IMP23ABSU       (uint32_t)(32768 * 3)
//#define AMIC_SAMPLING_FREQUENCY                  (uint32_t)(192000)
#define AMIC_SAMPLING_FREQUENCY                  (uint32_t)(16000)
#define AMIC_MS                                  (uint32_t)(1)

#ifndef M_PI
#define M_PI                                     3.14159265358979323846f
#endif

#define  IMP23ABSU_TASK_CFG_IN_QUEUE_LENGTH      100
#define  IMP23ABSU_TASK_CFG_IN_QUEUE_ITEM_SIZE   sizeof(SMMessage)

#ifndef HSD_USE_DUMMY_DATA
#define HSD_USE_DUMMY_DATA 0
#endif

#if (HSD_USE_DUMMY_DATA == 1)
static uint16_t dummyDataCounter = 0;
#endif


static const ISensor_vtbl sIMP23ABSUTask_MicSensorIF_vtbl = {
    IMP23ABSUTask_vtblMicGetId,
    IMP23ABSUTask_vtblGetEventSourceIF,
    IMP23ABSUTask_vtblMicGetODR,
    IMP23ABSUTask_vtblMicGetFS,
    IMP23ABSUTask_vtblMicGetSensitivity,
    IMP23ABSUTask_vtblSensorStart,
    IMP23ABSUTask_vtblSensorStop,
    IMP23ABSUTask_vtblSensorSetODR,
    IMP23ABSUTask_vtblSensorSetFS,
    IMP23ABSUTask_vtblSensorEnable,
    IMP23ABSUTask_vtblSensorDisable,
    IMP23ABSUTask_vtblSensorIsEnabled,
    IMP23ABSUTask_vtblSensorGetDescription
};

/**
 *  IMP23ABSUTask internal structure.
 */
struct _IMP23ABSUTask
{
  /**
   * Base class object.
   */
  AManagedTaskEx super;

  /**
   * Driver object.
   */
  IDriver *p_driver;

  /**
   * HAL DFSDM driver configuration parameters.
   */
  const void *p_mx_dfsdm_cfg;

  /**
   *
   */
  const void *p_mx_adc_cfg;

  /**
   * Implements the accelerometer ISensor interface.
   */
  ISensor_t mic_sensor_if;

  /**
   * Specifies sensor capabilities.
   */
  SensorDescriptor_t sensor_descriptor;

  /**
   * Specifies sensor configuration.
   */
  SensorStatus_t sensor_status;

  /**
   * Specifies the sensor ID for the microphone subsensor.
   */
  uint8_t mic_id;

  /**
   * Synchronization object used to send command to the task.
   */
  QueueHandle_t in_queue;

  /**
   * ::IEventSrc interface implementation for this class.
   */
  IEventSrc *p_event_src;

  /**
   * Buffer to store the data read from the sensor
   */
  uint32_t p_sensor_data_buff[((AMIC_SAMPLING_FREQUENCY / 1000) * AMIC_MS * 2)];

  /*
   * Calibration values, used for adjusting audio gain
   */
  int old_in;
  int old_out;

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

  uint8_t half;

};

/**
 * Class object declaration
 */
typedef struct _IMP23ABSUTaskClass {
  /**
   * IMP23ABSUTask class virtual table.
   */
  AManagedTaskEx_vtbl vtbl;

  /**
   * IMP23ABSUTask (PM_STATE, ExecuteStepFunc) map.
   */
  pExecuteStepFunc_t p_pm_state2func_map[];
} IMP23ABSUTaskClass_t;

/**
 * STM32 HAL callback function.
 *
 * @param hdfsdm_filter [IN] specifies a DFSDM instance.
 */
void DFSDM_Filter_1_Complete_Callback(DFSDM_Filter_HandleTypeDef *hdfsdm_filter);

/**
 * STM32 HAL callback function.
 *
 * @param hdfsdm_filter  [IN] specifies a DFSDM instance.
 */
void DFSDM_Filter_1_HalfComplete_Callback(DFSDM_Filter_HandleTypeDef *hdfsdm_filter);


// Private member function declaration
// ***********************************

/**
 * Obtain the task object.
 *
 * @return IMP23ABSUTask task object.
 */
static IMP23ABSUTask * IMP23ABSUTaskGetInstance(void);

/**
 * Execute one step of the task control loop while the system is in RUN mode.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return SYS_NO_EROR_CODE if success, a task specific error code otherwise.
 */
static sys_error_code_t IMP23ABSUTaskExecuteStepRun(AManagedTask *_this);

/**
 * Execute one step of the task control loop while the system is in SENSORS_ACTIVE mode.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return SYS_NO_EROR_CODE if success, a task specific error code otherwise.
 */
static sys_error_code_t IMP23ABSUTaskExecuteStepDatalog(AManagedTask *_this);

/**
 * Read the data from the sensor.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return SYS_NO_EROR_CODE if success, a task specific error code otherwise.
 */
static sys_error_code_t IMP23ABSUTaskSensorReadData(IMP23ABSUTask *_this);

/**
 * Register the sensor with the global DB and initialize the default parameters.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return SYS_NO_ERROR_CODE if success, an error code otherwise
 */
static sys_error_code_t IMP23ABSUTaskSensorRegister(IMP23ABSUTask *_this);

/**
 * Initialize the default parameters.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return SYS_NO_ERROR_CODE if success, an error code otherwise
 */
static sys_error_code_t IMP23ABSUTaskSensorInitTaskParams(IMP23ABSUTask *_this);

/**
 * Private implementation of sensor interface methods for IMP23ABSU sensor
 */
static sys_error_code_t IMP23ABSUTaskSensorStart(IMP23ABSUTask *_this, SMMessage report);
static sys_error_code_t IMP23ABSUTaskSensorStop(IMP23ABSUTask *_this, SMMessage report);
static sys_error_code_t IMP23ABSUTaskSensorSetODR(IMP23ABSUTask *_this, SMMessage report);
static sys_error_code_t IMP23ABSUTaskSensorSetFS(IMP23ABSUTask *_this, SMMessage report);
static sys_error_code_t IMP23ABSUTaskSensorEnable(IMP23ABSUTask *_this, SMMessage report);
static sys_error_code_t IMP23ABSUTaskSensorDisable(IMP23ABSUTask *_this, SMMessage report);

/**
 * Check if the sensor is active. The sensor is active if at least one of the sub sensor is active.
 * @param _this [IN] specifies a pointer to a task object.
 * @return TRUE if the sensor is active, FALSE otherwise.
 */
static boolean_t IMP23ABSUTaskSensorIsActive(const IMP23ABSUTask *_this);


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
static inline sys_error_code_t IMP23ABSUTaskPostReportToFront(IMP23ABSUTask *_this, SMMessage *pReport);

/**
 * Private function used to post a report into the back of the task queue.
 * Used to resume the task when the required by the INIT task.
 *
 * @param this [IN] specifies a pointer to the task object.
 * @param pReport [IN] specifies a report to send.
 * @return SYS_NO_EROR_CODE if success, SYS_APP_TASK_REPORT_LOST_ERROR_CODE.
 */
static inline sys_error_code_t IMP23ABSUTaskPostReportToBack(IMP23ABSUTask *_this, SMMessage *pReport);

#if defined (__GNUC__)
// Inline function defined inline in the header file IMP23ABSUTask.h must be declared here as extern function.
#endif


/* Objects instance */
/********************/

/**
 * The only instance of the task object.
 */
static IMP23ABSUTask sTaskObj;

/**
 * The class object.
 */
static const IMP23ABSUTaskClass_t sTheClass = {
    /* Class virtual table */
    {
        IMP23ABSUTask_vtblHardwareInit,
        IMP23ABSUTask_vtblOnCreateTask,
        IMP23ABSUTask_vtblDoEnterPowerMode,
        IMP23ABSUTask_vtblHandleError,
        IMP23ABSUTask_vtblOnEnterTaskControlLoop,
        IMP23ABSUTask_vtblForceExecuteStep,
        IMP23ABSUTask_vtblOnEnterPowerMode
    },

    /* class (PM_STATE, ExecuteStepFunc) map */
    {
        IMP23ABSUTaskExecuteStepRun,
        NULL,
        IMP23ABSUTaskExecuteStepDatalog,
    }
};


// Public API definition
// *********************

ISourceObservable *IMP23ABSUTaskGetMicSensorIF(IMP23ABSUTask *_this){
  return (ISourceObservable *)&(_this->mic_sensor_if);
}

AManagedTaskEx* IMP23ABSUTaskAlloc(const void *p_mx_dfsdm_cfg, const void *p_mx_adc_cfg)
{
  // In this application there is only one Keyboard task,
  // so this allocator implement the singleton design pattern.

  // Initialize the super class
  AMTInitEx(&sTaskObj.super);

  sTaskObj.super.vptr = &sTheClass.vtbl;
  sTaskObj.p_mx_adc_cfg = p_mx_adc_cfg;
  sTaskObj.p_mx_dfsdm_cfg = p_mx_dfsdm_cfg;
  sTaskObj.mic_sensor_if.vptr = &sIMP23ABSUTask_MicSensorIF_vtbl;

  return (AManagedTaskEx*) &sTaskObj;
}

IEventSrc* IMP23ABSUTaskGetEventSrcIF(IMP23ABSUTask *_this)
{
  assert_param(_this);

  return _this->p_event_src;
}

// AManagedTask virtual functions definition
// *****************************************

sys_error_code_t IMP23ABSUTask_vtblHardwareInit(AManagedTask *_this, void *pParams)
{
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  IMP23ABSUTask *p_obj = (IMP23ABSUTask*)_this;

  p_obj->p_driver = AnalogMicDriverAlloc();
  if (p_obj == NULL)
  {
    res = SYS_OUT_OF_MEMORY_ERROR_CODE;
    SYS_SET_LOW_LEVEL_ERROR_CODE(SYS_OUT_OF_MEMORY_ERROR_CODE);
  }
  else
  {
    AnalogMicDriverParams_t cfg_params = {
        .p_mx_adc_cfg = (void*)p_obj->p_mx_adc_cfg,
        .p_mx_dfsdm_cfg = (void*)p_obj->p_mx_dfsdm_cfg
    };

    res = IDrvInit(p_obj->p_driver, &cfg_params);
    if (!SYS_IS_ERROR_CODE(res))
    {
      DFSDMDriverFilterRegisterCallback((DFSDMDriver_t*)p_obj->p_driver, HAL_DFSDM_FILTER_REGCONV_HALFCOMPLETE_CB_ID, DFSDM_Filter_1_HalfComplete_Callback);
      DFSDMDriverFilterRegisterCallback((DFSDMDriver_t*)p_obj->p_driver, HAL_DFSDM_FILTER_REGCONV_COMPLETE_CB_ID, DFSDM_Filter_1_Complete_Callback);
    }
  }

  return res;
}

sys_error_code_t IMP23ABSUTask_vtblOnCreateTask(AManagedTask *_this, TaskFunction_t *pTaskCode, const char **pName, unsigned short *pStackDepth,
                                                void **pParams, UBaseType_t *pPriority)
{
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  IMP23ABSUTask *p_obj = (IMP23ABSUTask*) _this;

  *pTaskCode = AMTExRun;
  *pName = "IMP23ABSU";
  *pStackDepth = IMP23ABSU_TASK_CFG_STACK_DEPTH;
  *pParams = _this;
  *pPriority = IMP23ABSU_TASK_CFG_PRIORITY;

  // Create task specific sw resources.
  p_obj->in_queue = xQueueCreate(IMP23ABSU_TASK_CFG_IN_QUEUE_LENGTH, IMP23ABSU_TASK_CFG_IN_QUEUE_ITEM_SIZE);
  if(p_obj->in_queue == NULL)
  {
    res = SYS_TASK_HEAP_OUT_OF_MEMORY_ERROR_CODE;
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(res);
    return res;
  }

#ifdef DEBUG
  vQueueAddToRegistry(p_obj->in_queue, "IMP23ABSU_Q");
#endif

  p_obj->p_event_src = (IEventSrc*) SensorEventSrcAlloc();
  if(p_obj->p_event_src == NULL)
  {
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_OUT_OF_MEMORY_ERROR_CODE);
    res = SYS_OUT_OF_MEMORY_ERROR_CODE;
    return res;
  }
  IEventSrcInit(p_obj->p_event_src);

  memset(p_obj->p_sensor_data_buff, 0, sizeof(p_obj->p_sensor_data_buff));
  p_obj->mic_id = 0;
  p_obj->timestamp_tick = 0;
  p_obj->old_timestamp_tick = 0;
  p_obj->timestamp = 0;
  p_obj->half = 0;
  p_obj->old_in = 0;
  p_obj->old_out = 0;
  _this->m_pfPMState2FuncMap = sTheClass.p_pm_state2func_map;

  res = IMP23ABSUTaskSensorInitTaskParams(p_obj);
  if (SYS_IS_ERROR_CODE(res)) {
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_OUT_OF_MEMORY_ERROR_CODE);
    res = SYS_OUT_OF_MEMORY_ERROR_CODE;
    return res;
  }

  res = IMP23ABSUTaskSensorRegister(p_obj);
  if(SYS_IS_ERROR_CODE(res))
  {
    SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("IMP23ABSU: unable to register with DB\r\n"));
    sys_error_handler();
  }

  return res;
}

sys_error_code_t IMP23ABSUTask_vtblDoEnterPowerMode(AManagedTask *_this, const EPowerMode ActivePowerMode, const EPowerMode NewPowerMode)
{
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  IMP23ABSUTask *p_obj = (IMP23ABSUTask*) _this;

  if(IMP23ABSUTaskSensorIsActive(p_obj))
  {
    IDrvDoEnterPowerMode(p_obj->p_driver, ActivePowerMode, NewPowerMode);
  }

  if(NewPowerMode == E_POWER_MODE_SENSORS_ACTIVE)
  {
    if(IMP23ABSUTaskSensorIsActive(p_obj))
    {
      SMMessage xReport =
          { .sensorMessage.messageId = SM_MESSAGE_ID_SENSOR_CMD, .sensorMessage.nCmdID = SENSOR_CMD_ID_START };

      if(xQueueSendToBack(p_obj->in_queue, &xReport, pdMS_TO_TICKS(50)) != pdTRUE)
      {  //TD FF pdMS_TO_TICKS(100)
        res = SYS_APP_TASK_REPORT_LOST_ERROR_CODE;
        SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_APP_TASK_REPORT_LOST_ERROR_CODE);
      }

      // reset the variables for the time stamp computation.
      p_obj->timestamp_tick = 0;
      p_obj->old_timestamp_tick = 0;
      p_obj->timestamp = 0;
    }

    SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("IMP23ABSU: -> SENSORS_ACTIVE\r\n"));
  }
  else if(NewPowerMode == E_POWER_MODE_STATE1)
  {
    if (ActivePowerMode == E_POWER_MODE_SENSORS_ACTIVE)
    {
      xQueueReset(p_obj->in_queue);
    }

    SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("IMP23ABSU: -> STATE1\r\n"));
  }
  else if(NewPowerMode == E_POWER_MODE_SLEEP_1)
  {
    /* the MCU is going in stop so I put the sensor in low power (this code is executed from the INIT task).*/


    SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("IMP23ABSU: -> SLEEP_1\r\n"));
  }
  return res;
}

sys_error_code_t IMP23ABSUTask_vtblHandleError(AManagedTask *_this, SysEvent Error)
{
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  //  IMP23ABSUTask *p_obj = (IMP23ABSUTask*)_this;

  return res;
}

sys_error_code_t IMP23ABSUTask_vtblOnEnterTaskControlLoop(AManagedTask *_this) {
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("IMP23ABSU: start.\r\n"));


#if (configUSE_APPLICATION_TASK_TAG == 1 && defined(DEBUG_PIN_IMP23ABSU))
  vTaskSetApplicationTaskTag( NULL, (TaskHookFunction_t)DEBUG_PIN_IMP23ABSU );
#endif

  return res;
}

// AManagedTaskEx virtual functions definition
// *******************************************

sys_error_code_t IMP23ABSUTask_vtblForceExecuteStep(AManagedTaskEx *_this, EPowerMode ActivePowerMode)
{
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  IMP23ABSUTask *p_obj = (IMP23ABSUTask*) _this;

  SMMessage xReport =
      { .internalMessageFE.messageId = SM_MESSAGE_ID_FORCE_STEP, .internalMessageFE.nData = 0 };

  if((ActivePowerMode == E_POWER_MODE_STATE1) || (ActivePowerMode == E_POWER_MODE_SENSORS_ACTIVE))
  {
    if(AMTExIsTaskInactive(_this))
    {
      res = IMP23ABSUTaskPostReportToFront(p_obj, (SMMessage*) &xReport);
    }
  }
  else
  {
    if(eTaskGetState(_this->m_xThaskHandle) == eSuspended)
    {
      vTaskResume(_this->m_xThaskHandle);
    }
  }
  return res;
}

sys_error_code_t IMP23ABSUTask_vtblOnEnterPowerMode(AManagedTaskEx *_this, const EPowerMode ActivePowerMode, const EPowerMode NewPowerMode)
{
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  //  IMP23ABSUTask *p_obj = (IMP23ABSUTask*) _this;

  return res;
}


// ISensor virtual functions definition
// *******************************************

uint8_t IMP23ABSUTask_vtblMicGetId(ISourceObservable *_this){
  assert_param(_this);
  IMP23ABSUTask *p_imp23absu_instance = IMP23ABSUTaskGetInstance();
  uint8_t res = p_imp23absu_instance->mic_id;

  return res;
}

IEventSrc *IMP23ABSUTask_vtblGetEventSourceIF(ISourceObservable *_this){
  assert_param(_this);
  IMP23ABSUTask *p_imp23absu_instance = IMP23ABSUTaskGetInstance();
  return p_imp23absu_instance->p_event_src;
}

sys_error_code_t IMP23ABSUTask_vtblMicGetODR(ISourceObservable *_this, float *p_measured, float *p_nominal){
  assert_param(_this != NULL);
  /*get the object implementing the ISourceObservable IF */
  IMP23ABSUTask *p_if_owner = (IMP23ABSUTask*)((uint32_t)_this - offsetof(IMP23ABSUTask, mic_sensor_if));
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

float IMP23ABSUTask_vtblMicGetFS(ISourceObservable *_this){
  assert_param(_this);
  IMP23ABSUTask *p_imp23absu_instance = IMP23ABSUTaskGetInstance();
  float res = p_imp23absu_instance->sensor_status.pSubSensorStatus[0].FS;

  return res;
}

float IMP23ABSUTask_vtblMicGetSensitivity(ISourceObservable *_this){
  assert_param(_this);
  IMP23ABSUTask *p_imp23absu_instance = IMP23ABSUTaskGetInstance();
  float res = p_imp23absu_instance->sensor_status.pSubSensorStatus[0].Sensitivity;

  return res;
}

sys_error_code_t IMP23ABSUTask_vtblSensorStart(ISensor_t *_this){
  assert_param(_this);
  sys_error_code_t res = SYS_NOT_IMPLEMENTED_ERROR_CODE;

  return res;
}

sys_error_code_t IMP23ABSUTask_vtblSensorStop(ISensor_t *_this){
  assert_param(_this);
  sys_error_code_t res = SYS_NOT_IMPLEMENTED_ERROR_CODE;

  return res;
}

sys_error_code_t IMP23ABSUTask_vtblSensorSetODR(ISensor_t *_this, float ODR){
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  IMP23ABSUTask *p_imp23absu_instance = IMP23ABSUTaskGetInstance();
  EPowerMode log_status = AMTGetTaskPowerMode((AManagedTask *)p_imp23absu_instance);
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
    res = IMP23ABSUTaskPostReportToBack(p_imp23absu_instance, (SMMessage*)&report);
  }

  return res;
}

sys_error_code_t IMP23ABSUTask_vtblSensorSetFS(ISensor_t *_this, float FS){
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  IMP23ABSUTask *p_imp23absu_instance = IMP23ABSUTaskGetInstance();
  EPowerMode log_status = AMTGetTaskPowerMode((AManagedTask *)p_imp23absu_instance);
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
    res = IMP23ABSUTaskPostReportToBack(p_imp23absu_instance, (SMMessage*)&report);
  }

  return res;
}

sys_error_code_t IMP23ABSUTask_vtblSensorEnable(ISensor_t *_this){
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  IMP23ABSUTask *p_imp23absu_instance = IMP23ABSUTaskGetInstance();
  EPowerMode log_status = AMTGetTaskPowerMode((AManagedTask *)p_imp23absu_instance);
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
    res = IMP23ABSUTaskPostReportToBack(p_imp23absu_instance, (SMMessage*)&report);
  }

  return res;
}

sys_error_code_t IMP23ABSUTask_vtblSensorDisable(ISensor_t *_this){
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  IMP23ABSUTask *p_imp23absu_instance = IMP23ABSUTaskGetInstance();
  EPowerMode log_status = AMTGetTaskPowerMode((AManagedTask *)p_imp23absu_instance);
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
    res = IMP23ABSUTaskPostReportToBack(p_imp23absu_instance, (SMMessage*)&report);
  }

  return res;
}

boolean_t IMP23ABSUTask_vtblSensorIsEnabled(ISensor_t *_this){
  boolean_t res = FALSE;

  IMP23ABSUTask *p_imp23absu_instance = IMP23ABSUTaskGetInstance();

  if(ISourceGetId((ISourceObservable *)_this) == p_imp23absu_instance->mic_id)
    res = p_imp23absu_instance->sensor_status.pSubSensorStatus[0].IsActive;

  return res;
}

SensorDescriptor_t IMP23ABSUTask_vtblSensorGetDescription(ISensor_t *_this){

  IMP23ABSUTask *p_imp23absu_instance = IMP23ABSUTaskGetInstance();
  return p_imp23absu_instance->sensor_descriptor;
}


// Private function definition
// ***************************

static IMP23ABSUTask* IMP23ABSUTaskGetInstance(){
  return (IMP23ABSUTask*)&sTaskObj;
}

static sys_error_code_t IMP23ABSUTaskExecuteStepRun(AManagedTask *_this)
{
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  IMP23ABSUTask *p_obj = (IMP23ABSUTask*)_this;
  SMMessage report = {0};

  AMTExSetInactiveState((AManagedTaskEx*) _this, TRUE);
  if(pdTRUE == xQueueReceive(p_obj->in_queue, &report, portMAX_DELAY))
  {
    AMTExSetInactiveState((AManagedTaskEx*) _this, FALSE);

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
            res = IMP23ABSUTaskSensorStart(p_obj, report);
            break;
          case SENSOR_CMD_ID_STOP:
            res = IMP23ABSUTaskSensorStop(p_obj, report);
            break;
          case SENSOR_CMD_ID_SET_ODR:
            res = IMP23ABSUTaskSensorSetODR(p_obj, report);
            break;
          case SENSOR_CMD_ID_SET_FS:
            res = IMP23ABSUTaskSensorSetFS(p_obj, report);
            break;
          case SENSOR_CMD_ID_ENABLE:
            res = IMP23ABSUTaskSensorEnable(p_obj, report);
            break;
          case SENSOR_CMD_ID_DISABLE:
            res = IMP23ABSUTaskSensorDisable(p_obj, report);
            break;
          default:
            // unwanted report
            res = SYS_APP_TASK_UNKNOWN_REPORT_ERROR_CODE;
            SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_APP_TASK_UNKNOWN_REPORT_ERROR_CODE);

            SYS_DEBUGF(SYS_DBG_LEVEL_WARNING, ("IMP23ABSU: unexpected report in Run: %i\r\n", report.messageID));
            SYS_DEBUGF3(SYS_DBG_APP, SYS_DBG_LEVEL_WARNING, ("IMP23ABSU: unexpected report in Run: %i\r\n", report.messageID));
            break;
        }
        break;
      }
      default:
      {
        // unwanted report
        res = SYS_APP_TASK_UNKNOWN_REPORT_ERROR_CODE;
        SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_APP_TASK_UNKNOWN_REPORT_ERROR_CODE);

        SYS_DEBUGF(SYS_DBG_LEVEL_WARNING, ("IMP23ABSU: unexpected report in Run: %i\r\n", report.messageID));
        SYS_DEBUGF3(SYS_DBG_APP, SYS_DBG_LEVEL_WARNING, ("IMP23ABSU: unexpected report in Run: %i\r\n", report.messageID));
        break;
      }
    }
  }

  return res;
}

static sys_error_code_t IMP23ABSUTaskExecuteStepDatalog(AManagedTask *_this)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  IMP23ABSUTask *p_obj = (IMP23ABSUTask*)_this;
  SMMessage report = {0};

  static int s_counter = 0;

  AMTExSetInactiveState((AManagedTaskEx*) _this, TRUE);
  if(pdTRUE == xQueueReceive(p_obj->in_queue, &report, portMAX_DELAY))
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
      case SM_MESSAGE_ID_IMP23ABSU:
      {
        /*SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("IMP23ABSU: new data.\r\n"));*/

        p_obj->half = report.imp23absuMessage.half;
        res = IMP23ABSUTaskSensorReadData(p_obj);

        if (++s_counter > 20000) {
          GPIOD->ODR ^= GPIO_PIN_0;
          s_counter = 0;
        }

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
          double delta_timestamp = (double)period/(double)(SystemCoreClock);

          SensorEvent evt;

          AI_SP_Stream_t stream = {
              .packet.payload = (uint8_t * )&p_obj->p_sensor_data_buff[(p_obj->half - 1) * ((AMIC_SAMPLING_FREQUENCY / 1000) * AMIC_MS )],
              .packet.payload_fmt = AI_SP_FMT_INT16_RESET(),
              .mode = AI_SP_MODE_FULL
              // bonus question: what is AI_LOGGING_SHAPES_DEPTH ??
              // (can I represent anomogeneous matrix [4*4] with this data format ?
          };
          ai_logging_create_shape_1d(&stream.packet.shape, ((AMIC_SAMPLING_FREQUENCY / 1000) * AMIC_MS));

          SensorEventInit((IEvent*) &evt, p_obj->p_event_src, (ai_logging_packet_t*)&stream, timestamp, p_obj->mic_id);
          IEventSrcSendEvent(p_obj->p_event_src, (IEvent*) &evt, NULL);

          /* update measuredODR */
          p_obj->sensor_status.pSubSensorStatus[0].MeasuredODR = ((AMIC_SAMPLING_FREQUENCY / 1000) * AMIC_MS)/delta_timestamp;

          /*SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("IMP23ABSU: ts = %f\r\n", (float)timestamp));*/
        }
        break;
      }
      case SM_MESSAGE_ID_SENSOR_CMD:
      {
        switch (report.sensorMessage.nCmdID)
        {
          case SENSOR_CMD_ID_START:
            res = AnalogMicDrvSetDataBuffer((AnalogMicDriver_t*)p_obj->p_driver, p_obj->p_sensor_data_buff, (AMIC_SAMPLING_FREQUENCY / 1000) * AMIC_MS * 2);
            if (!SYS_IS_ERROR_CODE(res))
            {
              res = IDrvStart(p_obj->p_driver);
            }
            break;
          case SENSOR_CMD_ID_STOP:
            res = IMP23ABSUTaskSensorStop(p_obj, report);
            break;
          case SENSOR_CMD_ID_SET_ODR:
            res = IMP23ABSUTaskSensorSetODR(p_obj, report);
            break;
          case SENSOR_CMD_ID_SET_FS:
            res = IMP23ABSUTaskSensorSetFS(p_obj, report);
            break;
          case SENSOR_CMD_ID_ENABLE:
            res = IMP23ABSUTaskSensorEnable(p_obj, report);
            break;
          case SENSOR_CMD_ID_DISABLE:
            res = IMP23ABSUTaskSensorDisable(p_obj, report);
            break;
          default:
            // unwanted report
            res = SYS_APP_TASK_UNKNOWN_REPORT_ERROR_CODE;
            SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_APP_TASK_UNKNOWN_REPORT_ERROR_CODE);

            SYS_DEBUGF(SYS_DBG_LEVEL_WARNING, ("IMP23ABSU: unexpected report in Datalog: %i\r\n", report.messageID));
            SYS_DEBUGF3(SYS_DBG_APP, SYS_DBG_LEVEL_WARNING, ("IMP23ABSU: unexpected report in Datalog: %i\r\n", report.messageID));
            break;
        }
        break;
      }
      default:
      {
        // unwanted report
        res = SYS_APP_TASK_UNKNOWN_REPORT_ERROR_CODE;
        SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_APP_TASK_UNKNOWN_REPORT_ERROR_CODE);

        SYS_DEBUGF(SYS_DBG_LEVEL_WARNING, ("IMP23ABSU: unexpected report in Datalog: %i\r\n", report.messageID));
        SYS_DEBUGF3(SYS_DBG_APP, SYS_DBG_LEVEL_WARNING, ("IMP23ABSU: unexpected report in Datalog: %i\r\n", report.messageID));
        break;
      }
    }
  }

  return res;
}

static inline sys_error_code_t IMP23ABSUTaskPostReportToFront(IMP23ABSUTask *_this, SMMessage *pReport)
{
  assert_param(_this);
  assert_param(pReport);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  if(SYS_IS_CALLED_FROM_ISR())
  {
    if(pdTRUE != xQueueSendToFrontFromISR(_this->in_queue, pReport, NULL))
    {
      res = SYS_APP_TASK_REPORT_LOST_ERROR_CODE;
      // this function is private and the caller will ignore this return code.
    }
  }
  else
  {
    if(pdTRUE != xQueueSendToFront(_this->in_queue, pReport, pdMS_TO_TICKS(100)))
    {
      res = SYS_APP_TASK_REPORT_LOST_ERROR_CODE;
      // this function is private and the caller will ignore this return code.
    }
  }

  return res;
}

static inline sys_error_code_t IMP23ABSUTaskPostReportToBack(IMP23ABSUTask *_this, SMMessage *pReport)
{
  assert_param(_this);
  assert_param(pReport);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  if(SYS_IS_CALLED_FROM_ISR())
  {
    if(pdTRUE != xQueueSendToBackFromISR(_this->in_queue, pReport, NULL))
    {
      res = SYS_APP_TASK_REPORT_LOST_ERROR_CODE;
      // this function is private and the caller will ignore this return code.
    }
  }
  else
  {
    if(pdTRUE != xQueueSendToBack(_this->in_queue, pReport, pdMS_TO_TICKS(100)))
    {
      res = SYS_APP_TASK_REPORT_LOST_ERROR_CODE;
      // this function is private and the caller will ignore this return code.
    }
  }

  return res;
}

static sys_error_code_t IMP23ABSUTaskSensorReadData(IMP23ABSUTask *_this)
{
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  void *p_data_ptr = &_this->p_sensor_data_buff[(_this->half - 1) * ((AMIC_SAMPLING_FREQUENCY / 1000) * AMIC_MS )]; /* void since it is independent from data format*/
  //  /* Do something */
  uint16_t idx = 0;

#if (HSD_USE_DUMMY_DATA != 1)
  int32_t *p32 = (int32_t*) p_data_ptr;
#endif
  int16_t *p16 = (int16_t*) p_data_ptr;

  for(idx = 0; idx < ((AMIC_SAMPLING_FREQUENCY / 1000) * AMIC_MS ); idx++)
  {
#if (HSD_USE_DUMMY_DATA == 1)
    *p16++ = dummyDataCounter++;
#else
    *p16++ = _this->old_out = (0xFC * (_this->old_out + ((*p32) >> 10) - _this->old_in)) / 0xFF;
    _this->old_in = (*p32++) >> 10;
#endif
  }

  return res;
}


static sys_error_code_t IMP23ABSUTaskSensorRegister(IMP23ABSUTask *_this)
{
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  ISensor_t *mic_if = (ISensor_t *)IMP23ABSUTaskGetMicSensorIF(_this);
  _this->mic_id = SMAddSensor(mic_if);

  return res;
}


static sys_error_code_t IMP23ABSUTaskSensorInitTaskParams(IMP23ABSUTask *_this) {
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  /* SENSOR DESCRIPTOR */
  strcpy(_this->sensor_descriptor.Name, "IMP23ABSU");
  _this->sensor_descriptor.NumberSubSensors = 1;

  /* SUBSENSOR 0 DESCRIPTOR */
  _this->sensor_descriptor.pSubSensorDescriptor[0].IdSub = _this->mic_id;
  _this->sensor_descriptor.pSubSensorDescriptor[0].SensorType = COM_TYPE_MIC;
  _this->sensor_descriptor.pSubSensorDescriptor[0].DataType = DATA_TYPE_INT16;
//  _this->sensor_descriptor.pSubSensorDescriptor[0].pODR[0] = 192000.0f;
  _this->sensor_descriptor.pSubSensorDescriptor[0].pODR[0] = 16000.0f;
  _this->sensor_descriptor.pSubSensorDescriptor[0].pODR[1] = COM_END_OF_LIST_FLOAT;  /* Terminate list */
  _this->sensor_descriptor.pSubSensorDescriptor[0].pSamplesPerTimestamp[0] = 0;
  _this->sensor_descriptor.pSubSensorDescriptor[0].pSamplesPerTimestamp[1] = 1000;
  _this->sensor_descriptor.pSubSensorDescriptor[0].Dimensions = 1;
  strcpy(_this->sensor_descriptor.pSubSensorDescriptor[0].DimensionsLabel[0], "aud");
  strcpy(_this->sensor_descriptor.pSubSensorDescriptor[0].unit, "Waveform");
  _this->sensor_descriptor.pSubSensorDescriptor[0].pFS[0] = 130.0f;
  _this->sensor_descriptor.pSubSensorDescriptor[0].pFS[4] = COM_END_OF_LIST_FLOAT;

  /* SUBSENSOR 0 STATUS */
  _this->sensor_status.pSubSensorStatus[0].IsActive = TRUE;
  _this->sensor_status.pSubSensorStatus[0].FS = 130.0f;
  _this->sensor_status.pSubSensorStatus[0].Sensitivity = 1.0f;
//  _this->sensor_status.pSubSensorStatus[0].ODR = 192000.0f;
  _this->sensor_status.pSubSensorStatus[0].ODR = 16000.0f;
  _this->sensor_status.pSubSensorStatus[0].MeasuredODR = 0.0f;
  _this->sensor_status.pSubSensorStatus[0].InitialOffset = 0.0f;
#if (HSD_USE_DUMMY_DATA == 1)
  _this->sensor_status.pSubSensorStatus[0].SamplesPerTimestamp = 0;
#else
  _this->sensor_status.pSubSensorStatus[0].SamplesPerTimestamp = 1000;
#endif

  return res;
}


static sys_error_code_t IMP23ABSUTaskSensorStart(IMP23ABSUTask *_this, SMMessage report){
  assert_param(_this);
  sys_error_code_t res = SYS_NOT_IMPLEMENTED_ERROR_CODE;

  return res;
}

static sys_error_code_t IMP23ABSUTaskSensorStop(IMP23ABSUTask *_this, SMMessage report){
  assert_param(_this);
  sys_error_code_t res = SYS_NOT_IMPLEMENTED_ERROR_CODE;

  return res;
}

static sys_error_code_t IMP23ABSUTaskSensorSetODR(IMP23ABSUTask *_this, SMMessage report){
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  float ODR = (float)report.sensorMessage.nParam;
  uint8_t id = report.sensorMessage.nSensorId;

  if (id == _this->mic_id)
  {
//    if(ODR != 192000.0f)
    if(ODR != 16000.0f)
        res = SYS_INVALID_PARAMETER_ERROR_CODE;
    if (!SYS_IS_ERROR_CODE(res))
    {
      _this->sensor_status.pSubSensorStatus[0].ODR = ODR;
      _this->sensor_status.pSubSensorStatus[0].MeasuredODR = 0.0f;
    }
  }
  else
  {
    res = SYS_INVALID_PARAMETER_ERROR_CODE;
  }

  return res;
}

static sys_error_code_t IMP23ABSUTaskSensorSetFS(IMP23ABSUTask *_this, SMMessage report){
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  float FS = (float)report.sensorMessage.nParam;
  uint8_t id = report.sensorMessage.nSensorId;

  if(id == _this->mic_id)
  {
    if(FS != 130.0f)
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

static sys_error_code_t IMP23ABSUTaskSensorEnable(IMP23ABSUTask *_this, SMMessage report){
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  uint8_t id = report.sensorMessage.nSensorId;

  if(id == _this->mic_id)
    _this->sensor_status.pSubSensorStatus[0].IsActive = TRUE;
  else
    res = SYS_INVALID_PARAMETER_ERROR_CODE;

  return res;
}

static sys_error_code_t IMP23ABSUTaskSensorDisable(IMP23ABSUTask *_this, SMMessage report){
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  uint8_t id = report.sensorMessage.nSensorId;

  if(id == _this->mic_id)
    _this->sensor_status.pSubSensorStatus[0].IsActive = FALSE;
  else
    res = SYS_INVALID_PARAMETER_ERROR_CODE;

  return res;
}

static boolean_t IMP23ABSUTaskSensorIsActive(const IMP23ABSUTask *_this)
{
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

void DFSDM_Filter_1_Complete_Callback(DFSDM_Filter_HandleTypeDef *hdfsdm_filter)
{
  struct imp23absuMessage_t report =
      { .messageId = SM_MESSAGE_ID_IMP23ABSU, .half = 2, .bDataReady = 1 };

  if(sTaskObj.in_queue != NULL)
  {
    if(pdTRUE != xQueueSendToBackFromISR(sTaskObj.in_queue, &report, NULL))
    {
      // unable to send the report. Signal the error
      //sys_error_handler();
    }
    sTaskObj.timestamp_tick = SMUtilGetTimeStamp();
  }
}

void DFSDM_Filter_1_HalfComplete_Callback(DFSDM_Filter_HandleTypeDef *hdfsdm_filter)
{
  struct imp23absuMessage_t report =
      { .messageId = SM_MESSAGE_ID_IMP23ABSU, .half = 1, .bDataReady = 1 };

  if(sTaskObj.in_queue != NULL)
  {
    if(pdTRUE != xQueueSendToBackFromISR(sTaskObj.in_queue, &report, NULL))
    {
      // unable to send the report. Signal the error
      //sys_error_handler();
    }
    sTaskObj.timestamp_tick = SMUtilGetTimeStamp();
  }
}


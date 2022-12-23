/**
 ******************************************************************************
 * @file    IMP23ABSUTask.c
 * @author  SRA - MCD
 * @version 1.1.0
 * @date    10-Dec-2021
 *
 * @brief
 *
 * <DESCRIPTIOM>
 *
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
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
#include "services/SysTimestamp.h"
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

#define SYS_DEBUGF(level, message)               SYS_DEBUGF3(SYS_DBG_HTS221/*SYS_DBG_IMP23ABSU*/, level, message)

#if defined(DEBUG) || defined (SYS_DEBUG)
#define sTaskObj                                 sIMP23ABSUTaskObj
#endif

#define WRITE_BUFFER_SIZE_IMP23ABSU       (uint32_t)(32768 * 3)
#define AMIC_SAMPLING_FREQUENCY                  (uint32_t)(192000)
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
   * Specifies mic sensor capabilities.
   */
  const SensorDescriptor_t *mic_sensor_descriptor;

  /**
   * Specifies mic sensor configuration.
   */
  SensorStatus_t mic_sensor_status;

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
   * Used to update the instantaneous ODR.
   */
  double prev_timestamp;

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
   * Microphone IF virtual table.
   */
  ISensor_vtbl mic_sensor_if_vtbl;

  /**
   * Specifies mic sensor capabilities.
   */
  SensorDescriptor_t mic_class_descriptor;

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

    /* class::mic_sensor_if_vtbl virtual table */
    {
        IMP23ABSUTask_vtblMicGetId,
        IMP23ABSUTask_vtblGetEventSourceIF,
        IMP23ABSUTask_vtblMicGetODR,
        IMP23ABSUTask_vtblMicGetFS,
        IMP23ABSUTask_vtblMicGetSensitivity,
        IMP23ABSUTask_vtblMicGetDataType,
        IMP23ABSUTask_vtblMicGetDimensions,
        IMP23ABSUTask_vtblSensorSetODR,
        IMP23ABSUTask_vtblSensorSetFS,
        IMP23ABSUTask_vtblSensorEnable,
        IMP23ABSUTask_vtblSensorDisable,
        IMP23ABSUTask_vtblSensorIsEnabled,
        IMP23ABSUTask_vtblSensorGetDescription,
        IMP23ABSUTask_vtblSensorGetStatus
    },

    /* MIC DESCRIPTOR */
    {
        "imp23absu",
        COM_TYPE_MIC,
        {
            16000.0,
            32000.0,
            48000.0,
            96000.0,
            192000.0,
            COM_END_OF_LIST_FLOAT,
        },
        {
            130.0,
            COM_END_OF_LIST_FLOAT,
        },
        {
            "aud",
        },
        "Waveform"
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
  sTaskObj.mic_sensor_if.vptr = &sTheClass.mic_sensor_if_vtbl;
  sTaskObj.mic_sensor_descriptor = &sTheClass.mic_class_descriptor;

  return (AManagedTaskEx*) &sTaskObj;
}

IEventSrc* IMP23ABSUTaskGetEventSrcIF(IMP23ABSUTask *_this)
{
  assert_param(_this != NULL);

  return _this->p_event_src;
}

// AManagedTask virtual functions definition
// *****************************************

sys_error_code_t IMP23ABSUTask_vtblHardwareInit(AManagedTask *_this, void *pParams)
{
  assert_param(_this != NULL);
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
  assert_param(_this != NULL);
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
  p_obj->prev_timestamp = 0.0f;
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
  assert_param(_this != NULL);
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
          { .sensorMessage.messageId = SM_MESSAGE_ID_SENSOR_CMD, .sensorMessage.nCmdID = SENSOR_CMD_ID_INIT };

      if(xQueueSendToBack(p_obj->in_queue, &xReport, pdMS_TO_TICKS(50)) != pdTRUE)
      {  //TD FF pdMS_TO_TICKS(100)
        res = SYS_APP_TASK_REPORT_LOST_ERROR_CODE;
        SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_APP_TASK_REPORT_LOST_ERROR_CODE);
      }

      // reset the variables for the actual ODR computation.
      p_obj->prev_timestamp = 0.0f;
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
  assert_param(_this != NULL);
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
  assert_param(_this != NULL);
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
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  //  IMP23ABSUTask *p_obj = (IMP23ABSUTask*) _this;

  return res;
}


// ISensor virtual functions definition
// *******************************************

uint8_t IMP23ABSUTask_vtblMicGetId(ISourceObservable *_this){
  assert_param(_this != NULL);
  IMP23ABSUTask *p_if_owner = (IMP23ABSUTask*)((uint32_t)_this - offsetof(IMP23ABSUTask, mic_sensor_if));
  uint8_t res = p_if_owner->mic_id;

  return res;
}

IEventSrc *IMP23ABSUTask_vtblGetEventSourceIF(ISourceObservable *_this){
  assert_param(_this != NULL);
  IMP23ABSUTask *p_if_owner = (IMP23ABSUTask*)((uint32_t)_this - offsetof(IMP23ABSUTask, mic_sensor_if));
  return p_if_owner->p_event_src;
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
  	*p_measured =  p_if_owner->mic_sensor_status.MeasuredODR;
  	*p_nominal =  p_if_owner->mic_sensor_status.ODR;
  }

  return res;
}

float IMP23ABSUTask_vtblMicGetFS(ISourceObservable *_this){
  assert_param(_this != NULL);
  IMP23ABSUTask *p_if_owner = (IMP23ABSUTask*)((uint32_t)_this - offsetof(IMP23ABSUTask, mic_sensor_if));
  float res = p_if_owner->mic_sensor_status.FS;

  return res;
}

float IMP23ABSUTask_vtblMicGetSensitivity(ISourceObservable *_this){
  assert_param(_this != NULL);
  IMP23ABSUTask *p_if_owner = (IMP23ABSUTask*)((uint32_t)_this - offsetof(IMP23ABSUTask, mic_sensor_if));
  float res = p_if_owner->mic_sensor_status.Sensitivity;

  return res;
}

uint8_t IMP23ABSUTask_vtblMicGetDataType(ISourceObservable *_this){
  assert_param(_this != NULL);
  IMP23ABSUTask *p_if_owner = (IMP23ABSUTask*)((uint32_t)_this - offsetof(IMP23ABSUTask, mic_sensor_if));
  uint8_t res = p_if_owner->mic_sensor_status.DataType;

  return res;
}

uint16_t IMP23ABSUTask_vtblMicGetDimensions(ISourceObservable *_this){
  assert_param(_this != NULL);
  IMP23ABSUTask *p_if_owner = (IMP23ABSUTask*)((uint32_t)_this - offsetof(IMP23ABSUTask, mic_sensor_if));
  uint16_t res = p_if_owner->mic_sensor_status.Dimensions;

  return res;
}

sys_error_code_t IMP23ABSUTask_vtblSensorSetODR(ISensor_t *_this, float ODR){
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  IMP23ABSUTask *p_if_owner = (IMP23ABSUTask*)((uint32_t)_this - offsetof(IMP23ABSUTask, mic_sensor_if));

  EPowerMode log_status = AMTGetTaskPowerMode((AManagedTask *)p_if_owner);
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
    res = IMP23ABSUTaskPostReportToBack(p_if_owner, (SMMessage*)&report);
  }

  return res;
}

sys_error_code_t IMP23ABSUTask_vtblSensorSetFS(ISensor_t *_this, float FS){
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  IMP23ABSUTask *p_if_owner = (IMP23ABSUTask*)((uint32_t)_this - offsetof(IMP23ABSUTask, mic_sensor_if));

  EPowerMode log_status = AMTGetTaskPowerMode((AManagedTask *)p_if_owner);
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
    res = IMP23ABSUTaskPostReportToBack(p_if_owner, (SMMessage*)&report);
  }

  return res;
}

sys_error_code_t IMP23ABSUTask_vtblSensorEnable(ISensor_t *_this){
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  IMP23ABSUTask *p_if_owner = (IMP23ABSUTask*)((uint32_t)_this - offsetof(IMP23ABSUTask, mic_sensor_if));

  EPowerMode log_status = AMTGetTaskPowerMode((AManagedTask *)p_if_owner);
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
    res = IMP23ABSUTaskPostReportToBack(p_if_owner, (SMMessage*)&report);
  }

  return res;
}

sys_error_code_t IMP23ABSUTask_vtblSensorDisable(ISensor_t *_this){
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  IMP23ABSUTask *p_if_owner = (IMP23ABSUTask*)((uint32_t)_this - offsetof(IMP23ABSUTask, mic_sensor_if));

  EPowerMode log_status = AMTGetTaskPowerMode((AManagedTask *)p_if_owner);
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
    res = IMP23ABSUTaskPostReportToBack(p_if_owner, (SMMessage*)&report);
  }

  return res;
}

boolean_t IMP23ABSUTask_vtblSensorIsEnabled(ISensor_t *_this){
  assert_param(_this != NULL);
  boolean_t res = FALSE;
  IMP23ABSUTask *p_if_owner = (IMP23ABSUTask*)((uint32_t)_this - offsetof(IMP23ABSUTask, mic_sensor_if));

  if(ISourceGetId((ISourceObservable *)_this) == p_if_owner->mic_id)
    res = p_if_owner->mic_sensor_status.IsActive;

  return res;
}

SensorDescriptor_t IMP23ABSUTask_vtblSensorGetDescription(ISensor_t *_this){
  assert_param(_this != NULL);
  IMP23ABSUTask *p_if_owner = (IMP23ABSUTask*)((uint32_t)_this - offsetof(IMP23ABSUTask, mic_sensor_if));

  return *p_if_owner->mic_sensor_descriptor;
}

SensorStatus_t IMP23ABSUTask_vtblSensorGetStatus(ISensor_t *_this){
  assert_param(_this != NULL);
  IMP23ABSUTask *p_if_owner = (IMP23ABSUTask*)((uint32_t)_this - offsetof(IMP23ABSUTask, mic_sensor_if));

  return p_if_owner->mic_sensor_status;
}

// Private function definition
// ***************************

static sys_error_code_t IMP23ABSUTaskExecuteStepRun(AManagedTask *_this)
{
  assert_param(_this != NULL);
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
      case SM_MESSAGE_ID_DATA_READY:
      {
        p_obj->half = report.sensorDataReadyMessage.half;
        res = IMP23ABSUTaskSensorReadData(p_obj);

        if(!SYS_IS_ERROR_CODE(res))
        {
          /* notify the listeners*/
          double timestamp = report.sensorDataReadyMessage.fTimestamp;
          double delta_timestamp = timestamp - p_obj->prev_timestamp;
          p_obj->prev_timestamp = timestamp;

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
          p_obj->mic_sensor_status.MeasuredODR = ((AMIC_SAMPLING_FREQUENCY / 1000) * AMIC_MS)/delta_timestamp;

          SYS_DEBUGF(SYS_DBG_LEVEL_ALL, ("IMP23ABSU: ts = %f\r\n", (float)timestamp));
        }
        break;
      }
      case SM_MESSAGE_ID_SENSOR_CMD:
      {
        switch (report.sensorMessage.nCmdID)
        {
          case SENSOR_CMD_ID_INIT:
            res = AnalogMicDrvSetDataBuffer((AnalogMicDriver_t*)p_obj->p_driver, p_obj->p_sensor_data_buff, (AMIC_SAMPLING_FREQUENCY / 1000) * AMIC_MS * 2);
            if (!SYS_IS_ERROR_CODE(res))
            {
              res = IDrvStart(p_obj->p_driver);
            }
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
  assert_param(_this != NULL);
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
  assert_param(_this != NULL);
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
  assert_param(_this != NULL);
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
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  ISensor_t *mic_if = (ISensor_t *)IMP23ABSUTaskGetMicSensorIF(_this);
  _this->mic_id = SMAddSensor(mic_if);

  return res;
}


static sys_error_code_t IMP23ABSUTaskSensorInitTaskParams(IMP23ABSUTask *_this) {
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  /* MIC STATUS */
  _this->mic_sensor_status.DataType = DATA_TYPE_INT16;
  _this->mic_sensor_status.Dimensions = 1;
  _this->mic_sensor_status.IsActive = TRUE;
  _this->mic_sensor_status.FS = 130.0f;
  _this->mic_sensor_status.Sensitivity = 1.0f;
  _this->mic_sensor_status.ODR = 192000.0f;
  _this->mic_sensor_status.MeasuredODR = 0.0f;
  return res;
}

static sys_error_code_t IMP23ABSUTaskSensorSetODR(IMP23ABSUTask *_this, SMMessage report){
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  float ODR = (float)report.sensorMessage.nParam;
  uint8_t id = report.sensorMessage.nSensorId;

  if (id == _this->mic_id)
  {
    if(ODR != 192000.0f)
      res = SYS_INVALID_PARAMETER_ERROR_CODE;
    if (!SYS_IS_ERROR_CODE(res))
    {
      _this->mic_sensor_status.ODR = ODR;
      _this->mic_sensor_status.MeasuredODR = 0.0f;
    }
  }
  else
  {
    res = SYS_INVALID_PARAMETER_ERROR_CODE;
  }

  return res;
}

static sys_error_code_t IMP23ABSUTaskSensorSetFS(IMP23ABSUTask *_this, SMMessage report){
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  float FS = (float)report.sensorMessage.nParam;
  uint8_t id = report.sensorMessage.nSensorId;

  if(id == _this->mic_id)
  {
    if(FS != 130.0f)
      res = SYS_INVALID_PARAMETER_ERROR_CODE;

    if (!SYS_IS_ERROR_CODE(res))
    {
      _this->mic_sensor_status.FS = FS;
    }
  }
  else
  {
    res = SYS_INVALID_PARAMETER_ERROR_CODE;
  }

  return res;
}

static sys_error_code_t IMP23ABSUTaskSensorEnable(IMP23ABSUTask *_this, SMMessage report){
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  uint8_t id = report.sensorMessage.nSensorId;

  if(id == _this->mic_id)
    _this->mic_sensor_status.IsActive = TRUE;
  else
    res = SYS_INVALID_PARAMETER_ERROR_CODE;

  return res;
}

static sys_error_code_t IMP23ABSUTaskSensorDisable(IMP23ABSUTask *_this, SMMessage report){
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  uint8_t id = report.sensorMessage.nSensorId;

  if(id == _this->mic_id)
    _this->mic_sensor_status.IsActive = FALSE;
  else
    res = SYS_INVALID_PARAMETER_ERROR_CODE;

  return res;
}

static boolean_t IMP23ABSUTaskSensorIsActive(const IMP23ABSUTask *_this)
{
  assert_param(_this != NULL);
  return _this->mic_sensor_status.IsActive;
}

void DFSDM_Filter_1_Complete_Callback(DFSDM_Filter_HandleTypeDef *hdfsdm_filter)
{
  SMMessage report = {
      .sensorDataReadyMessage.messageId = SM_MESSAGE_ID_DATA_READY,
      .sensorDataReadyMessage.half = 2,
      .sensorDataReadyMessage.fTimestamp = SysTsGetTimestampF(SysGetTimestampSrv())
  };

  if(sTaskObj.in_queue != NULL)
  {
    if(pdTRUE != xQueueSendToBackFromISR(sTaskObj.in_queue, &report, NULL))
    {
      // unable to send the report. Signal the error
      //sys_error_handler();
    }
  }
}

void DFSDM_Filter_1_HalfComplete_Callback(DFSDM_Filter_HandleTypeDef *hdfsdm_filter)
{
  SMMessage report = {
      .sensorDataReadyMessage.messageId = SM_MESSAGE_ID_DATA_READY,
      .sensorDataReadyMessage.half = 1,
      .sensorDataReadyMessage.fTimestamp = SysTsGetTimestampF(SysGetTimestampSrv())
  };

  if(sTaskObj.in_queue != NULL)
  {
    if(pdTRUE != xQueueSendToBackFromISR(sTaskObj.in_queue, &report, NULL))
    {
      // unable to send the report. Signal the error
      //sys_error_handler();
    }
  }
}


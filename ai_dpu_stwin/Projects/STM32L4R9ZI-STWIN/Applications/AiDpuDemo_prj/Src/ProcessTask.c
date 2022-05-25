/**
 ******************************************************************************
 * @file    ProcessTask.c
 * @author  STMicroelectronics - AIS - MCD Team
 * @version M.m.b
 * @date    Mar 23, 2022
 *
 * @brief
 *
 *
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file in
 * the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 ******************************************************************************
 */

#include "ProcessTask.h"
#include "ProcessTask_vtbl.h"
#include "app_messages_parser.h"
#include "ProcMessagesDef.h"
#include "services/SQuery.h"
#include "task.h"
#include "services/sysdebug.h"


#ifndef PRC_TASK_CFG_STACK_DEPTH
#define PRC_TASK_CFG_STACK_DEPTH            (120)
#endif

#ifndef PRC_TASK_CFG_PRIORITY
#define PRC_TASK_CFG_PRIORITY               (tskIDLE_PRIORITY)
#endif

#ifndef PRC_TASK_CFG_IN_QUEUE_LENGTH
#define PRC_TASK_CFG_IN_QUEUE_LENGTH        10
#endif

#define PRC_TASK_CFG_IN_QUEUE_ITEM_SIZE     (sizeof(struct prcMessage_t))

#define PRC_TASK_DPU_TAG                    (0x30U) // must be equal to CTRL_CMD_PARAM_AI

#ifndef PRC_TASK_CFG_AXIS_NUMBER
#define PRC_TASK_CFG_AXIS_NUMBER            AIDPU_NB_AXIS
#endif

#ifndef PRC_TASK_CFG_DATA_INPUT_USER
#define PRC_TASK_CFG_DATA_INPUT_USER        AIDPU_NB_SAMPLE
#endif

#define AI_LSB_16B                          (1.0F/32768)    // Value of an LSB for a 16 bit signed arithmetic

#define SYS_DEBUGF(level, message)          SYS_DEBUGF3(SYS_DBG_PRC, level, message)

#if defined(DEBUG) || defined (SYS_DEBUG)
#define sTaskObj                            sPrcTaskObj
#endif

/**
 * Class object declaration. The class object encapsulates members that are shared between
 * all instance of the class.
 */
typedef struct _ProcessTaskClass_t {
  /**
   * ProcessTask class virtual table.
   */
  AManagedTaskEx_vtbl vtbl;

  /**
   * ProcessTask class (PM_STATE, ExecuteStepFunc) map. The map is implemente with an array and
   * the key is the index. Number of items of this array must be equal to the number of PM state
   * of the application. If the managed task does nothing in a PM state, then set to NULL the
   * relative entry in the map.
   */
  pExecuteStepFunc_t p_pm_state2func_map[];
} ProcessTaskClass_t;


/* Private member function declaration */
/***************************************/

/**
 * Execute one step of the task control loop while the system is in STATE1.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return SYS_NO_EROR_CODE if success, a task specific error code otherwise.
 */
static sys_error_code_t ProcessTaskExecuteStepState1(AManagedTask *_this);

/**
 * Execute one step of the task control loop while the system is in SENSORS_ACTIVE.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return SYS_NO_EROR_CODE if success, a task specific error code otherwise.
 */
static sys_error_code_t ProcessTaskExecuteStepSensorsActive(AManagedTask *_this);

/**
 * Callback used get the notification from the DPU.
 *
 * @param _this [IN] specifies the DPU that triggered the callback.
 * @param p_param [IN] specifies an application specific parameter.
 */
static void ProcessTaskDPUCallback(IDPU *_this, void* p_param);

/**
 * Disconnect a sensor to the task as data source. Data are collected to form a signal of n axes and signal_size number of sample per axis,
 * and stored in a circular buffer of cb_items signals.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @param p_sensor [IN] specifies a pointer to a sensor.
 * @return SYS_NO_ERROR_CODE if success, and error code otherwise.
 */
static sys_error_code_t ProcessTaskDetachFromSensor(ProcessTask_t *_this, ISourceObservable *p_sensor);

/**
 * Set the default configuration for the sensor. This method is a quick way to do that, only for the purpose of this demo.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @param p_active_sensor_id [OUT} specifies the ID of the active sensor.
 * @return SYS_NO_ERROR_CODE
 */
static sys_error_code_t ProcessTaskSetSensorsConfig(ProcessTask_t *_this, uint16_t *p_active_sensor_id);


/* Inline function forward declaration */
/***************************************/

#if defined (__GNUC__) || defined (__ICCARM__)
/* Inline function defined inline in the header file ProcessTask.h must be declared here as extern function. */
#endif


/**
 * The only instance of the task object.
 */
static ProcessTask_t sTaskObj;

/**
 * The class object.
 */
static const ProcessTaskClass_t sTheClass = {
    /* Class virtual table */
    {
        ProcessTask_vtblHardwareInit,
        ProcessTask_vtblOnCreateTask,
        ProcessTask_vtblDoEnterPowerMode,
        ProcessTask_vtblHandleError,
        ProcessTask_vtblOnEnterTaskControlLoop,
        ProcessTask_vtblForceExecuteStep,
        ProcessTask_vtblOnEnterPowerMode
    },

    /* class (PM_STATE, ExecuteStepFunc) map */
    {
        ProcessTaskExecuteStepState1,
        NULL,
        ProcessTaskExecuteStepSensorsActive,
    }
};

/* Public API definition */
/*************************/

AManagedTaskEx *ProcessTaskAlloc(void)
{
  /* In this application there is only one Keyboard task,
   * so this allocator implement the singleton design pattern.
   */

  /* Initialize the super class */
  AMTInitEx(&sTaskObj.super);

  sTaskObj.super.vptr = &sTheClass.vtbl;;

  return (AManagedTaskEx*)&sTaskObj;
}

sys_error_code_t ProcessTaskAttachToSensorEx(ProcessTask_t *_this, ISourceObservable *p_sensor, uint16_t signal_size, uint8_t axes, uint8_t cb_items)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  /* check if there is a sensor already attached */
  if (_this->p_dpu_buff != NULL)
  {
    vPortFree(_this->p_dpu_buff);
    _this->p_dpu_buff = NULL;
  }


  uint16_t buff_size = AiDPUSetStreamsParam(_this->p_dpu, signal_size, axes, cb_items);
  _this->p_dpu_buff = pvPortMalloc(buff_size);
  if (_this->p_dpu_buff != NULL)
  {
    res = IDPU_AttachToSensor((IDPU*)_this->p_dpu, p_sensor, _this->p_dpu_buff);
    AiDPUSetSensitivity(_this->p_dpu, ISourceGetFS(p_sensor)*AI_LSB_16B);

    SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("PRC: DPU buffer = %i byte\r\n", buff_size));
  }
  else
  {
    res = SYS_OUT_OF_MEMORY_ERROR_CODE;
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_OUT_OF_MEMORY_ERROR_CODE);
  }

  return res;
}

sys_error_code_t ProcessTaskAttachToSensor(ProcessTask_t *_this, ISourceObservable *p_sensor, uint8_t cb_items)
{
  return ProcessTaskAttachToSensorEx(_this, p_sensor, PRC_TASK_CFG_DATA_INPUT_USER, PRC_TASK_CFG_AXIS_NUMBER, cb_items);
}

sys_error_code_t ProcessTaskAddDPUListener(ProcessTask_t *_this, IProcessEventListener *p_listener)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  struct prcMessage_t msg = {
      .msgId = APP_MESSAGE_ID_PRC,
      .cmd_id = PRC_CMD_ADD_DPU_LISTENER,
      .param = (uint32_t)p_listener
  };

  /* first find out if we are in the system initialization phase*/
  if (xTaskGetSchedulerState() != taskSCHEDULER_RUNNING) {
    /* at this moment I cannot send a message to the queue, so I set the DPU listener directly.*/
    IEventSrc *p_evt_src = ADPU_GetEventSrcIF((ADPU*)_this->p_dpu);
    res = IEventSrcAddEventListener(p_evt_src, (IEventListener*)p_listener);
  }
  else
  {
    if (SYS_IS_CALLED_FROM_ISR())
    {
      if (pdTRUE != xQueueSendToBackFromISR(_this->in_queue, &msg, NULL))
      {
        res = SYS_PRC_TASK_IN_QUEUE_FULL_ERROR_CODE;
        SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_PRC_TASK_IN_QUEUE_FULL_ERROR_CODE);
      }
    }
    else
    {
      if (pdTRUE != xQueueSendToBack(_this->in_queue, &msg, pdMS_TO_TICKS(100)))
      {
        res = SYS_PRC_TASK_IN_QUEUE_FULL_ERROR_CODE;
        SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_PRC_TASK_IN_QUEUE_FULL_ERROR_CODE);
      }
    }
  }

  return res;
}

sys_error_code_t ProcessTaskRemoveDPUListener(ProcessTask_t *_this, IProcessEventListener *p_listener)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  struct prcMessage_t msg = {
      .msgId = APP_MESSAGE_ID_PRC,
      .cmd_id = PRC_CMD_REMOVE_DPU_LISTENER,
      .param = (uint32_t)p_listener
  };

  if (SYS_IS_CALLED_FROM_ISR())
  {
    if (pdTRUE != xQueueSendToBackFromISR(_this->in_queue, &msg, NULL))
    {
      res = SYS_PRC_TASK_IN_QUEUE_FULL_ERROR_CODE;
      SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_PRC_TASK_IN_QUEUE_FULL_ERROR_CODE);
    }
  }
  else
  {
    if (pdTRUE != xQueueSendToBack(_this->in_queue, &msg, pdMS_TO_TICKS(100)))
    {
      res = SYS_PRC_TASK_IN_QUEUE_FULL_ERROR_CODE;
      SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_PRC_TASK_IN_QUEUE_FULL_ERROR_CODE);
    }
  }

  return res;
}


/* AManagedTask virtual functions definition */
/*********************************************/

sys_error_code_t ProcessTask_vtblHardwareInit(AManagedTask *_this, void *p_params)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
/*  ProcessTask_t *p_obj = (ProcessTask_t*)_this; */

  __HAL_RCC_CRC_CLK_ENABLE();

  return res;
}

sys_error_code_t ProcessTask_vtblOnCreateTask(AManagedTask *_this, TaskFunction_t *p_task_code, const char **p_name, unsigned short *p_stack_depth, void **p_params, UBaseType_t *p_priority)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  ProcessTask_t *p_obj = (ProcessTask_t*)_this;

  /* initialize the object software resource here. */

  /* allocate and initialize  the DPU*/

//  p_obj->p_dpu = (Dummy_DPU*)Dummy_DPUAlloc();
//  if (p_obj->p_dpu == NULL)
//  {
//    res = SYS_TASK_HEAP_OUT_OF_MEMORY_ERROR_CODE;
//    SYS_SET_SERVICE_LEVEL_ERROR_CODE(res);
//    return res;
//  }
  p_obj->p_dpu = (AiDPU_t*)pvPortMalloc(sizeof(AiDPU_t));
  IDPU *idpu = AiDPUStaticAlloc(p_obj->p_dpu);

  res = IDPU_Init(idpu);
  if (SYS_IS_ERROR_CODE(res))
  {
    return res;
  }
  IDPU_RegisterNotifyCallback((IDPU*)p_obj->p_dpu, ProcessTaskDPUCallback, p_obj);
  ADPU_SetTag((ADPU*)p_obj->p_dpu, PRC_TASK_DPU_TAG); //TODO: STF - what is its purpose?

  /* Change the CLASS for the power mode switch because I want to do the transaction after all
     sensors task. */
  AMTExSetPMClass((AManagedTaskEx*)_this, E_PM_CLASS_1);

  /* create the task input queue.*/
  p_obj->in_queue = xQueueCreate(PRC_TASK_CFG_IN_QUEUE_LENGTH, PRC_TASK_CFG_IN_QUEUE_ITEM_SIZE);
  if (p_obj->in_queue == NULL) {
    res = SYS_TASK_HEAP_OUT_OF_MEMORY_ERROR_CODE;
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(res);
    return res;
  }
#ifdef DEBUG
  vQueueAddToRegistry(p_obj->in_queue, "PRC_Q");
#endif

  /* Change the CLASS for the power mode switch because I want to do the transaction after all
     sensors task. */
  AMTExSetPMClass((AManagedTaskEx*)_this, E_PM_CLASS_1);


  /* set the (PM_STATE, ExecuteStepFunc) map from the class object.  */
  _this->m_pfPMState2FuncMap = sTheClass.p_pm_state2func_map;

  *p_task_code = AMTExRun;
  *p_name = "PRC";
  *p_stack_depth = PRC_TASK_CFG_STACK_DEPTH;
  *p_params = _this;
  *p_priority = PRC_TASK_CFG_PRIORITY;

  return res;
}

sys_error_code_t ProcessTask_vtblDoEnterPowerMode(AManagedTask *_this, const EPowerMode active_power_mode, const EPowerMode new_power_mode)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  ProcessTask_t *p_obj = (ProcessTask_t*)_this;

  struct prcMessage_t msg = {
      .msgId = APP_MESSAGE_ID_PRC
  };

  if (new_power_mode == E_POWER_MODE_STATE1)
  {
    msg.cmd_id = PRC_CMD_STOP_PROCESSING;
    if (xQueueSend(p_obj->in_queue, &msg, pdMS_TO_TICKS(100)) != pdTRUE)
    {
      res = SYS_PRC_TASK_IN_QUEUE_FULL_ERROR_CODE;
      SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_PRC_TASK_IN_QUEUE_FULL_ERROR_CODE);

      SYS_DEBUGF(SYS_DBG_LEVEL_WARNING, ("PRC: input queue full!\r\n"));
    }
  }

  SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("PRC: -> %d \r\n", (uint32_t)new_power_mode));

  return res;
}

sys_error_code_t ProcessTask_vtblHandleError(AManagedTask *_this, SysEvent error)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  /*ProcessTask_t *p_obj = (ProcessTask_t*)_this;*/

  return res;
}

sys_error_code_t ProcessTask_vtblOnEnterTaskControlLoop(AManagedTask *_this)
{
  assert_param(_this != NULL);
  sys_error_code_t xRes = SYS_NO_ERROR_CODE;
  ProcessTask_t *p_obj = (ProcessTask_t*)_this;

  SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("PRC: start.\r\n"));

  SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("PRC: sensors configuration.\r\n"));
  uint16_t active_sensor_id = SI_NULL_SENSOR_ID;
  ProcessTaskSetSensorsConfig(p_obj, &active_sensor_id);

  SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("PRC: connect to sensor %d.\r\n", active_sensor_id));
  ProcessTaskAttachToSensor(p_obj, SMGetSensorObserver(active_sensor_id), 2);

  return xRes;
}


/* AManagedTaskEx virtual functions definition */
/***********************************************/

sys_error_code_t ProcessTask_vtblForceExecuteStep(AManagedTaskEx *_this, EPowerMode active_power_mode)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  ProcessTask_t *p_obj = (ProcessTask_t*)_this;

  struct prcMessage_t msg = {
      .msgId = APP_REPORT_ID_FORCE_STEP
  };
  if (xQueueSendToFront(p_obj->in_queue, &msg, pdMS_TO_TICKS(100)) != pdTRUE)
  {
    res = SYS_PRC_TASK_IN_QUEUE_FULL_ERROR_CODE;
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_PRC_TASK_IN_QUEUE_FULL_ERROR_CODE);

    SYS_DEBUGF(SYS_DBG_LEVEL_WARNING, ("PRC: input queue full!\r\n"));
  }

  return res;
}

sys_error_code_t ProcessTask_vtblOnEnterPowerMode(AManagedTaskEx *_this, const EPowerMode active_power_mode, const EPowerMode new_power_mode)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
/*  ProcessTask_t *p_obj = (ProcessTask_t*)_this; */

  return res;
}


/* Private function definition */
/*******************************/

static sys_error_code_t ProcessTaskExecuteStepState1(AManagedTask *_this)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  ProcessTask_t *p_obj = (ProcessTask_t*)_this;

  struct prcMessage_t msg = {0};

  AMTExSetInactiveState((AManagedTaskEx*)_this, TRUE);
  if (pdTRUE == xQueueReceive(p_obj->in_queue, &msg, portMAX_DELAY))
  {
    AMTExSetInactiveState((AManagedTaskEx*)_this, FALSE);
    if (msg.msgId == APP_MESSAGE_ID_PRC)
    {
      switch (msg.cmd_id)
      {
        case PRC_CMD_STOP_PROCESSING:
          /* release the resource (also memory) allocated by the DPU.*/
          ADPU_Reset((ADPU*)p_obj->p_dpu);
          break;

        case PRC_CMD_CONNECT_TO_SENSOR:
          res = ProcessTaskAttachToSensor(p_obj, (ISourceObservable*)msg.param, msg.sparam);
          break;

        case PRC_CMD_DETACH_FROM_SENSOR:
          res = ProcessTaskDetachFromSensor(p_obj, (ISourceObservable*)msg.param);
          break;

        case PRC_CMD_ADD_DPU_LISTENER:
        {
          IEventSrc *p_evt_src = ADPU_GetEventSrcIF((ADPU*)p_obj->p_dpu);
          res = IEventSrcAddEventListener(p_evt_src, (IEventListener*)msg.param);
        }
        break;

        case PRC_CMD_REMOVE_DPU_LISTENER:
        {
          IEventSrc *p_evt_src = ADPU_GetEventSrcIF((ADPU*)p_obj->p_dpu);
          res = IEventSrcRemoveEventListener(p_evt_src, (IEventListener*)msg.param);
        }
        break;

        case PRC_CMD_SUSPEND_DPU:
          ADPU_Suspend((ADPU*)p_obj->p_dpu);
          SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("PRC: suspend DPU\r\n"));
          break;

        case PRC_CMD_RESUME_DPU:
          ADPU_Resume((ADPU*)p_obj->p_dpu);
          SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("PRC: resume DPU\r\n"));
          break;

        default:
          SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("PRC: unexpected command ID:0x%x\r\n", msg.cmd_id));
          break;
        }
    }
    else if (msg.msgId == APP_REPORT_ID_FORCE_STEP)
    {
      /* we need only to resume the task because of a power mode switch pending. */
      __NOP();
    }
  }

  return res;
}

static sys_error_code_t ProcessTaskExecuteStepSensorsActive(AManagedTask *_this)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  ProcessTask_t *p_obj = (ProcessTask_t*)_this;

  struct prcMessage_t msg = {0};

  AMTExSetInactiveState((AManagedTaskEx*)_this, TRUE);
  if (pdTRUE == xQueueReceive(p_obj->in_queue, &msg, portMAX_DELAY))
  {
    AMTExSetInactiveState((AManagedTaskEx*)_this, FALSE);
    if (msg.msgId == APP_MESSAGE_ID_PRC)
    {
      switch (msg.cmd_id)
      {
        case PRC_CMD_NEW_DATA_READY:
          res = IDPU_Process((IDPU*)p_obj->p_dpu);
          //SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("PRC: processed new data.\r\n"));
          break;

        case PRC_CMD_CONNECT_TO_SENSOR:
          res = ProcessTaskAttachToSensor(p_obj, (ISourceObservable*)msg.param, msg.sparam);
          break;

        case PRC_CMD_DETACH_FROM_SENSOR:
          res = ProcessTaskDetachFromSensor(p_obj, (ISourceObservable*)msg.param);
          break;

        case PRC_CMD_SUSPEND_DPU:
          ADPU_Suspend((ADPU*)p_obj->p_dpu);
          SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("PRC: suspend DPU\r\n"));
          break;

        case PRC_CMD_RESUME_DPU:
          ADPU_Resume((ADPU*)p_obj->p_dpu);
          SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("PRC: resume DPU\r\n"));
          break;

        default:
          SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("PRC: unexpected command ID:0x%x\r\n", msg.cmd_id));
          break;
      }
    }
    else if (msg.msgId == APP_REPORT_ID_FORCE_STEP)
    {
      /* we need only to resume the task because of a power mode switch pending. */
      __NOP();
    }
  }

  return res;
}

static void ProcessTaskDPUCallback(IDPU *_this, void* p_param)
{
  assert_param(_this != NULL);
  ProcessTask_t *p_obj = (ProcessTask_t*)p_param;

  struct prcMessage_t msg = {
      .msgId = APP_MESSAGE_ID_PRC,
      .cmd_id = PRC_CMD_NEW_DATA_READY
  };

  if (xQueueSendToBack(p_obj->in_queue, &msg, pdMS_TO_TICKS(100)) != pdPASS)
  {
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_PRC_TASK_IN_QUEUE_FULL_ERROR_CODE);
  }
}

static sys_error_code_t ProcessTaskDetachFromSensor(ProcessTask_t *_this, ISourceObservable *p_sensor)
{
  assert_param(_this != NULL);
  assert_param(p_sensor != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  if (_this->p_dpu_buff != NULL)
  {
    vPortFree(_this->p_dpu_buff);
    _this->p_dpu_buff = NULL;

    SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("PRC: free DPU buffer\r\n"));
  }
  res = IDPU_DetachFromSensor((IDPU*)_this->p_dpu, p_sensor);

  return res;
}

static sys_error_code_t ProcessTaskSetSensorsConfig(ProcessTask_t *_this, uint16_t *p_active_sensor_id)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  uint16_t sensor_id;
  SensorManager_t *p_sm = SMGetSensorManager();

  /* first disable all sensors.*/
  SIterator_t iterator;
  sensor_id = SI_NULL_SENSOR_ID;
  SIInit(&iterator, p_sm);
  while (SIHasNext(&iterator))
  {
    sensor_id = SINext(&iterator);
    SMSensorDisable(sensor_id);
  }

  /*configure only one sensor*/
  SQuery_t query;
  SQInit(&query, p_sm);
  sensor_id = SQNextByNameAndType(&query, "ism330dhcx",  COM_TYPE_ACC);
  if (sensor_id != SI_NULL_SENSOR_ID)
  {
    SMSensorEnable(sensor_id);
    SMSensorSetODR(sensor_id, SET_ODR);
    SMSensorSetFS(sensor_id, SET_FS);
  }

  *p_active_sensor_id = sensor_id;

  return res;
}

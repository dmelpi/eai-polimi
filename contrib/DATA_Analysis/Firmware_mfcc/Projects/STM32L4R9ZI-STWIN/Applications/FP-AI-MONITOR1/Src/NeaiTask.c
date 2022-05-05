/**
 ******************************************************************************
 * @file    NeaiTask.c
 * @author  STMicroelectronics - AIS - MCD Team
 * @version V1.0.0
 * @date    15-September-2021
 *
 * @brief
 *
 * <DESCRIPTIOM>
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

#include "NeaiTask.h"
#include "NeaiTask_vtbl.h"
#include "NanoEdgeAI.h"
#include "NeaiMessagesDef.h"
#include "app_messages_parser.h"
#include "services/sysdebug.h"

/* TODO: change XXX with a short id for the task */

#ifndef NAI_TASK_CFG_STACK_DEPTH
#define NAI_TASK_CFG_STACK_DEPTH        (120)
#endif

#ifndef NAI_TASK_CFG_PRIORITY
#define NAI_TASK_CFG_PRIORITY           (tskIDLE_PRIORITY)
#endif

#ifndef NAI_TASK_CFG_IN_QUEUE_LENGTH
#define NAI_TASK_CFG_IN_QUEUE_LENGTH    10
#endif

#define NAI_TASK_CFG_IN_QUEUE_ITEM_SIZE (sizeof(struct NeaiMessage_t))

#define NAI_STUB_LIB_ID                 "st_stub21212121212121212"

/* TODO: define the symbol SYS_DBG_NAI in the file sysdebug_config.h */
#define SYS_DEBUGF(level, message)      SYS_DEBUGF3(SYS_DBG_NAI, level, message)

#if defined(DEBUG) || defined (SYS_DEBUG)
#define sTaskObj                        sNeaiTaskObj
#endif



/**
 * Class object declaration. The class object encapsulates members that are shared between
 * all instance of the class.
 */
typedef struct _NeaiTaskClass_t {
  /**
   * NeaiTask class virtual table.
   */
  AManagedTaskEx_vtbl vtbl;

  /**
   * NeaiTask class (PM_STATE, ExecuteStepFunc) map. The map is implemente with an array and
   * the key is the index. Number of items of this array must be equal to the number of PM state
   * of the application. If the managed task does nothing in a PM state, then set to NULL the
   * relative entry in the map.
   */
  pExecuteStepFunc_t p_pm_state2func_map[];
} NeaiTaskClass_t;


/* Private member function declaration */
/***************************************/

/**
 * Execute one step of the task control loop while the system is in STATE1.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return SYS_NO_EROR_CODE if success, a task specific error code otherwise.
 */
static sys_error_code_t NeaiTaskExecuteStepState1(AManagedTask *_this);

/**
 * Execute one step of the task control loop while the system is in SENSORS_ACTIVE.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return SYS_NO_EROR_CODE if success, a task specific error code otherwise.
 */
static sys_error_code_t NeaiTaskExecuteStepNEAIActive(AManagedTask *_this);

/**
 * Callback used get the notification from the DPU.
 *
 * @param _this [IN] specifies teh DPU that triggered the callback.
 * @param p_param [IN] specifies an application specific parameter.
 */
static void NeaiTaskDPUCallback(IDPU *_this, void* p_param);

/**
 * Disconnect a sensor to the task as data source. Data are collected to form a signal of n axes and signal_size number of sample per axis,
 * and stored in a circular buffer of cb_items signals.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @param p_sensor [IN] specifies a pointer to a sensor.
 * @return SYS_NO_ERROR_CODE if success, and error code otherwise.
 */
static sys_error_code_t NeaiTaskDetachFromSensor(NeaiTask_t *_this, ISourceObservable *p_sensor);


/* Inline function forward declaration */
/***************************************/

#if defined (__GNUC__) || defined (__ICCARM__)
/* Inline function defined inline in the header file NeaiTask.h must be declared here as extern function. */
extern QueueHandle_t NeaiTaskGetInQueue(NeaiTask_t *_this);
extern IDPU *NeaiTaskGetDPU(NeaiTask_t *_this);
#endif


/**
 * The only instance of the task object.
 */
static NeaiTask_t sTaskObj;

/**
 * The class object.
 */
static const NeaiTaskClass_t sTheClass = {
    /* Class virtual table */
    {
        NeaiTask_vtblHardwareInit,
        NeaiTask_vtblOnCreateTask,
        NeaiTask_vtblDoEnterPowerMode,
        NeaiTask_vtblHandleError,
        NeaiTask_vtblOnEnterTaskControlLoop,
        NeaiTask_vtblForceExecuteStep,
        NeaiTask_vtblOnEnterPowerMode
    },

    /* class (PM_STATE, ExecuteStepFunc) map */
    {
        NeaiTaskExecuteStepState1,
        NULL,
        NULL,
        NULL,
        NULL,
        NeaiTaskExecuteStepNEAIActive,
    }
};

/* Public API definition */
/*************************/

AManagedTaskEx *NeaiTaskAlloc(void)
{
  /* In this application there is only one Keyboard task,
   * so this allocator implement the singleton design pattern.
   */

  /* Initialize the super class */
  AMTInitEx(&sTaskObj.super);

  sTaskObj.super.vptr = &sTheClass.vtbl;;

  return (AManagedTaskEx*)&sTaskObj;
}

sys_error_code_t NeaiTaskAttachToSensorEx(NeaiTask_t *_this, ISourceObservable *p_sensor, uint16_t signal_size, uint8_t axes, uint8_t cb_items)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  /* check if there is a sensor already attached */
  if (_this->p_dpu_buff != NULL)
  {
    vPortFree(_this->p_dpu_buff);
    _this->p_dpu_buff = NULL;
  }

  uint32_t buff_size = NeaiDPUSetStreamsParam(&_this->dpu, signal_size, axes, cb_items);
  _this->p_dpu_buff = pvPortMalloc(buff_size);
  if (_this->p_dpu_buff != NULL)
  {
    res = IDPU_AttachToSensor((IDPU*)&_this->dpu, p_sensor, _this->p_dpu_buff);

    SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("NAI: dpu buffer = %i byte\r\n", buff_size));
  }
  else
  {
    res = SYS_OUT_OF_MEMORY_ERROR_CODE;
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_OUT_OF_MEMORY_ERROR_CODE);
  }

  return res;
}

sys_error_code_t NeaiTaskAttachToSensor(NeaiTask_t *_this, ISourceObservable *p_sensor, uint8_t cb_items)
{
  return NeaiTaskAttachToSensorEx(_this, p_sensor, DATA_INPUT_USER, AXIS_NUMBER, cb_items);
}

sys_error_code_t NeaiTaskAddDPUListener(NeaiTask_t *_this, IProcessEventListener *p_listener)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  struct NeaiMessage_t msg = {
      .msgId = APP_MESSAGE_ID_NEAI,
      .cmd_id = NAI_CMD_ADD_DPU_LISTNER,
      .param.n_param = (uint32_t)p_listener
  };

  if (SYS_IS_CALLED_FROM_ISR())
  {
    if (pdTRUE != xQueueSendToBackFromISR(_this->in_queue, &msg, NULL))
    {
      res = SYS_NAI_TASK_IN_QUEUE_FULL_ERROR_CODE;
    }
  }
  else
  {
    if (pdTRUE != xQueueSendToBack(_this->in_queue, &msg, pdMS_TO_TICKS(100)))
    {
      res = SYS_NAI_TASK_IN_QUEUE_FULL_ERROR_CODE;
    }
  }

  return res;
}

sys_error_code_t NeaiTaskRemoveDPUListener(NeaiTask_t *_this, IProcessEventListener *p_listener)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  struct NeaiMessage_t msg = {
      .msgId = APP_MESSAGE_ID_NEAI,
      .cmd_id = NAI_CMD_REMOVE_DPU_LISTNER,
      .param.n_param = (uint32_t)p_listener
  };

  if (SYS_IS_CALLED_FROM_ISR())
  {
    if (pdTRUE != xQueueSendToBackFromISR(_this->in_queue, &msg, NULL))
    {
      res = SYS_NAI_TASK_IN_QUEUE_FULL_ERROR_CODE;
    }
  }
  else
  {
    if (pdTRUE != xQueueSendToBack(_this->in_queue, &msg, pdMS_TO_TICKS(100)))
    {
      res = SYS_NAI_TASK_IN_QUEUE_FULL_ERROR_CODE;
    }
  }

  return res;
}

sys_error_code_t NeaiTaskSetSensitivity(NeaiTask_t *_this, float sensitivity)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  /* parameter validation */
  if (sensitivity > 100)
  {
    sensitivity = 100;
  }

  struct NeaiMessage_t msg = {
      .msgId = APP_MESSAGE_ID_NEAI,
      .cmd_id = NAI_CMD_SET_SENSITIVITY,
      .param.n_param = (uint32_t)sensitivity
  };

  if (SYS_IS_CALLED_FROM_ISR())
  {
    if (pdTRUE != xQueueSendToBackFromISR(_this->in_queue, &msg, NULL))
    {
      res = SYS_NAI_TASK_IN_QUEUE_FULL_ERROR_CODE;
      SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_NAI_TASK_IN_QUEUE_FULL_ERROR_CODE);
    }
  }
  else
  {
    if (pdTRUE != xQueueSendToBack(_this->in_queue, &msg, pdMS_TO_TICKS(100)))
    {
      res = SYS_NAI_TASK_IN_QUEUE_FULL_ERROR_CODE;
      SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_NAI_TASK_IN_QUEUE_FULL_ERROR_CODE);
    }
  }

  return res;
}

bool NeaiTaskIsStubLibrary()
{
  if (strcmp(NAI_STUB_LIB_ID, NEAI_ID) == 0)
  {
    return true;
  }
  else
  {
    return false;
  }
}


/* AManagedTask virtual functions definition */
/*********************************************/

sys_error_code_t NeaiTask_vtblHardwareInit(AManagedTask *_this, void *p_params)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
/*  NeaiTask_t *p_obj = (NeaiTask_t*)_this; */

  return res;
}

sys_error_code_t NeaiTask_vtblOnCreateTask(AManagedTask *_this, TaskFunction_t *p_task_code, const char **p_name, unsigned short *p_stack_depth, void **p_params, UBaseType_t *p_priority)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  NeaiTask_t *p_obj = (NeaiTask_t*)_this;

  /* initialize the object software resource here. */

  p_obj->in_queue = xQueueCreate(NAI_TASK_CFG_IN_QUEUE_LENGTH, NAI_TASK_CFG_IN_QUEUE_ITEM_SIZE);
  if (p_obj->in_queue == NULL) {
    res = SYS_TASK_HEAP_OUT_OF_MEMORY_ERROR_CODE;
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(res);
    return res;
  }

#ifdef DEBUG
  vQueueAddToRegistry(p_obj->in_queue, "NAI_Q");
#endif

  /* Initialize DPU */
  IDPU * idpu = NeaiDPUStaticAlloc(&p_obj->dpu);
  res = IDPU_Init(idpu);
  IDPU_RegisterNotifyCallback((IDPU*)&p_obj->dpu, NeaiTaskDPUCallback, p_obj);

  /* Change the CLASS for the power mode switch because I want to do the transaction after all
     sensors task. */
  AMTExSetPMClass((AManagedTaskEx*)_this, E_PM_CLASS_1);

  if (!SYS_IS_ERROR_CODE(res))
  {
    /* set the (PM_STATE, ExecuteStepFunc) map from the class object.  */
    _this->m_pfPMState2FuncMap = sTheClass.p_pm_state2func_map;

    *p_task_code = AMTExRun;
    *p_name = "NAI";
    *p_stack_depth = NAI_TASK_CFG_STACK_DEPTH;
    *p_params = _this;
    *p_priority = NAI_TASK_CFG_PRIORITY;
  }

  return res;
}

sys_error_code_t NeaiTask_vtblDoEnterPowerMode(AManagedTask *_this, const EPowerMode active_power_mode, const EPowerMode new_power_mode)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  NeaiTask_t *p_obj = (NeaiTask_t*)_this;
  struct NeaiMessage_t msg = {
      .msgId = APP_MESSAGE_ID_NEAI
  };

  if (new_power_mode == E_POWER_MODE_STATE1)
  {
    msg.cmd_id = NAI_CMD_STOP_PROCESSING;
    if (xQueueSend(p_obj->in_queue, &msg, pdMS_TO_TICKS(100)) != pdTRUE)
    {
      res = SYS_NAI_TASK_IN_QUEUE_FULL_ERROR_CODE;
      SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_NAI_TASK_IN_QUEUE_FULL_ERROR_CODE);
    }
  }

  if (new_power_mode == E_POWER_MODE_NEAI_ACTIVE) {
    /* set the TAG in the DPU. This information is used in the process event */
    res = ADPU_SetTag((ADPU*)&p_obj->dpu, (uint32_t)NeaiDPUGetProcessingMode(&p_obj->dpu));
  }

  SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("NAI: ->%d.\r\n", (uint32_t)new_power_mode));

  return res;
}

sys_error_code_t NeaiTask_vtblHandleError(AManagedTask *_this, SysEvent error)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
/*  NeaiTask_t *p_obj = (NeaiTask_t*)_this; */

  return res;
}

sys_error_code_t NeaiTask_vtblOnEnterTaskControlLoop(AManagedTask *_this)
{
  assert_param(_this);
  sys_error_code_t xRes = SYS_NO_ERROR_CODE;
/*  NeaiTask_t *p_obj = (NeaiTask_t*)_this; */

  SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("NAI: start.\r\n"));

  return xRes;
}


/* AManagedTaskEx virtual functions definition */
/***********************************************/

sys_error_code_t NeaiTask_vtblForceExecuteStep(AManagedTaskEx *_this, EPowerMode active_power_mode)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  NeaiTask_t *p_obj = (NeaiTask_t*)_this;

  struct NeaiMessage_t msg = {
      .msgId = APP_REPORT_ID_FORCE_STEP
  };
  if (xQueueSendToFront(p_obj->in_queue, &msg, pdMS_TO_TICKS(100)) != pdTRUE)
  {
    res = SYS_NAI_TASK_IN_QUEUE_FULL_ERROR_CODE;
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_NAI_TASK_IN_QUEUE_FULL_ERROR_CODE);
  }

  return res;
}

sys_error_code_t NeaiTask_vtblOnEnterPowerMode(AManagedTaskEx *_this, const EPowerMode active_power_mode, const EPowerMode new_power_mode)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  /*NeaiTask_t *p_obj = (NeaiTask_t*)_this;*/

  return res;
}


/* Private function definition */
/*******************************/

static sys_error_code_t NeaiTaskExecuteStepState1(AManagedTask *_this)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  NeaiTask_t *p_obj = (NeaiTask_t*)_this;
  struct NeaiMessage_t msg = {0};

  AMTExSetInactiveState((AManagedTaskEx*)_this, TRUE);
  if (pdTRUE == xQueueReceive(p_obj->in_queue, &msg, portMAX_DELAY))
  {
    AMTExSetInactiveState((AManagedTaskEx*)_this, FALSE);
    if (msg.msgId == APP_MESSAGE_ID_NEAI)
    {
      switch (msg.cmd_id)
      {
        case NAI_CMD_STOP_PROCESSING:
          NeaiDPUPrepareToProcessData(&p_obj->dpu);
          break;

        case NAI_CMD_CONNECT_TO_SENSOR:
          res = NeaiTaskAttachToSensor(p_obj, (ISourceObservable*)msg.param.n_param, msg.sparam);
          break;

        case NAI_CMD_DETACH_FROM_SENSOR:
          res = NeaiTaskDetachFromSensor(p_obj, (ISourceObservable*)msg.param.n_param);
          break;

        case NAI_CMD_SET_MODE:
          res = NeaiDPUSetProcessingMode(&p_obj->dpu, msg.param.n_param == NAI_CMD_PARAM_LEARN ? E_NEAI_LEARNING : E_NEAI_DETECTION);
          break;

        case NAI_CMD_SET_SENSITIVITY:
          res = NeaiDPUSetSensitivity(&p_obj->dpu, msg.param.f_param);
          break;

        case NAI_CMD_INIT:
          NanoEdgeAI_initialize();
          break;

        case NAI_CMD_ADD_DPU_LISTNER:
        {
          IEventSrc *p_evt_src = ADPU_GetEventSrcIF((ADPU*)&p_obj->dpu);
          res = IEventSrcAddEventListener(p_evt_src, (IEventListener*)msg.param.n_param);
        }
        break;

        case NAI_CMD_REMOVE_DPU_LISTNER:
        {
          IEventSrc *p_evt_src = ADPU_GetEventSrcIF((ADPU*)&p_obj->dpu);
          res = IEventSrcRemoveEventListener(p_evt_src, (IEventListener*)msg.param.n_param);
        }
        break;

        default:
          SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("NAI: unexpected command ID:0x%x\r\n", msg.cmd_id));
          break;
      }
    }
    else if (msg.msgId == APP_REPORT_ID_FORCE_STEP)
    {
      __NOP();
    }
  }

  return res;
}

static sys_error_code_t NeaiTaskExecuteStepNEAIActive(AManagedTask *_this)
{
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  NeaiTask_t *p_obj = (NeaiTask_t*)_this;
  struct NeaiMessage_t msg = {0};

  AMTExSetInactiveState((AManagedTaskEx*)_this, TRUE);
  if (pdTRUE == xQueueReceive(p_obj->in_queue, &msg, portMAX_DELAY))
  {
    AMTExSetInactiveState((AManagedTaskEx*)_this, FALSE);
    if (msg.msgId == APP_MESSAGE_ID_NEAI)
    {
      switch (msg.cmd_id)
      {
        case NAI_CMD_NEW_DATA_READY:
          res = IDPU_Process((IDPU*)&p_obj->dpu);
          break;

        case NAI_CMD_SET_SENSITIVITY:
          res = NeaiDPUSetSensitivity(&p_obj->dpu, msg.param.f_param);
          break;

        default:
          SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("NAI: unexpected message ID:0x%x\r\n", msg.cmd_id));
          break;
      }
    }
    else if (msg.msgId == APP_REPORT_ID_FORCE_STEP)
    {
      __NOP();
    }
  }

  return res;
}

static void NeaiTaskDPUCallback(IDPU *_this, void* p_param)
{
  NeaiTask_t *p_obj = (NeaiTask_t*)p_param;
  struct NeaiMessage_t msg = {
      .msgId = APP_MESSAGE_ID_NEAI,
      .cmd_id = NAI_CMD_NEW_DATA_READY
  };

  if (xQueueSendToBack(p_obj->in_queue, &msg, pdMS_TO_TICKS(100)) != pdPASS)
  {
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_NAI_TASK_IN_QUEUE_FULL_ERROR_CODE);
  }
}

static sys_error_code_t NeaiTaskDetachFromSensor(NeaiTask_t *_this, ISourceObservable *p_sensor)
{
  assert_param(_this != NULL);
  assert_param(p_sensor != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  if (_this->p_dpu_buff != NULL)
  {
    vPortFree(_this->p_dpu_buff);
    _this->p_dpu_buff = NULL;
  }
  res = IDPU_DetachFromSensor((IDPU*)&_this->dpu, p_sensor);

  return res;
}

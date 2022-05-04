/**
 ******************************************************************************
 * @file    AITask.c
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

#include "AITask.h"
#include "AITask_vtbl.h"
#include "app_messages_parser.h"
#include "AIMessagesDef.h"
#include "services/sysdebug.h"
/* AI Run-time header files */
#include "ai_platform_interface.h"
/* AI x-cube-ai files */
#include "app_x-cube-ai.h"
#include "SensorManager.h"


/* TODO: change XXX with a short id for the task */

#ifndef AI_TASK_CFG_STACK_DEPTH
#define AI_TASK_CFG_STACK_DEPTH            (120)
#endif

#ifndef AI_TASK_CFG_PRIORITY
#define AI_TASK_CFG_PRIORITY               (tskIDLE_PRIORITY)
#endif

#ifndef AI_TASK_CFG_IN_QUEUE_LENGTH
#define AI_TASK_CFG_IN_QUEUE_LENGTH        10
#endif

#define AI_TASK_CFG_IN_QUEUE_ITEM_SIZE    (sizeof(struct AIMessage_t))

#define AI_TASK_DPU_TAG                   (0x30U) // must be equal to CTRL_CMD_PARAM_AI


/* TODO: define the symbol SYS_DBG_AI in the file sysdebug_config.h */
#define SYS_DEBUGF(level, message)      SYS_DEBUGF3(SYS_DBG_AI, level, message)



/**
 * Class object declaration. The class object encapsulates members that are shared between
 * all instance of the class.
 */
typedef struct _AITaskClass_t {
  /**
   * AITask class virtual table.
   */
  AManagedTaskEx_vtbl vtbl;

  /**
   * AITask class (PM_STATE, ExecuteStepFunc) map. The map is implemente with an array and
   * the key is the index. Number of items of this array must be equal to the number of PM state
   * of the application. If the managed task does nothing in a PM state, then set to NULL the
   * relative entry in the map.
   */
  pExecuteStepFunc_t p_pm_state2func_map[];
} AITaskClass_t;


/* Private member function declaration */
/***************************************/

/**
 * Execute one step of the task control loop while the system is in STATE1.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return SYS_NO_EROR_CODE if success, a task specific error code otherwise.
 */
static sys_error_code_t AITaskExecuteStepState1(AManagedTask *_this);

/**
 * Execute one step of the task control loop while the system is in X_CUBE_AI_ACTIVE.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return SYS_NO_EROR_CODE if success, a task specific error code otherwise.
 */
static sys_error_code_t AITaskExecuteStepAIActive(AManagedTask *_this);

/**
 * Callback used get the notification from the DPU.
 *
 * @param _this [IN] specifies teh DPU that triggered the callback.
 * @param p_param [IN] specifies an application specific parameter.
 */
static void AiTaskDPUCallback(IDPU *_this, void* p_param);

/**
 * Disconnect a sensor to the task as data source. Data are collected to form a signal of n axes and signal_size number of sample per axis,
 * and stored in a circular buffer of cb_items signals.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @param p_sensor [IN] specifies a pointer to a sensor.
 * @return SYS_NO_ERROR_CODE if success, and error code otherwise.
 */
static sys_error_code_t AiTaskDetachFromSensor(AITask_t *_this, ISourceObservable *p_sensor);


/* Inline function forward declaration */
/***************************************/

#if defined (__GNUC__) || defined (__ICCARM__)
/* Inline function defined inline in the header file AITask.h must be declared here as extern function. */
extern QueueHandle_t AITaskGetInQueue(AITask_t *_this);
#endif


/**
 * The only instance of the task object.
 */
static AITask_t sTaskObj;

/**
 * The class object.
 */
static const AITaskClass_t sTheClass = {
    /* Class virtual table */
    {
        AITask_vtblHardwareInit,
        AITask_vtblOnCreateTask,
        AITask_vtblDoEnterPowerMode,
        AITask_vtblHandleError,
        AITask_vtblOnEnterTaskControlLoop,
        AITask_vtblForceExecuteStep,
        AITask_vtblOnEnterPowerMode
    },

    /* class (PM_STATE, ExecuteStepFunc) map */
    {
        AITaskExecuteStepState1,
        NULL,
        NULL,
        NULL,
        AITaskExecuteStepAIActive,
        NULL
    }
};

/* Public API definition */
/*************************/

AManagedTaskEx *AITaskAlloc(void)
{
  /* In this application there is only one Keyboard task,
   * so this allocator implement the singleton design pattern.
   */

  /* Initialize the super class */
  AMTInitEx(&sTaskObj.super);

  sTaskObj.super.vptr = &sTheClass.vtbl;;

  return (AManagedTaskEx*)&sTaskObj;
}

sys_error_code_t AiTaskAttachToSensorEx(AITask_t *_this, ISourceObservable *p_sensor, uint16_t signal_size, uint8_t axes, uint8_t cb_items)
{
  assert_param(_this != NULL);
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  /* check if there is a sensor already attached */
  if (_this->p_dpu_buff != NULL)
  {
    vPortFree(_this->p_dpu_buff);
    _this->p_dpu_buff = NULL;
  }

  uint16_t buff_size = AiDPUSetStreamsParam(&_this->dpu, signal_size, axes, cb_items);
  _this->p_dpu_buff = pvPortMalloc(buff_size);
  if (_this->p_dpu_buff != NULL)
  {
    res = IDPU_AttachToSensor((IDPU*)&_this->dpu, p_sensor, _this->p_dpu_buff);

    SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("AI: dpu buffer = %i byte\r\n", buff_size));
  }
  else
  {
    res = SYS_OUT_OF_MEMORY_ERROR_CODE;
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_OUT_OF_MEMORY_ERROR_CODE);
  }


  return res;
}

sys_error_code_t AiTaskAttachToSensor(AITask_t *_this, ISourceObservable *p_sensor, uint8_t cb_items)
{
	return AiTaskAttachToSensorEx(_this, p_sensor, AIDPU_NB_SAMPLE, AIDPU_NB_AXIS, cb_items);
}

sys_error_code_t AiTaskAddDPUListener(AITask_t *_this, IProcessEventListener *p_listener)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  struct AIMessage_t msg = {
      .msgId = APP_MESSAGE_ID_AI,
      .cmd_id = AI_CMD_ADD_DPU_LISTNER,
      .param = (uint32_t)p_listener
  };

  if (SYS_IS_CALLED_FROM_ISR())
  {
    if (pdTRUE != xQueueSendToBackFromISR(_this->in_queue, &msg, NULL))
    {
      res = SYS_AI_TASK_IN_QUEUE_FULL_ERROR_CODE;
    }
  }
  else
  {
    if (pdTRUE != xQueueSendToBack(_this->in_queue, &msg, pdMS_TO_TICKS(100)))
    {
      res = SYS_AI_TASK_IN_QUEUE_FULL_ERROR_CODE;
    }
  }

  return res;
}

sys_error_code_t AiTaskRemoveDPUListener(AITask_t *_this, IProcessEventListener *p_listener)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  struct AIMessage_t msg = {
      .msgId = APP_MESSAGE_ID_AI,
      .cmd_id = AI_CMD_REMOVE_DPU_LISTNER,
      .param = (uint32_t)p_listener
  };

  if (SYS_IS_CALLED_FROM_ISR())
  {
    if (pdTRUE != xQueueSendToBackFromISR(_this->in_queue, &msg, NULL))
    {
      res = SYS_AI_TASK_IN_QUEUE_FULL_ERROR_CODE;
    }
  }
  else
  {
    if (pdTRUE != xQueueSendToBack(_this->in_queue, &msg, pdMS_TO_TICKS(100)))
    {
      res = SYS_AI_TASK_IN_QUEUE_FULL_ERROR_CODE;
    }
  }

  return res;
}


/* AManagedTask virtual functions definition */
/*********************************************/

sys_error_code_t AITask_vtblHardwareInit(AManagedTask *_this, void *p_params)
{
  assert_param(_this != NULL);
  /* By default the CRC IP clock is enabled */
  __HAL_RCC_CRC_CLK_ENABLE();
  sys_error_code_t res = SYS_NO_ERROR_CODE;
/*  AITask_t *p_obj = (AITask_t*)_this; */

  return res;
}

sys_error_code_t AITask_vtblOnCreateTask(AManagedTask *_this, TaskFunction_t *p_task_code, const char **p_name, unsigned short *p_stack_depth, void **p_params, UBaseType_t *p_priority)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  AITask_t *p_obj = (AITask_t*)_this;

  /* initialize the object software resource here. */

  p_obj->in_queue = xQueueCreate(AI_TASK_CFG_IN_QUEUE_LENGTH, AI_TASK_CFG_IN_QUEUE_ITEM_SIZE);
  if (p_obj->in_queue == NULL) {
    res = SYS_TASK_HEAP_OUT_OF_MEMORY_ERROR_CODE;
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(res);
    return res;
  }
#ifdef DEBUG
  vQueueAddToRegistry(p_obj->in_queue, "AI_Q");
#endif

  /* Initialize DPU */
  IDPU * idpu = AiDPUStaticAlloc(&p_obj->dpu);
  res = IDPU_Init(idpu);
  IDPU_RegisterNotifyCallback((IDPU*)&p_obj->dpu, AiTaskDPUCallback, p_obj);
  ADPU_SetTag((ADPU*)&p_obj->dpu, AI_TASK_DPU_TAG);

  /* Change the CLASS for the power mode switch because I want to do the transaction after all
     sensors task. */
  AMTExSetPMClass((AManagedTaskEx*)_this, E_PM_CLASS_1);

  /* set the (PM_STATE, ExecuteStepFunc) map from the class object.  */
  _this->m_pfPMState2FuncMap = sTheClass.p_pm_state2func_map;

  *p_task_code = AMTExRun;
  *p_name = "AI";
  *p_stack_depth = AI_TASK_CFG_STACK_DEPTH;
  *p_params = _this;
  *p_priority = AI_TASK_CFG_PRIORITY;

  return res;
}

sys_error_code_t AITask_vtblDoEnterPowerMode(AManagedTask *_this, const EPowerMode active_power_mode, const EPowerMode new_power_mode)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  AITask_t *p_obj = (AITask_t*)_this;

  struct AIMessage_t msg = {
      .msgId = APP_MESSAGE_ID_AI
  };

  if (new_power_mode == E_POWER_MODE_STATE1)
  {
    msg.cmd_id = AI_CMD_STOP_PROCESSING;
    if (xQueueSend(p_obj->in_queue, &msg, pdMS_TO_TICKS(100)) != pdTRUE)
    {
      res = SYS_AI_TASK_IN_QUEUE_FULL_ERROR_CODE;
      SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_AI_TASK_IN_QUEUE_FULL_ERROR_CODE);
    }
  }

  return res;
}

sys_error_code_t AITask_vtblHandleError(AManagedTask *_this, SysEvent error)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
/*  AITask_t *p_obj = (AITask_t*)_this; */

  return res;
}

sys_error_code_t AITask_vtblOnEnterTaskControlLoop(AManagedTask *_this)
{
  assert_param(_this);
  sys_error_code_t xRes = SYS_NO_ERROR_CODE;
/*  AITask_t *p_obj = (AITask_t*)_this; */

  SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("AI: start.\r\n"));

  return xRes;
}


/* AManagedTaskEx virtual functions definition */
/***********************************************/

sys_error_code_t AITask_vtblForceExecuteStep(AManagedTaskEx *_this, EPowerMode active_power_mode)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  AITask_t *p_obj = (AITask_t*)_this;

  struct AIMessage_t msg = {
      .msgId = APP_REPORT_ID_FORCE_STEP
  };
  if (xQueueSendToFront(p_obj->in_queue, &msg, pdMS_TO_TICKS(100)) != pdTRUE)
  {
    res = SYS_NAI_TASK_IN_QUEUE_FULL_ERROR_CODE;
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_NAI_TASK_IN_QUEUE_FULL_ERROR_CODE);
  }

  return res;
}

sys_error_code_t AITask_vtblOnEnterPowerMode(AManagedTaskEx *_this, const EPowerMode active_power_mode, const EPowerMode new_power_mode)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
/*  AITask_t *p_obj = (AITask_t*)_this; */

  return res;
}


/* Private function definition */
/*******************************/

static sys_error_code_t AITaskExecuteStepState1(AManagedTask *_this)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  AITask_t *p_obj = (AITask_t*)_this;
  struct AIMessage_t msg = {0};

  AMTExSetInactiveState((AManagedTaskEx*)_this, TRUE);
  if (pdTRUE == xQueueReceive(p_obj->in_queue, &msg, portMAX_DELAY))
  {
    AMTExSetInactiveState((AManagedTaskEx*)_this, FALSE);
    if (msg.msgId == APP_MESSAGE_ID_AI)
    {
      switch (msg.cmd_id)
      {
        case AI_CMD_STOP_PROCESSING:
            AiDPUPrepareToProcessData(&p_obj->dpu);
            AiDPUSetProcessingMode(&p_obj->dpu, E_AI_DETECTION);
          break;

        case AI_CMD_CONNECT_TO_SENSOR:
          res = AiTaskAttachToSensor(p_obj, (ISourceObservable*)msg.param, msg.sparam);
          break;

        case AI_CMD_DETACH_FROM_SENSOR:
          res = AiTaskDetachFromSensor(p_obj, (ISourceObservable*)msg.param);
          break;

        case AI_CMD_ADD_DPU_LISTNER:
        {
          IEventSrc *p_evt_src = ADPU_GetEventSrcIF((ADPU*)&p_obj->dpu);
          res = IEventSrcAddEventListener(p_evt_src, (IEventListener*)msg.param);
        }
        break;

        case AI_CMD_REMOVE_DPU_LISTNER:
        {
          IEventSrc *p_evt_src = ADPU_GetEventSrcIF((ADPU*)&p_obj->dpu);
          res = IEventSrcRemoveEventListener(p_evt_src, (IEventListener*)msg.param);
        }
        break;

        default:
          SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("AI: unexpected command ID:0x%x\r\n", msg.cmd_id));
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

uint32_t g_resume_count = 0;
uint32_t g_dec_clock_count = 0;
uint32_t g_count_us = 0;

static sys_error_code_t AITaskExecuteStepAIActive(AManagedTask *_this)
{
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  AITask_t *p_obj = (AITask_t*)_this;
  struct NeaiMessage_t msg = {0};

  AMTExSetInactiveState((AManagedTaskEx*)_this, TRUE);
  if (pdTRUE == xQueueReceive(p_obj->in_queue, &msg, portMAX_DELAY))
  {
    AMTExSetInactiveState((AManagedTaskEx*)_this, FALSE);
    if (msg.msgId == APP_MESSAGE_ID_AI)
    {
      switch (msg.cmd_id)
      {
      case AI_CMD_NEW_DATA_READY:

        CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
        /* reset the counter */
        DWT->CYCCNT = 0;
        /* enable the counter */
        DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

        res = IDPU_Process((IDPU*)&p_obj->dpu);

        /* disable the counter */
        DWT->CTRL &= ~DWT_CTRL_CYCCNTENA_Msk;
        g_dec_clock_count = DWT->CYCCNT;
        g_count_us = g_dec_clock_count / 120;
        SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("AI: detect time = %lu us rcnt=%lu\r\n", g_count_us, g_resume_count));


        /* check if the DPU has become inactive (because theCB was full during the processing */
        if(ADPU_IsSuspended((ADPU*)&p_obj->dpu))
        {
          bool item_ready = false;
          /* if there are pending messages ...  */
          if (xQueuePeek(p_obj->in_queue, &msg, 0) == pdTRUE)
          {
            /* if there are other CBItem ready to be processed ...  */
            if ((msg.msgId == APP_MESSAGE_ID_AI) && (msg.cmd_id == AI_CMD_NEW_DATA_READY))
            {
              /* do nothing so I process the next item */
              item_ready = true;
            }
          }
          if (!item_ready)
          {
            /* reset the DPU before enable it  again*/
            AiDPUPrepareToProcessData(&p_obj->dpu);
            AiDPUSetProcessingMode(&p_obj->dpu, E_AI_DETECTION);
            g_resume_count++;
            ADPU_Resume((ADPU*)&p_obj->dpu);
          }

        }

        break;

        default:
          SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("AI: unexpected command ID:0x%x\r\n", msg.cmd_id));
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

static void AiTaskDPUCallback(IDPU *_this, void* p_param)
{
  AITask_t *p_obj = (AITask_t*)p_param;
  struct NeaiMessage_t msg = {
      .msgId = APP_MESSAGE_ID_AI,
      .cmd_id = AI_CMD_NEW_DATA_READY
  };

  if (xQueueSendToBack(p_obj->in_queue, &msg, pdMS_TO_TICKS(100)) != pdPASS)
  {
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_AI_TASK_IN_QUEUE_FULL_ERROR_CODE);
  }
}

static sys_error_code_t AiTaskDetachFromSensor(AITask_t *_this, ISourceObservable *p_sensor)
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

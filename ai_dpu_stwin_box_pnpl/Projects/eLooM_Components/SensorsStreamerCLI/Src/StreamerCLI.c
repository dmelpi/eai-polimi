/**
 ******************************************************************************
 * @file    StreamerCLI.c
 * @author  STMicroelectronics - AIS - MCD Team
 * @version v1.0
 * @date    Jan 17, 2022
 *
 * @brief   application controller for the CLI Streamer.
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

#include "StreamerCLI.h"
#include "StreamerCLI_vtbl.h"
#include "StreamerCLIMessagesDef.h"
#include "app_messages_parser.h"
#include "services/cli_commands.h"
#include "services/SQuery.h"
#include "events/IEventSrc.h"
#include "events/IEventSrc_vtbl.h"
#include "services/sysdebug.h"


#ifndef SCLI_TASK_CFG_STACK_DEPTH
#define SCLI_TASK_CFG_STACK_DEPTH                      (120)
#endif

#ifndef SCLI_TASK_CFG_PRIORITY
#define SCLI_TASK_CFG_PRIORITY                         (tskIDLE_PRIORITY)
#endif

#ifndef SCLI_TASK_CFG_IN_QUEUE_LENGTH
#define SCLI_TASK_CFG_IN_QUEUE_LENGTH                  10
#endif
#define SCLI_TASK_CFG_IN_QUEUE_ITEM_SIZE               (sizeof(struct CtrlMessage_t))

#if defined(DEBUG) || defined (SYS_DEBUG)
#define SCLI_TASK_CFG_OUT_CH                           stderr
#else
#define SCLI_TASK_CFG_OUT_CH                           stdout
#endif
#define SCLI_MAX_TIME_MS_PARAM                         (1000U*60U*60U*24U*2U) ///< Max time in ms to pass to as parameter to the "neai_set timer <value>" command. It is 2 days.
#define SCLI_CB_N_ITEMS                                2
#define SCLI_CB_ITEM_MAX_N_ELEMENTS                    (256*3) ///< number of elements per item: 256 sample, 3 axis per sample (x,y,z)
#define SCLI_CB_ITEM_ELEMENT_SIZE_B                    sizeof(int16_t)
#define SCLI_CB_MAX_ITEM_SIZE_B                        (SCLI_CB_ITEM_MAX_N_ELEMENTS * SCLI_CB_ITEM_ELEMENT_SIZE_B)
#define SCLI_CB_ITEMS_MAX_BUFF_SIZE_B                  (SCLI_CB_N_ITEMS * SCLI_CB_MAX_ITEM_SIZE_B)
#define SCLI_IS_BUFF_READY(cbh)                        ((cbh)->data_idx >= SCLI_CB_ITEM_MAX_N_ELEMENTS)

#ifndef SCLI_CFG_CLI_APP_NAME_STR
#define SCLI_CFG_CLI_APP_NAME_STR                      "! CLI Sensor Streamer !"
#endif


#define CLI_PARAM_START_STREAMER                       "streamer"
#define CLI_PARAM_SENSOR_ODR                           "ODR"
#define CLI_PARAM_SENSOR_FULL_SCALE                    "FS"
#define CLI_PARAM_SENSOR_ENABLE                        "enable"
#define CLI_PARAM_SENSOR_GET_AVAILABLE_ODRS            "ODR_List"
#define CLI_PARAM_SENSOR_GET_AVAILABLE_FULLSCALES      "FS_List"
#define CLI_PARAM_GET_ALL                              "all"
#define CLI_PARAM_TIMER                                "timer"
#define CLI_PARAM_INFO                                 "info"
#define CLI_INVALID_PARAMETER_ERROR_MSG                "Invalid parameter: %s\r\n"
#define CLI_GENERIC_TIMEOUT_ERROR_MSG                  "CLI: cannot execute the commands. Try again later\r\n"
#define CLI_SET_ODR_MSG                                "nominal ODR = %0.2f Hz, latest measured ODR = %0.2f Hz\r\n"

#define CLI_KEY_CODE_ESC                               (0x1B)


#define SYS_DEBUGF(level, message)                     SYS_DEBUGF3(SYS_DBG_SCLI, level, message)

#if defined(DEBUG) || defined (SYS_DEBUG)
#define sTaskObj                                       sAppCTRLTaskObj
#endif

/**
 * Map the sensor type ID to a string. Note that the sensor type is an unsigned integer starting from 1.
 */
static const char *sSensorTypeDecoder[] = {
  "ACC",
  "MAG",
  "GYRO",
  "TEMP",
  "PRESS",
  "HUM",
  "MIC",
  "MLC"
};

/**
 * Class object declaration. The class object encapsulates members that are shared between
 * all instance of the class.
 */
typedef struct _StreamerCLIClass_t {
  /**
   * StreamerCLI class virtual table.
   */
  AManagedTaskEx_vtbl vtbl;

  /**
   * Specifies the listener interface to receive the ::SensorEvent.
   */
  ISensorEventListener_vtbl sensor_liestener_vtbl;

  /**
   * StreamerCLI class (PM_STATE, ExecuteStepFunc) map. The map is implemented with an array and
   * the key is the index. Number of items of this array must be equal to the number of PM state
   * of the application. If the managed task does nothing in a PM state, then set to NULL the
   * relative entry in the map.
   */
  pExecuteStepFunc_t p_pm_state2func_map[];
} StreamerCLIClass_t;


/* Private member function declaration */
/***************************************/

/**
 * Execute one step of the task control loop while the system is in STATE1.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return SYS_NO_EROR_CODE if success, a task specific error code otherwise.
 */
static sys_error_code_t StreamerCLIExecuteStepState1(AManagedTask *_this);

/**
 * Execute one step of the task control loop while the system is in SENSORS_ACTIVE.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return SYS_NO_EROR_CODE if success, a task specific error code otherwise.
 */
static sys_error_code_t StreamerCLIExecuteStepSensorsActive(AManagedTask *_this);

/**
 * Initialize the USB CDC device
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return SYS_NO_ERROR_CODE
 */
static sys_error_code_t StreamerCLIInitUsbDevice(StreamerCLI_t *_this);

#if 0
/**
 * Reset the USB CDC device to its initial state.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return SYS_NO_ERROR_CODE
 */
static sys_error_code_t StreamerCLIDeInitUsbDevice(StreamerCLI_t *_this);
#endif

/**
 * Print the welcome message
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return SYS_NO_ERROR_CODE
 */
static sys_error_code_t StreamerCLIDisplayWelcomeMessage(StreamerCLI_t _this);

/**
 * Process a new char.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @param ch [IN] specifies a new input char.
 * @return SYS_NO_ERROR_CODE if success, an error code otherwise.
 */
static sys_error_code_t StreamerCLIProcessNewChar(StreamerCLI_t *_this, char ch);

/**
 * Parse the string parameter and check the syntax. A valid ID is an integer in the range [0, MAX_SENSORS):
 * <id> ::= <integer>
 * <integer> ::= <digit> | <integer><digit>
 * <digit> ::= [0..9]
 *
 * If the syntax is correct the valid ID is returned in the OUT parameter p_sensor_id.
 *
 * @param p_param [IN] specifies a string to parse
 * @param param_length [IN] specifies the length of the string
 * @param p_sensor_id [OUT] specifies the variable where is stored the parsed ID.
 * @return SYS_NO_ERROR_CODE if success, SYS_INVALID_PARAMETER_ERROR_CODE if the string parameter doesn't contain a
 *         valid ID.
 */
static sys_error_code_t StreamerCLIParseSensorID(const char *p_param, BaseType_t param_length, uint8_t *p_sensor_id);

/**
 * Start an execution phase. Processing this command will trigger an PM transaction.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @param exec_phase [IN] specifies an execution phase. Valid value are:
 *   - SCLI_CMD_PARAM_NEAI_LEARN
 *   - SCLI_CMD_PARAM_NEAI_DETECT
 *   - SCLI_CMD_PARAM_AI
 * @return SYS_NO_ERROR_CODE if success, an error code otherwise.
 */
static sys_error_code_t StreamerCLIStartExecutionPhase(StreamerCLI_t *_this, uint32_t exec_phase);

/**
 * Print in the console all available parameters for a sensor.
 *
 * @param pcWriteBuffer [IN] specifies the console write buffer.
 * @param xWriteBufferLen [IN] specifies the size in byte of teh console write buffer.
 * @param p_sensor [IN] specifies a sensor.
 * @return
 */
static BaseType_t StreamerCLIParseSensorGetAllCmd(char *pcWriteBuffer, size_t xWriteBufferLen, ISourceObservable *p_sensor);

/**
 * Set the default configuration for the sensors.
 * @param _this [IN] specifies a pointer to a task object.
 * @return SYS_NO_ERROR_CODE if success, an error code otherwise.
 */
static sys_error_code_t StreamerCLISetDefaultConfig(StreamerCLI_t *_this);

/**
 * Function called when the Stop timer expires.
 *
 * @param xTimer [IN] specifies an handle to the expired timer.
 */
static void StreamerCLITimerStopCallback(TimerHandle_t xTimer);

/**
 * Stream the raw data from the CBItem.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @param p_item [IN] specifies a circular buffer item containing the raw sensor data.
 * @return SYS_NO_ERROR_CODE if success, an error code otherwise.
 */
static sys_error_code_t StreamerCLIStreamData(StreamerCLI_t *_this, CBItem *p_item);

/**
 * This configuration function disable all sensors.
 *
 * @param p_sm [IN] specify a the SensorManager object.
 * @param p_active_sensor_id [OUT] specifies the ID of the active sensor.
 * @return SYS_NO_ERROR_CODE if success, an application error code otherwise.
 */
static sys_error_code_t StreamerCLISensorsConfigF(SensorManager_t *p_sm, uint16_t *p_active_sensor_id);


/* Inline function forward declaration */
/***************************************/

#if defined (__GNUC__) || defined (__ICCARM__)
/* Inline function defined inline in the header file StreamerCLI.h must be declared here as extern function. */

#endif


/**
 * Handle to USB FS device.
 */
extern USBD_HandleTypeDef hUsbDeviceFS;

/**
 * The only instance of the task object.
 */
static StreamerCLI_t sTaskObj;

/**
 * Specifies the memory buffer used to store the raw data from the sensor.
 */
static uint8_t s_cb_items_buff[SCLI_CB_ITEMS_MAX_BUFF_SIZE_B];

/**
 * The class object.
 */
static const StreamerCLIClass_t sTheClass = {
  /* Class virtual table */
  {
      StreamerCLI_vtblHardwareInit,
      StreamerCLI_vtblOnCreateTask,
      StreamerCLI_vtblDoEnterPowerMode,
      StreamerCLI_vtblHandleError,
      StreamerCLI_vtblOnEnterTaskControlLoop,
      StreamerCLI_vtblForceExecuteStep,
      StreamerCLI_vtblOnEnterPowerMode
  },

	/* ISensorEventLestener virtual table */
	{
    StreamerCLI_vtblOnStatusChange,
		StreamerCLI_vtblSetOwner,
		StreamerCLI_vtblGetOwner,
		StreamerCLI_vtblOnNewDataReady
	},

  /* class (PM_STATE, ExecuteStepFunc) map */
  {
      StreamerCLIExecuteStepState1,
      NULL,
      StreamerCLIExecuteStepSensorsActive,
  }
};

static const char * const sWelcomeMessage =
  "\r\nConsole command server.\r\nType 'help' to view a list of registered commands.\r\n";
static const char * const sPromptMessage = "\r\n$ ";


/* Define the FreeRTOS CLI commands */
/************************************/

static BaseType_t StreamerCLIParseSensorInfoCmd(char *p_write_buffer, size_t write_buffer_len, const char *p_command_string);
static BaseType_t StreamerCLIParseSensorGetCommand(char *p_write_buffer, size_t write_buffer_len, const char *p_command_string);
static BaseType_t StreamerCLIParseSensorSetCommand(char *p_write_buffer, size_t write_buffer_len, const char *p_command_string);
static BaseType_t StreamerCLIParseStartCommand(char *p_write_buffer, size_t write_buffer_len, const char *p_command_string);
static BaseType_t StreamerCLIParseStreamerSetCmd(char *p_write_buffer, size_t write_buffer_len, const char *p_command_string);
static BaseType_t StreamerCLIParseStreamerGetCmd(char *p_write_buffer, size_t write_buffer_len, const char *p_command_string);
static BaseType_t StreamerCLIParseStreamerSetTimerCmd(char *p_write_buffer, size_t write_buffer_len, const char *p_command_string);


static const CLI_Command_Definition_t sCLICommands[] = {
    {
        "sensor_info",
        "\r\nsensor_info:\r\n Get a list of all supported sensors and their ID.\r\n",
        StreamerCLIParseSensorInfoCmd,
        0
    },
    {
        "sensor_get",
        "\r\nsensor_get <id> <param>:\r\n Gets the value of a \'parameter\' for a sensor with sensor id provided in \'id\'.\r\n"\
        " <param> ::= enable | ODR | ODR_List | FS | FS_List |\r\n"\
        "             all\r\n"\
        " eg.: \'sensor_get 1 ODR\' prints nominal and latest measured ODR for the sensor.\r\n"\
        " parameters.\r\n",
        StreamerCLIParseSensorGetCommand,
        2 /* 2 parameters are expected. */
    },
    {
        "sensor_set",
        "\r\nsensor_set <id> <param> <value>:\r\n Sets the \'value\' of a \'parameter\' for a sensor with sensor id provided in \'id\'.\r\n"\
        " <param> ::= enable | ODR | FS\r\n"\
        " eg.: \'sensor_set 1 enable 1\' enables the sensor 1\r\n",
        StreamerCLIParseSensorSetCommand,
        3
    },
    {
        "streamer_get",
        "\r\nstreamer_get <param>:\r\n Display the value of sensor streamer parameters. Valid parameters are:"\
        "\r\n - timer"\
        "\r\n - all\r\n",
        StreamerCLIParseStreamerGetCmd,
        1
    },
    {
        "streamer_set",
        "\r\nstreamer_set <parameter> <value>:\r\n Set a Sensor Streamer specific parameter.\r\n"\
        " Valid parameters and values are:\r\n"\
        " - timer: integer [0ms, MAX_TIME_MS]\r\n",
        StreamerCLIParseStreamerSetCmd,
        2
    },
    {
        "start",
        "\r\nstart [streamer]:\r\n Start an execution phase:\r\n"\
        " - streamer: start to stream data from the sensor raw data\r\n",
        StreamerCLIParseStartCommand,
        1
    },
    {
        "null",               /* command string to type */
        "",                   /* command online help string */
        CLIParseNullCmd,      /* function to run */
        0                     /* No parameters are expected. */
    }
};

/* Public API definition */
/*************************/

AManagedTaskEx *StreamerCLIAlloc(void)
{
  /* In this application there is only one Keyboard task,
   * so this allocator implement the singleton design pattern.
   */

  /* Initialize the super class */
  AMTInitEx(&sTaskObj.super);

  sTaskObj.super.vptr = &sTheClass.vtbl;
  sTaskObj.sensor_listener.vptr = &sTheClass.sensor_liestener_vtbl;

  return (AManagedTaskEx*)&sTaskObj;
}

sys_error_code_t StreamerCLISetDefSensorsConfig(StreamerCLI_t *_this, SensorsConfigF def_sensors_config_f)
{
  assert_param(_this != NULL);

  _this->def_sensors_config_f = def_sensors_config_f;

  return SYS_NO_ERROR_CODE;
}

/* AManagedTask virtual functions definition */
/*********************************************/

sys_error_code_t StreamerCLI_vtblHardwareInit(AManagedTask *_this, void *p_params)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  StreamerCLI_t *p_obj = (StreamerCLI_t*)_this;

  /*
   * The USB initialization is done here.
   */

  res = StreamerCLIInitUsbDevice(p_obj);

  return res;
}

sys_error_code_t StreamerCLI_vtblOnCreateTask(AManagedTask *_this, TaskFunction_t *p_task_code, const char **p_name, unsigned short *p_stack_depth, void **p_params, UBaseType_t *p_priority)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  StreamerCLI_t *p_obj = (StreamerCLI_t*)_this;

  /* initialize the object software resource here. */
  p_obj->in_queue = xQueueCreate(SCLI_TASK_CFG_IN_QUEUE_LENGTH, SCLI_TASK_CFG_IN_QUEUE_ITEM_SIZE);
  if (p_obj->in_queue == NULL) {
    res = SYS_TASK_HEAP_OUT_OF_MEMORY_ERROR_CODE;
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(res);
    return res;
  }

  /* create the software timer: one-shot timer. The period is changed in the START command execution. */
  p_obj->stop_timer = xTimerCreate("CTRLTim", 1, pdFALSE, _this, StreamerCLITimerStopCallback);
  if (p_obj->stop_timer == NULL) {
    res = SYS_TASK_HEAP_OUT_OF_MEMORY_ERROR_CODE;
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(res);
    return res;
  }

  /* initialize the circular buffer*/
  p_obj->cb.p_circular_buffer = CB_Alloc(SCLI_CB_N_ITEMS);
  if (p_obj->cb.p_circular_buffer == NULL)
  {
    res = SYS_TASK_HEAP_OUT_OF_MEMORY_ERROR_CODE;
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(res);
    return res;
  }
  CB_Init(p_obj->cb.p_circular_buffer, s_cb_items_buff, SCLI_CB_MAX_ITEM_SIZE_B);
  p_obj->cb.pConsumerDataBuff = NULL;
  p_obj->cb.p_producer_data_buff = NULL;
  p_obj->cb.data_idx = 0;

#ifdef DEBUG
  vQueueAddToRegistry(p_obj->in_queue, "SCLI_Q");
#endif

  p_obj->in_index = 0;
  p_obj->timer_period_ms = 0;
  p_obj->p_selected_sensor = NULL;
  p_obj->def_sensors_config_f = StreamerCLISensorsConfigF;

  /* register the CLI commands */
  RegisterGenericCLICommands();
  RegisterCLICommands(sCLICommands);

  /* set the (PM_STATE, ExecuteStepFunc) map from the class object.  */
  _this->m_pfPMState2FuncMap = sTheClass.p_pm_state2func_map;

  *p_task_code = AMTExRun;
  *p_name = "CTRL";
  *p_stack_depth = SCLI_TASK_CFG_STACK_DEPTH;
  *p_params = _this;
  *p_priority = SCLI_TASK_CFG_PRIORITY;

  return res;
}

sys_error_code_t StreamerCLI_vtblDoEnterPowerMode(AManagedTask *_this, const EPowerMode active_power_mode, const EPowerMode new_power_mode)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  StreamerCLI_t *p_obj = (StreamerCLI_t*)_this;

  if (new_power_mode == E_POWER_MODE_STATE1)
  {
    xQueueReset(p_obj->in_queue);
    struct CtrlMessage_t msg = {
        .msgId = APP_MESSAGE_ID_CTRL,
        .cmd_id = SCLI_CMD_DID_STOP
    };
    if (active_power_mode == E_POWER_MODE_SENSORS_ACTIVE)
    {
      msg.param = SCLI_CMD_PARAM_STREAMER;
    }
    xQueueSendToFront(p_obj->in_queue, &msg, pdMS_TO_TICKS(50));
  }
  else if (new_power_mode == E_POWER_MODE_SENSORS_ACTIVE)
  {
    /* start the stop timer if the period is > 0 */
    if (p_obj->timer_period_ms > 0)
    {
      if (pdFAIL  == xTimerChangePeriod(p_obj->stop_timer, pdMS_TO_TICKS(p_obj->timer_period_ms), pdMS_TO_TICKS(100)))
      {
        res = SYS_SCLI_TIMER_ERROR_CODE;
        SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_SCLI_TIMER_ERROR_CODE);
      }
    }
  }

  return res;
}

sys_error_code_t StreamerCLI_vtblHandleError(AManagedTask *_this, SysEvent error)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
/*  StreamerCLI_t *p_obj = (StreamerCLI_t*)_this; */

  return res;
}

sys_error_code_t StreamerCLI_vtblOnEnterTaskControlLoop(AManagedTask *_this)
{
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  StreamerCLI_t *p_obj = (StreamerCLI_t*)_this;

  SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("CTRL: start.\r\n"));

  /* Set the default configuration for the sensors */
  uint16_t sensor_id = SI_NULL_SENSOR_ID;
  if (p_obj->def_sensors_config_f != NULL)
  {
    p_obj->def_sensors_config_f(SMGetSensorManager(), &sensor_id);
    if (sensor_id != SI_NULL_SENSOR_ID)
    {
      p_obj->p_selected_sensor = SMGetSensorObserver(sensor_id);
    }
  }

  /* Register the CTRL as listener. */
  SIterator_t itarator;
  SIInit(&itarator, SMGetSensorManager());
  while (SIHasNext(&itarator))
  {
    sensor_id = SINext(&itarator);
    IEventSrc *p_src_if = ISourceGetEventSrcIF(SMGetSensorObserver(sensor_id));
    if (IEventSrcAddEventListener(p_src_if, (IEventListener*)&p_obj->sensor_listener) != SYS_NO_ERROR_CODE)
    {
      sys_error_handler();
    }
  }

  /*USB CDC Device start */

#if !defined(DEGUG) && !defined(SYS_DEBUG)
  /* in RELEASE disable the buffering of the standard out in order to have the console*/
  setvbuf(stdout, NULL, _IONBF, 0);
#endif

  /* Start Device Process */
  if (USBD_Start(&hUsbDeviceFS) != USBD_OK)
  {
    sys_error_handler();
  }

  vTaskDelay(pdMS_TO_TICKS(1000));

  StreamerCLIDisplayWelcomeMessage(*p_obj);

  return res;
}


/* AManagedTaskEx virtual functions definition */
/***********************************************/

sys_error_code_t StreamerCLI_vtblForceExecuteStep(AManagedTaskEx *_this, EPowerMode active_power_mode)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  StreamerCLI_t *p_obj = (StreamerCLI_t*)_this;

  struct CtrlMessage_t msg = {
      .msgId = APP_REPORT_ID_FORCE_STEP
  };
  if (xQueueSendToFront(p_obj->in_queue, &msg, pdMS_TO_TICKS(100)) != pdTRUE)
  {
    res = SYS_NAI_TASK_IN_QUEUE_FULL_ERROR_CODE;
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_NAI_TASK_IN_QUEUE_FULL_ERROR_CODE);
  }

  return res;
}

sys_error_code_t StreamerCLI_vtblOnEnterPowerMode(AManagedTaskEx *_this, const EPowerMode active_power_mode, const EPowerMode new_power_mode)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
/*  StreamerCLI_t *p_obj = (StreamerCLI_t*)_this; */

  return res;
}


/* IListener virtual functions definition */
/******************************************/

sys_error_code_t StreamerCLI_vtblOnStatusChange(IListener *this)
{
  /* not implemented */

  SYS_DEBUGF(SYS_DBG_LEVEL_WARNING, ("CTRL: IListener::OnStatusChange() not implemented.\r\n"));

  return SYS_NO_ERROR_CODE;
}

/* IEventListener virtual functions definition */
/***********************************************/

void *StreamerCLI_vtblGetOwner(IEventListener *_this)
{
  assert_param(_this != NULL);

  StreamerCLI_t *p_if_owner = (StreamerCLI_t*)((uint32_t)_this - offsetof(StreamerCLI_t, sensor_listener));

  return (void*)p_if_owner;
}

void StreamerCLI_vtblSetOwner(IEventListener *_this, void *p_owner)
{
  /* not implemented. The IF owner is always the task object.*/
  UNUSED(_this);
  UNUSED(p_owner);

  SYS_DEBUGF(SYS_DBG_LEVEL_WARNING, ("CTRL: no need to implement IEventListener::SetOwner().\r\n"));

  __NOP();
}


/* IEventListener virtual functions definition */
/***********************************************/
uint32_t g_counter = 0;
sys_error_code_t StreamerCLI_vtblOnNewDataReady(IEventListener *_this, const SensorEvent *p_evt)
{
  assert_param(_this != NULL);
  assert_param(p_evt != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  uint32_t cb_res = CB_NO_ERROR_CODE;
  UNUSED(cb_res);
  g_counter++;
  StreamerCLI_t *p_if_owner = (StreamerCLI_t*)((uint32_t)_this - offsetof(StreamerCLI_t, sensor_listener));

  if (p_if_owner->cb.p_producer_data_buff == NULL)
  {
    /* try to get a new free buffer to acquire the next signal.*/
    cb_res = CB_GetFreeItemFromHead(p_if_owner->cb.p_circular_buffer, &p_if_owner->cb.p_producer_data_buff);
    if (p_if_owner->cb.p_producer_data_buff != NULL)
    {
      /*we have a new buffer, so then we start to acquire the data from zero*/
      p_if_owner->cb.data_idx = 0;
    }
  }

  if (p_if_owner->cb.p_producer_data_buff != NULL)
  {
    if (!SCLI_IS_BUFF_READY(&p_if_owner->cb))
    {
      const uint32_t data_count = p_evt->stream->payload_size;
      uint8_t *p_src = p_evt->stream->payload;
      uint8_t *p_dest = &((uint8_t*)CB_GetItemData(p_if_owner->cb.p_producer_data_buff))[p_if_owner->cb.data_idx*3*SCLI_CB_ITEM_ELEMENT_SIZE_B];
      for (uint32_t i=0; i<data_count*3*SCLI_CB_ITEM_ELEMENT_SIZE_B; ++i)
      {
        p_dest[i] = *p_src++;
      }
      p_if_owner->cb.data_idx += (data_count * 3);

      /* if the signal is ready, notify the task*/
      if (SCLI_IS_BUFF_READY(&p_if_owner->cb))
      {
        CB_SetItemReady(p_if_owner->cb.p_circular_buffer, p_if_owner->cb.p_producer_data_buff);
        p_if_owner->cb.p_producer_data_buff = NULL;

        /*notify the task that a new data buffer is ready*/
        struct CtrlMessage_t msg = {
            .msgId = APP_MESSAGE_ID_CTRL,
            .cmd_id = SCLI_CMD_NEW_DATA_READY
        };
        if (pdTRUE != xQueueSendToBack(p_if_owner->in_queue, &msg, pdMS_TO_TICKS(100)))
        {
          res = SYS_SCLI_IN_BUFF_FULL_ERROR_CODE;
          SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_SCLI_IN_BUFF_FULL_ERROR_CODE);

          sys_error_handler();
        }
      }
    }
  }

  return res;
}


/* Private function definition */
/*******************************/

static sys_error_code_t StreamerCLIExecuteStepState1(AManagedTask *_this)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  StreamerCLI_t *p_obj = (StreamerCLI_t*)_this;
  struct CtrlMessage_t msg = {0};
  char ch; /* char received from the USB CDC */

  AMTExSetInactiveState((AManagedTaskEx*)_this, TRUE);
  if (pdTRUE == xQueueReceive(p_obj->in_queue, &msg, portMAX_DELAY))
  {
    AMTExSetInactiveState((AManagedTaskEx*)_this, FALSE);
    if (msg.msgId == APP_MESSAGE_ID_CTRL)
    {
      switch (msg.cmd_id)
      {
        case SCLI_CMD_NEW_CHAR:
          for (uint8_t i=0; i<msg.param; ++i)
          {
            ch = (char)msg.data[i];
            res = StreamerCLIProcessNewChar(p_obj, ch);
          }
          break;

        case SCLI_CMD_STRAT:
          if (msg.sparam == SCLI_CMD_PARAM_FROM_AUTOMODE)
          {
            StreamerCLISetDefaultConfig(p_obj);
          }
          res = StreamerCLIStartExecutionPhase(p_obj, msg.param);
          break;

        case SCLI_CMD_DID_STOP:
          fprintf(SCLI_TASK_CFG_OUT_CH, "End of execution phase\r\n");
          fprintf(SCLI_TASK_CFG_OUT_CH, sPromptMessage);
          break;

        default:
          SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("CTRL: unexpected command ID:0x%x\r\n", msg.cmd_id));
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

static sys_error_code_t StreamerCLIExecuteStepSensorsActive(AManagedTask *_this)
{
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  StreamerCLI_t *p_obj = (StreamerCLI_t*)_this;
  struct CtrlMessage_t msg = {0};
  char ch = '\0';

  AMTExSetInactiveState((AManagedTaskEx*)_this, TRUE);
  if (pdTRUE == xQueueReceive(p_obj->in_queue, &msg, portMAX_DELAY))
  {
    AMTExSetInactiveState((AManagedTaskEx*)_this, FALSE);
    if (msg.msgId == APP_MESSAGE_ID_CTRL)
    {
      switch (msg.cmd_id)
      {
        case SCLI_CMD_NEW_CHAR:
          for (uint8_t i=0; i<msg.param; ++i)
          {
            ch = (char)msg.data[i];
            /* check for special case */
            if (ch == CLI_KEY_CODE_ESC)
            {
              /* in MEAI_ACTIVE the ESC key is the only we process*/
              /* generate the system event.*/
              SysEvent evt = {
                  .nRawEvent = SYS_PM_MAKE_EVENT(SYS_PM_EVT_SRC_CTRL, SYS_PM_EVENT_PARAM_STOP_STREAMER)
              };
              SysPostPowerModeEvent(evt);
            }
          }
          break;

        case SCLI_CMD_NEW_DATA_READY:
          CB_GetReadyItemFromTail(p_obj->cb.p_circular_buffer, &p_obj->cb.pConsumerDataBuff);
          if (StreamerCLIStreamData(p_obj, p_obj->cb.pConsumerDataBuff) == SYS_NO_ERROR_CODE)
          {
            CB_ReleaseItem(p_obj->cb.p_circular_buffer, p_obj->cb.pConsumerDataBuff);
          }
          else
          {
            sys_error_handler();
          }
          break;

        default:
          SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("CTRL: unexpected command ID:0x%x\r\n", msg.cmd_id));
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

static sys_error_code_t StreamerCLIInitUsbDevice(StreamerCLI_t *_this)
{
  UNUSED(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  /* Enable USB power */
  HAL_PWREx_EnableVddUSB();
  /* Init Device Library, add supported class and start the library. */
  if (USBD_Init(&hUsbDeviceFS, &CDC_Desc, DEVICE_FS) != USBD_OK)
  {
    sys_error_handler();
  }
  /* Add Supported Class */
  if (USBD_RegisterClass(&hUsbDeviceFS, &USBD_CDC) != USBD_OK)
  {
    sys_error_handler();
  }
  /* Add Interface callbacks for CDC Class */
  if (USBD_CDC_RegisterInterface(&hUsbDeviceFS, &USBD_Interface_fops_FS) != USBD_OK)
  {
    sys_error_handler();
  }

  return res;
}
#if 0
static sys_error_code_t StreamerCLIDeInitUsbDevice(StreamerCLI_t *_this)
{
  UNUSED(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  /* Stop USB */
  USBD_Stop(&hUsbDeviceFS);

  /* DeInit Device Library */
  USBD_DeInit(&hUsbDeviceFS);

  return res;
}
#endif

static sys_error_code_t StreamerCLIDisplayWelcomeMessage(StreamerCLI_t _this)
{
  UNUSED(_this);

  fprintf(SCLI_TASK_CFG_OUT_CH, "\n\n\r-----------------------------------------------------------------\n\r");
  fprintf(SCLI_TASK_CFG_OUT_CH, SCLI_CFG_CLI_APP_NAME_STR);
  fprintf(SCLI_TASK_CFG_OUT_CH, "\n\r-----------------------------------------------------------------\n\r");
  fprintf(SCLI_TASK_CFG_OUT_CH, sWelcomeMessage);
  fprintf(SCLI_TASK_CFG_OUT_CH, sPromptMessage);

  return SYS_NO_ERROR_CODE;
}

static sys_error_code_t StreamerCLIProcessNewChar(StreamerCLI_t *_this, char ch)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  BaseType_t more_data_to_follow = 0;

  /* check for special case */
  if (ch == CLI_KEY_CODE_ESC)
  {
    /* in STATE1 ignore the ESC key */
    __NOP();
  }
  if (ch == '\b')
  {
    if (_this->in_index > 0)
    {
      /* Backspace was pressed.  Erase the last character in the input
      buffer - if there are any. */
      _this->in_string[--_this->in_index] = '\0';
      /* Hack to erase characters */
      fprintf(SCLI_TASK_CFG_OUT_CH,"\b \b");
    }
  }
  else if (ch == '\r')
  {
    /* Send LF to avoid issues when only CR has been sent */
    fprintf(SCLI_TASK_CFG_OUT_CH, "\r\n");
    if (strlen(_this->in_string) != 0)
    {
      do
      {
        /* Send the command string to the command interpreter.  Any
        output generated by the command interpreter will be placed in the
        out_string buffer. */
        more_data_to_follow = FreeRTOS_CLIProcessCommand(_this->in_string, _this->out_string, SCLI_TASK_CFG_MAX_OUT_LENGTH);

        /* Write the output generated by the command interpreter to the console. */
        fprintf(SCLI_TASK_CFG_OUT_CH, _this->out_string);
      } while(more_data_to_follow != 0);
    }
    /* All the strings generated by the input command have been sent.
    Processing of the command is complete.  Clear the input string ready
    to receive the next command. */
    _this->in_index = 0;
    memset(_this->in_string, 0x00, SCLI_TASK_CFG_MAX_IN_LENGTH);

    /* Display the prompt */
    fprintf(SCLI_TASK_CFG_OUT_CH, sPromptMessage);
    fflush(SCLI_TASK_CFG_OUT_CH);
  }
  else if (_this->in_index < SCLI_TASK_CFG_MAX_IN_LENGTH)
  {
      /* A character was entered.  It was not a new line, backspace
      or carriage return, so it is accepted as part of the input and
      placed into the input buffer.  When a \n is entered the complete
      string will be passed to the command interpreter. */
    _this->in_string[_this->in_index++] = ch;

    /* print the char in the console */
    fprintf(SCLI_TASK_CFG_OUT_CH, "%c", ch);
    fflush(SCLI_TASK_CFG_OUT_CH);
  }
  else
  {
    /* the input buffer is full. Track the error */
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_SCLI_IN_BUFF_FULL_ERROR_CODE);
    res = SYS_SCLI_IN_BUFF_FULL_ERROR_CODE;
  }

  return res;
}

static sys_error_code_t StreamerCLIStartExecutionPhase(StreamerCLI_t *_this, uint32_t exec_phase)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  /* 1. find the active sensor and check that there is sonly one sensor active */
  SQuery_t query;
  SQInit(&query, SMGetSensorManager());
  uint16_t sensor_id = SQNextByStatusEnable(&query, true);
  if (sensor_id == SI_NULL_SENSOR_ID)
  {
    fprintf(SCLI_TASK_CFG_OUT_CH, "CTRL: unable to start the execution phase with no sensors active\r\n");
    fprintf(SCLI_TASK_CFG_OUT_CH, sPromptMessage);

    return res;
  }

  if (SQNextByStatusEnable(&query, true) != SI_NULL_SENSOR_ID)
  {
    fprintf(SCLI_TASK_CFG_OUT_CH, "CTRL: unable to start the execution phase with more than one sensor active\r\n");
    fprintf(SCLI_TASK_CFG_OUT_CH, sPromptMessage);

    return res;
  }

  /* 2. find the active sensor*/
  _this->p_selected_sensor = SMGetSensorObserver(sensor_id);

  if (!SYS_IS_ERROR_CODE(res))
  {
    /* 5. wait for the task to process the messages */
    vTaskDelay(pdMS_TO_TICKS(200));

    /* 6. notify the user about the start of teh execution phase */
    {
      fprintf(SCLI_TASK_CFG_OUT_CH, "Streaming data from the active sensor...\r\n");
    }

    /* 7 trigger the power mode transaction */
    SysEvent evt = {
        .nRawEvent = SYS_PM_MAKE_EVENT(SYS_PM_EVT_SRC_CTRL, SYS_PM_EVENT_PARAM_START_STREAMER)
    };
    SysPostPowerModeEvent(evt);
  }

  return res;
}


static BaseType_t StreamerCLIParseSensorGetAllCmd(char *p_write_buffer, size_t write_buffer_len, ISourceObservable *p_sensor)
{
  assert_param(p_sensor != NULL);
  portBASE_TYPE res = pdTRUE;
  static uint16_t sAvailableODRIdx = 0;
  static uint16_t sAvailableFullScaleIdx = 0;
  static uint8_t sPrintedParamsIdx = 0;
  ISensor_t *p_sensor_ex = NULL; /* using this interface is a workaround for some missing API of SensorManager */
  float measuredORD, nominalODR;
  SensorDescriptor_t descriptor = {0};

  switch (sPrintedParamsIdx)
  {
    case 0:
      /* print sensor enable */
      p_sensor_ex = (ISensor_t*)p_sensor;
      snprintf(p_write_buffer, write_buffer_len, "enable = %s\r\n", ISensorIsEnabled(p_sensor_ex) ? "true" : "false");
      break;

    case 1:
      /* print sensor ODR */
      ISourceGetODR(p_sensor, &measuredORD, &nominalODR);
      snprintf(p_write_buffer, write_buffer_len, "nominal ODR = %0.2f Hz, latest measured ODR = %0.2f Hz\r\nAvailabe ODRs:\r\n", nominalODR, measuredORD);
      break;

    case 2:
      /* print sensor available ODRs */
      descriptor = SMSensorGetDescription(ISourceGetId(p_sensor));
      snprintf(p_write_buffer, write_buffer_len, "%0.2f Hz\r\n", descriptor.pODR[sAvailableODRIdx]);
      sAvailableODRIdx++;
      if (descriptor.pODR[sAvailableODRIdx] == COM_END_OF_LIST_FLOAT) {
        // this is the last available ODR
       sAvailableODRIdx = 0;
      }
      break;

    case 3:
      /* print sensor full scale */
      descriptor = SMSensorGetDescription(ISourceGetId(p_sensor));
      snprintf(p_write_buffer, write_buffer_len, "fullScale = %0.2f %s\r\nAvailable fullScales:\r\n", ISourceGetFS(p_sensor), descriptor.unit);
      break;

    case 4:
      /* print sensor available fullScales */
      descriptor = SMSensorGetDescription(ISourceGetId(p_sensor));
      snprintf(p_write_buffer, write_buffer_len, "%0.2f %s\r\n", descriptor.pFS[sAvailableFullScaleIdx], descriptor.unit);
      sAvailableFullScaleIdx++;
      if (descriptor.pFS[sAvailableFullScaleIdx] == COM_END_OF_LIST_FLOAT) {
        // this is the last available fullScale
        sAvailableFullScaleIdx = 0;
      }
      break;

    default:
      res = pdFALSE;
      break;
  }

  if (!sAvailableODRIdx && !sAvailableFullScaleIdx) {
    if (++sPrintedParamsIdx > 4) {
      sPrintedParamsIdx = 0;
      res = pdFALSE;
    }
  }

  return res;
}

static BaseType_t StreamerCLIParseSensorInfoCmd(char *p_write_buffer, size_t write_buffer_len, const char *p_command_string)
{
  static SIterator_t iterator;
  static bool is_iterator_initialized = false;
  BaseType_t res = pdTRUE;

  if (!is_iterator_initialized)
  {
    SIInit(&iterator, SMGetSensorManager());
    is_iterator_initialized = true;
  }
  if(SIHasNext(&iterator))
  {
    uint16_t sensor_id = SINext(&iterator);
    SensorDescriptor_t descriptor = SMSensorGetDescription(sensor_id);
    snprintf(p_write_buffer, write_buffer_len, "%s ID=%d, type=%s\r\n", descriptor.Name, sensor_id, sSensorTypeDecoder[descriptor.SensorType-1]);
  }
  else
  {
    snprintf(p_write_buffer, write_buffer_len, "%d sensors supported\r\n", SMGetNsensor());
    is_iterator_initialized = false;
    res = pdFALSE;
  }

  return res;
}

static BaseType_t StreamerCLIParseSensorGetCommand(char *p_write_buffer, size_t write_buffer_len, const char *p_command_string)
{
  portBASE_TYPE res = pdFALSE;
  BaseType_t  param1_str_length = 0;
  BaseType_t  param2_str_length = 0;
  uint8_t sensor_id = 0;
  ISourceObservable *p_sensor = NULL;
  ISensor_t *p_sensor_ex = NULL; /* using this interface is a workaround for some missing API of SensorManager */
  SensorDescriptor_t descriptor = {0};

  static uint16_t sAvailableODRIdx = 0;
  static uint16_t sAvailableFullScaleIdx = 0;

  // validate the parameter
  const char *p_aram1 = FreeRTOS_CLIGetParameter(p_command_string, 1, &param1_str_length);
  const char *p_aram2 = FreeRTOS_CLIGetParameter(p_command_string, 2, &param2_str_length);

  if (SYS_NO_ERROR_CODE != StreamerCLIParseSensorID(p_aram1, param1_str_length, &sensor_id))
  {
    snprintf(p_write_buffer, write_buffer_len, CLI_INVALID_PARAMETER_ERROR_MSG, p_aram1);
  }
  else
  {
    p_sensor = SMGetSensorObserver(sensor_id);
    // check parameter two
    if (!strncmp(p_aram2, CLI_PARAM_SENSOR_ENABLE, strlen(CLI_PARAM_SENSOR_ENABLE)))
    {
      p_sensor_ex = (ISensor_t*)p_sensor;
      snprintf(p_write_buffer, write_buffer_len, "enable = %s\r\n", ISensorIsEnabled(p_sensor_ex) ? "true" : "false");
    }
    else if (!strncmp(p_aram2, CLI_PARAM_SENSOR_GET_AVAILABLE_ODRS, strlen(CLI_PARAM_SENSOR_GET_AVAILABLE_ODRS)))
    {
      descriptor = SMSensorGetDescription(ISourceGetId(p_sensor));
      snprintf(p_write_buffer, write_buffer_len, "%0.2f Hz\r\n", descriptor.pODR[sAvailableODRIdx]);
      sAvailableODRIdx++;
      if (descriptor.pODR[sAvailableODRIdx] == COM_END_OF_LIST_FLOAT) {
        // this is the last available ODR
       sAvailableODRIdx = 0;
      }
      else {
        // we need to print another line.
        res = pdTRUE;
      }
    }
    else if (!strncmp(p_aram2, CLI_PARAM_SENSOR_GET_AVAILABLE_FULLSCALES, strlen(CLI_PARAM_SENSOR_GET_AVAILABLE_FULLSCALES)))
    {
      descriptor = SMSensorGetDescription(ISourceGetId(p_sensor));
      snprintf(p_write_buffer, write_buffer_len, "%0.2f %s\r\n", descriptor.pFS[sAvailableFullScaleIdx], descriptor.unit);
      sAvailableFullScaleIdx++;
      if (descriptor.pFS[sAvailableFullScaleIdx] == COM_END_OF_LIST_FLOAT) {
        // this is the last available fullScale
        sAvailableFullScaleIdx = 0;
      }
      else {
        // we need to print another line.
        res = pdTRUE;
      }
    }
    else if (!strncmp(p_aram2, CLI_PARAM_SENSOR_ODR, strlen(CLI_PARAM_SENSOR_ODR)))
    {
      float measuredORD, nominalODR;
      ISourceGetODR(p_sensor, &measuredORD, &nominalODR);
      snprintf(p_write_buffer, write_buffer_len, CLI_SET_ODR_MSG, nominalODR, measuredORD);
    }
    else if (!strncmp(p_aram2, CLI_PARAM_SENSOR_FULL_SCALE, strlen(CLI_PARAM_SENSOR_FULL_SCALE)))
    {
      descriptor = SMSensorGetDescription(ISourceGetId(p_sensor));
      snprintf(p_write_buffer,write_buffer_len, "fullScale = %0.2f %s\r\n", ISourceGetFS(p_sensor), descriptor.unit);
    }
    else if (!strncmp(p_aram2, CLI_PARAM_GET_ALL, strlen(CLI_PARAM_GET_ALL)))
    {
      res = StreamerCLIParseSensorGetAllCmd(p_write_buffer, write_buffer_len, p_sensor);
    }
    else
    {
      snprintf(p_write_buffer, write_buffer_len, CLI_INVALID_PARAMETER_ERROR_MSG, p_aram2);
    }
  }

  return res;
}

static sys_error_code_t StreamerCLIParseSensorID(const char *p_param, BaseType_t param_length, uint8_t *p_sensor_id)
{
  assert_param(p_sensor_id);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  // check if all char are digit
  const char *p_ch = p_param;
  for (int i=0; i<param_length; ++i)
  {
    if (!((*p_ch >= '0') && (*p_ch <= '9')))
    {
      res = SYS_INVALID_PARAMETER_ERROR_CODE;
      break;
    }
    p_ch++;
  }

  if (res == SYS_NO_ERROR_CODE)
  {
    // parse the string to integer
    *p_sensor_id = (uint8_t)atoi(p_param);
    if (*p_sensor_id >= SMGetNsensor())
    {
      res = SYS_INVALID_PARAMETER_ERROR_CODE;
    }
  }

  return res;
}

static BaseType_t StreamerCLIParseSensorSetCommand(char *p_write_buffer, size_t write_buffer_len, const char *p_command_string)
{
  portBASE_TYPE res = pdFALSE;
  BaseType_t  param1_str_length = 0;
  BaseType_t  param2_str_length = 0;
  BaseType_t  param3_str_length = 0;
  uint8_t sensor_id = 0;
  ISourceObservable *p_sensor = NULL;
  ISensor_t *p_sensor_ex = NULL; /* using this interface is a workaround for some missing API of SensorManager */

  // validate the parameter
  const char *p_param1 = FreeRTOS_CLIGetParameter(p_command_string, 1, &param1_str_length);
  const char *p_param2 = FreeRTOS_CLIGetParameter(p_command_string, 2, &param2_str_length);
  const char *p_param3 = FreeRTOS_CLIGetParameter(p_command_string, 3, &param3_str_length);

  if (SYS_NO_ERROR_CODE != StreamerCLIParseSensorID(p_param1, param1_str_length, &sensor_id))
  {
    snprintf(p_write_buffer, write_buffer_len, CLI_INVALID_PARAMETER_ERROR_MSG, p_param1);
  }
  else
  {
    p_sensor = SMGetSensorObserver(sensor_id);
    // check parameter two
    if (!strncmp(p_param2, CLI_PARAM_SENSOR_ENABLE, strlen(CLI_PARAM_SENSOR_ENABLE))) {
      // validate parameter 3
      if ((param3_str_length == 1) && (*p_param3 >= '0') && (*p_param3 <= '1')) {
        bool enable =  *p_param3 == '0' ? false : true ;
        // enable/disable the sensor
        if (enable)
        {
          SMSensorEnable(sensor_id);
        }
        else
        {
          SMSensorDisable(sensor_id);
        }
        // read the status to print the actual value.
        p_sensor_ex = (ISensor_t*)p_sensor;
        snprintf(p_write_buffer, write_buffer_len,  "sensor %d: %s\r\n",
            sensor_id,
            ISensorIsEnabled(p_sensor_ex) ? "enable" : "disable"
        );
      }
      else
      {
        snprintf(p_write_buffer, write_buffer_len, CLI_INVALID_PARAMETER_ERROR_MSG, p_param3);
      }
    }
    else if (!strncmp(p_param2, CLI_PARAM_SENSOR_ODR, strlen(CLI_PARAM_SENSOR_ODR)))
    {
      float odr = atof(p_param3);
      /* read the nominal ODR of the sensor */
      float measured_odr, nominal_odr;
      ISourceGetODR(p_sensor, &measured_odr, &nominal_odr);
      /* change the ODR using the user parameter */
      if (SMSensorSetODR(sensor_id, odr) == SYS_NO_ERROR_CODE)
      {
        float new_nominal_odr;
        ISourceGetODR(p_sensor, &measured_odr, &new_nominal_odr);
        /* check if the new ODR is good */
        if (new_nominal_odr == odr)
        {
          snprintf(p_write_buffer, write_buffer_len, CLI_SET_ODR_MSG, new_nominal_odr, measured_odr);
        }
        else
        {
          /* the user parameter is not a valid ODR. Restore the previous data and inform the user*/
          SMSensorSetODR(sensor_id, nominal_odr);
          snprintf(p_write_buffer, write_buffer_len, "ODR %0.2f Hz is not supported\r\n", odr);
        }
      }
      else
      {
        snprintf(p_write_buffer, write_buffer_len, CLI_INVALID_PARAMETER_ERROR_MSG, p_param3);
      }
    }
    else if (!strncmp(p_param2, CLI_PARAM_SENSOR_FULL_SCALE, strlen(CLI_PARAM_SENSOR_FULL_SCALE)))
    {
      float fs = atof(p_param3);
      /* read the nominal FS of the sensor */
      float nominal_fs;
      nominal_fs = ISourceGetFS(p_sensor);
      /* change the FS using the user parameter */
      if (SMSensorSetFS(sensor_id, fs) == SYS_NO_ERROR_CODE)
      {
        float new_nominal_fs =  ISourceGetFS(p_sensor);
        /* check if the new FS is good */
        if (new_nominal_fs == fs)
        {
          snprintf(p_write_buffer, write_buffer_len,"sensor FS: %0.2f\r\n", new_nominal_fs);
        }
        else
        {
          /* the user parameter is not a valid FS. Restore the previous data and inform the user*/
          SMSensorSetFS(sensor_id, nominal_fs);
          snprintf(p_write_buffer, write_buffer_len, "FS %0.2f is not supported\r\n", fs);
        }
      }
      else
      {
        snprintf(p_write_buffer, write_buffer_len, CLI_INVALID_PARAMETER_ERROR_MSG, p_param3);
      }
    }
    else
    {
      snprintf(p_write_buffer, write_buffer_len, CLI_INVALID_PARAMETER_ERROR_MSG, p_param2);
    }
  }

  return res;
}

static BaseType_t StreamerCLIParseStartCommand(char *p_write_buffer, size_t write_buffer_len, const char *p_command_string)
{
  BaseType_t  param1_str_length= 0;
  StreamerCLI_t *p_obj = &sTaskObj;
  bool error = false;
  struct CtrlMessage_t msg = {
      .msgId = APP_MESSAGE_ID_CTRL,
      .cmd_id = SCLI_CMD_STRAT,
      .sparam = SCLI_CMD_PARAM_FROM_CLI
  };

  // validate the parameter
  const char *pcParam1 = FreeRTOS_CLIGetParameter(p_command_string, 1, &param1_str_length);
  if (!strncmp(pcParam1, CLI_PARAM_START_STREAMER, strlen(CLI_PARAM_START_STREAMER)))
  {
    msg.param  = SCLI_CMD_PARAM_STREAMER;
    if (xQueueSendToBack(p_obj->in_queue, &msg, pdMS_TO_TICKS(100)) == pdTRUE)
    {
      snprintf(p_write_buffer, write_buffer_len, "Sensor streamer: start streaming sensor data...\r\n");
    }
    else
    {
      error = true;
    }
  }
  else
  {
    snprintf(p_write_buffer, write_buffer_len, CLI_INVALID_PARAMETER_ERROR_MSG, pcParam1);
  }

  if (error)
  {
    snprintf(p_write_buffer, write_buffer_len, "error starting the sensor streamer.\r\n");
  }

  return pdFALSE;
}

static BaseType_t StreamerCLIParseStreamerGetCmd(char *p_write_buffer, size_t write_buffer_len, const char *p_command_string)
{
  BaseType_t  param1_str_length = 0;
  static int8_t sShowAllStep = 0;

  /* parse param 1 */
  const char *p_param1 = FreeRTOS_CLIGetParameter(p_command_string, 1, &param1_str_length);
  if (!strncmp(p_param1, CLI_PARAM_TIMER, strlen(CLI_PARAM_TIMER)))
  {
    snprintf(p_write_buffer, write_buffer_len, "Sensor streamer: timer = %lu ms\r\n", sTaskObj.timer_period_ms);
  }
  else if (!strncmp(p_param1, CLI_PARAM_GET_ALL, strlen(CLI_PARAM_GET_ALL)))
  {
    sShowAllStep++;
    if (sShowAllStep == 1) {
      snprintf(p_write_buffer, write_buffer_len, "Sensor streamer: timer = %lu ms\r\n", sTaskObj.timer_period_ms);
      sShowAllStep = 0;
    }
    return sShowAllStep == 0 ? pdFALSE : pdTRUE;
  }
  else
  {
    // if we are here the parameter is not valid.
    sprintf(p_write_buffer, CLI_INVALID_PARAMETER_ERROR_MSG, p_param1);
  }

  return pdFALSE;
}

static BaseType_t StreamerCLIParseStreamerSetCmd(char *p_write_buffer, size_t write_buffer_len, const char *p_command_string)
{

  BaseType_t  param1_str_length = 0;

  /* parse param 1 */
  const char *p_param1 = FreeRTOS_CLIGetParameter(p_command_string, 1, &param1_str_length);
  if (!strncmp(p_param1, CLI_PARAM_TIMER, strlen(CLI_PARAM_TIMER)))
  {
    return StreamerCLIParseStreamerSetTimerCmd(p_write_buffer, write_buffer_len, p_command_string);
  }

  // if we are here the parameter is not valid.
  sprintf(p_write_buffer, CLI_INVALID_PARAMETER_ERROR_MSG, p_param1);

  return pdFALSE;
}

static BaseType_t StreamerCLIParseStreamerSetTimerCmd(char *p_write_buffer, size_t write_buffer_len, const char *p_command_string)
{
  BaseType_t  param2_str_length = 0;

  const char *p_param2 = FreeRTOS_CLIGetParameter(p_command_string, 2, &param2_str_length);
  /* validate the param */
  uint32_t new_timer_period_ms = atoi(p_param2);

  if (new_timer_period_ms > SCLI_MAX_TIME_MS_PARAM) {
    snprintf(p_write_buffer, write_buffer_len, "NanoEdge AI: invalid timeout. Value out of range.\r\n");
  }
  else
  {
    sTaskObj.timer_period_ms = new_timer_period_ms;
    snprintf(p_write_buffer, write_buffer_len, "NanoEdge AI: timer set to %lu ms\r\n", new_timer_period_ms);
  }

  return pdFALSE;
}

static void StreamerCLITimerStopCallback(TimerHandle_t xTimer)
{
  /* generate the system event to stop stop the execution.*/
  SysEvent evt = {
      .nRawEvent = SYS_PM_MAKE_EVENT(SYS_PM_EVT_SRC_CTRL, SYS_PM_EVENT_PARAM_STOP_STREAMER)
  };
  SysPostPowerModeEvent(evt);
}

static sys_error_code_t StreamerCLISetDefaultConfig(StreamerCLI_t *_this)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  /* First disable all sensors*/
  SIterator_t itarator;
  uint16_t sensor_id = SI_NULL_SENSOR_ID;
  SIInit(&itarator, SMGetSensorManager());
  while (SIHasNext(&itarator))
  {
    sensor_id = SINext(&itarator);
    SMSensorDisable(sensor_id);
  }
  /* Enable only the accelerometer of the combo sensor "ism330dhcx". */
  /* This is the default configuration. */
  SQuery_t query;
  SQInit(&query, SMGetSensorManager());
  sensor_id = SQNextByNameAndType(&query, "ism330dhcx",  COM_TYPE_ACC);
  if (sensor_id != SI_NULL_SENSOR_ID)
  {
    SMSensorEnable(sensor_id);
    /* set the default configuration for the X-CUBE-AI HAR demo*/
    fprintf(SCLI_TASK_CFG_OUT_CH, "CTRL: automode. Set default configuration.\r\n");
    SMSensorSetODR(sensor_id, 1666.0f);
  }
  else
  {
    res = SYS_SCLI_WRONG_CONF_ERROR_CODE;
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_SCLI_WRONG_CONF_ERROR_CODE);

    SYS_DEBUGF(SYS_DBG_LEVEL_WARNING, ("CTRL: automode. ism330dhcx not found.\r\n"));
  }

  return res;
}

static sys_error_code_t StreamerCLIStreamData(StreamerCLI_t *_this, CBItem *p_item)
{
  assert_param(_this != NULL);
  assert_param(p_item != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  int16_t *p_data = (int16_t*)CB_GetItemData(p_item);
  register int16_t x, y, z;

  for (uint32_t i=0; i<256; ++i)
  {
    x = p_data[i*3];
    y = p_data[(i*3) + 1];
    z = p_data[(i*3) + 2];
    fprintf(SCLI_TASK_CFG_OUT_CH, "%d, %d, %d\r\n", x,y,z);
  }

  return res;
}

static sys_error_code_t StreamerCLISensorsConfigF(SensorManager_t *p_sm, uint16_t *p_active_sensor_id)
{
  assert_param(p_sm != NULL);
  assert_param(p_active_sensor_id != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  /* First disable all sensors */
  SIterator_t itarator;
  uint16_t sensor_id = SI_NULL_SENSOR_ID;
  SIInit(&itarator, p_sm);
  while (SIHasNext(&itarator))
  {
    sensor_id = SINext(&itarator);
    SMSensorDisable(sensor_id);
  }

  return res;
}


/* C-Runtime integration */
/*************************/
#if defined (__IAR_SYSTEMS_ICC__)

/** @brief IAR specific low level standard output
 * @param Handle IAR internal handle
 * @param Buf Buffer containing characters to be written to stdout
 * @param Bufsize Number of characters to write
 * @retval Number of characters write
 */
int __io_write_in_console(const unsigned char *ptr, int len)
{
  if (len == 0)
  {
    return 0;
  }

  // try to transmit data through the USB
  uint16_t timeout = 3000; // try to send the data timeout time.
  for (; timeout; --timeout)
  {
    if (CDC_Transmit_FS((uint8_t *)ptr, len) == USBD_OK)
    {
      break;
    }
  }
  if (timeout)
  {
    timeout = 3000;
    /* Transmit zero-length packet to complete transfer */
    for (; timeout; --timeout)
    {
      if (CDC_Transmit_FS((uint8_t *)ptr, 0) == USBD_OK)
      {
        break;
      }
    }
  }

  return len;
}

#elif defined (__CC_ARM)

/* integration with eLooM framework: */
/* 1. we map the low level put_char to the framework function used for the log. */
#if defined(DEBUG) || defined(SYS_DEBUG)
extern int __io_putchar(int x);
#endif

/**
 * @brief fputc call for standard output implementation
 * @param ch Character to print
 * @param f File pointer
 * @retval Character printed
 */
int fputc(int ch, FILE *f)
{
  //int filen = 1;//fileno(f);
#if defined(DEGUG) || defined(SYS_DEBUG)
	if (f == &__stderr)
  {
    /* remap the stderr on the console */

    // try to transmit data through the USB
    uint16_t nTimeout = 3000; // try to send the data nTimout time.
    for (; nTimeout; --nTimeout)
    {
      if (CDC_Transmit_FS((uint8_t *)&ch, 1) == USBD_OK)
      {
        break;
      }
    }
    if (nTimeout)
    {
      nTimeout = 3000;
      /* Transmit zero-length packet to complete transfer */
      for (; nTimeout; --nTimeout)
      {
        if (CDC_Transmit_FS((uint8_t *)&ch, 0) == USBD_OK)
        {
          break;
        }
      }
    }
  }
  else {
    __io_putchar(ch);
  }
#else
  /* in RELEASE the log is disabled so we use stdout for the console. */
  if (f == &__stdout)
  {
    // try to transmit data through the USB
    uint16_t nTimeout = 3000; // try to send the data nTimout time.
    for (; nTimeout; --nTimeout)
    {
      if (CDC_Transmit_FS((uint8_t *)&ch, 1) == USBD_OK)
      {
        break;
      }
    }
    if (nTimeout)
    {
      nTimeout = 3000;
      /* Transmit zero-length packet to complete transfer */
      for (; nTimeout; --nTimeout)
      {
        if (CDC_Transmit_FS((uint8_t *)&ch, 0) == USBD_OK)
        {
          break;
        }
      }
    }
  }
#endif


  return ch;
}

/** @brief fgetc call for standard input implementation
 * @param f File pointer
 * @retval Character acquired from standard input
 */
int fgetc(FILE *f)
{
  return -1;
}

#elif defined (__GNUC__)
int __io_write_in_console(char *ptr, int len)
{
  if (len == 0)
  {
    return 0;
  }

  // try to transmit data through the USB
  uint16_t timeout = 3000; // try to send the data timeout time.
  for (; timeout; --timeout)
  {
    if (CDC_Transmit_FS((uint8_t *)ptr, len) == USBD_OK)
    {
      break;
    }
  }
  if (timeout)
  {
    timeout = 3000;
    /* Transmit zero-length packet to complete transfer */
    for (; timeout; --timeout)
    {
      if (CDC_Transmit_FS((uint8_t *)ptr, 0) == USBD_OK)
      {
        break;
      }
    }
  }

  return len;
}

#else
#error "Toolchain not supported"
#endif

/**
 ******************************************************************************
 * @file    AppController.c
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

#include "AppController.h"
#include "AppController_vtbl.h"
#include "AppControllerMessagesDef.h"
#include "app_messages_parser.h"
#include "AIMessagesDef.h"
#include "NeaiMessagesDef.h"
#include "cli_commands.h"
#include "events/IProcessEventListener.h"
#include "events/IProcessEventListener_vtbl.h"
#include "ADPU.h"
#include "services/ai_helper.h"
#include "services/sysdebug.h"


#ifndef CTRL_TASK_CFG_STACK_DEPTH
#define CTRL_TASK_CFG_STACK_DEPTH                      (120)
#endif

#ifndef CTRL_TASK_CFG_PRIORITY
#define CTRL_TASK_CFG_PRIORITY                         (tskIDLE_PRIORITY)
#endif

#ifndef CTRL_TASK_CFG_IN_QUEUE_LENGTH
#define CTRL_TASK_CFG_IN_QUEUE_LENGTH                  10
#endif
#define CTRL_TASK_CFG_IN_QUEUE_ITEM_SIZE               (sizeof(struct CtrlMessage_t))

#if defined(DEBUG) || defined (SYS_DEBUG)
#define CTRL_TASK_CFG_OUT_CH                           stderr
#else
#define CTRL_TASK_CFG_OUT_CH                           stdout
#endif
#define CTRL_TASK_SENSORS_N                            2
#define CTRL_TASK_DEF_THRESHOLD                        90
#define CTRL_MAX_SIGNALS_PARAM                         0x100000U ///< Max number of signals to pass as parameter to the "neai_set signals <value>" command
#define CTRL_MAX_TIME_MS_PARAM                         (1000U*60U*60U*24U*2U) ///< Man time in ms to pass to as parameter to the "neai_set timer <value>" command. It is 2 days.

#define CTRL_NEAI_CB_ITEMS                             2
#define CTRL_AI_CB_ITEMS                               3

#define CTRL_HAR_CLASSES                               4

#define CLI_PARAM_START_NEAI_LEARN                     "neai_learn"
#define CLI_PARAM_START_NEAI_DETECT                    "neai_detect"
#define CLI_PARAM_START_AI                             "ai"
#define CLI_PARAM_SENSOR_ODR                           "ODR"
#define CLI_PARAM_SENSOR_FULL_SCALE                    "FS"
#define CLI_PARAM_SENSOR_ENABLE                        "enable"
#define CLI_PARAM_SENSOR_GET_AVAILABLE_ODRS            "ODR_List"
#define CLI_PARAM_SENSOR_GET_AVAILABLE_FULLSCALES      "FS_List"
#define CLI_PARAM_GET_ALL                              "all"
#define CLI_PARAM_TIMER                                "timer"
#define CLI_PARAM_SIGNALS                              "signals"
#define CLI_PARAM_SENSITIVITY                          "sensitivity"
#define CLI_PARAM_THRESHOLD                            "threshold"
#define CLI_PARAM_INFO                                 "info"
#define CLI_INVALID_PARAMETER_ERROR_MSG                "Invalid parameter: %s\r\n"
#define CLI_GENERIC_TIMEOUT_ERROR_MSG                  "CLI: cannot execute the commands. Try again later\r\n"
#define CLI_SET_ODR_MSG                                "nominal ODR = %0.2f Hz, latest measured ODR = %0.2f Hz\r\n"
#define CLI_DETECT_MSG                                 "{\"signal\": %lu, \"similarity\": %lu, \"status\": \"anomaly\"}\r\n"
#define CLI_LEARN_MSG                                  "{\"signal\": %lu, \"status\": %s}\r\n"
#define CLI_AI_HAR_MSG                                 "{\"signal\": %lu, \"class\": %s, \"percentage\": %lu}\r\n"
#define CLI_LEARN_SUCCESS_STATUS                       "\"success\""
#define CLI_LEARN_FAILED_STATUS                        "\"failed\""
#define CLI_DETECT_NOMINAL_STATUS                      "\"nominal\""
#define CLI_DETECT_ANOMALY_STATUS                      "\"anomaly\""

#define KEY_CODE_ESC                                   (0x1B)

#ifndef UTIL_CMD_ID_LED2_SET_ENABLE
#define UTIL_CMD_ID_LED2_SET_ENABLE                   ((uint16_t)0x0006)              ///< LED2 set enable command. Valid value are: 1(enable), 0(disable)
#endif
#ifndef UTIL_CMD_ID_LED2_SET_BLK_PERIOD
#define UTIL_CMD_ID_LED2_SET_BLK_PERIOD               ((uint16_t)0x0007)              ///< LED2 set the blinking period. Valid value are: 0(always off), 255(always on), n(multiple of 60ms)
#endif

#define SYS_DEBUGF(level, message)                     SYS_DEBUGF3(SYS_DBG_CTRL, level, message)

#if defined(DEBUG) || defined (SYS_DEBUG)
#define sTaskObj                                       sAppCTRLTaskObj
#endif


static const IProcessEventListener_vtbl sACProcesEventListener_vtbl = {
    ACProcEvtListener_vtblOnStatusChange,
    ACProcEvtListener_vtblSetOwner,
    ACProcEvtListener_vtblGetOwner,
    ACProcEvtListener_vtblOnProcessedDataReady
};

/**
 * Class object declaration. The class object encapsulates members that are shared between
 * all instance of the class.
 */
typedef struct _AppControllerClass_t {
  /**
   * AppController class virtual table.
   */
  AManagedTaskEx_vtbl vtbl;

  /**
   * AppController class (PM_STATE, ExecuteStepFunc) map. The map is implemented with an array and
   * the key is the index. Number of items of this array must be equal to the number of PM state
   * of the application. If the managed task does nothing in a PM state, then set to NULL the
   * relative entry in the map.
   */
  pExecuteStepFunc_t p_pm_state2func_map[];
} AppControllerClass_t;


/* Private member function declaration */
/***************************************/

/**
 * Execute one step of the task control loop while the system is in STATE1.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return SYS_NO_EROR_CODE if success, a task specific error code otherwise.
 */
static sys_error_code_t AppControllerExecuteStepState1(AManagedTask *_this);

/**
 * Execute one step of the task control loop while the system is in AI_ACTIVE.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return SYS_NO_EROR_CODE if success, a task specific error code otherwise.
 */
static sys_error_code_t AppControllerExecuteStepAIActive(AManagedTask *_this);

/**
 * Execute one step of the task control loop while the system is in NEAI_ACTIVE.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return SYS_NO_EROR_CODE if success, a task specific error code otherwise.
 */
static sys_error_code_t AppControllerExecuteStepNEAIActive(AManagedTask *_this);

/**
 * Initialize the USB CDC device
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return SYS_NO_ERROR_CODE
 */
static sys_error_code_t AppControllerInitUsbDevice(AppController_t *_this);

#if 0
/**
 * Reset the USB CDC device to its initial state.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return SYS_NO_ERROR_CODE
 */
static sys_error_code_t AppControllerDeInitUsbDevice(AppController_t *_this);
#endif

/**
 * Print the welcome message
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return SYS_NO_ERROR_CODE
 */
static sys_error_code_t AppControllerDisplayWelcomeMessage(AppController_t _this);

/**
 * Process a new char.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @param ch [IN] specifies a new input char.
 * @return SYS_NO_ERROR_CODE if success, an error code otherwise.
 */
static sys_error_code_t AppControllerProcessNewChar(AppController_t *_this, char ch);

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
static sys_error_code_t AppControllerParseSensorID(const char *p_param, BaseType_t param_length, uint8_t *p_sensor_id);

/**
 * Start an execution phase. Processing this command will trigger an PM transaction.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @param exec_phase [IN] specifies an execution phase. Valid value are:
 *   - CTRL_CMD_PARAM_NEAI_LEARN
 *   - CTRL_CMD_PARAM_NEAI_DETECT
 *   - CTRL_CMD_PARAM_AI
 * @return SYS_NO_ERROR_CODE if success, an error code otherwise.
 */
static sys_error_code_t AppControllerStartExecutionPhase(AppController_t *_this, uint32_t exec_phase);

/**
 * Detach the sensor from the active AI.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @param active_ai_proc [IN] specifies teh active AI process. Valid param are:
 *  - CTRL_CMD_PARAM_NEAI_DETECT
 *  - CTRL_CMD_PARAM_AI
 * @return SYS_NO_ERROR_CODE if success, an error code otherwise.
 */
static sys_error_code_t AppControllerDetachSensorFromAIProc(AppController_t *_this, uint32_t active_ai_proc);

/**
 * This function is a workaround to a minor issue of the SensorManager.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @param sensor_id [IN] specifies a sensor ID
 * @return The ::SubSensorDescriptor_t of the sensor.
 */
static SubSensorDescriptor_t AppControllerGetSubSensorDescriptor(AppController_t *_this, uint8_t sensor_id);

/**
 * Print in the console all available parameters for a sensor.
 *
 * @param pcWriteBuffer [IN] specifies the console write buffer.
 * @param xWriteBufferLen [IN] specifies the size in byte of teh console write buffer.
 * @param p_sensor [IN] specifies a sensor.
 * @return
 */
static BaseType_t AppControllerParseSensorGetAllCmd(char *pcWriteBuffer, size_t xWriteBufferLen, ISourceObservable *p_sensor);

static BaseType_t AppControllerParseNeaiParseNeaiSetSensitivityCmd(char *p_write_buffer, size_t write_buffer_len, const char *p_command_string);
static BaseType_t AppControllerParseNeaiParseNeaiSetThresholdCmd(char *p_write_buffer, size_t write_buffer_len, const char *p_command_string);
static BaseType_t AppControllerParseNeaiParseNeaiSetSignalsCmd(char *p_write_buffer, size_t write_buffer_len, const char *p_command_string);
static BaseType_t AppControllerParseNeaiParseNeaiSetTimerCmd(char *p_write_buffer, size_t write_buffer_len, const char *p_command_string);

/**
 * Check if a NanoEdge AI command can be executed according to the task object state.
 *
 * @param _this [IN] specifies a pointer to the task object.
 * @param p_msg [IN] specifies a pointer to a command received by the CLI parser
 * @param p_write_buffer [OUT] specifies the output buffer to send error message in the CLI
 * @param write_buffer_len [IN] specifies the size of the pcWriteBuffer
 *
 * @return SYS_NO_ERROR_CODE if the command can be executed, SYS_PMTASK_INVALID_CMD_ERROR_CODE oterwise
 */
static sys_error_code_t AppControllerCanExecuteNeaiCLICommand(AppController_t *_this, const struct NeaiMessage_t *p_msg, char *p_write_buffer, size_t write_buffer_len);

/**
 * Function called when the Stop timer expires.
 *
 * @param xTimer [IN] specifies an handle to the expired timer.
 */
static void AppControllerTimerStopCallback(TimerHandle_t xTimer);

/**
 * Check teh precondition to run the X-CUBE-AI use case:
 * - sensor ODR must be 26 Hz
 *
 * @param _this [IN] specifies a pointer to the task object.
 * @param p_sensor [IN] specifies the active sensor.
 * @return true if all precondition are verified, false otherwise.
 */
static bool AppControllerCanStartAI(AppController_t *_this, ISourceObservable *p_sensor);

/**
 * Set the default configuration to start an execution phase through the Automode
 *
 * @param _this [IN] specifies a pointer to the task object.
 * @param mode [IN] specify the execution phase. Valid value are:
 *          - CTRL_CMD_PARAM_NEAI_LEARN
 *          - CTRL_CMD_PARAM_NEAI_DETECT
 *          - CTRL_CMD_PARAM_AI
 * @return SYS_NO_ERROR_CODE if success, an error code otherwise
 */
static sys_error_code_t AppControllerSetAutomodeConfig(AppController_t *_this, uint32_t mode);


static void AppControllerEnableDisableDualBoot(void);


/* Inline function forward declaration */
/***************************************/

#if defined (__GNUC__) || defined (__ICCARM__)
/* Inline function defined inline in the header file AppController.h must be declared here as extern function. */
extern QueueHandle_t AppControllerGetInQueue(AppController_t *_this);
#endif

float NanoEdgeAI_get_sensitivity(void);
bool NeaiTaskIsStubLibrary(void);

/**
 * USB FS Device descriptor.
 */
extern USBD_DescriptorsTypeDef FS_Desc;

/**
 * Handle to USB FS device.
 */
extern USBD_HandleTypeDef hUsbDeviceFS;

/**
 * The only instance of the task object.
 */
static AppController_t sTaskObj;

/**
 * The class object.
 */
static const AppControllerClass_t sTheClass = {
    /* Class virtual table */
    {
        AppController_vtblHardwareInit,
        AppController_vtblOnCreateTask,
        AppController_vtblDoEnterPowerMode,
        AppController_vtblHandleError,
        AppController_vtblOnEnterTaskControlLoop,
        AppController_vtblForceExecuteStep,
        AppController_vtblOnEnterPowerMode
    },

    /* class (PM_STATE, ExecuteStepFunc) map */
    {
        AppControllerExecuteStepState1,
        NULL,
        NULL,
        NULL,
        AppControllerExecuteStepAIActive,
        AppControllerExecuteStepNEAIActive
    }
};

static const char * const sWelcomeMessage =
  "\r\nConsole command server.\r\nType 'help' to view a list of registered commands.\r\n";
static const char * const sPromptMessage = "\r\n$ ";

/**
 * Specifies the label for the four class of th HAR demo.
 */
static const char* sHarClassLabels[] = {
    "\"Slow\"",
    "\"Fast\"",
    "\"Slow Disturbances\"",
    "\"Fast Disturbances\"",
    "\"Unknown\""
};

/* Define the FreeRTOS CLI commands */
/************************************/

static BaseType_t AppControllerParseSensorInfoCmd(char *p_write_buffer, size_t write_buffer_len, const char *p_command_string);
static BaseType_t AppControllerParseSensorGetCommand(char *p_write_buffer, size_t write_buffer_len, const char *p_command_string);
static BaseType_t AppControllerParseSensorSetCommand(char *p_write_buffer, size_t write_buffer_len, const char *p_command_string);
static BaseType_t AppControllerParseStartCommand(char *p_write_buffer, size_t write_buffer_len, const char *p_command_string);
static BaseType_t AppControllerParseNeaiInitCmd(char *p_write_buffer, size_t write_buffer_len, const char *p_command_string);
static BaseType_t AppControllerParseNeaiSetCmd(char *p_write_buffer, size_t write_buffer_len, const char *p_command_string);
static BaseType_t AppControllerParseNeaiGetCmd(char *p_write_buffer, size_t write_buffer_len, const char *p_command_string);
static BaseType_t AppControllerParseAiGetCmd(char *p_write_buffer, size_t write_buffer_len, const char *p_command_string);
static BaseType_t AppControllerParseSwitchBankCmd(char *p_write_buffer, size_t write_buffer_len, const char *p_command_string);

static const CLI_Command_Definition_t sCLICommands[] = {
    {
        "sensor_info",
        "\r\nsensor_info:\r\n Get a list of all supported sensors and their ID.\r\n",
        AppControllerParseSensorInfoCmd,
        0
    },
    {
        "sensor_get",
        "\r\nsensor_get <id> <param>:\r\n Gets the value of a \'parameter\' for a sensor with sensor id provided in \'id\'.\r\n"\
        " <param> ::= enable | ODR | ODR_List | FS | FS_List |\r\n"\
        "             all\r\n"\
        " eg.: \'sensor_get 1 ODR\' prints nominal and latest measured ODR for the sensor.\r\n"\
        " parameters.\r\n",
        AppControllerParseSensorGetCommand,
        2 /* 2 parameters are expected. */
    },
    {
        "sensor_set",
        "\r\nsensor_set <id> <param> <value>:\r\n Sets the \'value\' of a \'parameter\' for a sensor with sensor id provided in \'id\'.\r\n"\
        " <param> ::= enable | ODR | FS\r\n"\
        " eg.: \'sensor_set 1 enable 1\' enables the sensor 1\r\n",
        AppControllerParseSensorSetCommand,
        3
    },
    {
        "neai_init",
        "\r\nneai_init:\r\n Initialize the memory of the NanoEdge AI Library's model.\r\n",
        AppControllerParseNeaiInitCmd,
        0
    },
    {
        "neai_get",
        "\r\nneai_get <param>:\r\n Display the value of the NanoEdge AI parameters. Valid parameters are:"\
        "\r\n - timer"\
        "\r\n - signals"\
        "\r\n - sensitivity"\
        "\r\n - threshold"\
        "\r\n - all\r\n",
        AppControllerParseNeaiGetCmd,
        1
    },
    {
        "neai_set",
        "\r\nneai_set <parameter> <value>:\r\n Set a NanoEdge AI specific parameter.\r\n"\
        " Valid parameters and values are:\r\n"\
        " - sensitivity: float [0, 100]\r\n"\
        " - threshold: integer [0%, 100%]\r\n"\
        " - signals: integer [0, MAX_SIGNALS]\r\n"\
        " - timer: integer [0ms, MAX_TIME_MS]\r\n",
        AppControllerParseNeaiSetCmd,
        2
    },
    {
        "ai_get",
        "\r\nai_get <param>:\r\n Display the value of the X-CUBE-AI parameters. Valid parameters are:"\
        "\r\n - info\r\n",
        AppControllerParseAiGetCmd,
        1
    },
    {
        "start",
        "\r\nstart [ai | neai_learn | neai_detect]:\r\n Start an execution phase:\r\n"\
        " - ai: X-CUBE-AI evaluates signals from the sensor raw data\r\n"\
        " - neai_learn: NanoEdge AI library learns new signals from the sensor raw data\r\n"\
        " - neai_detect: NanoEdge AI library evaluates signals from the sensor raw data\r\n",
        AppControllerParseStartCommand,
        1
    },
    {
        "switch_bank",
        "\r\nswitch_bank:\r\n Switch the BOOT banck and reset the MCU.\r\n",
        AppControllerParseSwitchBankCmd,
        0
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

AManagedTaskEx *AppControllerAlloc(void)
{
  /* In this application there is only one Keyboard task,
   * so this allocator implement the singleton design pattern.
   */

  /* Initialize the super class */
  AMTInitEx(&sTaskObj.super);

  sTaskObj.super.vptr = &sTheClass.vtbl;

  /* "alloc" the listener IF */
  sTaskObj.listenetIF.super.vptr = &sACProcesEventListener_vtbl;

  return (AManagedTaskEx*)&sTaskObj;
}

sys_error_code_t AppControllerSetSensorsIF(AppController_t *_this, ISourceObservable *p_ism330dhcx_acc, ISourceObservable *p_ism330dhcx_gyr, ISourceObservable *p_is3dwb)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  if ((p_is3dwb == NULL) || (p_ism330dhcx_acc == NULL) || (p_ism330dhcx_gyr == NULL))
  {
    res = SYS_INVALID_PARAMETER_ERROR_CODE;
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_INVALID_PARAMETER_ERROR_CODE);
  }
  else
  {
    _this->p_is3dwb_acc = p_is3dwb;
    _this->p_ism330dhcx_acc = p_ism330dhcx_acc;
    _this->p_ism330dhcx_gyr = p_ism330dhcx_gyr;
  }

  return res;
}

sys_error_code_t AppControllerSetAIProcessesInQueue(AppController_t *_this, QueueHandle_t ai_queue, QueueHandle_t neai_queue, QueueHandle_t util_queue)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  _this->ai_in_queue = ai_queue;
  _this->neai_in_queue = neai_queue;
  _this->util_in_queue = util_queue;

  return res;
}

/* AManagedTask virtual functions definition */
/*********************************************/

sys_error_code_t AppController_vtblHardwareInit(AManagedTask *_this, void *p_params)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  AppController_t *p_obj = (AppController_t*)_this;

  /*
   * The USB initialization is done here.
   */

  res = AppControllerInitUsbDevice(p_obj);

  return res;
}

sys_error_code_t AppController_vtblOnCreateTask(AManagedTask *_this, TaskFunction_t *p_task_code, const char **p_name, unsigned short *p_stack_depth, void **p_params, UBaseType_t *p_priority)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  AppController_t *p_obj = (AppController_t*)_this;

  /* initialize the object software resource here. */
  p_obj->in_queue = xQueueCreate(CTRL_TASK_CFG_IN_QUEUE_LENGTH, CTRL_TASK_CFG_IN_QUEUE_ITEM_SIZE);
  if (p_obj->in_queue == NULL) {
    res = SYS_TASK_HEAP_OUT_OF_MEMORY_ERROR_CODE;
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(res);
    return res;
  }

  /* create the software timer: one-shot timer. The period is changed in the START command execution. */
  p_obj->stop_timer = xTimerCreate("CTRLTim", 1, pdFALSE, _this, AppControllerTimerStopCallback);
  if (p_obj->stop_timer == NULL) {
    res = SYS_TASK_HEAP_OUT_OF_MEMORY_ERROR_CODE;
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(res);
    return res;
  }

  /* initialize the IProcessEventListener interface */
  IEventListenerSetOwner((IEventListener*)&p_obj->listenetIF, p_obj);

#ifdef DEBUG
  vQueueAddToRegistry(p_obj->in_queue, "CTRL_Q");
#endif

  p_obj->in_index = 0;
  p_obj->detect_threshold = CTRL_TASK_DEF_THRESHOLD;
  p_obj->signal_count = 0;
  p_obj->signals = 0;
  p_obj->timer_period_ms = 0;
  p_obj->ai_in_queue = NULL;
  p_obj->neai_in_queue = NULL;
  p_obj->util_in_queue = NULL;

  /* register the CLI commands */
  RegisterGenericCLICommands();
  RegisterCLICommands(sCLICommands);

  /* set the (PM_STATE, ExecuteStepFunc) map from the class object.  */
  _this->m_pfPMState2FuncMap = sTheClass.p_pm_state2func_map;

  *p_task_code = AMTExRun;
  *p_name = "CTRL";
  *p_stack_depth = CTRL_TASK_CFG_STACK_DEPTH;
  *p_params = _this;
  *p_priority = CTRL_TASK_CFG_PRIORITY;

  return res;
}

sys_error_code_t AppController_vtblDoEnterPowerMode(AManagedTask *_this, const EPowerMode active_power_mode, const EPowerMode new_power_mode)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  AppController_t *p_obj = (AppController_t*)_this;

  if (new_power_mode == E_POWER_MODE_STATE1)
  {
    xQueueReset(p_obj->in_queue);
    struct CtrlMessage_t msg = {
        .msgId = APP_MESSAGE_ID_CTRL,
        .cmd_id = CTRL_CMD_DID_STOP
    };
    if (active_power_mode == E_POWER_MODE_NEAI_ACTIVE)
    {
      msg.param = CTRL_CMD_PARAM_NEAI_DETECT; //TODO: check this value? Why I use only NEAI_DETECT ? What about NEAI_LEARN ?
    }
    else if (active_power_mode == E_POWER_MODE_X_CUBE_AI_ACTIVE)
    {
      msg.param = CTRL_CMD_PARAM_AI;
    }
    xQueueSendToFront(p_obj->in_queue, &msg, pdMS_TO_TICKS(50));
  }
  else if ((new_power_mode == E_POWER_MODE_NEAI_ACTIVE) /*|| (active_power_mode == E_POWER_MODE_X_CUBE_AI_ACTIVE)*/)
  {
    /* start the stop timer if the period is > 0 */
    if (p_obj->timer_period_ms > 0)
    {
      if (pdFAIL  == xTimerChangePeriod(p_obj->stop_timer, pdMS_TO_TICKS(p_obj->timer_period_ms), pdMS_TO_TICKS(100)))
      {
        res = SYS_CTRL_TIMER_ERROR_CODE;
        SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_CTRL_TIMER_ERROR_CODE);
      }
    }
  }

  return res;
}

sys_error_code_t AppController_vtblHandleError(AManagedTask *_this, SysEvent error)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
/*  AppController_t *p_obj = (AppController_t*)_this; */

  return res;
}

sys_error_code_t AppController_vtblOnEnterTaskControlLoop(AManagedTask *_this)
{
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  AppController_t *p_obj = (AppController_t*)_this;

  SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("CTRL: start.\r\n"));

  /* Set the default configuration for the sensors */
  /* Keep enable only the acc of the combo sensor. */
  SMSensorDisable(ISourceGetId(p_obj->p_ism330dhcx_gyr));
  SMSensorDisable(ISourceGetId(p_obj->p_is3dwb_acc));
  SMSensorSetODR(ISourceGetId(p_obj->p_ism330dhcx_acc), 1667);
  SMSensorSetFS(ISourceGetId(p_obj->p_ism330dhcx_acc), 4.0);

  struct NeaiMessage_t msg1 = {
      .msgId = APP_MESSAGE_ID_NEAI,
      .cmd_id = NAI_CMD_ADD_DPU_LISTNER,
      .param.n_param = (uint32_t)&p_obj->listenetIF
  };
  if (pdTRUE != xQueueSendToBack(p_obj->neai_in_queue, &msg1, pdMS_TO_TICKS(100)))
  {
    res = SYS_NAI_TASK_IN_QUEUE_FULL_ERROR_CODE;
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_NAI_TASK_IN_QUEUE_FULL_ERROR_CODE);
  }

  struct AIMessage_t msg2 = {
      .msgId = APP_MESSAGE_ID_AI,
      .cmd_id = AI_CMD_ADD_DPU_LISTNER,
      .param = (uint32_t)&p_obj->listenetIF
  };
  if (pdTRUE != xQueueSendToBack(p_obj->ai_in_queue, &msg2, pdMS_TO_TICKS(100)))
  {
    res = SYS_AI_TASK_IN_QUEUE_FULL_ERROR_CODE;
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_AI_TASK_IN_QUEUE_FULL_ERROR_CODE);
  }

  /*USB CDC Device start */

#if !defined(DEGUG) && !defined(SYS_DEBUG)
  /* in RELEASE disable the buffering of teh standard out in order to have the console*/
  setvbuf(stdout, NULL, _IONBF, 0);
#endif

  /* Start Device Process */
  if (USBD_Start(&hUsbDeviceFS) != USBD_OK)
  {
    sys_error_handler();
  }

  vTaskDelay(pdMS_TO_TICKS(1000));

  AppControllerDisplayWelcomeMessage(*p_obj);

  return res;
}


/* AManagedTaskEx virtual functions definition */
/***********************************************/

sys_error_code_t AppController_vtblForceExecuteStep(AManagedTaskEx *_this, EPowerMode active_power_mode)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  AppController_t *p_obj = (AppController_t*)_this;

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

sys_error_code_t AppController_vtblOnEnterPowerMode(AManagedTaskEx *_this, const EPowerMode active_power_mode, const EPowerMode new_power_mode)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
/*  AppController_t *p_obj = (AppController_t*)_this; */

  return res;
}


/* IListener virtual functions definition */
/******************************************/

sys_error_code_t ACProcEvtListener_vtblOnStatusChange(IListener *_this)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  /*ACProcessEventListener_t *p_obj = (ACProcessEventListener_t*)_this; */

  return res;
}


/* IEventListener virtual functions definition */
/***********************************************/

void ACProcEvtListener_vtblSetOwner(IEventListener *_this, void *pxOwner)
{
  assert_param(_this != NULL);
  ACProcessEventListener_t *p_obj = (ACProcessEventListener_t*)_this;
  p_obj->p_owner = pxOwner;
}

void *ACProcEvtListener_vtblGetOwner(IEventListener *_this)
{
  assert_param(_this != NULL);
  ACProcessEventListener_t *p_obj = (ACProcessEventListener_t*)_this;
  return p_obj->p_owner;
}


/* IEventListener virtual functions definition */
/***********************************************/

sys_error_code_t ACProcEvtListener_vtblOnProcessedDataReady(IEventListener *_this, const ProcessEvent *pxEvt)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  /*ACProcessEventListener_t *p_obj = (ACProcessEventListener_t*)_this; */
  AppController_t *p_owner = (AppController_t*) (((size_t)_this) - offsetof(AppController_t, listenetIF));
  struct CtrlMessage_t msg = {
      .msgId = APP_MESSAGE_ID_CTRL,
  };

  if(pxEvt->tag == CTRL_CMD_PARAM_AI)
  {
    /* this result come from X-CUBE-AI process. We know the fortmat of teh stream */
    float *proc_res = ((float*) pxEvt->stream->payload);
    msg.cmd_id = CTRL_CMD_AI_PROC_RES;
    msg.param = (uint32_t)(proc_res[1] * 100.0f); // convert HAR percentage in integer value
    msg.sparam = (uint8_t) proc_res[0]; // convert HAR class in integer value
  }
  else
  {
    /* this result come from X-CUBE-AI process. We know the fortmat of teh stream */
    uint16_t proc_res = (uint16_t)(*((float*)pxEvt->stream->payload));
    uint8_t neai_mode = pxEvt->tag == 1 ? CTRL_CMD_PARAM_NEAI_LEARN : CTRL_CMD_PARAM_NEAI_DETECT;
    msg.cmd_id = CTRL_CMD_NEAI_PROC_RES;
    msg.param = proc_res;
    msg.sparam = neai_mode;
  }

  if (pdTRUE != xQueueSendToBack(p_owner->in_queue, &msg, pdMS_TO_TICKS(100)))
  {
    res = SYS_CTRL_IN_QUEUE_FULL_ERROR_CODE;
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_CTRL_IN_QUEUE_FULL_ERROR_CODE);
  }

  return res;
}


/* Private function definition */
/*******************************/

static sys_error_code_t AppControllerExecuteStepState1(AManagedTask *_this)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  AppController_t *p_obj = (AppController_t*)_this;
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
        case CTRL_CMD_NEW_CHAR:
          ch = (char)msg.param;
          res = AppControllerProcessNewChar(p_obj, ch);
          break;

        case CTRL_CMD_STRAT:
          if (msg.sparam == CTRL_CMD_PARAM_FROM_AUTOMODE)
          {
            AppControllerSetAutomodeConfig(p_obj, msg.param);
          }
          res = AppControllerStartExecutionPhase(p_obj, msg.param);
          break;

        case CTRL_CMD_DID_STOP:
          res = AppControllerDetachSensorFromAIProc(p_obj, msg.param);
          fprintf(CTRL_TASK_CFG_OUT_CH, "End of execution phase\r\n");
          fprintf(CTRL_TASK_CFG_OUT_CH, sPromptMessage);
          break;

        case CTRL_CMD_SWITCH_BANK:
          AppControllerEnableDisableDualBoot();
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

static sys_error_code_t AppControllerExecuteStepAIActive(AManagedTask *_this)
{
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  AppController_t *p_obj = (AppController_t*)_this;
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
        case CTRL_CMD_NEW_CHAR:
          ch = (char)msg.param;
          /* check for special case */
          if (ch == KEY_CODE_ESC)
          {
            /* in MEAI_ACTIVE the ESC key is the only we process*/
            /* generate the system event.*/
            SysEvent evt = {
                .nRawEvent = SYS_PM_MAKE_EVENT(SYS_PM_EVT_SRC_CTRL, SYS_PM_EVENT_PARAM_STOP_PROCESSING)
            };
            SysPostPowerModeEvent(evt);
          }
          break;

        case CTRL_CMD_AI_PROC_RES:
          /* I am going to consume the data. Increase the signal_count. */
          ++p_obj->signal_count;
          if (msg.sparam >= CTRL_HAR_CLASSES)
          {
            msg.sparam = CTRL_HAR_CLASSES;
          }
          fprintf(CTRL_TASK_CFG_OUT_CH, CLI_AI_HAR_MSG, p_obj->signal_count, sHarClassLabels[msg.sparam], msg.param);
          /* send a message to the UTIL task to notify the HAR class trough the ORANGE LED */
          if (p_obj->util_in_queue != NULL)
          {
            struct utilMessage_t outMsg = {
                .msgId = APP_MESSAGE_ID_UTIL,
                .cmd_id = UTIL_CMD_ID_LED2_SET_BLK_PERIOD,
            };
            switch (msg.sparam)
            {
              case 0: /* class 0 stand still */
                outMsg.sparam = 0;
                break;
              case 1: /* class 0 walking */
                outMsg.sparam = 3;
                break;
              case 2: /* class 0 jogging */
                outMsg.sparam = 1;
                break;
              case 3: /* class 0 biking */
                outMsg.sparam = 0xFF;
                break;
              default:
                outMsg.sparam = 0;
                break;
            }
            xQueueSendToBack(p_obj->util_in_queue, &outMsg, pdMS_TO_TICKS(100));
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

static sys_error_code_t AppControllerExecuteStepNEAIActive(AManagedTask *_this)
{
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  AppController_t *p_obj = (AppController_t*)_this;
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
        case CTRL_CMD_NEW_CHAR:
          ch = (char)msg.param;
          /* check for special case */
          if (ch == KEY_CODE_ESC)
          {
            /* in NEAI_ACTIVE the ESC key is the only we process*/
            /* stop the timer if it is started*/
            if (p_obj->timer_period_ms > 0)
            {
              xTimerStop(p_obj->stop_timer, pdMS_TO_TICKS(200));
            }
            /* generate the system event.*/
            SysEvent evt = {
                .nRawEvent = SYS_PM_MAKE_EVENT(SYS_PM_EVT_SRC_CTRL, SYS_PM_EVENT_PARAM_STOP_PROCESSING)
            };
            SysPostPowerModeEvent(evt);
          }
          break;

        case CTRL_CMD_NEAI_PROC_RES:
          /* I am going to consume the data. Increase the signal_count. */
          ++p_obj->signal_count;
          if (msg.sparam == CTRL_CMD_PARAM_NEAI_LEARN)
          {
            fprintf(CTRL_TASK_CFG_OUT_CH, CLI_LEARN_MSG, p_obj->signal_count, msg.param == 1 ? CLI_LEARN_SUCCESS_STATUS : CLI_LEARN_FAILED_STATUS);
            /*check the result to blink LED2 in case of signal not learned*/
            if (msg.param != 1)
            {
              if (p_obj->util_in_queue != NULL)
              {
                struct utilMessage_t outMsg = {
                    .msgId = APP_MESSAGE_ID_UTIL,
                    .cmd_id = UTIL_CMD_ID_LED2_SET_ENABLE,
                    .sparam = 1
                };
                xQueueSendToBack(p_obj->util_in_queue, &outMsg, pdMS_TO_TICKS(100));
              }
            }
          }
          else if (msg.sparam == CTRL_CMD_PARAM_NEAI_DETECT)
          { /* we are in detection mode*/
            if (msg.param <= p_obj->detect_threshold)
            {
              fprintf(CTRL_TASK_CFG_OUT_CH, CLI_DETECT_MSG, p_obj->signal_count, msg.param);

              /*blink LED2*/
              if (p_obj->util_in_queue != NULL)
              {
                struct utilMessage_t outMsg = {
                    .msgId = APP_MESSAGE_ID_UTIL,
                    .cmd_id = UTIL_CMD_ID_LED2_SET_ENABLE,
                    .sparam = 1
                };
                xQueueSendToBack(p_obj->util_in_queue, &outMsg, pdMS_TO_TICKS(100));
              }
            }
          }

          if ((p_obj->signals != 0) && !(p_obj->signal_count < p_obj->signals))
          {
            /* stop the timer if it is started*/
            if (p_obj->timer_period_ms > 0)
            {
              xTimerStop(p_obj->stop_timer, pdMS_TO_TICKS(200));
            }
            /* generate the system event.*/
            SysEvent evt = {
                .nRawEvent = SYS_PM_MAKE_EVENT(SYS_PM_EVT_SRC_CTRL, SYS_PM_EVENT_PARAM_STOP_PROCESSING)
            };
            SysPostPowerModeEvent(evt);
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

static sys_error_code_t AppControllerInitUsbDevice(AppController_t *_this)
{
  UNUSED(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  /* Enable USB power */
  HAL_PWREx_EnableVddUSB();
  /* Init Device Library, add supported class and start the library. */
  if (USBD_Init(&hUsbDeviceFS, &FS_Desc, DEVICE_FS) != USBD_OK)
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
static sys_error_code_t AppControllerDeInitUsbDevice(AppController_t *_this)
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

static sys_error_code_t AppControllerDisplayWelcomeMessage(AppController_t _this)
{
  UNUSED(_this);

  fprintf(CTRL_TASK_CFG_OUT_CH, "\n\n\r-----------------------------------------------------------------\n\r");
  fprintf(CTRL_TASK_CFG_OUT_CH, "! FP-AI-MONITOR1 !");
  fprintf(CTRL_TASK_CFG_OUT_CH, "\n\r-----------------------------------------------------------------\n\r");
  fprintf(CTRL_TASK_CFG_OUT_CH, sWelcomeMessage);
  fprintf(CTRL_TASK_CFG_OUT_CH, sPromptMessage);

  return SYS_NO_ERROR_CODE;
}

static sys_error_code_t AppControllerProcessNewChar(AppController_t *_this, char ch)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  BaseType_t more_data_to_follow = 0;

  /* check for special case */
  if (ch == KEY_CODE_ESC)
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
      fprintf(CTRL_TASK_CFG_OUT_CH,"\b \b");
    }
  }
  else if (ch == '\r')
  {
    /* Send LF to avoid issues when only CR has been sent */
    fprintf(CTRL_TASK_CFG_OUT_CH, "\r\n");
    if (strlen(_this->in_string) != 0)
    {
      do
      {
        /* Send the command string to the command interpreter.  Any
        output generated by the command interpreter will be placed in the
        out_string buffer. */
        more_data_to_follow = FreeRTOS_CLIProcessCommand(_this->in_string, _this->out_string, CTRL_TASK_CFG_MAX_OUT_LENGTH);

        /* Write the output generated by the command interpreter to the console. */
        fprintf(CTRL_TASK_CFG_OUT_CH, _this->out_string);
      } while(more_data_to_follow != 0);
    }
    /* All the strings generated by the input command have been sent.
    Processing of the command is complete.  Clear the input string ready
    to receive the next command. */
    _this->in_index = 0;
    memset(_this->in_string, 0x00, CTRL_TASK_CFG_MAX_IN_LENGTH);

    /* Display the prompt */
    fprintf(CTRL_TASK_CFG_OUT_CH, sPromptMessage);
    fflush(CTRL_TASK_CFG_OUT_CH);
  }
  else if (_this->in_index < CTRL_TASK_CFG_MAX_IN_LENGTH)
  {
      /* A character was entered.  It was not a new line, backspace
      or carriage return, so it is accepted as part of the input and
      placed into the input buffer.  When a \n is entered the complete
      string will be passed to the command interpreter. */
    _this->in_string[_this->in_index++] = ch;

    /* print the char in the console */
    fprintf(CTRL_TASK_CFG_OUT_CH, "%c", ch);
    fflush(CTRL_TASK_CFG_OUT_CH);
  }
  else
  {
    /* the input buffer is full. Track the error */
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_CTRL_IN_BUFF_FULL_ERROR_CODE);
    res = SYS_CTRL_IN_BUFF_FULL_ERROR_CODE;
  }

  return res;
}

static sys_error_code_t AppControllerStartExecutionPhase(AppController_t *_this, uint32_t exec_phase)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  ISourceObservable *p_active_sensor = NULL;
  uint8_t sys_evt_param = 0;
//  ISensor_t *p_active_sensor_ext = NULL;  /* using this interface is a workaround for some missing API of SensorManager */

  /* 1. find the active sensor and check that there is sonly one sensor active */
  if (ISensorIsEnabled((ISensor_t*)_this->p_is3dwb_acc) && ISensorIsEnabled((ISensor_t*)_this->p_ism330dhcx_acc))
  {
    fprintf(CTRL_TASK_CFG_OUT_CH, "CTRL: unable to start the execution phase with two sensors active\r\n");
    fprintf(CTRL_TASK_CFG_OUT_CH, sPromptMessage);

    return res;
  }

  /* 2. find the active sensor*/
  if (ISensorIsEnabled((ISensor_t*)_this->p_is3dwb_acc))
  {
    p_active_sensor = _this->p_is3dwb_acc;
  }
  else if (ISensorIsEnabled((ISensor_t*)_this->p_ism330dhcx_acc))
  {
    p_active_sensor = _this->p_ism330dhcx_acc;
  }
  if (p_active_sensor == NULL)
  {
    fprintf(CTRL_TASK_CFG_OUT_CH, "CTRL: unable to start the execution phase with no sensors active\r\n");
    fprintf(CTRL_TASK_CFG_OUT_CH, sPromptMessage);

    return res;
  }

  /* 3. connect the sensor to the selected AI engine */
  if (exec_phase == CTRL_CMD_PARAM_AI)
  {
    /* 3.1 check the precondition for teh specific X-CUBE-AI use case*/
//    if (!AppControllerCanStartAI(_this, p_active_sensor))
//    {
//      fprintf(CTRL_TASK_CFG_OUT_CH, "CTRL: unable to start X-CUBE-AI detect phase.\r\nSet sensor ODR to 26.0\r\n");
//      fprintf(CTRL_TASK_CFG_OUT_CH, sPromptMessage);
//
//      return res;
//    }

    struct AIMessage_t msg = {
        .msgId = APP_MESSAGE_ID_AI,
        .cmd_id = AI_CMD_CONNECT_TO_SENSOR,
        .sparam = CTRL_AI_CB_ITEMS,
        .param = (uint32_t)p_active_sensor
    };
    if (xQueueSendToBack(_this->ai_in_queue, &msg, pdMS_TO_TICKS(100)) != pdTRUE)
    {
      res = SYS_AI_TASK_IN_QUEUE_FULL_ERROR_CODE;
      SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_AI_TASK_IN_QUEUE_FULL_ERROR_CODE);
    }
    sys_evt_param = SYS_PM_EVENT_PARAM_START_ML;
  }
  else if (exec_phase == CTRL_CMD_PARAM_NEAI_DETECT)
  {
    struct NeaiMessage_t msg = {
        .msgId = APP_MESSAGE_ID_NEAI,
        .cmd_id = NAI_CMD_CONNECT_TO_SENSOR,
        .sparam = CTRL_NEAI_CB_ITEMS,
        .param.n_param = (uint32_t)p_active_sensor
    };
    if (xQueueSendToBack(_this->neai_in_queue, &msg, pdMS_TO_TICKS(100)) != pdTRUE)
    {
      res = SYS_NAI_TASK_IN_QUEUE_FULL_ERROR_CODE;
      SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_NAI_TASK_IN_QUEUE_FULL_ERROR_CODE);
    }
    else
    {
      /* 4. set the processing mode */
      msg.cmd_id = NAI_CMD_SET_MODE;
      msg.param.n_param = NAI_CMD_PARAM_DETECT;
      if (xQueueSendToBack(_this->neai_in_queue, &msg, pdMS_TO_TICKS(100)) != pdTRUE)
      {
        res = SYS_NAI_TASK_IN_QUEUE_FULL_ERROR_CODE;
        SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_NAI_TASK_IN_QUEUE_FULL_ERROR_CODE);
      }
    }
    sys_evt_param = SYS_PM_EVENT_PARAM_START_NEAI_DETECT;
  }
  else if (exec_phase == CTRL_CMD_PARAM_NEAI_LEARN)
  {
    struct NeaiMessage_t msg = {
        .msgId = APP_MESSAGE_ID_NEAI,
        .cmd_id = NAI_CMD_CONNECT_TO_SENSOR,
        .sparam = CTRL_NEAI_CB_ITEMS,
        .param.n_param = (uint32_t)p_active_sensor
    };
    if (xQueueSendToBack(_this->neai_in_queue, &msg, pdMS_TO_TICKS(100)) != pdTRUE)
    {
      res = SYS_NAI_TASK_IN_QUEUE_FULL_ERROR_CODE;
      SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_NAI_TASK_IN_QUEUE_FULL_ERROR_CODE);
    }
    else
    {
      /* 4. set the processing mode */
      msg.cmd_id = NAI_CMD_SET_MODE;
      msg.param.n_param = NAI_CMD_PARAM_LEARN;
      if (xQueueSendToBack(_this->neai_in_queue, &msg, pdMS_TO_TICKS(100)) != pdTRUE)
      {
        res = SYS_NAI_TASK_IN_QUEUE_FULL_ERROR_CODE;
        SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_NAI_TASK_IN_QUEUE_FULL_ERROR_CODE);
      }
    }
    sys_evt_param = SYS_PM_EVENT_PARAM_START_NEAI_LEARN;
  }

  if (!SYS_IS_ERROR_CODE(res))
  {
    /* 5. wait for the task to process the messages */
    vTaskDelay(pdMS_TO_TICKS(200));

    /* 6. reset part of the task internal state */
    _this->signal_count = 0;
    /* _this->stop_timer, if enabled, is started during the state transaction in order to have a bit more accurate time measurement. */

    if ((exec_phase == CTRL_CMD_PARAM_NEAI_LEARN) || (exec_phase == CTRL_CMD_PARAM_NEAI_DETECT))
    {
      /* 6.1 check if we are using the stub libray */
      if (NeaiTaskIsStubLibrary())
      {
        /* display the warning message to inform the user */
        fprintf(CTRL_TASK_CFG_OUT_CH, "CTRL: ! This is a stubbed version, please install NanoEdge AI library !\r\n");
      }
      else
      {
        /* notify the user that the neai library seems valid. This help to have a better visualization
         * in the console because the output from the inference will be aligned. */
        fprintf(CTRL_TASK_CFG_OUT_CH, "CTRL: ! Powered by NanoEdge AI library !\r\n");
      }
      /* display additional message to clarify the execution context if teh user is using teh auto mode */
      if (exec_phase == CTRL_CMD_PARAM_NEAI_LEARN)
      {
        fprintf(CTRL_TASK_CFG_OUT_CH, "NanoEdge AI: learn\r\n");
      }
      else
      {
        fprintf(CTRL_TASK_CFG_OUT_CH, "NanoEdge AI: detect\r\n");
      }
    }
    else if (exec_phase == CTRL_CMD_PARAM_AI)
    {
      fprintf(CTRL_TASK_CFG_OUT_CH, "X-CUBE-AI: detect\r\n");
    }

    /* 7 trigger the power mode transaction */
    SysEvent evt = {
        .nRawEvent = SYS_PM_MAKE_EVENT(SYS_PM_EVT_SRC_CTRL, sys_evt_param)
    };
    SysPostPowerModeEvent(evt);
  }

  return res;
}

static sys_error_code_t AppControllerDetachSensorFromAIProc(AppController_t *_this, uint32_t active_ai_proc)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  ISourceObservable *p_active_sensor = NULL;

  /* find the active sensor */
  if (ISensorIsEnabled((ISensor_t*)_this->p_is3dwb_acc))
  {
    p_active_sensor = _this->p_is3dwb_acc;
  }
  else if (ISensorIsEnabled((ISensor_t*)_this->p_ism330dhcx_acc))
  {
    p_active_sensor = _this->p_ism330dhcx_acc;
  }

  if (active_ai_proc == CTRL_CMD_PARAM_NEAI_DETECT)
  {
    struct NeaiMessage_t msg = {
        .msgId = APP_MESSAGE_ID_NEAI,
        .cmd_id = NAI_CMD_DETACH_FROM_SENSOR,
        .param.n_param = (uint32_t)p_active_sensor
    };
    if (pdTRUE != xQueueSendToBack(_this->neai_in_queue, &msg, pdMS_TO_TICKS(100)))
    {
      res = SYS_TASK_QUEUE_FULL_ERROR_CODE;
      SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_TASK_QUEUE_FULL_ERROR_CODE);
    }
  }
  else if (active_ai_proc == CTRL_CMD_PARAM_AI)
  {
    struct AIMessage_t msg = {
        .msgId = APP_MESSAGE_ID_AI,
        .cmd_id = AI_CMD_DETACH_FROM_SENSOR,
        .param = (uint32_t)p_active_sensor
    };
    if (pdTRUE != xQueueSendToBack(_this->ai_in_queue, &msg, pdMS_TO_TICKS(100)))
    {
      res = SYS_TASK_QUEUE_FULL_ERROR_CODE;
      SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_TASK_QUEUE_FULL_ERROR_CODE);
    }
  }

  return res;
}

static SubSensorDescriptor_t AppControllerGetSubSensorDescriptor(AppController_t *_this, uint8_t sensor_id)
{
  assert_param(_this != NULL);

  SensorDescriptor_t descriptor = SMSensorGetDescription(sensor_id);
  if (sensor_id == ISourceGetId(_this->p_ism330dhcx_acc))
  {
    return descriptor.pSubSensorDescriptor[0];
  }
  else if (sensor_id == ISourceGetId(_this->p_is3dwb_acc))
  {
    return descriptor.pSubSensorDescriptor[0];
  }

  SubSensorDescriptor_t null_descriptor = {0};

  SYS_DEBUGF(SYS_DBG_LEVEL_WARNING, ("CTRL: warning, wrong sensor id: %d\r\n", sensor_id));

  return null_descriptor;
}

static BaseType_t AppControllerParseSensorGetAllCmd(char *p_write_buffer, size_t write_buffer_len, ISourceObservable *p_sensor)
{
  assert_param(p_sensor != NULL);
  portBASE_TYPE res = pdTRUE;
  static uint16_t sAvailableODRIdx = 0;
  static uint16_t sAvailableFullScaleIdx = 0;
  static uint8_t sPrintedParamsIdx = 0;
  ISensor_t *p_sensor_ex = NULL; /* using this interface is a workaround for some missing API of SensorManager */
  float measuredORD, nominalODR;
  SubSensorDescriptor_t descriptor = {0};

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
      descriptor = AppControllerGetSubSensorDescriptor(&sTaskObj, ISourceGetId(p_sensor));
      snprintf(p_write_buffer, write_buffer_len, "%0.2f Hz\r\n", descriptor.pODR[sAvailableODRIdx]);
      sAvailableODRIdx++;
      if (descriptor.pODR[sAvailableODRIdx] == COM_END_OF_LIST_FLOAT) {
        // this is the last available ODR
       sAvailableODRIdx = 0;
      }
      break;

    case 3:
      /* print sensor full scale */
      descriptor = AppControllerGetSubSensorDescriptor(&sTaskObj, ISourceGetId(p_sensor));
      snprintf(p_write_buffer, write_buffer_len, "fullScale = %0.2f %s\r\nAvailable fullScales:\r\n", ISourceGetFS(p_sensor), descriptor.unit);
      break;

    case 4:
      /* print sensor available fullScales */
      descriptor = AppControllerGetSubSensorDescriptor(&sTaskObj, ISourceGetId(p_sensor));
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

static BaseType_t AppControllerParseSensorInfoCmd(char *p_write_buffer, size_t write_buffer_len, const char *p_command_string)
{
  AppController_t *p_obj = &sTaskObj;
  uint8_t sensor_id = ISourceGetId(p_obj->p_is3dwb_acc);
  SensorDescriptor_t descriptor = SMSensorGetDescription(sensor_id);
  size_t str_len = 0;
  snprintf(p_write_buffer, write_buffer_len, "%s ID=%d, type=ACC\r\n", descriptor.Name, sensor_id);
  str_len = strlen(p_write_buffer);
  p_write_buffer += str_len;
  write_buffer_len -= str_len;
  sensor_id = ISourceGetId(p_obj->p_ism330dhcx_acc);
  descriptor = SMSensorGetDescription(sensor_id);
  snprintf(p_write_buffer, write_buffer_len, "%s ID=%d, type=ACC\r\n", descriptor.Name, sensor_id);
  str_len = strlen(p_write_buffer);
  p_write_buffer += str_len;
  write_buffer_len -= str_len;
  snprintf(p_write_buffer, write_buffer_len, "2 sensors supported\r\n");

  return pdFALSE;
}

static BaseType_t AppControllerParseSensorGetCommand(char *p_write_buffer, size_t write_buffer_len, const char *p_command_string)
{
  portBASE_TYPE res = pdFALSE;
  BaseType_t  param1_str_length = 0;
  BaseType_t  param2_str_length = 0;
  uint8_t sensor_id = 0;
  ISourceObservable *p_sensor = NULL;
  ISensor_t *p_sensor_ex = NULL; /* using this interface is a workaround for some missing API of SensorManager */
  SubSensorDescriptor_t descriptor = {0};

  static uint16_t sAvailableODRIdx = 0;
  static uint16_t sAvailableFullScaleIdx = 0;

  // validate the parameter
  const char *p_aram1 = FreeRTOS_CLIGetParameter(p_command_string, 1, &param1_str_length);
  const char *p_aram2 = FreeRTOS_CLIGetParameter(p_command_string, 2, &param2_str_length);

  if (SYS_NO_ERROR_CODE != AppControllerParseSensorID(p_aram1, param1_str_length, &sensor_id))
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
      descriptor = AppControllerGetSubSensorDescriptor(&sTaskObj, sensor_id);
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
      descriptor = AppControllerGetSubSensorDescriptor(&sTaskObj, sensor_id);
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
      descriptor = AppControllerGetSubSensorDescriptor(&sTaskObj, sensor_id);
      snprintf(p_write_buffer,write_buffer_len, "fullScale = %0.2f %s\r\n", ISourceGetFS(p_sensor), descriptor.unit);
    }
    else if (!strncmp(p_aram2, CLI_PARAM_GET_ALL, strlen(CLI_PARAM_GET_ALL)))
    {
      res = AppControllerParseSensorGetAllCmd(p_write_buffer, write_buffer_len, p_sensor);
    }
    else
    {
      snprintf(p_write_buffer, write_buffer_len, CLI_INVALID_PARAMETER_ERROR_MSG, p_aram2);
    }
  }

  return res;
}

static sys_error_code_t AppControllerParseSensorID(const char *p_param, BaseType_t param_length, uint8_t *p_sensor_id)
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

static BaseType_t AppControllerParseSensorSetCommand(char *p_write_buffer, size_t write_buffer_len, const char *p_command_string)
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

  if (SYS_NO_ERROR_CODE != AppControllerParseSensorID(p_param1, param1_str_length, &sensor_id))
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

static BaseType_t AppControllerParseStartCommand(char *p_write_buffer, size_t write_buffer_len, const char *p_command_string)
{
  BaseType_t  param1_str_length= 0;
  AppController_t *p_obj = &sTaskObj;
  bool error = false;
  struct CtrlMessage_t msg = {
      .msgId = APP_MESSAGE_ID_CTRL,
      .cmd_id = CTRL_CMD_STRAT,
      .sparam = CTRL_CMD_PARAM_FROM_CLI
  };

  // validate the parameter
  const char *pcParam1 = FreeRTOS_CLIGetParameter(p_command_string, 1, &param1_str_length);
  if (!strncmp(pcParam1, CLI_PARAM_START_NEAI_LEARN, strlen(CLI_PARAM_START_NEAI_LEARN)))
  {
    msg.param  = CTRL_CMD_PARAM_NEAI_LEARN;
    if (xQueueSendToBack(p_obj->in_queue, &msg, pdMS_TO_TICKS(100)) == pdTRUE)
    {
      snprintf(p_write_buffer, write_buffer_len, "NanoEdgeAI: starting learn phase...\r\n");
    }
    else
    {
      error = true;
    }
  }
  else if (!strncmp(pcParam1, CLI_PARAM_START_NEAI_DETECT, strlen(CLI_PARAM_START_NEAI_DETECT)))
  {
    msg.param  = CTRL_CMD_PARAM_NEAI_DETECT;
    if (xQueueSendToBack(p_obj->in_queue, &msg, pdMS_TO_TICKS(100)) == pdTRUE)
    {
      snprintf(p_write_buffer, write_buffer_len, "NanoEdgeAI: starting detect phase...\r\n");
    }
    else
    {
      error = true;
    }
  }
  else if (!strncmp(pcParam1, CLI_PARAM_START_AI, strlen(CLI_PARAM_START_AI)))
  {
    msg.param  = CTRL_CMD_PARAM_AI;
    if (xQueueSendToBack(p_obj->in_queue, &msg, pdMS_TO_TICKS(100)) == pdTRUE)
    {
      snprintf(p_write_buffer, write_buffer_len, "X-CUBE-AI: starting detect phase...\r\n");
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
    snprintf(p_write_buffer, write_buffer_len, "error starting the AI processing.\r\n");
  }

  return pdFALSE;
}

static BaseType_t AppControllerParseNeaiInitCmd(char *p_write_buffer, size_t write_buffer_len, const char *p_command_string)
{
  struct NeaiMessage_t msg = {
      .msgId = APP_MESSAGE_ID_NEAI,
      .cmd_id = NAI_CMD_INIT
  };

  if (pdTRUE != xQueueSendToBack(sTaskObj.neai_in_queue, &msg, pdMS_TO_TICKS(100)))
  {
    snprintf(p_write_buffer, write_buffer_len, "error: command not executed. Try again.\r\n");
  }
  else
  {
    snprintf(p_write_buffer, write_buffer_len, "NanoEdge AI: initializing the model.\r\n");
  }

  return pdFALSE;
}

static BaseType_t AppControllerParseNeaiGetCmd(char *p_write_buffer, size_t write_buffer_len, const char *p_command_string)
{
  BaseType_t  param1_str_length = 0;
  static int8_t sShowAllStep = 0;

  /* parse param 1 */
  const char *p_param1 = FreeRTOS_CLIGetParameter(p_command_string, 1, &param1_str_length);
  if (!strncmp(p_param1, CLI_PARAM_SENSITIVITY, strlen(CLI_PARAM_SENSITIVITY)))
  {
    float sensitivity = NanoEdgeAI_get_sensitivity();
    snprintf(p_write_buffer, write_buffer_len, "NanoEdge AI: sensitivity = %0.2f\r\n", sensitivity);
  }
  else if (!strncmp(p_param1, CLI_PARAM_THRESHOLD, strlen(CLI_PARAM_THRESHOLD)))
  {
    snprintf(p_write_buffer, write_buffer_len, "NanoEdge AI: threshold = %d%%\r\n", sTaskObj.detect_threshold);
  }
  else if (!strncmp(p_param1, CLI_PARAM_SIGNALS, strlen(CLI_PARAM_SIGNALS)))
  {
    snprintf(p_write_buffer, write_buffer_len, "NanoEdge AI: signals = %lu\r\n", sTaskObj.signals);
  }
  else if (!strncmp(p_param1, CLI_PARAM_TIMER, strlen(CLI_PARAM_TIMER)))
  {
    snprintf(p_write_buffer, write_buffer_len, "NanoEdge AI: timer = %lu ms\r\n", sTaskObj.timer_period_ms);
  }
  else if (!strncmp(p_param1, CLI_PARAM_GET_ALL, strlen(CLI_PARAM_GET_ALL)))
  {
    sShowAllStep++;
    if (sShowAllStep == 1) {
      snprintf(p_write_buffer, write_buffer_len, "NanoEdge AI: signals = %lu\r\n", sTaskObj.signals);
    }
    else if (sShowAllStep == 2) {
      float sensitivity = NanoEdgeAI_get_sensitivity();
      snprintf(p_write_buffer, write_buffer_len, "NanoEdge AI: sensitivity = %0.2f\r\n", sensitivity);
    }
    else if (sShowAllStep == 3) {
      snprintf(p_write_buffer, write_buffer_len, "NanoEdge AI: threshold = %d%%\r\n", sTaskObj.detect_threshold);
    }
    else if (sShowAllStep == 4) {
      snprintf(p_write_buffer, write_buffer_len, "NanoEdge AI: timer = %lu ms\r\n", sTaskObj.timer_period_ms);
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

static BaseType_t AppControllerParseNeaiSetCmd(char *p_write_buffer, size_t write_buffer_len, const char *p_command_string)
{

  BaseType_t  param1_str_length = 0;

  /* parse param 1 */
  const char *p_param1 = FreeRTOS_CLIGetParameter(p_command_string, 1, &param1_str_length);
  if (!strncmp(p_param1, CLI_PARAM_SENSITIVITY, strlen(CLI_PARAM_SENSITIVITY)))
  {
    return AppControllerParseNeaiParseNeaiSetSensitivityCmd(p_write_buffer, write_buffer_len, p_command_string);
  }
  else if (!strncmp(p_param1, CLI_PARAM_THRESHOLD, strlen(CLI_PARAM_THRESHOLD)))
  {
    return AppControllerParseNeaiParseNeaiSetThresholdCmd(p_write_buffer, write_buffer_len, p_command_string);
  }
  else if (!strncmp(p_param1, CLI_PARAM_SIGNALS, strlen(CLI_PARAM_SIGNALS)))
  {
    return AppControllerParseNeaiParseNeaiSetSignalsCmd(p_write_buffer, write_buffer_len, p_command_string);
  }
  else if (!strncmp(p_param1, CLI_PARAM_TIMER, strlen(CLI_PARAM_TIMER)))
  {
    return AppControllerParseNeaiParseNeaiSetTimerCmd(p_write_buffer, write_buffer_len, p_command_string);
  }

  // if we are here the parameter is not valid.
  sprintf(p_write_buffer, CLI_INVALID_PARAMETER_ERROR_MSG, p_param1);

  return pdFALSE;
}

static BaseType_t AppControllerParseNeaiParseNeaiSetSensitivityCmd(char *p_write_buffer, size_t write_buffer_len, const char *p_command_string)
{
  BaseType_t  param2_str_length = 0;
  struct NeaiMessage_t msg = {
      .msgId = APP_MESSAGE_ID_NEAI,
      .cmd_id = NAI_CMD_SET_SENSITIVITY
  };

  const char *p_param2 = FreeRTOS_CLIGetParameter(p_command_string, 2, &param2_str_length);
  msg.param.f_param = atof(p_param2);
  /* validate teh param */
  if (AppControllerCanExecuteNeaiCLICommand(&sTaskObj, &msg, p_write_buffer, write_buffer_len) != SYS_NO_ERROR_CODE)
  {
    return pdFALSE;
  }

  if (xQueueSendToBack(sTaskObj.neai_in_queue, &msg, pdMS_TO_TICKS(100)) != pdTRUE)
  {
    /* cannot pot the command. Notify the erro in the CLI */
    snprintf(p_write_buffer, write_buffer_len, CLI_GENERIC_TIMEOUT_ERROR_MSG);
  }
  else {
    snprintf(p_write_buffer, write_buffer_len, "NanoEdge AI: sensitivity set to %0.2f\r\n", msg.param.f_param);
  }
  return pdFALSE;
}

static BaseType_t AppControllerParseNeaiParseNeaiSetThresholdCmd(char *p_write_buffer, size_t write_buffer_len, const char *p_command_string)
{
  BaseType_t  param2_str_length = 0;

  const char *p_param2 = FreeRTOS_CLIGetParameter(p_command_string, 2, &param2_str_length);
  /* validate the param */
  uint8_t new_threshold = atoi(p_param2);

  if (new_threshold > 100) {
    snprintf(p_write_buffer, write_buffer_len, "NanoEdge AI: invalid threshold. Value out of range.\r\n");
  }
  else
  {
    sTaskObj.detect_threshold = new_threshold;
    snprintf(p_write_buffer, write_buffer_len, "NanoEdge AI: threshold set to %d%%\r\n", new_threshold);
  }

  return pdFALSE;
}

static BaseType_t AppControllerParseNeaiParseNeaiSetSignalsCmd(char *p_write_buffer, size_t write_buffer_len, const char *p_command_string)
{
  BaseType_t  param2_str_length = 0;

  const char *p_param2 = FreeRTOS_CLIGetParameter(p_command_string, 2, &param2_str_length);
  /* validate the param */
  uint32_t new_signals = atoi(p_param2);

  if (new_signals > CTRL_MAX_SIGNALS_PARAM) {
    snprintf(p_write_buffer, write_buffer_len, "NanoEdge AI: invalid signals. Value out of range.\r\n");
  }
  else
  {
    sTaskObj.signals = new_signals;
    snprintf(p_write_buffer, write_buffer_len, "NanoEdge AI: signals set to %lu\r\n", new_signals);
  }

  return pdFALSE;
}

static BaseType_t AppControllerParseNeaiParseNeaiSetTimerCmd(char *p_write_buffer, size_t write_buffer_len, const char *p_command_string)
{
  BaseType_t  param2_str_length = 0;

  const char *p_param2 = FreeRTOS_CLIGetParameter(p_command_string, 2, &param2_str_length);
  /* validate the param */
  uint32_t new_timer_period_ms = atoi(p_param2);

  if (new_timer_period_ms > CTRL_MAX_TIME_MS_PARAM) {
    snprintf(p_write_buffer, write_buffer_len, "NanoEdge AI: invalid timeout. Value out of range.\r\n");
  }
  else
  {
    sTaskObj.timer_period_ms = new_timer_period_ms;
    snprintf(p_write_buffer, write_buffer_len, "NanoEdge AI: timer set to %lu ms\r\n", new_timer_period_ms);
  }

  return pdFALSE;
}

static sys_error_code_t AppControllerCanExecuteNeaiCLICommand(AppController_t *_this, const struct NeaiMessage_t *p_msg, char *p_write_buffer, size_t write_buffer_len)
{
  assert_param(_this != NULL);
  assert_param(p_msg != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  switch (p_msg->cmd_id) {

  case NAI_CMD_SET_SENSITIVITY:
    if ((p_msg->param.f_param < 0) || (p_msg->param.f_param > 100))
    {
      snprintf(p_write_buffer, write_buffer_len, "NanoEdge AI: invalid sensitivity. Value out of range.\r\n");
      res = SYS_CTRL_INVALID_PARAM_ERROR_CODE;
    }
    break;

  default:
    break;
  }

  return res;
}

static void AppControllerTimerStopCallback(TimerHandle_t xTimer)
{
  /* generate the system event to stop stop the execution.*/
  SysEvent evt = {
      .nRawEvent = SYS_PM_MAKE_EVENT(SYS_PM_EVT_SRC_CTRL, SYS_PM_EVENT_PARAM_STOP_PROCESSING)
  };
  SysPostPowerModeEvent(evt);
}

static bool AppControllerCanStartAI(AppController_t *_this, ISourceObservable *p_sensor)
{
  assert_param(_this != NULL);
  float measuredODR = 0.0f;
  float nominalODR = 0.0f;

  ISourceGetODR(p_sensor, &measuredODR, &nominalODR);

  return nominalODR == 26.0f;
}

static sys_error_code_t AppControllerSetAutomodeConfig(AppController_t *_this, uint32_t mode)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  ISensorDisable((ISensor_t*)_this->p_is3dwb_acc);
  ISensorDisable((ISensor_t*)_this->p_ism330dhcx_gyr);
  ISensorEnable((ISensor_t*)_this->p_ism330dhcx_acc);

  if (mode == CTRL_CMD_PARAM_AI)
  {
    /* set the default configuration for the X-CUBE-AI HAR demo*/
    fprintf(CTRL_TASK_CFG_OUT_CH, "CTRL: automode. Set configuration for X-CUBE-AI demo.\r\n");
    ISensorSetODR((ISensor_t*)_this->p_ism330dhcx_acc, 26.0f);
  }
  else
  {
    /* set the default configuration for for NanoEdgeAI demo*/
    fprintf(CTRL_TASK_CFG_OUT_CH, "CTRL: automode. Set configuration for NanoEdge AI demo.\r\n");
    ISensorSetODR((ISensor_t*)_this->p_ism330dhcx_acc, 1666.0f);
  }

  return res;
}

static BaseType_t AppControllerParseAiGetCmd(char *p_write_buffer, size_t write_buffer_len, const char *p_command_string)
{
  BaseType_t  param1_str_length = 0;

  /* parse param 1 */
  const char *p_param1 = FreeRTOS_CLIGetParameter(p_command_string, 1, &param1_str_length);
  if (!strncmp(p_param1, CLI_PARAM_INFO, strlen(CLI_PARAM_INFO)))
  {
    ai_print_network_info(CTRL_TASK_CFG_OUT_CH);
  }
  snprintf(p_write_buffer, write_buffer_len, "\r\n");

  return pdFALSE;
}

static BaseType_t AppControllerParseSwitchBankCmd(char *p_write_buffer, size_t write_buffer_len, const char *p_command_string)
{
  AppController_t *p_obj = &sTaskObj;
  struct CtrlMessage_t msg = {
      .msgId = APP_MESSAGE_ID_CTRL,
      .cmd_id = CTRL_CMD_SWITCH_BANK
  };

  if (xQueueSendToBack(p_obj->in_queue, &msg, pdMS_TO_TICKS(100)) != pdTRUE)
  {
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_AI_TASK_IN_QUEUE_FULL_ERROR_CODE);
  }

  snprintf(p_write_buffer, write_buffer_len, "system reboot after bank switch...\r\n");

  return pdFALSE;
}

/**
* @brief  Enable Disable the jump to second flash bank and reboot board
* @param  None
* @retval None
*/
static void AppControllerEnableDisableDualBoot(void)
{
  FLASH_OBProgramInitTypeDef OBInit;
  /* Set BFB2 bit to enable boot from Flash Bank2 */
  /* Allow Access to Flash control registers and user Flash */
  HAL_FLASH_Unlock();

  /* Clear OPTVERR bit set on virgin samples */
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);

  /* Allow Access to option bytes sector */
  HAL_FLASH_OB_Unlock();

  /* Get the Dual boot configuration status */
  HAL_FLASHEx_OBGetConfig(&OBInit);

  /* Enable/Disable dual boot feature */
  OBInit.OptionType = OPTIONBYTE_USER;
  OBInit.USERType = OB_USER_BFB2;

  if(((OBInit.USERConfig) & (OB_BFB2_ENABLE)) == OB_BFB2_ENABLE)
  {
    OBInit.USERConfig = OB_BFB2_DISABLE;
    SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("CTRL: ->Disable DualBoot\r\n"));
  }
  else
  {
    OBInit.USERConfig = OB_BFB2_ENABLE;
    SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("CTRL: ->Enable DualBoot\r\n"));
  }

  if(HAL_FLASHEx_OBProgram(&OBInit) != HAL_OK)
  {
    /*
    Error occurred while setting option bytes configuration.
    User can add here some code to deal with this error.
    To know the code error, user can call function 'HAL_FLASH_GetError()'
    */
    while(1);
  }

  /* Start the Option Bytes programming process */
  if(HAL_FLASH_OB_Launch() != HAL_OK)
  {
    /*
    Error occurred while reloading option bytes configuration.
    User can add here some code to deal with this error.
    To know the code error, user can call function 'HAL_FLASH_GetError()'
    */
    while(1);
  }

  SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("CTRL: switch bank\r\n"));

  HAL_FLASH_OB_Lock();
  HAL_FLASH_Lock();
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
extern int SysDebugLowLevelPutchar(int x);
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
    SysDebugLowLevelPutchar(ch);
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

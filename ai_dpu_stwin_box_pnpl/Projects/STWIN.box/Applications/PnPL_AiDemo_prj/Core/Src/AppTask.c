/**
 ******************************************************************************
 * @file    AppTask.c
 * @author  STMicroelectronics
 * @version 1.0.0
 * @date    Jun 14, 2022
 *
 * @brief Application Controller Task
 *
 ******************************************************************************
 * @attention
 *
 * <h2><center>COPYRIGHT &copy; 2022 STMicroelectronics</center></h2>
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file in
 * the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
#include "AppTask.h"
#include "AppTask_vtbl.h"
#include "drivers/PushButtonDrv.h"
#include "drivers/PushButtonDrv_vtbl.h"
#include "services/sysdebug.h"
#include "ux_api.h"
#include "app_usbx_device.h"
#include "UsbCdcTask.h"
#include "parson.h"
#include "services/sysmem.h"

#ifndef APP_TASK_CFG_STACK_DEPTH
#define APP_TASK_CFG_STACK_DEPTH           TX_MINIMUM_STACK
#endif

#ifndef APP_TASK_CFG_PRIORITY
#define APP_TASK_CFG_PRIORITY              (TX_MAX_PRIORITIES-2)
#endif

#define APP_TASK_ANTI_DEBOUNCE_PERIOD_TICK 7U

#define SYS_DEBUGF(level, message)         SYS_DEBUGF3(SYS_DBG_HW, level, message)


/**
 * The only instance of the task object.
 */
static AppTask s_xTaskObj;

// Private member function declaration
// ***********************************

/**
 * Execute one step of the task control loop while the system is in RUN mode.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return SYS_NO_EROR_CODE if success, a task specific error code otherwise.
 */
static sys_error_code_t AppTaskExecuteStepRun(AManagedTask *_this);

/**
 * @brief  PnPL_msg_check
 *         Check if a valid PnPL message is present in the input buffer
 *
 * @param input [IN] pointer the input buffer.
 * @param size  [IN] size of input buffer.
 * @param json_offset  [OUT] Index of json first characters = '{'. Characters different from '{' are ignored.
 *
 * @return Length of the message found in the buffer. Zero means no complete message was found.
 *
 */
static uint32_t PnPL_msg_check(const uint8_t *input, uint32_t size, uint32_t *json_offset);

/**
 * @brief  Switch boot bank in STM32 option bytes. Will cause board reset.
 * @param  None
 * @retval None
 */
static void ToggleFlashBank(void);

/**
 * Class object declaration
 */
typedef struct _AppClass {
  /**
   * AppTask class virtual table.
   */
  AManagedTask_vtbl vtbl;

  /**
   * IProcessEventListener virtual table.
   */
  IProcessEventListener_vtbl process_listener_vtbl;

  /**
   * IUsbDelegate virtual table.
   */
  IUsbDelegate_vtbl usb_delegate_vtbl;

  /**
   * IAiApplication virtual table.
   */
  IAiApplication_vtbl ai_application_controller_vtbl;

  /**
   * AppTask (PM_STATE, ExecuteStepFunc) map.
   */
  pExecuteStepFunc_t p_pm_state2func_map[];
} AppClass;

/**
 * The class object.
 */
static const AppClass sTheClass = {
    /* Class virtual table */
    {
        AppTask_vtblHardwareInit,
        AppTask_vtblOnCreateTask,
        AppTask_vtblDoEnterPowerMode,
        AppTask_vtblHandleError,
        AppTask_vtblOnEnterTaskControlLoop
    },

    {
        AppTask_vtblOnStatusChange,
        AppTask_vtblSetOwner,
        AppTask_vtblGetOwner,
        AppTask_vtblOnProcessedDataReady
    },
    {
        AppTask_vtblOnUsbData,
    },
    {
        AppTask_vtblStart,
        AppTask_vtblStop,
        AppTask_vtblSwitchBank
    },
    /* class (PM_STATE, ExecuteStepFunc) map */
    {
        AppTaskExecuteStepRun,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
    }
};


// Inline function forward declaration
// ***********************************

#if defined (__GNUC__)
#endif


// Public API definition
// *********************

AManagedTask *AppTaskAlloc(const void *p_mx_drv_cfg)
{
  // In this application there is only one Keyboard task,
  // so this allocator implement the singleton design pattern.

  // Initialize the super class
  AMTInit(&s_xTaskObj.super);

  s_xTaskObj.super.vptr = &sTheClass.vtbl;

  s_xTaskObj.p_mx_drv_cfg = p_mx_drv_cfg;

  s_xTaskObj.dpu_listener_if.vptr = &sTheClass.process_listener_vtbl;

  s_xTaskObj.usb_delegate.vptr = &sTheClass.usb_delegate_vtbl;

  s_xTaskObj.ai_application_controller.vptr = &sTheClass.ai_application_controller_vtbl;


  return (AManagedTask*)&s_xTaskObj;
}

// AManagedTask virtual functions definition
// ***********************************************

sys_error_code_t AppTask_vtblHardwareInit(AManagedTask *_this, void *pParams)
{
  assert_param(_this != NULL);
  sys_error_code_t xRes = SYS_NO_ERROR_CODE;
  AppTask *pObj = (AppTask*) _this;
  UINT status = TX_SUCCESS;

  if(pObj->p_mx_drv_cfg != NULL)
  {
    pObj->p_driver = PushButtonDrvAlloc();
    if(pObj->p_driver == NULL)
    {
      SYS_DEBUGF(SYS_DBG_LEVEL_SEVERE, ("HW: unable to alloc driver object.\r\n"));
      xRes = SYS_GET_LAST_LOW_LEVEL_ERROR_CODE();
    }
    else
    {
      PushButtonDrvParams_t driver_cfg = {
          .p_mx_gpio_cfg = (void*) pObj->p_mx_drv_cfg
      };
      xRes = IDrvInit((IDriver*) pObj->p_driver, &driver_cfg);
      if(SYS_IS_ERROR_CODE(xRes))
      {
        SYS_DEBUGF(SYS_DBG_LEVEL_SEVERE, ("HW: error during driver initialization\r\n"));
      }
    }
  }

  if(status != UX_SUCCESS)
  {
    while(1);
  }

  return xRes;
}

sys_error_code_t AppTask_vtblOnCreateTask(AManagedTask *_this, tx_entry_function_t *pTaskCode, CHAR **pName,
    VOID **pStackStart, ULONG *pStackDepth,
    UINT *pPriority, UINT *pPreemptThreshold,
    ULONG *pTimeSlice, ULONG *pAutoStart,
    ULONG *pParams)
{
  assert_param(_this != NULL);
  sys_error_code_t xRes = SYS_NO_ERROR_CODE;
//  AppTask *pObj = (AppTask*)_this;

  //Set the Malloc/Free Functions  used inside the Json Parser
  json_set_allocation_functions(SysAlloc, SysFree);

  _this->m_pfPMState2FuncMap = sTheClass.p_pm_state2func_map;

//  *pvTaskCode = AppTaskRun;
  *pTaskCode = AMTRun;
  *pName = "HW";
  *pStackStart = NULL; // allocate the task stack in the system memory pool.
  *pStackDepth = APP_TASK_CFG_STACK_DEPTH;
  *pParams = (ULONG)_this;
  *pPriority = APP_TASK_CFG_PRIORITY;
  *pPreemptThreshold = APP_TASK_CFG_PRIORITY;
  *pTimeSlice = TX_NO_TIME_SLICE;
  *pAutoStart = TX_AUTO_START;

  return xRes;
}

sys_error_code_t AppTask_vtblDoEnterPowerMode(AManagedTask *_this, const EPowerMode eActivePowerMode, const EPowerMode eNewPowerMode)
{
  assert_param(_this);
  sys_error_code_t xRes = SYS_NO_ERROR_CODE;

  return xRes;
}

sys_error_code_t AppTask_vtblHandleError(AManagedTask *_this, SysEvent xError)
{
  assert_param(_this);
  sys_error_code_t xRes = SYS_NO_ERROR_CODE;

  return xRes;
}

sys_error_code_t AppTask_vtblOnEnterTaskControlLoop(AManagedTask *_this)
{
  assert_param(_this);
  sys_error_code_t xRes = SYS_NO_ERROR_CODE;
  AppTask *p_obj = (AppTask*)_this;

  SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("HW: start.\r\n"));

  if (p_obj->p_driver != NULL)
  {
    IDrvStart(p_obj->p_driver);
  }

  return xRes;
}


// Private function definition
// ***************************

static sys_error_code_t AppTaskExecuteStepRun(AManagedTask *_this)
{
  assert_param(_this != NULL);
  sys_error_code_t xRes = SYS_NO_ERROR_CODE;
//  ULONG actual_length;

  tx_thread_sleep(1000);

//  sys_error_code_t usbx_cdc_acm_write(UCHAR *buffer, ULONG requested_length, ULONG *actual_length);
//  CHAR msg[] = "Hello World! - USB-CDC\r\n";
//  UsbCdcTask_Write((UCHAR*)msg, strlen(msg), &actual_length);

//  SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("Hello STWIN.box!!\r\n"));
  __NOP();
  __NOP();

  return xRes;
}

// IProcessEventListener virtual functions
//****************************************

sys_error_code_t AppTask_vtblOnProcessedDataReady(IEventListener *_this, const ProcessEvent *pxEvt)
{
  assert_param(_this != NULL);
  sys_error_code_t xRes = SYS_NO_ERROR_CODE;
  AppTask *p_if_owner = (AppTask*)((uint32_t)_this - offsetof(AppTask, dpu_listener_if));
  UNUSED(p_if_owner);
  char *telemetry;
  uint32_t size;
  uint32_t actual_size;

  /* Processed data can be found here */
//  pxEvt->stream->payload

//  PnPLSerializeTelemetry("ai_application", "label_id", "{\"a\":7}", &telemetry_buffer, &telemetry_size, 0);
// TODO change Pnpl message
  PnPLSerializeTelemetry("ai_application", "accuracy", "1.5", &telemetry, &size, 0);

  /* Send the PnPL command via USB CDC interface */
  UsbCdcTask_Write((uint8_t*) telemetry, size, &actual_size);

  SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("HW: observed new processed data.\r\n"));

  return xRes;
}


// IListener virtual functions definition
//***************************************

sys_error_code_t AppTask_vtblOnStatusChange(IListener *_this)
{
  assert_param(_this != NULL);

  SYS_DEBUGF(SYS_DBG_LEVEL_WARNING, ("HW: OnStatusChange not implemented.\r\n"));

  return SYS_NO_ERROR_CODE;
}

// IEventListener virtual functions
//*********************************

void AppTask_vtblSetOwner(IEventListener *_this, void *pxOwner)
{
  assert_param(_this != NULL);
  UNUSED(pxOwner);

  SYS_DEBUGF(SYS_DBG_LEVEL_WARNING, ("HW: SetOwner not implemented.\r\n"));
}

void *AppTask_vtblGetOwner(IEventListener *_this)
{
  assert_param(_this != NULL);

  SYS_DEBUGF(SYS_DBG_LEVEL_WARNING, ("HW: GetOwner not implemented.\r\n"));

  return NULL;
}

// IUsbDelegate virtual functions
//****************************************

sys_error_code_t AppTask_vtblOnUsbData(IUsbDelegate *_this, const uint8_t *buffer, uint32_t length)
{
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  AppTask *p_if_owner = (AppTask*) ((uint32_t) _this - offsetof(AppTask, usb_delegate));

  uint8_t *rx_buf = p_if_owner->rx_buffer;
  uint32_t idx = p_if_owner->rx_buffer_idx;
  uint32_t pnpl_size;
  uint32_t json_offset;

  /* Copy the data in the RX buffer if there is enough space left */
  if((idx + length) < USB_CDC_RX_BUFFER_SIZE)
  {
    memcpy(&rx_buf[idx], buffer, length);
    idx += length;
  }
  else /* Fill the buffer and ignore the other data */
  {
    memcpy(&rx_buf[idx], buffer, USB_CDC_RX_BUFFER_SIZE - idx);
    idx += USB_CDC_RX_BUFFER_SIZE - idx;
    res = SYS_APPLICATION_TASK_BUFFER_OVF_ERROR_CODE;
  }

  do
  {
    /* Check if a valid PnPL message is present in the input buffer */
    pnpl_size = PnPL_msg_check(rx_buf, idx, &json_offset);

    /* A PnPL message was found in the buffer */
    if(pnpl_size > 0)
    {
      uint8_t pnp_res;
      char *p_buffer;
      uint32_t size;
      uint32_t actual_size;

#if 0
      // print the received JSON (without last '}')
      rx_buf[pnpl_size-1] = '\0'; // Replace last character '}' with '\0' for the printf
      // ToDo: call PnPL Parse
      SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ((char*)(rx_buf+json_offset)));
      SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("\n\r"));

      rx_buf[pnpl_size-1] = '}';
#endif

      /* Parse the PnPL message */
      pnp_res = PnPLParseCommand((char*) &rx_buf[json_offset], &p_if_owner->outPnPLCommand);

      if(pnp_res == PNPL_CMD_NO_ERROR_CODE)
      {
        /* If the message was "PNPL_CMD_GET" or "PNPL_CMD_SYSTEM_INFO" we need to reply */
        if(p_if_owner->outPnPLCommand.comm_type == PNPL_CMD_GET || p_if_owner->outPnPLCommand.comm_type == PNPL_CMD_SYSTEM_INFO)
        {
          /* Build the PnPL response string */
          PnPLSerializeResponse(&p_if_owner->outPnPLCommand, &p_buffer, &size, 0);

          /* Send the PnPL command via USB CDC interface */
          UsbCdcTask_Write((uint8_t*) p_buffer, size, &actual_size);

          /* Free the command buffer */
          SysFree(p_buffer);
        }
      }

      /* If the buffer is not empty, copy remaining data at the beginning of the buffer,
       * otherwise reset the index
       */
      if(pnpl_size < idx)
      {
        idx -= pnpl_size;
        memcpy(rx_buf, &rx_buf[pnpl_size], idx);
      }
      else
      {
        idx = 0;
      }
    }
    else /* Discard any character != '{' after the first json */
    {
      if(json_offset > 0)
      {
        idx -= json_offset;
        memcpy(rx_buf, &rx_buf[json_offset], idx);
      }
    }
  } while(pnpl_size != 0);

  /* Store the buffer index into the class field */
  p_if_owner->rx_buffer_idx = idx;

  return res;
}


// IAiApplication virtual functions
//****************************************
sys_error_code_t AppTask_vtblStart(IAiApplication_t *_this)
{
  /* generate the system event to change the PM state*/
  SysEvent evt =
  {
      .nRawEvent = SYS_PM_MAKE_EVENT(SYS_PM_EVT_SRC_DTDL, SYS_PM_EVT_DTDL_START)
  };
  SysPostPowerModeEvent(evt);

  return SYS_NO_ERROR_CODE;
}

sys_error_code_t AppTask_vtblStop(IAiApplication_t *_this)
{
  /* generate the system event to change the PM state*/
  SysEvent evt =
  {
      .nRawEvent = SYS_PM_MAKE_EVENT(SYS_PM_EVT_SRC_DTDL, SYS_PM_EVT_DTDL_STOP)
  };
  SysPostPowerModeEvent(evt);

  return SYS_NO_ERROR_CODE;
}

sys_error_code_t AppTask_vtblSwitchBank(IAiApplication_t *_this)
{
  ToggleFlashBank();
  return SYS_NO_ERROR_CODE;
}


static uint32_t PnPL_msg_check(const uint8_t *input, uint32_t size, uint32_t *json_offset)
{
  uint8_t brackets = 0;
  uint32_t i = 0 ;
  uint32_t ret = 0;

  while(*input != '{' && i<size)
  {
    i++;
    input++;
  }
  *json_offset = i;

  for (;i<size;i++)
  {
    if(*input == '{')
    {
      brackets++;
    }
    else if(*input == '}')
    {
      brackets--;
    }
    input++;
    if(brackets == 0)
    {
      i++;
      return i;
    }
  }

  return ret;
}


/* Static Functions */


/**
 * @brief  Enable Disable the jump to second flash bank and reboot board
 * @param  None
 * @retval None
 */
static void ToggleFlashBank(void)
{
  FLASH_OBProgramInitTypeDef    OBInit;
  /* Set BFB2 bit to enable boot from Flash Bank2 */
  /* Allow Access to Flash control registers and user Flash */
  HAL_FLASH_Unlock();

  /* Allow Access to option bytes sector */
  HAL_FLASH_OB_Unlock();

  /* Get the Dual boot configuration status */
  HAL_FLASHEx_OBGetConfig(&OBInit);

  /* Enable/Disable dual boot feature */
  OBInit.OptionType = OPTIONBYTE_USER;
  OBInit.USERType   = OB_USER_SWAP_BANK;

  if (((OBInit.USERConfig) & (FLASH_OPTR_SWAP_BANK)) == FLASH_OPTR_SWAP_BANK)
  {
    OBInit.USERConfig &= ~FLASH_OPTR_SWAP_BANK;
  }
  else
  {
    OBInit.USERConfig = FLASH_OPTR_SWAP_BANK;
  }

  //SYS_DEBUGF(SYS_DBG_LEVEL_WARNING, ("HW: Switching Bank\r\n"));
  if(HAL_FLASHEx_OBProgram (&OBInit) != HAL_OK)
  {
    /*
    Error occurred while setting option bytes configuration.
    User can add here some code to deal with this error.
    To know the code error, user can call function 'HAL_FLASH_GetError()'
    */
    while(1);
  }

  /* Start the Option Bytes programming process */
  if (HAL_FLASH_OB_Launch() != HAL_OK) {
    /*
    Error occurred while reloading option bytes configuration.
    User can add here some code to deal with this error.
    To know the code error, user can call function 'HAL_FLASH_GetError()'
    */
    while(1);
  }
  HAL_FLASH_OB_Lock();
  HAL_FLASH_Lock();
}


/* CubeMX Integration */
/**********************/

void App_PB_EXTI_Callback(uint16_t pin)
{
  /* anti debounce */
  static uint32_t t_start = 0;
  if(HAL_GetTick() - t_start > 10*APP_TASK_ANTI_DEBOUNCE_PERIOD_TICK)
  {
    if(pin == USER_BUTTON_Pin)
    {
      /* generate the system event to change the PM state*/
      SysEvent evt = {
          .nRawEvent = SYS_PM_MAKE_EVENT(SYS_PM_EVT_SRC_PB, SYS_PM_EVT_PARAM_SHORT_PRESS)
      };
      SysPostPowerModeEvent(evt);

      t_start = HAL_GetTick();
    }
  }
}


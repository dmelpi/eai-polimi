/**
 ******************************************************************************
 * @file    AppTask.c
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

#include <stdio.h>
#include "AppTask.h"
#include "AppTask_vtbl.h"
#include "drivers/PushButtonDrv.h"
#include "drivers/PushButtonDrv_vtbl.h"
#include "services/sysdebug.h"

#if defined(DEBUG) || defined (SYS_DEBUG)
#define CTRL_TASK_CFG_OUT_CH                      (stderr)
#else
#define CTRL_TASK_CFG_OUT_CH                      (stdout)
#endif

#ifndef APP_TASK_CFG_STACK_DEPTH
#define APP_TASK_CFG_STACK_DEPTH                  (120)
#endif

#ifndef APP_TASK_CFG_PRIORITY
#define APP_TASK_CFG_PRIORITY                     (tskIDLE_PRIORITY+1)
#endif

#define APP_TASK_ANTI_DEBOUNCH_PERIOD_TICK        (7U)

#define SYS_DEBUGF(level, message)                SYS_DEBUGF3(SYS_DBG_HW, level, message)

/**
 * The only instance of the task object.
 */
static AppTask s_xHwTaskObj;

/**
 * USB FS Device descriptor.
 */
extern USBD_DescriptorsTypeDef FS_Desc;

/**
 * Handle to USB FS device.
 */
extern USBD_HandleTypeDef hUsbDeviceFS;

// Private member function declaration
// ***********************************

/**
 * Execute one step of the task control loop while the system is in RUN mode.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return SYS_NO_EROR_CODE if success, a task specific error code otherwise.
 */
static sys_error_code_t AppTaskExecuteStepState1(AManagedTask *_this);

/**
 * Initialize the USB CDC device
 *
 * @return SYS_NO_ERROR_CODE
 */
static sys_error_code_t AppTask_InitUsbDevice(void);

/**
 * Class object declaration
 */
typedef struct _HelloWorldClass
{
  /**
   * AppTask class virtual table.
   */
  AManagedTask_vtbl m_xVTBL;

  /**
   * IProcessEventListener virtual table.
   */
  IProcessEventListener_vtbl m_xProcListenerVTBL;

  /**
   * AppTask (PM_STATE, ExecuteStepFunc) map.
   */
  pExecuteStepFunc_t m_pfPMState2FuncMap[];
} HelloWorldClass;

/**
 * The class object.
 */
static const HelloWorldClass s_xTheClass = {
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

  /* class (PM_STATE, ExecuteStepFunc) map */
  {
    AppTaskExecuteStepState1,
    NULL,
    NULL
  }
};

// Inline function forward declaration
// ***********************************

#if defined (__GNUC__)
#endif

// Public API definition
// *********************

AManagedTask* AppTaskAlloc(const void *p_mx_drv_cfg)
{
  // In this application there is only one Keyboard task,
  // so this allocator implement the singleton design pattern.

  // Initialize the super class
  AMTInit(&s_xHwTaskObj.super);

  s_xHwTaskObj.super.vptr = &s_xTheClass.m_xVTBL;

  s_xHwTaskObj.p_mx_drv_cfg = p_mx_drv_cfg;

  s_xHwTaskObj.m_xDpuListenerIF.vptr = &s_xTheClass.m_xProcListenerVTBL;

  return (AManagedTask*) &s_xHwTaskObj;
}

// AManagedTask virtual functions definition
// ***********************************************

sys_error_code_t AppTask_vtblHardwareInit(AManagedTask *_this, void *pParams)
{
  assert_param(_this != NULL);
  sys_error_code_t xRes = SYS_NO_ERROR_CODE;
  AppTask *pObj = (AppTask*) _this;

  AppTask_InitUsbDevice();

  if (pObj->p_mx_drv_cfg != NULL)
  {
    pObj->m_pxDriver = PushButtonDrvAlloc();
    if (pObj->m_pxDriver == NULL)
    {
      SYS_DEBUGF(SYS_DBG_LEVEL_SEVERE, ("HW: unable to alloc driver object.\r\n"));
      xRes = SYS_GET_LAST_LOW_LEVEL_ERROR_CODE();
    }
    else {
      PushButtonDrvParams_t driver_cfg = {
          .p_mx_gpio_cfg = (void*)pObj->p_mx_drv_cfg
      };
      xRes = IDrvInit((IDriver*)pObj->m_pxDriver, &driver_cfg);
      if (SYS_IS_ERROR_CODE(xRes)) {
        SYS_DEBUGF(SYS_DBG_LEVEL_SEVERE, ("HW: error during driver initialization\r\n"));
      }
    }
  }

  return xRes;
}

sys_error_code_t AppTask_vtblOnCreateTask(AManagedTask *_this, TaskFunction_t *pvTaskCode, const char **pcName, unsigned short *pnStackDepth, void **pParams, UBaseType_t *pxPriority)
{
  assert_param(_this != NULL);
  sys_error_code_t xRes = SYS_NO_ERROR_CODE;
//  AppTask *pObj = (AppTask*)_this;

  _this->m_pfPMState2FuncMap = s_xTheClass.m_pfPMState2FuncMap;

//  *pvTaskCode = AppTaskRun;
  *pvTaskCode = AMTRun;
  *pcName = "HW";
  *pnStackDepth = APP_TASK_CFG_STACK_DEPTH;
  *pParams = _this;
  *pxPriority = APP_TASK_CFG_PRIORITY;

  return xRes;
}

sys_error_code_t AppTask_vtblDoEnterPowerMode(AManagedTask *_this, const EPowerMode eActivePowerMode, const EPowerMode eNewPowerMode)
{
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  AppTask *p_obj = (AppTask*)_this;

  /* first propagate the event to teh driver. */
  res = IDrvDoEnterPowerMode(p_obj->m_pxDriver, eActivePowerMode, eNewPowerMode);

  if(eNewPowerMode == E_POWER_MODE_STATE1)
  {
    __NOP();
    SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("HW: -> STATE1\r\n"));
  }
  else if(eNewPowerMode == E_POWER_MODE_SLEEP_1)
  {
    __NOP();
    SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("HW: -> SLEEP_1\r\n"));
  }
  else if(eNewPowerMode == E_POWER_MODE_SENSORS_ACTIVE)
  {
    __NOP();
    SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("HW: -> SENSORS_ACTIVE\r\n"));
  }


  return res;
}

sys_error_code_t AppTask_vtblHandleError(AManagedTask *_this, SysEvent xError)
{
  assert_param(_this);
  sys_error_code_t xRes = SYS_NO_ERROR_CODE;
//  AppTask *pObj = (AppTask*)_this;

  return xRes;
}

sys_error_code_t AppTask_vtblOnEnterTaskControlLoop(AManagedTask *_this)
{
  assert_param(_this);
  sys_error_code_t xRes = SYS_NO_ERROR_CODE;
  AppTask *pObj = (AppTask*)_this;

  SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("HW: start.\r\n"));

  IDrvStart(pObj->m_pxDriver);

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

// IProcessEventListener virtual functions
//****************************************

sys_error_code_t AppTask_vtblOnProcessedDataReady(IEventListener *_this, const ProcessEvent *pxEvt)
{
  assert_param(_this != NULL);
  sys_error_code_t xRes = SYS_NO_ERROR_CODE;
  AppTask *p_if_owner = (AppTask*)((uint32_t)_this - offsetof(AppTask, m_xDpuListenerIF));
  UNUSED(p_if_owner);

  //SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("HW: observed new processed data.\r\n"));

  // Getting NN classification results.
  float32_t *payload = (float32_t *) (pxEvt->stream->payload);
  float32_t label_id = payload[0];
  float32_t accuracy = payload[1];

  // Printing NN classification results to the serial interface.
  char data_s[64];
  uint32_t size = sprintf(data_s, "{\"label_id\": %ld, \"accuracy\": %f}\r\n", (uint32_t) label_id, accuracy);
  CDC_Transmit_FS(( uint8_t * ) data_s, size);

#if 0 //PnPL
  char value_s[32];
  char *telemetry;
  uint32_t size;
  uint32_t actual_size;

  sprintf(value_s, "%d", (int)label_id);
  //SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, (value_s));
  PnPLSerializeTelemetry("ai_application", "label_id", value_s, &telemetry, &size, 0);
  /* Send the PnPL command via USB CDC interface */
  UsbCdcTask_Write((uint8_t*) telemetry, size, &actual_size);

  sprintf(value_s, "%.2f", accuracy);
  //SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, (value_s));
  PnPLSerializeTelemetry("ai_application", "accuracy", value_s, &telemetry, &size, 0);
  /* Send the PnPL command via USB CDC interface */
  UsbCdcTask_Write((uint8_t*) telemetry, size, &actual_size);
#endif

  return xRes;
}


// Private function definition
// ***************************

static sys_error_code_t AppTaskExecuteStepState1(AManagedTask *_this)
{
  assert_param(_this != NULL);
  sys_error_code_t xRes = SYS_NO_ERROR_CODE;

  vTaskDelay(pdMS_TO_TICKS(1000));
  SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("Hello STWINCSV1!!\r\n"));
  __NOP();
  __NOP();

  return xRes;
}


/* CubeMX Integration */
/**********************/

void HW_PB_EXTI_Callback(uint16_t pin)
{
  /* anti debounch */
  static uint32_t t_start = 0;
  if(HAL_GetTick() - t_start > 10*APP_TASK_ANTI_DEBOUNCH_PERIOD_TICK)
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


/* USB. */
/**********************/

static sys_error_code_t AppTask_InitUsbDevice(void)
{
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

  USBD_Start(&hUsbDeviceFS);

  return res;
}

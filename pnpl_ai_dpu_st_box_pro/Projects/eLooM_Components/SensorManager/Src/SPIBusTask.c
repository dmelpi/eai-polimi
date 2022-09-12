/**
  ******************************************************************************
  * @file    SPIBusTask.c
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

#include "SPIBusTask.h"
#include "SPIBusTask_vtbl.h"
#include "drivers/SPIMasterDriver.h"
#include "drivers/SPIMasterDriver_vtbl.h"
#include "SMMessageParser.h"
#include "services/sysdebug.h"

#ifndef SPIBUS_TASK_CFG_STACK_DEPTH
#define SPIBUS_TASK_CFG_STACK_DEPTH        120
#endif

#ifndef SPIBUS_TASK_CFG_PRIORITY
#define SPIBUS_TASK_CFG_PRIORITY           (3)
#endif

#ifndef SPIBUS_TASK_CFG_INQUEUE_LENGTH
#define SPIBUS_TASK_CFG_INQUEUE_LENGTH     20
#endif

#define SPIBUS_OP_WAIT_MS                  50

#define SYS_DEBUGF(level, message)         SYS_DEBUGF3(SYS_DBG_SPIBUS, level, message)

#if defined(DEBUG) || defined (SYS_DEBUG)
#define sTaskObj                           sSPIBUSTaskObj
#endif

typedef struct _SPIBusTaskIBus
{
  IBus super;

  SPIBusTask *m_pxOwner;
} SPIBusTaskIBus;

/**
  * Class object declaration
  */
typedef struct _SPIBusTaskClass
{
  /**
    * SPIBusTask class virtual table.
    */
  AManagedTaskEx_vtbl vtbl;

  /**
    * SPIBusTask (PM_STATE, ExecuteStepFunc) map.
    */
  pExecuteStepFunc_t p_pm_state2func_map[3];
} SPIBusTaskClass_t;

/* Private member function declaration */
/***************************************/

/**
  * Execute one step of the task control loop while the system is in RUN mode.
  *
  * @param _this [IN] specifies a pointer to a task object.
  * @return SYS_NO_EROR_CODE if success, a task specific error code otherwise.
  */
static sys_error_code_t SPIBusTaskExecuteStep(AManagedTask *_this);

static int32_t SPIBusTaskWrite(void *pxSensor, uint8_t nRegAddr, uint8_t *pnData, uint16_t nSize);
static int32_t SPIBusTaskRead(void *pxSensor, uint8_t nRegAddr, uint8_t *pnData, uint16_t nSize);

static sys_error_code_t SPIBusTaskCtrl(ABusIF *_this, EBusCtrlCmd eCtrlCmd, uint32_t nParams);

/* Inline function forward declaration */
// ***********************************

#if defined (__GNUC__)
// Inline function defined inline in the header file SPIBusTask.h must be declared here as extern function.
#endif

/* Objects instance */
/********************/

/**
  * IBus virtual table.
  */
static const IBus_vtbl s_xIBus_vtbl =
{
  SPIBusTask_vtblCtrl,
  SPIBusTask_vtblConnectDevice,
  SPIBusTask_vtblDisconnectDevice
};

/**
  * The only instance of the task object.
  */
static SPIBusTask s_xTaskObj;

/**
  * The class object.
  */
static const SPIBusTaskClass_t sTheClass =
{
  /* Class virtual table */
  {
    SPIBusTask_vtblHardwareInit,
    SPIBusTask_vtblOnCreateTask,
    SPIBusTask_vtblDoEnterPowerMode,
    SPIBusTask_vtblHandleError,
    SPIBusTask_vtblOnEnterTaskControlLoop,
    SPIBusTask_vtblForceExecuteStep,
    SPIBusTask_vtblOnEnterPowerMode
  },

  /* class (PM_STATE, ExecuteStepFunc) map */
  {
    SPIBusTaskExecuteStep,
    NULL,
    SPIBusTaskExecuteStep,
  }
};

/* Public API definition */
/*************************/

AManagedTaskEx *SPIBusTaskAlloc(const void *p_mx_drv_cfg)
{
  /* This allocator implements the singleton design pattern. */

  /* Initialize the super class */
  AMTInitEx(&s_xTaskObj.super);

  s_xTaskObj.super.vptr = &sTheClass.vtbl;
  s_xTaskObj.p_mx_drv_cfg = p_mx_drv_cfg;

  return (AManagedTaskEx *) &s_xTaskObj;
}

sys_error_code_t SPIBusTaskConnectDevice(SPIBusTask *_this, SPIBusIF *pxBusIF)
{
  assert_param(_this);

  return IBusConnectDevice(_this->m_pBusIF, &pxBusIF->super);
}

sys_error_code_t SPIBusTaskDisconnectDevice(SPIBusTask *_this, SPIBusIF *pxBusIF)
{
  assert_param(_this);

  return IBusDisconnectDevice(_this->m_pBusIF, &pxBusIF->super);
}

IBus *SPIBusTaskGetBusIF(SPIBusTask *_this)
{
  assert_param(_this);

  return _this->m_pBusIF;
}

// AManagedTask virtual functions definition
// *****************************************

sys_error_code_t SPIBusTask_vtblHardwareInit(AManagedTask *_this, void *pParams)
{
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  SPIBusTask *p_obj = (SPIBusTask *) _this;

  p_obj->m_pxDriver = SPIMasterDriverAlloc();
  if (p_obj->m_pxDriver == NULL)
  {
    SYS_DEBUGF(SYS_DBG_LEVEL_SEVERE, ("SPIBus task: unable to alloc driver object.\r\n"));
    res = SYS_GET_LAST_LOW_LEVEL_ERROR_CODE();
  }
  else
  {
    SPIMasterDriverParams_t driver_cfg =
    {
      .p_mx_spi_cfg = (void *) p_obj->p_mx_drv_cfg
    };
    res = IDrvInit((IDriver *) p_obj->m_pxDriver, &driver_cfg);
    if (SYS_IS_ERROR_CODE(res))
    {
      SYS_DEBUGF(SYS_DBG_LEVEL_SEVERE, ("SPIBus task: error during driver initialization\r\n"));
    }
  }

  return res;
}

sys_error_code_t SPIBusTask_vtblOnCreateTask(AManagedTask *_this, tx_entry_function_t *pvTaskCode, CHAR **pcName,
                                             VOID **pvStackStart,
                                             ULONG *pnStackDepth, UINT *pxPriority, UINT *pnPreemptThreshold, ULONG *pnTimeSlice, ULONG *pnAutoStart,
                                             ULONG *pParams)
{
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  SPIBusTask *p_obj = (SPIBusTask *) _this;

  // initialize the software resources.

  uint32_t item_size = (uint32_t)SMMessageGetSize(SM_MESSAGE_ID_SPI_BUS_READ);
  VOID *p_queue_items_buff = SysAlloc(SPIBUS_TASK_CFG_INQUEUE_LENGTH * item_size);

  if (p_queue_items_buff != NULL)
  {
    if (TX_SUCCESS == tx_queue_create(&p_obj->m_xInQueue, "SPIBUS_Q", item_size / 4u, p_queue_items_buff,
                                      SPIBUS_TASK_CFG_INQUEUE_LENGTH * item_size))
    {
      p_obj->m_pBusIF = SysAlloc(sizeof(SPIBusTaskIBus));
      if (p_obj->m_pBusIF != NULL)
      {
        p_obj->m_pBusIF->vptr = &s_xIBus_vtbl;
        ((SPIBusTaskIBus *) p_obj->m_pBusIF)->m_pxOwner = p_obj;
        p_obj->m_nConnectedDevices = 0;
        _this->m_pfPMState2FuncMap = sTheClass.p_pm_state2func_map;

        *pvTaskCode = AMTExRun;
        *pcName = "SPIBUS";
        *pvStackStart = NULL; // allocate the task stack in the system memory pool.
        *pnStackDepth = SPIBUS_TASK_CFG_STACK_DEPTH;
        *pParams = (ULONG) _this;
        *pxPriority = SPIBUS_TASK_CFG_PRIORITY;
        *pnPreemptThreshold = SPIBUS_TASK_CFG_PRIORITY;
        *pnTimeSlice = TX_NO_TIME_SLICE;
        *pnAutoStart = TX_AUTO_START;
      }
      else
      {
        SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_OUT_OF_MEMORY_ERROR_CODE);
        res = SYS_OUT_OF_MEMORY_ERROR_CODE;
      }
    }
    else
    {
      SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_OUT_OF_MEMORY_ERROR_CODE);
      res = SYS_OUT_OF_MEMORY_ERROR_CODE;
    }
  }

  return res;
}

sys_error_code_t SPIBusTask_vtblDoEnterPowerMode(AManagedTask *_this, const EPowerMode eActivePowerMode,
                                                 const EPowerMode eNewPowerMode)
{
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  SPIBusTask *p_obj = (SPIBusTask *) _this;

  IDrvDoEnterPowerMode((IDriver *) p_obj->m_pxDriver, eActivePowerMode, eNewPowerMode);

  if (eNewPowerMode == E_POWER_MODE_SLEEP_1)
  {
    tx_queue_flush(&p_obj->m_xInQueue);
  }

  if ((eActivePowerMode == E_POWER_MODE_SENSORS_ACTIVE) && (eNewPowerMode == E_POWER_MODE_STATE1))
  {
    tx_queue_flush(&p_obj->m_xInQueue);
  }

  SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("SPIBUS: -> %d\r\n", eNewPowerMode));

  return res;
}

sys_error_code_t SPIBusTask_vtblHandleError(AManagedTask *_this, SysEvent xError)
{
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
//  SPIBusTask *p_obj = (SPIBusTask*)_this;

  return res;
}

sys_error_code_t SPIBusTask_vtblOnEnterTaskControlLoop(AManagedTask *_this)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  SPIBusTask *p_obj = (SPIBusTask *) _this;

  SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("SPI: start.\r\n"));

  SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("SPIBUS: start the driver.\r\n"));

  res = IDrvStart((IDriver *) p_obj->m_pxDriver);
  if (SYS_IS_ERROR_CODE(res))
  {
    SYS_DEBUGF(SYS_DBG_LEVEL_WARNING, ("SPIBUS - Driver start failed.\r\n"));
    res = SYS_BASE_LOW_LEVEL_ERROR_CODE;
  }

  return res;
}

sys_error_code_t SPIBusTask_vtblForceExecuteStep(AManagedTaskEx *_this, EPowerMode eActivePowerMode)
{
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  SPIBusTask *p_obj = (SPIBusTask *) _this;

  // to resume the task we send a fake empty message.
  SMMessage xReport =
  {
    .messageID = SM_MESSAGE_ID_FORCE_STEP
  };
  if ((eActivePowerMode == E_POWER_MODE_STATE1) || (eActivePowerMode == E_POWER_MODE_SENSORS_ACTIVE))
  {
    if (AMTExIsTaskInactive(_this))
    {
      if (TX_SUCCESS != tx_queue_front_send(&p_obj->m_xInQueue, &xReport, AMT_MS_TO_TICKS(100)))
      {

        SYS_DEBUGF(SYS_DBG_LEVEL_WARNING, ("SPIBUS: unable to resume the task.\r\n"));

        res = SYS_SPIBUS_TASK_RESUME_ERROR_CODE;
        SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_SPIBUS_TASK_RESUME_ERROR_CODE);
      }
    }
    else
    {
      // do nothing and wait for the step to complete.
//      _this->m_xStatus.nDelayPowerModeSwitch = 0;
    }
  }
  else
  {
    UINT state;
    if (TX_SUCCESS == tx_thread_info_get(&_this->m_xTaskHandle, TX_NULL, &state, TX_NULL, TX_NULL, TX_NULL, TX_NULL,
                                         TX_NULL, TX_NULL))
    {
      if (state == TX_SUSPENDED)
      {
        tx_thread_resume(&_this->m_xTaskHandle);
      }
    }
  }

  return res;
}

sys_error_code_t SPIBusTask_vtblOnEnterPowerMode(AManagedTaskEx *_this, const EPowerMode eActivePowerMode,
                                                 const EPowerMode eNewPowerMode)
{
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
//  SPIBusTask *p_obj = (SPIBusTask*)_this;

  AMTExSetPMClass(_this, E_PM_CLASS_1);

  return res;
}

// IBus virtual functions definition
// *********************************

sys_error_code_t SPIBusTask_vtblCtrl(IBus *_this, EBusCtrlCmd eCtrlCmd, uint32_t nParams)
{
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  return res;
}

sys_error_code_t SPIBusTask_vtblConnectDevice(IBus *_this, ABusIF *pxBusIF)
{
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  if (pxBusIF != NULL)
  {
    pxBusIF->m_xConnector.pfReadReg = SPIBusTaskRead;
    pxBusIF->m_xConnector.pfWriteReg = SPIBusTaskWrite;
    pxBusIF->m_pfBusCtrl = SPIBusTaskCtrl;
    pxBusIF->m_pxBus = _this;
    ((SPIBusTaskIBus *) _this)->m_pxOwner->m_nConnectedDevices++;

    SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("SPIBUS: connected device: %d\r\n",
                                       ((SPIBusTaskIBus *)_this)->m_pxOwner->m_nConnectedDevices));
  }
  else
  {
    res = SYS_INVALID_PARAMETER_ERROR_CODE;
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_INVALID_PARAMETER_ERROR_CODE);
  }

  return res;
}

sys_error_code_t SPIBusTask_vtblDisconnectDevice(IBus *_this, ABusIF *pxBusIF)
{
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  if (pxBusIF != NULL)
  {
    pxBusIF->m_xConnector.pfReadReg = ABusIFNullRW;
    pxBusIF->m_xConnector.pfWriteReg = ABusIFNullRW;
    pxBusIF->m_pfBusCtrl = NULL;
    pxBusIF->m_pxBus = NULL;
    ((SPIBusTaskIBus *) _this)->m_pxOwner->m_nConnectedDevices--;

    SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("SPIBUS: connected device: %d\r\n",
                                       ((SPIBusTaskIBus *)_this)->m_pxOwner->m_nConnectedDevices));
  }
  else
  {
    res = SYS_INVALID_PARAMETER_ERROR_CODE;
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_INVALID_PARAMETER_ERROR_CODE);
  }

  return res;
}

/* Private function definition */
// ***************************

static sys_error_code_t SPIBusTaskCtrl(ABusIF *_this, EBusCtrlCmd eCtrlCmd, uint32_t nParams)
{
  return IBusCtrl(_this->m_pxBus, eCtrlCmd, nParams);
}

static sys_error_code_t SPIBusTaskExecuteStep(AManagedTask *_this)
{
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  SPIBusTask *p_obj = (SPIBusTask *) _this;

  struct spiIOMessage_t xMsg =
  {
    0
  };
  AMTExSetInactiveState((AManagedTaskEx *) _this, TRUE);
  if (TX_SUCCESS == tx_queue_receive(&p_obj->m_xInQueue, &xMsg, TX_WAIT_FOREVER))
  {
    AMTExSetInactiveState((AManagedTaskEx *) _this, FALSE);
    switch (xMsg.messageId)
    {
      case SM_MESSAGE_ID_FORCE_STEP:
        __NOP();
        // do nothing. I need only to resume the task.
        break;

      case SM_MESSAGE_ID_SPI_BUS_READ:
        SPIMasterDriverSelectDevice((SPIMasterDriver_t *) p_obj->m_pxDriver, xMsg.pxSensor->m_pxSSPinPort,
                                    xMsg.pxSensor->m_nSSPin);
        res = IIODrvRead(p_obj->m_pxDriver, xMsg.pnData, xMsg.nDataSize, xMsg.nRegAddr);
        SPIMasterDriverDeselectDevice((SPIMasterDriver_t *) p_obj->m_pxDriver, xMsg.pxSensor->m_pxSSPinPort,
                                      xMsg.pxSensor->m_nSSPin);
        if (!SYS_IS_ERROR_CODE(res))
        {
          res = SPIBusIFNotifyIOComplete(xMsg.pxSensor);
        }
        break;

      case SM_MESSAGE_ID_SPI_BUS_WRITE:
        SPIMasterDriverSelectDevice((SPIMasterDriver_t *) p_obj->m_pxDriver, xMsg.pxSensor->m_pxSSPinPort,
                                    xMsg.pxSensor->m_nSSPin);
        res = IIODrvWrite(p_obj->m_pxDriver, xMsg.pnData, xMsg.nDataSize, xMsg.nRegAddr);
        SPIMasterDriverDeselectDevice((SPIMasterDriver_t *) p_obj->m_pxDriver, xMsg.pxSensor->m_pxSSPinPort,
                                      xMsg.pxSensor->m_nSSPin);
        if (!SYS_IS_ERROR_CODE(res))
        {
          res = SPIBusIFNotifyIOComplete(xMsg.pxSensor);
        }
        break;

      default:
        //TODO: STF -  need to notify the error
        break;
    }
  }

  return res;
}

static int32_t SPIBusTaskWrite(void *pxSensor, uint8_t nRegAddr, uint8_t *pnData, uint16_t nSize)
{
  assert_param(pxSensor);
  SPIBusIF *pxSPISensor = (SPIBusIF *) pxSensor;
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  uint8_t nAutoInc = pxSPISensor->m_nAutoInc;

  struct spiIOMessage_t xMsg =
  {
    .messageId = SM_MESSAGE_ID_SPI_BUS_WRITE,
    .pxSensor = pxSPISensor,
    .nRegAddr = nRegAddr | nAutoInc,
    .pnData = pnData,
    .nDataSize = nSize
  };

  // if (s_xTaskObj.m_xInQueue != NULL) {//TODO: STF.Port - how to know if the task has been initialized ??
  if (SYS_IS_CALLED_FROM_ISR())
  {
    // we cannot read and write in the SPI BUS from an ISR. Notify the error
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_SPIBUS_TASK_IO_ERROR_CODE);
    res = SYS_SPIBUS_TASK_IO_ERROR_CODE;
  }
  else
  {
    if (TX_SUCCESS != tx_queue_send(&s_xTaskObj.m_xInQueue, &xMsg, AMT_MS_TO_TICKS(SPIBUS_OP_WAIT_MS)))
    {
      SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_SPIBUS_TASK_IO_ERROR_CODE);
      res = SYS_SPIBUS_TASK_IO_ERROR_CODE;
    }
  }
//  }

  if (!SYS_IS_ERROR_CODE(res))
  {
    // suspend the sensor task.
    res = SPIBusIFWaitIOComplete(pxSPISensor);
  }

  return res;
}

static int32_t SPIBusTaskRead(void *pxSensor, uint8_t nRegAddr, uint8_t *pnData, uint16_t nSize)
{
  assert_param(pxSensor);
  SPIBusIF *pxSPISensor = (SPIBusIF *) pxSensor;
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  uint8_t nAutoInc = 0x00;
  struct spiIOMessage_t xMsg =
  {
    .messageId = SM_MESSAGE_ID_SPI_BUS_READ,
    .pxSensor = pxSPISensor,
    .nRegAddr = nRegAddr | 0x80 | nAutoInc,
    .pnData = pnData,
    .nDataSize = nSize
  };

  // if (s_xTaskObj.m_xInQueue != NULL) { //TODO: STF.Port - how to know if the task has been initialized ??
  if (SYS_IS_CALLED_FROM_ISR())
  {
    // we cannot read and write in the SPI BUS from an ISR. Notify the error
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_SPIBUS_TASK_IO_ERROR_CODE);
    res = SYS_SPIBUS_TASK_IO_ERROR_CODE;
  }
  else
  {
    if (TX_SUCCESS != tx_queue_send(&s_xTaskObj.m_xInQueue, &xMsg, AMT_MS_TO_TICKS(SPIBUS_OP_WAIT_MS)))
    {
      SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_SPIBUS_TASK_IO_ERROR_CODE);
      res = SYS_SPIBUS_TASK_IO_ERROR_CODE;
    }
  }
//  }

  if (!SYS_IS_ERROR_CODE(res))
  {
    res = SPIBusIFWaitIOComplete(pxSPISensor);
  }

  return res;
}

/**
  ******************************************************************************
  * @file    SPIBusIF.c
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

#include "SPIBusIF.h"
#include "services/sysmem.h"

// Private functions declaration
// *****************************

// Private variables
// *****************

// Public API implementation.
// **************************

ABusIF *SPIBusIFAlloc(uint8_t nWhoAmI, GPIO_TypeDef *pxSSPinPort, uint16_t nSSPin, uint8_t nAutoInc)
{
  SPIBusIF *_this = NULL;

  _this = SysAlloc(sizeof(SPIBusIF));
  if (_this != NULL)
  {
    ABusIFInit(&_this->super, nWhoAmI);

    _this->m_pxSSPinPort = pxSSPinPort;
    _this->m_nSSPin = nSSPin;
    _this->m_nAutoInc = nAutoInc;

    // initialize the software resources
    if (TX_SUCCESS != tx_semaphore_create(&_this->m_xSyncObj, "SPI_IP_S", 0))
    {
      SysFree(_this);
      _this = NULL;
    }
    else
    {
      ABusIFSetHandle(&_this->super, _this);
    }
  }

  return (ABusIF *)_this;
}

sys_error_code_t SPIBusIFWaitIOComplete(SPIBusIF *_this)
{
  assert_param(_this);
  sys_error_code_t xRes = SYS_NO_ERROR_CODE;

  if (TX_SUCCESS != tx_semaphore_get(&_this->m_xSyncObj, TX_WAIT_FOREVER))
  {
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_UNDEFINED_ERROR_CODE);
    xRes = SYS_UNDEFINED_ERROR_CODE;
  }

  return xRes;
}

sys_error_code_t SPIBusIFNotifyIOComplete(SPIBusIF *_this)
{
  assert_param(_this);
  sys_error_code_t xRes = SYS_NO_ERROR_CODE;

  if (TX_SUCCESS != tx_semaphore_put(&_this->m_xSyncObj))
  {
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_UNDEFINED_ERROR_CODE);
    xRes = SYS_UNDEFINED_ERROR_CODE;
  }

  return xRes;
}

// Private functions definition
// ****************************


/**
  ******************************************************************************
  * @file    I2CBusIF.c
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

#include "I2CBusIF.h"
#include "services/sysmem.h"

// Private functions declaration
// *****************************

// Private variables
// *****************

// Public API implementation.
// **************************

ABusIF *I2CBusIFAlloc(uint8_t nWhoAmI, uint8_t nAddress, uint8_t nAutoInc)
{
  I2CBusIF *_this = NULL;

  _this = (I2CBusIF *)SysAlloc(sizeof(I2CBusIF));
  if (_this != NULL)
  {
    ABusIFInit(&_this->super, nWhoAmI);

    _this->m_nAddress = nAddress;
    _this->m_nAutoInc = nAutoInc;

    // initialize the software resources
    if (TX_SUCCESS != tx_semaphore_create(&_this->m_xSyncObj, "I2C_IP_S", 0))
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

sys_error_code_t I2CBusIFWaitIOComplete(I2CBusIF *_this)
{
  assert_param(_this);
  sys_error_code_t xRes = SYS_NO_ERROR_CODE;

  // if (_this->m_xSyncObj != NULL){//TODO: STF.Port - how to check the sem is initialized ??
  if (TX_SUCCESS != tx_semaphore_get(&_this->m_xSyncObj, TX_WAIT_FOREVER))
  {
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_UNDEFINED_ERROR_CODE);
    xRes = SYS_UNDEFINED_ERROR_CODE;
  }
  // }

  return xRes;
}

sys_error_code_t I2CBusIFNotifyIOComplete(I2CBusIF *_this)
{
  assert_param(_this);
  sys_error_code_t xRes = SYS_NO_ERROR_CODE;

  // if (_this->m_xSyncObj != NULL){//TODO: STF.Port - how to check the sem is initialized ??
  if (TX_SUCCESS != tx_semaphore_put(&_this->m_xSyncObj))
  {
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_UNDEFINED_ERROR_CODE);
    xRes = SYS_UNDEFINED_ERROR_CODE;
  }
//  }

  return xRes;
}

// Private functions definition
// ****************************

int32_t I2CBusNullRW(void *pxSensor, uint8_t nRegAddr, uint8_t *pnData, uint16_t nSize)
{
  UNUSED(pxSensor);
  UNUSED(nRegAddr);
  UNUSED(pnData);
  UNUSED(nSize);

  return 0;
}

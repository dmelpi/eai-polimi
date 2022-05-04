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

// Private functions declaration
// *****************************


// Private variables
// *****************


// Public API implementation.
// **************************

// GCC requires one function forward declaration in only one .c source
// in order to manage the inline.
// See also http://stackoverflow.com/questions/26503235/c-inline-function-and-gcc
#if defined (__GNUC__)
extern sys_error_code_t I2CBusIFSetWhoAmI(I2CBusIF *_this, uint8_t nWhoAmI);
extern uint8_t I2CBusIFGetWhoAmI(const I2CBusIF *_this);
#endif

sys_error_code_t I2CBusIFInit(I2CBusIF *_this, uint8_t nWhoAmI, uint8_t nAddress) {
  assert_param(_this);
  sys_error_code_t xRes = SYS_NO_ERROR_CODE;

  ABusIFInit(&_this->super);

  _this->m_nWhoAmI = nWhoAmI;
  _this->m_nAddress = nAddress;

  // initialize the software resources
  _this->m_xSyncObj = xSemaphoreCreateBinary();
  if (_this->m_xSyncObj == NULL){
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_OUT_OF_MEMORY_ERROR_CODE);
    xRes = SYS_OUT_OF_MEMORY_ERROR_CODE;
  }
#ifdef DEBUG
  else {
    vQueueAddToRegistry(_this->m_xSyncObj, "I2C_IP_S");
  }
#endif

  return xRes;
}

sys_error_code_t I2CBusIFWaitIOComplete(I2CBusIF *_this) {
  assert_param(_this);
  sys_error_code_t xRes = SYS_NO_ERROR_CODE;

  if (_this->m_xSyncObj != NULL){
    if (pdTRUE != xSemaphoreTake(_this->m_xSyncObj, portMAX_DELAY)) {
      SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_UNDEFINED_ERROR_CODE);
      xRes = SYS_UNDEFINED_ERROR_CODE;
    }
  }
  else {
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_INVALID_FUNC_CALL_ERROR_CODE);
    xRes = SYS_INVALID_FUNC_CALL_ERROR_CODE;
  }

  return xRes;
}

sys_error_code_t I2CBusIFNotifyIOComplete(I2CBusIF *_this) {
  assert_param(_this);
  sys_error_code_t xRes = SYS_NO_ERROR_CODE;

  if (_this->m_xSyncObj != NULL){
    if (pdTRUE != xSemaphoreGive(_this->m_xSyncObj)) {
      SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_UNDEFINED_ERROR_CODE);
      xRes = SYS_UNDEFINED_ERROR_CODE;
    }
  }
  else {
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_INVALID_FUNC_CALL_ERROR_CODE);
    xRes = SYS_INVALID_FUNC_CALL_ERROR_CODE;
  }

  return xRes;
}


// Private functions definition
// ****************************

int32_t I2CBusNullRW(void *pxSensor, uint8_t nRegAddr, uint8_t* pnData, uint16_t nSize) {
  UNUSED(pxSensor);
  UNUSED(nRegAddr);
  UNUSED(pnData);
  UNUSED(nSize);

  return 0;
}

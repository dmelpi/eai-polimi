/**
 ******************************************************************************
 * @file    I2CBusIF.c
 * @author  SRA - MCD
 * @version 1.1.0
 * @date    10-Dec-2021
 *
 * @brief
 *
 * <DESCRIPTIOM>
 *
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *                             
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

//FIXME: to be removed and replaced by I2CBusIFAlloc()
sys_error_code_t I2CBusIFInit(I2CBusIF *_this, uint8_t nWhoAmI, uint8_t nAddress, uint8_t nAutoInc) {
  assert_param(_this);
  sys_error_code_t xRes = SYS_NO_ERROR_CODE;

  ABusIFInit(&_this->super, nWhoAmI);

  _this->m_nWhoAmI = nWhoAmI;
  _this->m_nAddress = nAddress;
  _this->m_nAutoInc = nAutoInc;

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

ABusIF *I2CBusIFAlloc(uint8_t nWhoAmI, uint8_t nAddress, uint8_t nAutoInc) {
  I2CBusIF *_this = NULL;

  _this = pvPortMalloc(sizeof(I2CBusIF));
  if(_this != NULL) {
    ABusIFInit(&_this->super, nWhoAmI);

    _this->m_nAddress = nAddress;
    _this->m_nAutoInc = nAutoInc;

    // initialize the software resources
    _this->m_xSyncObj = xSemaphoreCreateBinary();
    if (_this->m_xSyncObj == NULL) {
      vPortFree(_this);
      _this = NULL;
    }
    else {
      ABusIFSetHandle(&_this->super, _this);
#ifdef DEBUG
      vQueueAddToRegistry(_this->m_xSyncObj, "I2C_IP_S");
#endif
    }
  }

  return (ABusIF *)_this;
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

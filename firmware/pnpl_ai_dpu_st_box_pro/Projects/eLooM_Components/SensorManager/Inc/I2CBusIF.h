/**
  ******************************************************************************
  * @file    I2CBusIF.h
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
#ifndef I2CBUSIF_H_
#define I2CBUSIF_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ABusIF.h"
#include "tx_api.h"


/**
  * Create a type name for _I2CBusIF.
  */
typedef struct _I2CBusIF I2CBusIF;

/**
  * Specifies the I2C interface for a generic sensor.
  */
struct _I2CBusIF
{
  /**
    * The bus connector encapsulates the function pointer to read and write in the bus,
    * and it is compatible with the the ST universal sensor driver.
    */
  ABusIF super;

  /**
    * Slave address.
    */
  uint8_t m_nAddress;

  /**
    * Address auto-increment (Multi-byte read/write).
    */
  uint8_t m_nAutoInc;

  /**
    * Synchronization object used to synchronize the sensor with the bus.
    */
  TX_SEMAPHORE m_xSyncObj;
};


// Public API declaration
// **********************

/**
  * Initialize a sensor object. It must be called once before using the sensor.
  *
  * @param _this [IN] specifies a sensor object.
  * @param nWhoAmI [IN] specifies the sensor ID. It can be zero.
  * @param nAddress [IN] specifies the I2C address of the device.
  * @param nAutoInc [IN] specifies the I2C address auto-increment to allow multiple data read/write.
  * @return SYS_NO_EROR_CODE if success, an error code otherwise.
  */
ABusIF *I2CBusIFAlloc(uint8_t nWhoAmI, uint8_t nAddress, uint8_t nAutoInc);

sys_error_code_t I2CBusIFWaitIOComplete(I2CBusIF *_this);
sys_error_code_t I2CBusIFNotifyIOComplete(I2CBusIF *_this);

int32_t I2CBusNullRW(void *pxSensor, uint8_t nRegAddr, uint8_t *pnData, uint16_t nSize);

// Inline function definition
// **************************

#ifdef __cplusplus
}
#endif

#endif /* I2CBUSIF_H_ */

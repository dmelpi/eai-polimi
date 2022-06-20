/**
 ******************************************************************************
 * @file    I2CMasterDriver.h
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
#ifndef INCLUDE_DRIVERS_I2CMASTERDRIVER_H_
#define INCLUDE_DRIVERS_I2CMASTERDRIVER_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "drivers/IIODriver.h"
#include "drivers/IDriver_vtbl.h"
#include "drivers/IIODriver_vtbl.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "mx.h"


/**
 * Create  type name for _I2CMasterDriver_t.
 */
typedef struct _I2CMasterDriver_t I2CMasterDriver_t;

/**
 * Initialization parameters for the driver.
 */
typedef struct _I2CMasterDriverParams_t {

  /**
   * Specifies the peripheral HAL initialization parameters generated by CubeMX.
   */
  MX_I2CParams_t *p_mx_i2c_cfg;

  /**
   * Generic parameter.
   */
  uint32_t param;
}I2CMasterDriverParams_t;

/**
 *  I2CMasterDriver internal structure.
 */
struct _I2CMasterDriver_t {
  /**
   * Base class object.
   */
  IIODriver super;

  /* Driver variables should be added here. */

  /**
   * Specifies all the configuration parameters for the SPI peripheral linked to
   * an instance of this class. It based on the HAL driver generated by CubeMX.
   */
  I2CMasterDriverParams_t mx_handle;

  /**
   * Synchronization object used to synchronize the low lever driver with the task.
   */
  SemaphoreHandle_t sync_obj;

  /**
   * Specifies the address of the target device. All read and write operation are done with the device specified by this address.
   */
  uint16_t target_device_addr;

  /**
   * Count the number of errors reported by the hardware IP.
   */
  uint16_t ip_errors;
};


/* Public API declaration */
/**************************/

/**
 * Allocate an instance of I2CMasterDriver. The driver is allocated
 * in the FreeRTOS heap.
 *
 * @return a pointer to the generic interface ::IDriver if success,
 * or SYS_OUT_OF_MEMORY_ERROR_CODE otherwise.
 */
IIODriver *I2CMasterDriverAlloc(void);

/**
 * Set the address of a slave device in the I2C bus. All read and write operation are done with
 * the device specified by this address.
 *
 * @param _this [IN] specifies a pointer to a SPIMasterDriver object.
 * @param nAddress [IN] specifies address of the slave device in the I2C bus.
 * @return SYS_NO_ERROR_CODE
 */
sys_error_code_t I2CMasterDriverSetDeviceAddr(I2CMasterDriver_t *_this, uint16_t nAddress);


/* Inline functions definition */
/*******************************/


#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_DRIVERS_I2CMASTERDRIVER_H_ */

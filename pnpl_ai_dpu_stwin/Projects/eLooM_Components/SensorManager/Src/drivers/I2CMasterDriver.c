/**
 ******************************************************************************
 * @file    I2CMasterDriver.c
 * @author  SRA - MCD
 * @version 1.1.0
 * @date    10-Dec-2021
 *
 * @brief I2C driver definition.
 *
 * I2C driver definition.
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

#include "drivers/I2CMasterDriver.h"
#include "drivers/I2CMasterDriver_vtbl.h"
#include "dma.h"
#include "FreeRTOS.h"
#include "services/sysdebug.h"

#define I2CDRV_CFG_HARDWARE_PERIPHERALS_COUNT   1

#define SYS_DEBUGF(level, message)              SYS_DEBUGF3(SYS_DBG_DRIVERS, level, message)


/**
 * I2CMasterDriver Driver virtual table.
 */
static const IIODriver_vtbl sI2CMasterDriver_vtbl = {
    I2CMasterDriver_vtblInit,
    I2CMasterDriver_vtblStart,
    I2CMasterDriver_vtblStop,
    I2CMasterDriver_vtblDoEnterPowerMode,
    I2CMasterDriver_vtblReset,
    I2CMasterDriver_vtblWrite,
    I2CMasterDriver_vtblRead
};

/**
 * Data associated to the hardware peripheral.
 */
typedef struct _I2CPeripheralResources_t {
  /**
   * Synchronization object used by the driver to synchronize the I2C ISR with the task using the driver;
   */
  SemaphoreHandle_t sync_obj;

  /**
   * Count the number of errors reported by the hardware IP.
   */
  uint16_t *p_ip_errors;
}I2CPeripheralResources_t;

/**
 *
 */
static I2CPeripheralResources_t spHwResouces[I2CDRV_CFG_HARDWARE_PERIPHERALS_COUNT] = {
  {NULL}
};


/* Private member function declaration */
/***************************************/

static void I2CMasterDrvMemRxCpltCallback(I2C_HandleTypeDef *p_i2c);
static void I2CMasterDrvMemTxCpltCallback(I2C_HandleTypeDef *p_i2c);
static void I2CMasterDrvErrorCallback(I2C_HandleTypeDef *p_i2c);


/* Public API definition */
/*************************/

sys_error_code_t I2CMasterDriverSetDeviceAddr(I2CMasterDriver_t *_this, uint16_t address) {
  assert_param(_this);

  _this->target_device_addr = address;

  return SYS_NO_ERROR_CODE;
}


/* IIODriver virtual function definition */
/*****************************************/

IIODriver *I2CMasterDriverAlloc(void) {
  IIODriver *p_new_obj = (IIODriver*)pvPortMalloc(sizeof(I2CMasterDriver_t));

  if (p_new_obj == NULL) {
    SYS_SET_LOW_LEVEL_ERROR_CODE(SYS_OUT_OF_MEMORY_ERROR_CODE);
    SYS_DEBUGF(SYS_DBG_LEVEL_WARNING, ("I2CMasterDriver - alloc failed.\r\n"));
  }
  else {
    p_new_obj->vptr = &sI2CMasterDriver_vtbl;
  }

  return p_new_obj;
}

sys_error_code_t I2CMasterDriver_vtblInit(IDriver *_this, void *p_params) {
  assert_param(_this != NULL);
  assert_param(p_params != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  I2CMasterDriver_t *p_obj = (I2CMasterDriver_t*)_this;
  p_obj->mx_handle.p_mx_i2c_cfg = ((I2CMasterDriverParams_t*)p_params)->p_mx_i2c_cfg;
  I2C_HandleTypeDef *p_i2c = p_obj->mx_handle.p_mx_i2c_cfg->p_i2c_handle;

  p_obj->mx_handle.p_mx_i2c_cfg->p_mx_dma_init_f();
  p_obj->mx_handle.p_mx_i2c_cfg->p_mx_init_f();

  /* Register SPI DMA complete Callback*/
  if (HAL_OK != HAL_I2C_RegisterCallback(p_i2c, HAL_I2C_MEM_RX_COMPLETE_CB_ID, I2CMasterDrvMemRxCpltCallback)) {
    SYS_SET_LOW_LEVEL_ERROR_CODE(SYS_UNDEFINED_ERROR_CODE);
    res = SYS_UNDEFINED_ERROR_CODE;
  }
  else if (HAL_OK != HAL_I2C_RegisterCallback(p_i2c, HAL_I2C_MEM_TX_COMPLETE_CB_ID, I2CMasterDrvMemTxCpltCallback)) {
    SYS_SET_LOW_LEVEL_ERROR_CODE(SYS_UNDEFINED_ERROR_CODE);
    res = SYS_UNDEFINED_ERROR_CODE;
  }
  else if (HAL_OK != HAL_I2C_RegisterCallback(p_i2c, HAL_I2C_ERROR_CB_ID, I2CMasterDrvErrorCallback)) {
    SYS_SET_LOW_LEVEL_ERROR_CODE(SYS_UNDEFINED_ERROR_CODE);
    res = SYS_UNDEFINED_ERROR_CODE;
  }
  else {
    /* initialize the software resources*/
    p_obj->target_device_addr = 0;
    p_obj->ip_errors = 0;
    p_obj->sync_obj = xSemaphoreCreateBinary();
    if (p_obj->sync_obj == NULL){
      SYS_SET_LOW_LEVEL_ERROR_CODE(SYS_OUT_OF_MEMORY_ERROR_CODE);
      res = SYS_OUT_OF_MEMORY_ERROR_CODE;
    }

    spHwResouces[0].sync_obj = p_obj->sync_obj;
    spHwResouces[0].p_ip_errors = &p_obj->ip_errors;
  }

#ifdef DEBUG
  if (p_obj->sync_obj) {
    vQueueAddToRegistry(p_obj->sync_obj, "I2C2Drv");
  }
#endif

  SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("I2CMasterDriver: initialization done.\r\n"));

  return res;
}

sys_error_code_t I2CMasterDriver_vtblStart(IDriver *_this) {
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  I2CMasterDriver_t *p_obj = (I2CMasterDriver_t*)_this;

  /* I2C interrupt enable */
  HAL_NVIC_EnableIRQ(p_obj->mx_handle.p_mx_i2c_cfg->i2c_ev_irq_n);
  HAL_NVIC_EnableIRQ(p_obj->mx_handle.p_mx_i2c_cfg->i2c_er_irq_n);

  /* DMA RX and TX Channels IRQn interrupt enable */
  HAL_NVIC_EnableIRQ(p_obj->mx_handle.p_mx_i2c_cfg->i2c_dma_rx_irq_n);
  HAL_NVIC_EnableIRQ(p_obj->mx_handle.p_mx_i2c_cfg->i2c_dma_tx_irq_n);

  return res;
}

sys_error_code_t I2CMasterDriver_vtblStop(IDriver *_this) {
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  I2CMasterDriver_t *p_obj = (I2CMasterDriver_t*)_this;

  /* I2C interrupt disable */
  HAL_NVIC_DisableIRQ(p_obj->mx_handle.p_mx_i2c_cfg->i2c_ev_irq_n);
  HAL_NVIC_DisableIRQ(p_obj->mx_handle.p_mx_i2c_cfg->i2c_er_irq_n);

  /* DMA RX and TX Channels IRQn interrupt disable */
  HAL_NVIC_DisableIRQ(p_obj->mx_handle.p_mx_i2c_cfg->i2c_dma_rx_irq_n);
  HAL_NVIC_DisableIRQ(p_obj->mx_handle.p_mx_i2c_cfg->i2c_dma_tx_irq_n);

  return res;
}

sys_error_code_t I2CMasterDriver_vtblDoEnterPowerMode(IDriver *_this, const EPowerMode active_power_mode, const EPowerMode new_powerMode) {
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  /*I2CMasterDriver_t *p_obj = (I2CMasterDriver_t*)_this;*/

  return res;
}

sys_error_code_t I2CMasterDriver_vtblReset(IDriver *_this, void *p_params) {
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  /*I2CMasterDriver_t *p_obj = (I2CMasterDriver_t*)_this;*/

  return res;
}

sys_error_code_t I2CMasterDriver_vtblWrite(IIODriver *_this, uint8_t *p_data_buffer, uint16_t data_size, uint16_t channel) {
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  I2CMasterDriver_t *p_obj = (I2CMasterDriver_t*)_this;
  I2C_HandleTypeDef *p_i2c = p_obj->mx_handle.p_mx_i2c_cfg->p_i2c_handle;

  while (HAL_I2C_Mem_Write_DMA(p_i2c, p_obj->target_device_addr, channel, I2C_MEMADD_SIZE_8BIT, p_data_buffer, data_size) != HAL_OK) {
    if (HAL_I2C_GetError(p_i2c) != HAL_BUSY) {
      SYS_SET_LOW_LEVEL_ERROR_CODE(SYS_I2C_M_WRITE_ERROR_CODE);
      sys_error_handler();
    }
  }
  /* Suspend the calling task until the operation is completed.*/
  xSemaphoreTake(p_obj->sync_obj, portMAX_DELAY);

  return res;
}

sys_error_code_t I2CMasterDriver_vtblRead(IIODriver *_this, uint8_t *p_data_buffer, uint16_t data_size, uint16_t channel) {
  assert_param(_this);
  sys_error_code_t xRes = SYS_NO_ERROR_CODE;
  I2CMasterDriver_t *pObj = (I2CMasterDriver_t*)_this;
  I2C_HandleTypeDef *p_i2c = pObj->mx_handle.p_mx_i2c_cfg->p_i2c_handle;

  while (HAL_I2C_Mem_Read_DMA(p_i2c, pObj->target_device_addr, channel, I2C_MEMADD_SIZE_8BIT, p_data_buffer, data_size) != HAL_OK) {
    if (HAL_I2C_GetError(p_i2c) != HAL_BUSY) {
      SYS_SET_LOW_LEVEL_ERROR_CODE(SYS_I2C_M_WRITE_ERROR_CODE);
      sys_error_handler();
    }
  }
  /* Suspend the calling task until the operation is completed.*/
  xSemaphoreTake(pObj->sync_obj, portMAX_DELAY);

  return xRes;
}


/* Private function definition */
/*******************************/


/* CubeMX integration */
/**********************/

static void I2CMasterDrvMemRxCpltCallback(I2C_HandleTypeDef *p_i2c) {
  UNUSED(p_i2c);

  if (spHwResouces[0].sync_obj) {
    xSemaphoreGiveFromISR(spHwResouces[0].sync_obj, NULL);
  }
}

static void I2CMasterDrvMemTxCpltCallback(I2C_HandleTypeDef *p_i2c) {
  UNUSED(p_i2c);

  if (spHwResouces[0].sync_obj) {
    xSemaphoreGiveFromISR(spHwResouces[0].sync_obj, NULL);
  }
}

static void I2CMasterDrvErrorCallback(I2C_HandleTypeDef *p_i2c) {
  UNUSED(p_i2c);

  *(spHwResouces[0].p_ip_errors) += 1;
}

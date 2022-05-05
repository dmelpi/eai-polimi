/**
 ******************************************************************************
 * @file    DFSDMDriver.c
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

#include "drivers/DFSDMDriver.h"
#include "drivers/DFSDMDriver_vtbl.h"
#include "FreeRTOS.h"
#include "services/sysdebug.h"

#define SYS_DEBUGF(level, message)      SYS_DEBUGF3(SYS_DBG_DRIVERS, level, message)


/**
 * DFSDMDriver Driver virtual table.
 */
static const IDriver_vtbl sDFSDMDriver_vtbl = {
    DFSDMDriver_vtblInit,
    DFSDMDriver_vtblStart,
    DFSDMDriver_vtblStop,
    DFSDMDriver_vtblDoEnterPowerMode,
    DFSDMDriver_vtblReset
};

#if defined (__GNUC__) || defined (__ICCARM__)
extern sys_error_code_t DFSDMDriverFilterRegisterCallback(DFSDMDriver_t *_this, HAL_DFSDM_Filter_CallbackIDTypeDef CallbackID, pDFSDM_Filter_CallbackTypeDef pCallback);
#endif

/* Private member function declaration */
/***************************************/


/* Public API definition */
/*************************/

sys_error_code_t DFSDMDrvSetDataBuffer(DFSDMDriver_t *_this, uint32_t *p_buffer, uint32_t buffer_size)
{
  assert_param(_this != NULL);

  _this->p_buffer = p_buffer;
  _this->buffer_size = buffer_size;

  return SYS_NO_ERROR_CODE;
}

/* IDriver virtual functions definition */
/****************************************/

IDriver *DFSDMDriverAlloc(void)
{
  IDriver *p_new_obj = (IDriver*)pvPortMalloc(sizeof(DFSDMDriver_t));

  if (p_new_obj == NULL) {
    SYS_SET_LOW_LEVEL_ERROR_CODE(SYS_OUT_OF_MEMORY_ERROR_CODE);
    SYS_DEBUGF(SYS_DBG_LEVEL_WARNING, ("DFSDMDriver - alloc failed.\r\n"));
  }
  else {
    p_new_obj->vptr = &sDFSDMDriver_vtbl;
  }

  return p_new_obj;
}

sys_error_code_t DFSDMDriver_vtblInit(IDriver *_this, void *p_params)
{
  assert_param(_this != NULL);
  assert_param(p_params != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  DFSDMDriver_t *p_obj = (DFSDMDriver_t*)_this;
  DFSDMDriverParams_t *p_init_param = (DFSDMDriverParams_t*)p_params;
  p_obj->mx_handle.p_mx_dfsdm_cfg = p_init_param->p_mx_dfsdm_cfg;

  /* Initialize the DMA IRQ */
  p_obj->mx_handle.p_mx_dfsdm_cfg->p_mx_dma_init_f();

  /* Initialize the DFSM */
  p_obj->mx_handle.p_mx_dfsdm_cfg->p_mx_init_f();

  return res;
}

sys_error_code_t DFSDMDriver_vtblStart(IDriver *_this)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  DFSDMDriver_t *p_obj = (DFSDMDriver_t*)_this;

  /* check if the buffer as been set, otherwise I cannot start the operation */
  if (p_obj->p_buffer != NULL)
  {
    if (HAL_OK != HAL_DFSDM_FilterRegularStart_DMA(p_obj->mx_handle.p_mx_dfsdm_cfg->p_dfsdm_filter, (int32_t*) p_obj->p_buffer, p_obj->buffer_size))
    {
      res =SYS_DFSDM_DRV_GENERIC_ERROR_CODE;
      SYS_SET_LOW_LEVEL_ERROR_CODE(SYS_DFSDM_DRV_GENERIC_ERROR_CODE);
    }
    else
    {
      HAL_NVIC_EnableIRQ(p_obj->mx_handle.p_mx_dfsdm_cfg->irq_n);
    }
  }
  else
  {
    res = SYS_INVALID_FUNC_CALL_ERROR_CODE;
    SYS_SET_LOW_LEVEL_ERROR_CODE(SYS_INVALID_FUNC_CALL_ERROR_CODE);
  }

  return res;
}

sys_error_code_t DFSDMDriver_vtblStop(IDriver *_this)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  DFSDMDriver_t *p_obj = (DFSDMDriver_t*)_this;

  HAL_NVIC_DisableIRQ(p_obj->mx_handle.p_mx_dfsdm_cfg->irq_n);

  return res;
}

sys_error_code_t DFSDMDriver_vtblDoEnterPowerMode(IDriver *_this, const EPowerMode active_power_mode, const EPowerMode new_power_mode)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
/*  DFSDMDriver_t *p_obj = (DFSDMDriver_t*)_this; */

  return res;
}

sys_error_code_t DFSDMDriver_vtblReset(IDriver *_this, void *p_params)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
/*  DFSDMDriver_t *p_obj = (DFSDMDriver_t*)_this; */

  return res;
}


/* Private function definition */
/*******************************/

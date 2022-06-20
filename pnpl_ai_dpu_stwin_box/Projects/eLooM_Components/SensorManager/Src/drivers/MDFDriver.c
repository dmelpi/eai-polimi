/**
  ******************************************************************************
  * @file    MDFDriver.c
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

#include "drivers/MDFDriver.h"
#include "drivers/MDFDriver_vtbl.h"
#include "services/sysdebug.h"

#define SYS_DEBUGF(level, message)      SYS_DEBUGF3(SYS_DBG_DRIVERS, level, message)

/**
  * MDFDriver Driver virtual table.
  */
static const IDriver_vtbl sMDFDriver_vtbl =
{
  MDFDriver_vtblInit,
  MDFDriver_vtblStart,
  MDFDriver_vtblStop,
  MDFDriver_vtblDoEnterPowerMode,
  MDFDriver_vtblReset
};

#if defined (__GNUC__) || defined (__ICCARM__)
extern sys_error_code_t MDFDriverFilterRegisterCallback(MDFDriver_t *_this, HAL_MDF_CallbackIDTypeDef CallbackID,
                                                        pMDF_CallbackTypeDef pCallback);
#endif

/* Private member function declaration */
/***************************************/

/* Public API definition */
/*************************/

sys_error_code_t MDFDrvSetDataBuffer(MDFDriver_t *_this, int16_t *p_buffer, uint32_t buffer_size)
{
  assert_param(_this != NULL);

  /* Set buffer and buffer_size for the driver */
  _this->p_buffer = p_buffer;
  _this->buffer_size = buffer_size;

  /* Configure the DMA for MDF */
  _this->mx_handle.p_mx_mdf_cfg->p_mdf_dma_config->Address = (uint32_t) &p_buffer[0];
  _this->mx_handle.p_mx_mdf_cfg->p_mdf_dma_config->DataLength = buffer_size * 2U;
  _this->mx_handle.p_mx_mdf_cfg->p_mdf_dma_config->MsbOnly = ENABLE;

  return SYS_NO_ERROR_CODE;
}

sys_error_code_t MDFSetMDFConfig(IDriver *_this, float ODR)
{
  MDFDriver_t *p_obj = (MDFDriver_t *) _this;

  if (p_obj->mx_handle.param == 7)  /* IMP34DT05 - Digital mic */
  {
    HAL_MDF_DeInit(p_obj->mx_handle.p_mx_mdf_cfg->p_mdf);

    if (ODR <= 16000.0f)
    {
      p_obj->mx_handle.p_mx_mdf_cfg->p_mdf->Init.CommonParam.OutputClock.Divider = 10;
      HAL_MDF_Init(p_obj->mx_handle.p_mx_mdf_cfg->p_mdf);
      p_obj->mx_handle.p_mx_mdf_cfg->p_mdf_config->Gain = -4;
      p_obj->mx_handle.p_mx_mdf_cfg->p_mdf_config->DecimationRatio = 24;
    }
    else if (ODR <= 32000.0f)
    {
      p_obj->mx_handle.p_mx_mdf_cfg->p_mdf->Init.CommonParam.OutputClock.Divider = 10;
      HAL_MDF_Init(p_obj->mx_handle.p_mx_mdf_cfg->p_mdf);
      p_obj->mx_handle.p_mx_mdf_cfg->p_mdf_config->Gain = 5;
      p_obj->mx_handle.p_mx_mdf_cfg->p_mdf_config->DecimationRatio = 12;
    }
    else
    {
      p_obj->mx_handle.p_mx_mdf_cfg->p_mdf->Init.CommonParam.OutputClock.Divider = 5;
      HAL_MDF_Init(p_obj->mx_handle.p_mx_mdf_cfg->p_mdf);
      p_obj->mx_handle.p_mx_mdf_cfg->p_mdf_config->Gain = 1;
      p_obj->mx_handle.p_mx_mdf_cfg->p_mdf_config->DecimationRatio = 16;
    }
  }
  else  /* IMP23ABSU - Analog mic */
  {
    if (ODR <= 16000.0f)
    {
      p_obj->mx_handle.p_mx_mdf_cfg->p_mdf_config->ReshapeFilter.Activation = ENABLE;
      p_obj->mx_handle.p_mx_mdf_cfg->p_mdf_config->ReshapeFilter.DecimationRatio = MDF_RSF_DECIMATION_RATIO_4;
      p_obj->mx_handle.p_mx_mdf_cfg->p_mdf_config->CicMode = MDF_TWO_FILTERS_MCIC_SINC3;
      p_obj->mx_handle.p_mx_mdf_cfg->p_mdf_config->Gain = -2;
      p_obj->mx_handle.p_mx_mdf_cfg->p_mdf_config->DecimationRatio = 12;
    }
    else if (ODR <= 32000.0f)
    {
      p_obj->mx_handle.p_mx_mdf_cfg->p_mdf_config->ReshapeFilter.Activation = ENABLE;
      p_obj->mx_handle.p_mx_mdf_cfg->p_mdf_config->ReshapeFilter.DecimationRatio = MDF_RSF_DECIMATION_RATIO_4;
      p_obj->mx_handle.p_mx_mdf_cfg->p_mdf_config->CicMode = MDF_ONE_FILTER_SINC4;
      p_obj->mx_handle.p_mx_mdf_cfg->p_mdf_config->Gain = 0;
      p_obj->mx_handle.p_mx_mdf_cfg->p_mdf_config->DecimationRatio = 6;
    }
    else if (ODR <= 48000.0f)
    {
      p_obj->mx_handle.p_mx_mdf_cfg->p_mdf_config->ReshapeFilter.Activation = ENABLE;
      p_obj->mx_handle.p_mx_mdf_cfg->p_mdf_config->ReshapeFilter.DecimationRatio = MDF_RSF_DECIMATION_RATIO_4;
      p_obj->mx_handle.p_mx_mdf_cfg->p_mdf_config->CicMode = MDF_ONE_FILTER_SINC5;
      p_obj->mx_handle.p_mx_mdf_cfg->p_mdf_config->Gain = 0;
      p_obj->mx_handle.p_mx_mdf_cfg->p_mdf_config->DecimationRatio = 4;
    }
    else if (ODR <= 96000.0f)
    {
      p_obj->mx_handle.p_mx_mdf_cfg->p_mdf_config->ReshapeFilter.Activation = ENABLE;
      p_obj->mx_handle.p_mx_mdf_cfg->p_mdf_config->ReshapeFilter.DecimationRatio = MDF_RSF_DECIMATION_RATIO_4;
      p_obj->mx_handle.p_mx_mdf_cfg->p_mdf_config->CicMode = MDF_ONE_FILTER_SINC5;
      p_obj->mx_handle.p_mx_mdf_cfg->p_mdf_config->Gain = 2;
      p_obj->mx_handle.p_mx_mdf_cfg->p_mdf_config->DecimationRatio = 2;
    }
    else
    {
      p_obj->mx_handle.p_mx_mdf_cfg->p_mdf_config->ReshapeFilter.Activation = DISABLE;
      p_obj->mx_handle.p_mx_mdf_cfg->p_mdf_config->CicMode = MDF_ONE_FILTER_SINC5;
      p_obj->mx_handle.p_mx_mdf_cfg->p_mdf_config->Gain = 4;
      p_obj->mx_handle.p_mx_mdf_cfg->p_mdf_config->DecimationRatio = 4;
    }
  }
  return SYS_NO_ERROR_CODE;
}

/* IDriver virtual functions definition */
/****************************************/

IDriver *MDFDriverAlloc(void)
{
  IDriver *p_new_obj = (IDriver *) SysAlloc(sizeof(MDFDriver_t));

  if (p_new_obj == NULL)
  {
    SYS_SET_LOW_LEVEL_ERROR_CODE(SYS_OUT_OF_MEMORY_ERROR_CODE);
    SYS_DEBUGF(SYS_DBG_LEVEL_WARNING, ("MDFDriver - alloc failed.\r\n"));
  }
  else
  {
    p_new_obj->vptr = &sMDFDriver_vtbl;
  }

  return p_new_obj;
}

sys_error_code_t MDFDriver_vtblInit(IDriver *_this, void *p_params)
{
  assert_param(_this != NULL);
  assert_param(p_params != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  MDFDriver_t *p_obj = (MDFDriver_t *) _this;
  MDFDriverParams_t *p_init_param = (MDFDriverParams_t *) p_params;
  p_obj->mx_handle.p_mx_mdf_cfg = p_init_param->p_mx_mdf_cfg;

  /* Initialize the DMA IRQ */
  p_obj->mx_handle.p_mx_mdf_cfg->p_mx_dma_init_f();

  /* Initialize the DFSM */
  p_obj->mx_handle.p_mx_mdf_cfg->p_mx_init_f();

  /* Save optional param */
  p_obj->mx_handle.param = p_init_param->param;

  return res;
}

sys_error_code_t MDFDriver_vtblStart(IDriver *_this)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  MDFDriver_t *p_obj = (MDFDriver_t *) _this;

  /* check if the buffer as been set, otherwise I cannot start the operation */
  if (p_obj->p_buffer != NULL)
  {
    if (HAL_OK
        != HAL_MDF_AcqStart_DMA(p_obj->mx_handle.p_mx_mdf_cfg->p_mdf, p_obj->mx_handle.p_mx_mdf_cfg->p_mdf_config,
                                p_obj->mx_handle.p_mx_mdf_cfg->p_mdf_dma_config))
    {
      res = SYS_MDF_DRV_GENERIC_ERROR_CODE;
      SYS_SET_LOW_LEVEL_ERROR_CODE(SYS_MDF_DRV_GENERIC_ERROR_CODE);
    }
    else
    {
      HAL_NVIC_EnableIRQ(p_obj->mx_handle.p_mx_mdf_cfg->irq_n);
      if (p_obj->mx_handle.param == 7)
      {
        HAL_MDF_GenerateTrgo(p_obj->mx_handle.p_mx_mdf_cfg->p_mdf);
      }
    }
  }
  else
  {
    res = SYS_INVALID_FUNC_CALL_ERROR_CODE;
    SYS_SET_LOW_LEVEL_ERROR_CODE(SYS_INVALID_FUNC_CALL_ERROR_CODE);
  }

  return res;
}

sys_error_code_t MDFDriver_vtblStop(IDriver *_this)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  MDFDriver_t *p_obj = (MDFDriver_t *) _this;

  if (p_obj->p_buffer != NULL)
  {
    if (HAL_OK != HAL_MDF_AcqStop_DMA(p_obj->mx_handle.p_mx_mdf_cfg->p_mdf))
    {
      res = SYS_MDF_DRV_GENERIC_ERROR_CODE;
      SYS_SET_LOW_LEVEL_ERROR_CODE(SYS_MDF_DRV_GENERIC_ERROR_CODE);
    }
    else
    {
      HAL_NVIC_DisableIRQ(p_obj->mx_handle.p_mx_mdf_cfg->irq_n);
    }
  }
  else
  {
    res = SYS_INVALID_FUNC_CALL_ERROR_CODE;
    SYS_SET_LOW_LEVEL_ERROR_CODE(SYS_INVALID_FUNC_CALL_ERROR_CODE);
  }

  return res;
}

sys_error_code_t MDFDriver_vtblDoEnterPowerMode(IDriver *_this, const EPowerMode active_power_mode,
                                                const EPowerMode new_power_mode)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  /*  MDFDriver_t *p_obj = (MDFDriver_t*)_this; */

  return res;
}

sys_error_code_t MDFDriver_vtblReset(IDriver *_this, void *p_params)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  /*  MDFDriver_t *p_obj = (MDFDriver_t*)_this; */

  return res;
}

/* Private function definition */
/*******************************/

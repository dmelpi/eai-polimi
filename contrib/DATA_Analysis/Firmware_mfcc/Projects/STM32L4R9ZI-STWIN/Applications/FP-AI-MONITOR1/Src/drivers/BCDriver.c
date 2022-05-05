/**
 ******************************************************************************
 * @file    BCDriver.c
 * @author  STMicroelectronics - AIS - MCD Team
 * @version 1.0.0
 * @date    Sep 3, 2021
 *
 * @brief  BCDriver_t definition.
 *
 * This file implements the BCDriver.
 *
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2021 STMicroelectronics
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file in
 * the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 ******************************************************************************
 */

#include "drivers/BCDriver.h"
#include "drivers/BCDriver_vtbl.h"
#include "FreeRTOS.h"
#include "services/sysdebug.h"

#define SYS_DEBUGF(level, message)      SYS_DEBUGF3(SYS_DBG_DRIVERS, level, message)


/**
 * BCDriver Driver virtual table.
 */
static const IDriver_vtbl sBCDriver_vtbl = {
    BCDriver_vtblInit,
    BCDriver_vtblStart,
    BCDriver_vtblStop,
    BCDriver_vtblDoEnterPowerMode,
    BCDriver_vtblReset
};


/* Private member function declaration */
/***************************************/


/* Inline function forward declaration */
/***************************************/

#if defined (__GNUC__) || defined (__ICCARM__)
/* Inline function defined inline in the header file UtilTask.h must be declared here as extern function. */
extern sys_error_code_t BCDriverToggleLine1W(BCDriver_t *_this);
extern sys_error_code_t BCDriverResetLine1W(BCDriver_t *_this);
#endif

/* Public API definition */
/*************************/

IDriver *BCDriverAlloc(void)
{
  IDriver *p_new_obj = (IDriver*)pvPortMalloc(sizeof(BCDriver_t));

  if (p_new_obj == NULL) {
    SYS_SET_LOW_LEVEL_ERROR_CODE(SYS_OUT_OF_MEMORY_ERROR_CODE);
    SYS_DEBUGF(SYS_DBG_LEVEL_WARNING, ("BCDriver - alloc failed.\r\n"));
  }
  else {
    p_new_obj->vptr = &sBCDriver_vtbl;
  }

  return p_new_obj;
}

sys_error_code_t BCDriverRegisterTickCallback(BCDriver_t *_this, pTIM_CallbackTypeDef callbacl_f)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  if( HAL_OK != HAL_TIM_RegisterCallback(_this->mx_handle.p_mx_tim_cfg->p_tim, HAL_TIM_PERIOD_ELAPSED_CB_ID, callbacl_f))
  {
    res = SYS_INVALID_FUNC_CALL_ERROR_CODE;
    SYS_SET_LOW_LEVEL_ERROR_CODE(SYS_INVALID_FUNC_CALL_ERROR_CODE);
  }

  return res;
}

/* IDriver virtual functions definition */
/****************************************/

sys_error_code_t BCDriver_vtblInit(IDriver *_this, void *p_params)
{
  assert_param(_this != NULL);
  assert_param(p_params != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  BCDriver_t *p_obj = (BCDriver_t*)_this;
  BCDriverParams_t *p_init_params = (BCDriverParams_t*)p_params;
  p_obj->mx_handle.p_mx_tim_cfg = p_init_params->p_mx_tim_cfg;
  p_obj->mx_handle.p_mx_gpio_cfg = p_init_params->p_mx_gpio_cfg;

  /* Initialize the timer for the 1-wire communication  */
  p_obj->mx_handle.p_mx_tim_cfg->p_mx_init_f();

  /* Initialize the GPIO used for the 1-wire communication */
  p_obj->mx_handle.p_mx_gpio_cfg->p_mx_init_f();

  return res;
}

sys_error_code_t BCDriver_vtblStart(IDriver *_this)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  BCDriver_t *p_obj = (BCDriver_t*)_this;
  TIM_HandleTypeDef *p_tim = p_obj->mx_handle.p_mx_tim_cfg->p_tim;

  (void)HAL_TIM_Base_Start_IT(p_tim);

  return res;
}

sys_error_code_t BCDriver_vtblStop(IDriver *_this)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  BCDriver_t *p_obj = (BCDriver_t*)_this;
  TIM_HandleTypeDef *p_tim = p_obj->mx_handle.p_mx_tim_cfg->p_tim;

  (void)HAL_TIM_Base_Stop_IT(p_tim);

  return res;
}

sys_error_code_t BCDriver_vtblDoEnterPowerMode(IDriver *_this, const EPowerMode active_power_mode, const EPowerMode new_power_mode)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
/*  BCDriver_t *p_obj = (BCDriver_t*)_this; */

  return res;
}

sys_error_code_t BCDriver_vtblReset(IDriver *_this, void *p_params)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
/*  BCDriver_t *p_obj = (BCDriver_t*)_this; */

  return res;
}


/* Private function definition */
/*******************************/

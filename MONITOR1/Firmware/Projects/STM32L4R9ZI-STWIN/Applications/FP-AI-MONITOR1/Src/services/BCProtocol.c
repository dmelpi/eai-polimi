/**
 ******************************************************************************
 * @file    BCProtocol.c
 * @author  STMicroelectronics - AIS - MCD Team
 * @version 1.0.0
 * @date    Sep 6, 2021
 *
 * @brief Battery Charger protocol definition.
 *
 * This file implements the Battery Charger protocol for STBC02 device.
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

#include "services/BCProtocol.h"
#include "drivers/BCDriver.h"
#include "FreeRTOS.h"
#include "services/sysdebug.h"


#define SYS_DEBUGF(level, message)      SYS_DEBUGF3(SYS_DBG_BCP, level, message)


/**
 * This is a why to link a HAL TIM callback to the protocol object that is using the timer.
 */
typedef struct _TimCallbakcMapEntry_t
{
  TIM_TypeDef *tim_instance;
  BCProtocol_t *p_owner;
} TimCallbakcMapEntry_t;

static TimCallbakcMapEntry_t sTimCallbackMap[1] = {
    {NULL, NULL}
};

/* Private member function declaration */
/***************************************/

/**
 * TIM Callback function.
 *
 * @param htim [IN] specifies the hardware timer that has triggered the ISR.
 */
static void  BCP1WStateMachine(TIM_HandleTypeDef *htim);


/* Public API definition */
/*************************/

BCProtocol_t *BCPAlloc(void)
{
  BCProtocol_t *p_new_obj = (BCProtocol_t*)pvPortMalloc(sizeof(BCProtocol_t));

  if (p_new_obj == NULL) {
    SYS_SET_LOW_LEVEL_ERROR_CODE(SYS_OUT_OF_MEMORY_ERROR_CODE);
    SYS_DEBUGF(SYS_DBG_LEVEL_WARNING, ("BCProtocol - alloc failed.\r\n"));
  }


  return p_new_obj;
}

sys_error_code_t BCPInit(BCProtocol_t *_this, IDriver *p_bc_driver)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  if (p_bc_driver == NULL)
  {
    res = SYS_INVALID_PARAMETER_ERROR_CODE;
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_INVALID_PARAMETER_ERROR_CODE);
  }
  else
  {
    _this->p_driver = p_bc_driver;
    _this->state = E_BC_IDLE;
    _this->pulse = 0;
    _this->pulse_tcs = 0;
    _this->prev_state = E_BC_IDLE;
    _this->cmd_pulse_count = 0;

    BCDriverRegisterTickCallback((BCDriver_t*)_this->p_driver, BCP1WStateMachine);

    /* for the moment I assume there is only one instance of this protocol in the system*/
    if (sTimCallbackMap[0].p_owner == NULL)
    {
      sTimCallbackMap[0].p_owner = _this;
      sTimCallbackMap[0].tim_instance = ((BCDriver_t*)_this->p_driver)->mx_handle.p_mx_tim_cfg->p_tim->Instance;
    }

    SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("BCP: initialization done.\r\n"));
  }

  return res;
}

sys_error_code_t BCPSendCmd(BCProtocol_t *_this, ESTBC02_SW_CMD_t cmd)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  uint32_t tick = HAL_GetTick();
  bool cmd_executed = false;

  _this->cmd = cmd;
  _this->state = E_BC_START;

  /* start the time  base*/
  IDrvStart(_this->p_driver);

  while ((_this->state != E_BC_IDLE) && (!cmd_executed))
  {
    if (HAL_GetTick() - tick > 10U)
    {
      cmd_executed = true;
      res = SYS_BCP_CMD_EXECUTION_ERROR_CODE;
      SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_BCP_CMD_EXECUTION_ERROR_CODE);
    }
  }

  /* stop the time base*/
  IDrvStop(_this->p_driver);

  return res;
}

sys_error_code_t BCPPowerOff(BCProtocol_t *_this)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("BCP: power-off cmd.\r\n"));

  res = BCPSendCmd(_this, E_SHIPPING_MODE_ON);

  return res;
}


/* Private API definition */
/**************************/

static void  BCP1WStateMachine(TIM_HandleTypeDef *htim)
{
  BCProtocol_t *p_owner = NULL;
  BCDriver_t *p_driver = NULL;

  if (htim->Instance == sTimCallbackMap[0].tim_instance)
  {
    p_owner = sTimCallbackMap[0].p_owner;
    p_driver = (BCDriver_t*)p_owner->p_driver;

    p_owner->pulse++;
    switch (p_owner->state)
    {
    case E_BC_IDLE:
      break;
    case E_BC_START:
//      HAL_GPIO_TogglePin(STBC02_SW_SEL_GPIO_PORT, STBC02_SW_SEL_PIN);
      BCDriverToggleLine1W(p_driver);
      p_owner->pulse_tcs = p_owner->pulse + (uint16_t)(350/5);
      p_owner->prev_state = p_owner->state;
      p_owner->state = E_BC_WAIT;
      break;
    case E_BC_PULSE_L:
//      HAL_GPIO_TogglePin(STBC02_SW_SEL_GPIO_PORT, STBC02_SW_SEL_PIN);
      BCDriverToggleLine1W(p_driver);
      p_owner->pulse_tcs = p_owner->pulse + (uint16_t)(100/5);
      p_owner->prev_state = p_owner->state;
      p_owner->state = E_BC_WAIT;
      break;
    case E_BC_PULSE_H:
//      HAL_GPIO_TogglePin(STBC02_SW_SEL_GPIO_PORT, STBC02_SW_SEL_PIN);
      BCDriverToggleLine1W(p_driver);
      p_owner->pulse_tcs = p_owner->pulse + (uint16_t)(100/5);
      p_owner->prev_state = p_owner->state;
      p_owner->state = E_BC_WAIT;
      break;
    case E_BC_STOP_L:
//      HAL_GPIO_TogglePin(STBC02_SW_SEL_GPIO_PORT, STBC02_SW_SEL_PIN);
      BCDriverToggleLine1W(p_driver);
      p_owner->pulse_tcs = p_owner->pulse + (uint16_t)(100/5);
      p_owner->prev_state = p_owner->state;
      p_owner->state = E_BC_WAIT;
      break;
    case E_BC_STOP_H:
//      HAL_GPIO_TogglePin(STBC02_SW_SEL_GPIO_PORT, STBC02_SW_SEL_PIN);
      BCDriverToggleLine1W(p_driver);
      p_owner->pulse_tcs = p_owner->pulse + (uint16_t)(500/5);
      p_owner->prev_state = p_owner->state;
      p_owner->state = E_BC_WAIT;
      break;
    case E_BC_WAIT:
      if ( p_owner->pulse > p_owner->pulse_tcs )
      {
        if ( p_owner->prev_state == E_BC_STOP_H )
        {
          p_owner->cmd_pulse_count = 0;
          p_owner->state = E_BC_IDLE;
//          HAL_GPIO_WritePin(STBC02_SW_SEL_GPIO_PORT, STBC02_SW_SEL_PIN, GPIO_PIN_RESET);
          BCDriverResetLine1W(p_driver);
        }
        else
        {
          if ( p_owner->prev_state == E_BC_PULSE_H )
          {
            p_owner->cmd_pulse_count++;
            if ( p_owner->cmd_pulse_count < (uint8_t)p_owner->cmd )
            {
              p_owner->prev_state = E_BC_START;
            }
            else
              __NOP();
          }
          p_owner->state = (ESTBC02_SW_STATE_t)(p_owner->prev_state + 1U);
          __NOP();
        }
      }
      break;
    default:
      break;
    }
  }
}


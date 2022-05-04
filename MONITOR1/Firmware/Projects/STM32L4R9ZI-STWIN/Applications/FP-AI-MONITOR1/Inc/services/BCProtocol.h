/**
 ******************************************************************************
 * @file    BCProtocol.h
 * @author  STMicroelectronics - AIS - MCD Team
 * @version 1.0.0
 * @date    Sep 6, 2021
 *
 * @brief   Battery Charger protocol declaration
 *
 * This file declare the public API for the Battery Charger protocol for
 * STBC02 device. This protocol class uses a driver of type ::BCDriver_t.
 *
 * To use the protocol the application must:
 * - allocate a protocol object. The allocation can be static or dynamic
 *   using the BCPAlloc() method.
 * - initialize the protocol object to connect it with the driver using the
 *   BCPInit() method.
 * - Use the generic BCPSendCommand() to control the battery charger, or the
 *   the others available public API.
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
#ifndef INC_SERVICES_BCPROTOCOL_H_
#define INC_SERVICES_BCPROTOCOL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "drivers/IDriver.h"
#include "drivers/IDriver_vtbl.h"


/**
  * SW selection pulse number
  */
typedef enum _ESTBC02_SW_CMD_t
{
  E_SW1_OA_OFF              = 1,
  E_SW1_OA_ON               = 2,
  E_SW1_OB_OFF              = 3,
  E_SW1_OB_ON               = 4,
  E_SW2_OA_OFF              = 5,
  E_SW2_OA_ON               = 6,
  E_SW2_OB_OFF              = 7,
  E_SW2_OB_ON               = 8,
  E_BATMS_OFF               = 9,
  E_BATMS_ON                = 10,
  E_IEND_OFF                = 11,
  E_IEND_5_PC_IFAST         = 12,
  E_IEND_2_5_PC_IFAST       = 13,
  E_IBAT_OCP_900_mA         = 14,
  E_IBAT_OCP_450_mA         = 15,
  E_IBAT_OCP_250_mA         = 16,
  E_IBAT_OCP_100_mA         = 17,
  E_VFLOAT_ADJ_OFF          = 18,
  E_VFLOAT_ADJ_PLUS_50_mV   = 19,
  E_VFLOAT_ADJ_PLUS_100_mV  = 20,
  E_VFLOAT_ADJ_PLUS_150_mV  = 21,
  E_VFLOAT_ADJ_PLUS_200_mV  = 22,
  E_SHIPPING_MODE_ON        = 23,
  E_AUTORECH_OFF            = 24,
  E_AUTORECH_ON             = 25,
  E_WATCHDOG_OFF            = 26,
  E_WATCHDOG_ON             = 27,
  E_IFAST_IPRE_50_PC_OFF    = 28,
  E_IFAST_IPRE_50_PC_ON     = 29
} ESTBC02_SW_CMD_t;

/**
 * Specifies the 1-wire pulse state.
 */
typedef enum _ESTBC02_SW_STATE_t
{
  E_BC_IDLE,   /**< IDLE */
  E_BC_START,  /**< START */
  E_BC_PULSE_L,/**< PULSE_L */
  E_BC_PULSE_H,/**< PULSE_H */
  E_BC_STOP_L, /**< STOP_L */
  E_BC_STOP_H, /**< STOP_H */
  E_BC_WAIT    /**< wait */
} ESTBC02_SW_STATE_t;

/**
 * Create  type name for struct _BCProtocol_t.
 */
typedef struct _BCProtocol_t BCProtocol_t;

/**
 * ::BCProtocol_t internal structure.
 */
struct _BCProtocol_t
{
  /**
   * Specifies the driver object used by the protocol to communicate with the component STBC02.
   * It must be an object of type ::BCDriver_t;
   */
  IDriver *p_driver;

  /**
   * Specifies the actual state of the 1-wire channel state machine.
   */
  ESTBC02_SW_STATE_t state;

  /**
   * Specifies the previous state of the 1-wire channel state machine.
   */
  ESTBC02_SW_STATE_t prev_state;

  /**
   * Specifies the current command.
   */
  ESTBC02_SW_CMD_t cmd;

  /**
   * Actual timer pulse number.
   */
  uint16_t pulse;

  /**
   * Timer pulse number to change state.
   */
  uint16_t pulse_tcs;

  /**
   * Command pulse number.
   */
  uint8_t cmd_pulse_count;
};

/* Public API declaration */
/**************************/

/**
 * Allocate an instance of ::BCProtocol_t. The protocol object is allocated
 * in the FreeRTOS heap.
 *
 * @return a pointer to the new object ::BCProtocol_t if success, or NULL if out of memory error occurs.
 */
BCProtocol_t *BCPAlloc(void);

/**
 * Initialize the protocol object. It must be called after the object allocation and before using it.
 *
 * @param _this [IN] specifies a pointer to the protocol object.
 * @param p_bc_driver [IN] specifies a ::BCDriver_t object.
 * @return SYS_NO_ERROR_CODE if success, an error code otherwise.
 */
sys_error_code_t BCPInit(BCProtocol_t *_this, IDriver *p_bc_driver);

/**
 * Send a command over the 1-wire channel to STBC02
 *
 * @param _this [IN] specifies a pointer to the protocol object.
 * @param cmd [IN] specifies a command. Supported commands are:
 *              -
 * @return SYS_NO_ERROR_CODE if success, SYS_BC_CMD_NOT_SUPPORTED otherwise
 */
sys_error_code_t BCPSendCmd(BCProtocol_t *_this, ESTBC02_SW_CMD_t cmd);

/**
 * Sends the command E_SHIPPING_MODE_ON to power off the battery charger.
 *
 * @param _this [IN] specifies a pointer to the protocol object.
 * @return SYS_NO_ERROR_CODE if success, an error code otherwise.
 */
sys_error_code_t BCPPowerOff(BCProtocol_t *_this);


#ifdef __cplusplus
}
#endif

#endif /* INC_SERVICES_BCPROTOCOL_H_ */

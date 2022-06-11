/**
 ******************************************************************************
 * @file    UtilTask.h
 * @author  SRA
 * @version 1.0.0
 * @date    6-Sep-2021
 *
 * @brief  Utility task declaration.
 *
 * The Utility task is in export minor functionalities of the application:
 * - Time stamp for the Sensor Manager
 * - Manage teh user button and the user LED1
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
#ifndef INC_UTILTASK_H_
#define INC_UTILTASK_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "services/AManagedTaskEx.h"
#include "services/AManagedTaskEx_vtbl.h"
#include "drivers/SMUtilityDriver.h"
#include "drivers/SMUtilityDriver_vtbl.h"
#include "tx_api.h"

#define UTIL_CMD_ID_START_LP_TIMER                    ((uint16_t)0x0001)              ///< START Low Power timer command ID.
#define UTIL_CMD_ID_STOP_LP_TIMER                     ((uint16_t)0x0002)              ///< STOP Low Power timer command ID.
#define UTIL_CMD_ID_RESET_LP_TIMER                    ((uint16_t)0x0003)              ///< RESET Low Power timer command ID.

/**
 * Create  type name for _UtilTask_t.
 */
typedef struct _UtilTask_t UtilTask_t;

/**
 *  UtilTask_t internal structure.
 */
struct _UtilTask_t
{
  /**
   * Base class object.
   */
  AManagedTaskEx super;

  /* Task variables should be added here. */

  /**
   * Specifies the driver used by the task. It is an instance of ::SMUtilityDriver
   */
  IDriver *p_driver;

  /**
   * HAL driver configuration parameters.
   */
  const void *p_mx_drv_cfg;

  /**
   * Input queue used by other task to request services.
   */
  TX_QUEUE in_queue;

  /**
   * Software timer used to generate a transaction into low-power mode.
   */
  TX_TIMER auto_low_power_timer;
};

/* Public API declaration */
/**************************/

/**
 * Allocate an instance of UtilTask.
 *
 * @param p_mx_drv_cfg [IN] specifies a ::MX_TIMParams_t instance declared in the mx.h file.
 * @return a pointer to the generic object ::AManagedTaskEx if success,
 * or NULL if out of memory error occurs.
 */
AManagedTaskEx* UtilTaskAlloc(const void *p_mx_drv_cfg);

/* Inline functions definition */
/*******************************/

#ifdef __cplusplus
}
#endif

#endif /* INC_UTILTASK_H_ */

/**
 ******************************************************************************
 * @file    UtilTask.h
 * @author  STMicroelectronics - AIS - MCD Team
 * @version $Version$
 * @date    $Date$
 *
 * @brief  Utility task declaration.
 *
 * The Utility task is in export minor functionalities of the application:
 * - Time stamp for the Sensor Manager
 * - Manage teh user button and the user LED1
 *
 *********************************************************************************
 * @attention
 *
 * <h2><center>COPYRIGHT &copy; 2021 STMicroelectronics</center></h2>
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file in
 * the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *********************************************************************************
 */
#ifndef INC_UTILTASK_H_
#define INC_UTILTASK_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "services/AManagedTaskEx.h"
#include "services/AManagedTaskEx_vtbl.h"
#include "events/SensorListener.h"
#include "events/SensorListener_vtbl.h"

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
struct _UtilTask_t {
  /**
   * Base class object.
   */
  AManagedTaskEx super;

  /* Task variables should be added here. */

  /**
   * HAL driver configuration parameters.
   */
  const void *p_mx_sm_drv_cfg;

  /**
   * HAL driver configuration parameters. This instance is used to
   * drive the LD2 LED.
   */
  const void *p_mx_ld_drv_cfg;

  /**
   * Input queue used by other task to request services.
   */
  TX_QUEUE in_queue;

  /**
   * Software timer used to generate a transaction into low-power mode.
   */
  TX_TIMER auto_low_power_timer;

  /**
   * Specifies a timeout in ms. If the timeout is zero then the timer is disabled.
   */
  uint32_t timeout_ms;

  /**
   * The sensor listener object used to receive and print the sensor data in the debug log.
   */
  SensorListener_t *p_sensor_listener;
};


/* Public API declaration */
/**************************/

/**
 * Allocate an instance of UtilTask.
 *
 * @param p_mx_sm_drv_cfg [IN] specifies a ::MX_TIMParams_t instance declared in the mx.h file.
 * @param p_mx_ld_drv_cfg [IN] specifies a ::MX_GPIOParams_t instance declared in the mx.h file. a GPIO connected to a LED. It can be NULL.
 * @return a pointer to the generic object ::AManagedTaskEx if success,
 * or NULL if out of memory error occurs.
 */
AManagedTaskEx *UtilTaskAlloc(const void *p_mx_sm_drv_cfg, const void *p_mx_sm_ld_cfg);

/**
 * Set the period of the automatic low power mode (software) timer.
 * If the period is greater than zero then the timer is enabled. If the period is set to zero then
 * the timer is disabled. The timer is started when the system enter in STATE_1 PM. When the timer
 * elapse then the system goes in SLEEP_1.
 *
 * @param _this [IN] specifies a pointer to the task object.
 * @param timeout_ms [IN] specifies a timeout in ms. If the timeout is zero then the timer is disabled.
 * @return
 */
sys_error_code_t UtilTaskSetAutoLowPowerModePeriod(UtilTask_t *_this, uint32_t timeout_ms);

/**
 * Set the sensor listener object used to receive and print the sensor data in the debug log.
 *
 * @param _this [IN] specifies a pointer to the task object.
 * @param p_sensor_listener [IN] specifies a sensor listener object.
 * @return SYS_NO_ERROR_CODE.
 */
sys_error_code_t UtilTaskSetSensorListener(UtilTask_t *_this, SensorListener_t *p_sensor_listener);


/* Inline functions definition */
/*******************************/


#ifdef __cplusplus
}
#endif

#endif /* INC_UTILTASK_H_ */

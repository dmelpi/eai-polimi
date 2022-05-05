/**
 ******************************************************************************
 * @file    UtilTask.h
 * @author  STMicroelectronics - AIS - MCD Team
 * @version V1.0.0
 * @date    15-September-2021
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
 * Copyright (c) 2021 STMicroelectronics
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
#include "drivers/SMUtilityDriver.h"
#include "drivers/SMUtilityDriver_vtbl.h"
#include "drivers/BCDriver.h"
#include "drivers/BCDriver_vtbl.h"
#include "services/BCProtocol.h"
#include "queue.h"
#include "timers.h"

#define UTIL_CMD_ID_START_LP_TIMER                    ((uint16_t)0x0001)              ///< START Low Power timer command ID.
#define UTIL_CMD_ID_STOP_LP_TIMER                     ((uint16_t)0x0002)              ///< STOP Low Power timer command ID.
#define UTIL_CMD_ID_RESET_LP_TIMER                    ((uint16_t)0x0003)              ///< RESET Low Power timer command ID.
#define UTIL_CMD_ID_BUTTON_EVT                        ((uint16_t)0x0004)              ///< Button event. It can be BUTTON_DOWN or BUTTON_UP
#define UTIL_CMD_ID_UB_TIMER_ELAPSED                  ((uint16_t)0x0005)              ///< UB timer is elapsed.
#define UTIL_CMD_ID_LED2_SET_ENABLE                   ((uint16_t)0x0006)              ///< LED2 set enable command. Valid value are: 1(enable), 0(disable)
#define UTIL_CMD_ID_LED2_SET_BLK_PERIOD               ((uint16_t)0x0007)              ///< LED2 set the blinking period. Valid value are: 0(always off), 0xFF(always on), n(multiple of 60ms)

#define UTIL_PARAM_BUTTON_EVT_DOWN                    ((uint16_t)0x0001)              ///< Button event parameter: BUTTON_DOWN
#define UTIL_PARAM_BUTTON_EVT_UP                      ((uint16_t)0x0002)              ///< Button event parameter: BUTTON_UP
#define UTIL_PARAM_BUTTON_EVT_POWER_PRESSED           ((uint16_t)0x0003)              ///< Button event parameter: POWER_BUTTON pressed.

#define UTIL_SHORT_PRESS_PERIOD_THRESHOLD_T           200
#define UTIL_DOUBLE_PRESS_PERIOD_MS                   300

/**
 * Create  type name for _UtilTask_t.
 */
typedef struct _UtilTask_t UtilTask_t;

/**
 *  ::UtilTask_t internal structure.
 */
struct _UtilTask_t {
  /**
   * Base class object.
   */
  AManagedTaskEx super;

  /* Task variables should be added here. */

  /**
   * Specifies the driver used by the task. It is an instance of ::SMUtilityDriver_t.
   */
  IDriver *p_sm_driver;

  /**
   * Specifies the battery charger driver used by the task. It is an instance of ::BCDrivet_t.
   */
  IDriver *p_bc_driver;

  /**
   * HAL driver configuration parameters for the ::SMUtilityDriver_t.
   */
  const void *p_mx_sm_drv_cfg;

  /**
   * HAL TIM driver configuration parameters for the ::BCDrivet_t.
   */
  const void *p_mx_bc_tim_drv_cfg;

  /**
   * HAL GPIO driver configuration parameters for the ::BCDrivet_t.
   */
  const void *p_mx_bc_gpio_drv_cfg;

  /**
   * Battery Charger Protocol object.
   */
  BCProtocol_t bc_protocol;

  /**
   * Input queue used by other task to request services.
   */
  QueueHandle_t in_queue;

  /**
   * Input queue of the AppController.
   */
  QueueHandle_t ctrl_queue;

  /**
   * Software timer used to generate a transaction into low-power mode.
   */
  TimerHandle_t auto_low_power_timer;

  /**
   * specifies a timeout in ms. If the timeout is zero then the timer is disabled.
   */
  uint32_t timeout_ms;

  /**
   * Software timer used to synthesize the button events:
   *  - SHORT_PRESS
   *  - LONG_PRESS
   *  - DOUBLE_PRESS
   */
  TimerHandle_t pb_timer;

  /**
   * Specifies the time stamp of teh button down event. It is used to compute the short or log button pressed event.
   */
  uint32_t button_down_ts_tick;

  /**
   * count the number of button pressed event in a predefined time interval. It is used to generate the double press sys event.
   */
  uint8_t button_pressed_count;

  /**
   * Software timer used to blink the LED.
   */
  TimerHandle_t led_timer;

  /**
   * Specified the frequency of the LED.
   */
  TickType_t led_blinking_period;

  /**
   * The first two bits specify the active LED:
   * - 00: LED2 not active, LED1 not active
   * - 01: LED2 not active, LED1     active
   * - 10: LED2     active, LED1 not active
   * - 11: LED2     active, LED1     active
   */
  uint8_t led_mask;

  /**
   * Count the number of time LED_2 blinks.
   * It is used with the macro UTIL_LED_2_BLINKING_TIME to notify an error to the user,
   * when NanoEdge AI is the active AI engine.
   */
  uint8_t led_2_neai_blinking_count;

  /**
   * Count the tick based on the led_timer software timer.
   * It is used with the led_2_ai_blinking_period member to notify the HAR class using the orange LED,
   * when X-CUBE-AI is the active AI engine.
   */
  uint8_t led_2_ai_blinking_count;

  /**
   * Specifies the period for the orange LED.
   * It is used with the led_2_ai_blinking_count member to notify the HAR class using the orange LED,
   * when X-CUBE-AI is the active AI engine.
   */
  uint8_t led_2_ai_blinking_period;

  /**
   * Specifies the number of POWER BUTTON pressed event to skip.
   * When the system start I need to skip the first IRQ that is the consequence
   * off the power-on event that is linked to the same button.
   */
  uint8_t pb_evt_skip_count;
};


/* Public API declaration */
/**************************/

/**
 * Allocate an instance of UtilTask.
 *
 * @param p_mx_sm_drv_cfg [IN] specifies a ::MX_TIMParams_t instance declared in the mx.h file.
 *        This is the hardware timer used by the ::SMUtilityDriver_t to generate the timestamp.
 * @param p_mx_bc_tim_drv_cfg [IN] specifies a ::MX_TIMParams_t instance declared in the mx.h file.
 *        This is the hardware timer used by the ::BCDriver_t to implement the 1-wire communication with STBC02.
 * @param p_mx_bc_gpio_drv_cfg [IN] specifies a ::MX_GPIOParams_t instance declared in the mx.h file.
 *        This is the GPIO used by the ::BCDriver_t to implement the 1-wire communication with STBC02.
 * @return a pointer to the generic object ::AManagedTaskEx if success, or NULL if out of memory error occurs.
 */
AManagedTaskEx *UtilTaskAlloc(const void *p_mx_sm_drv_cfg, const void *p_mx_bc_tim_drv_cfg, const void *p_mx_bc_gpio_drv_cfg);

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
 * Register the Application Controller processes input queue with the application ::UtilTask_t.
 * It is used to propagate the push button event in order to start and stop the AI processing.
 *
 * @param _this [IN] specifies a pointer to the task object.
 * @param ctrl_queue [IN] specifies the ::AppController_t input queue.
 * @return SYS_NOERROR_CODE
 */
inline sys_error_code_t UtilTaskSetCtrlInQueue(UtilTask_t *_this, QueueHandle_t ctrl_queue);

/**
 * Get the task input queue. The application can use the services exported by the task by sending message
 * of type struct UtilMessage_t to this queue.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return an handle to the task input message queue.
 */
inline QueueHandle_t UtilTaskGetInQueue(UtilTask_t *_this);

/**
 * Set the status of LED_2 to enable or disable. When LED_2 is enabled it blink with LED_1.
 * LED_2 is used only in SENSOR_ACTIVE state, otherwise it is off.
 *
 * @param _this [IN] specifies a pointer to the task object.
 * @param enable [IN] true to start LED_2 blinking, false to turn off LED_2
 * @return SYS_NO_ERROR_CODE
 */
inline sys_error_code_t UtilTaskSetLED2Enable(UtilTask_t *_this, bool enable);


/* Inline functions definition */
/*******************************/

SYS_DEFINE_INLINE
sys_error_code_t UtilTaskSetCtrlInQueue(UtilTask_t *_this, QueueHandle_t ctrl_queue)
{
  assert_param(_this != NULL);

  _this->ctrl_queue = ctrl_queue;

  return SYS_NO_ERROR_CODE;
}

SYS_DEFINE_INLINE
sys_error_code_t UtilTaskSetLED2Enable(UtilTask_t *_this, bool enable)
{
  assert_param(_this != NULL);

  if (enable)
  {
    _this->led_mask |= 0x2;
  }
  else
  {
    _this->led_mask &= (~0x2);
  }

  return SYS_NO_ERROR_CODE;
}

SYS_DEFINE_INLINE
QueueHandle_t UtilTaskGetInQueue(UtilTask_t *_this)
{
  assert_param(_this != NULL);
  return _this->in_queue;
}

#ifdef __cplusplus
}
#endif

#endif /* INC_UTILTASK_H_ */

/**
 ******************************************************************************
 * @file    applowpower.h
 * @author  STMicroelectronics - AIS - MCD Team
 * @version $Version$
 * @date    $Date$
 *
 * @brief   Declare the Power Mode State Machine events, state, and status.
 *
 * This file declares:
 * - the \b events that define the transactions in the Power Mode State Machine.
 * - the \b states of the Power Mode State Machine
 * - the \b status of the Power Mode state Machine. It is a bit of information
 *   linked to the PM State Machine available system wide using the API
 *   SysGetPowerStatus().
 *
 * All these three element together define the Power Mode State Machine. This
 * definition is application dependent and it is separate by the API to operate
 * the PM State Machine. The API are defined by the Application Power Mode
 * Helper object instance (::IAppPowerModeHelper > AppPowerModeHelper).
 *
 * The Power Mode State Machine for this application is displayed in
 * \ref fig41 "Fig.41".
 *
 * \anchor fig41 \image html 41_fp_ai_monitor1_app_power_management.png
 * "Fig.41 - App Power Mode State Machine" width=900px
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
#ifndef APPLOWPOWER_H_
#define APPLOWPOWER_H_

#ifdef __cplusplus
extern "C" {
#endif


#define SYS_PM_EVT_SRC_PB                   0x01U  ///< Event generated by the PushButton.
#define SYS_PM_EVT_SRC_LP_TIMER             0x02U  ///< Event generated by the Low Power timer.
#define SYS_PM_EVT_SRC_DTDL                 0x03U ///< Event generated by the DTDL Commands
/* PB parameters */
#define SYS_PM_EVT_PARAM_SHORT_PRESS        0x01U  ///< Event parameter: short press (< NNN ms).
#define SYS_PM_EVT_PARAM_LONG_PRESS         0x02U  ///< Event parameter: long press (>= NNN ms).
/* CTRL parameters */
#define SYS_PM_EVT_DTDL_START               0x01U  ///< Event parameter: DTDL start command.
#define SYS_PM_EVT_DTDL_STOP                0x02U  ///< Event parameter: DTDL stop command.
///* CTRL parameters */
//#define SYS_PM_EVENT_PARAM_START_ML         0x01U  ///< Event parameter: start the X-CUBE-AI processing.
//#define SYS_PM_EVENT_PARAM_START_NEAI       0x02U  ///< Event parameter: start the NanoEdge AI processing.
//#define SYS_PM_EVENT_PARAM_STOP_PROCESSING  0x03U  ///< Event parameter: stop processing.
// Generic event parameters
#define SYS_PM_EVT_PARAM_ENTER_LP           0x01U  ///< Event parameter: enter low power mode command.
#define SYS_PM_EVT_PARAM_EXIT_LP            0x02U  ///< Event parameter: exit low power mode command.

/**
 * Specifies the possible power states of the system.
 */
typedef enum {
  /**
   * The system is active waiting for input.
   */
  E_POWER_MODE_STATE1 = 0,         ///< E_POWER_MODE_STATE1

  /**
   * Specifies the lowest power mode for the system: the MCU is in STOP.
   */
  E_POWER_MODE_SLEEP_1 = 1,        ///< E_POWER_MODE_SLEEP_1

  /**
   * The system is active and the enabled sensors produce data.
   */
  E_POWER_MODE_SENSORS_ACTIVE = 2, ///< E_POWER_MODE_SENSORS_ACTIVE

  /**
   * Special value. This is not a real state of the PM state machine, instead it is used by the framework
   * to identify the numbers of state in the PM state machine.
   */
  E_POWER_MODE_NONE = 3           ///< E_POWER_MODE_NONE

}EPowerMode;

/**
 * Specifies some information about the current power mode of the system.
 */
typedef struct _SysPowerStatus {

  /**
   * Specifies the current power mode of the system. An application task can read this value.
   */
  EPowerMode active_power_mode;

}SysPowerStatus;


// Inline functions definition
// ***************************


#ifdef __cplusplus
}
#endif

#endif /* APPLOWPOWER_H_ */

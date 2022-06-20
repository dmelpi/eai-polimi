/**
 ******************************************************************************
 * @file    sysdebug.h
 * @author  STMicroelectronics - ST-Korea - MCD Team
 * @version 3.0.0
 * @date    Oct 10, 2016
 * @brief   Debug Log.
 *
 * This file implement a set of macro in order to provide a simple way to
 * create a log when the application is compiled in DEBUG.
 *
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2016 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file in
 * the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 ******************************************************************************
 */

#ifndef SYSDEBUG_H_
#define SYSDEBUG_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 *
 * The lower three bits of the debug control byte specifies debug level according to the following convention:
 * - 0 all
 * - 1 verbose: a general message for system developers.
 * - 2 low level api: a message for system developers generated by a Low Level API function.
 * - 3 service layer: a message for system developers generated by a Service Layer function.
 * - 4 default: a message generated by application code.
 * - 5 warning: a message generated when a minor error occurs. The system is probably able to auto-recovers.
 * - 6 severe: a message generated when a critical error occurs. The system probably hangs.
 */

#define SYS_DBG_LEVEL_ALL      0x00U ///< All debug message are sent to the debug channel.
#define SYS_DBG_LEVEL_VERBOSE  0x01U ///< A general message for system developers.
#define SYS_DBG_LEVEL_LLA      0x02U ///< A message for system developers generated by a Low Level API function.
#define SYS_DBG_LEVEL_SL       0x03U ///< A message for system developers generated by a Service Layer function.
#define SYS_DBG_LEVEL_DEFAULT  0x04U ///< A message generated by application code.
#define SYS_DBG_LEVEL_WARNING  0x05U ///< A message generated when a minor error occurs. The system is probably able to auto-recovers.
#define SYS_DBG_LEVEL_SEVERE   0x06U ///< A message generated when a critical error occurs. The system probably hangs.
#define SYS_DBG_MASK_LEVEL     0x07U

#define SYS_DBG_ON             0x80U ///< Flag for SYS_DEBUGF control byte to enable the debug message for that module.
#define SYS_DBG_OFF            0x00U ///< Flag for SYS_DEBUGF control byte to disable the debug message for that module.
#define SYS_DBG_HALT           0x08U ///< Flag for SYS_DEBUGF control byte to halt after printing a debug message for that module.


  /* Debug Configuration options. */
/********************************/

#include <stdint.h>
#include "sysdebug_config.h"


/* Callback type used to call printf function */
typedef int (* DebugPrintfFn) (const char *format, ...);
extern DebugPrintfFn xSysDebugPrintfFn;
extern uint8_t g_sys_dbg_min_level;


#ifdef SYS_DEBUG

typedef void (*xDebugLockUnlockFnType)(void);
extern xDebugLockUnlockFnType xSysDebugLockFn;
extern xDebugLockUnlockFnType xSysDebugUnlockFn;

/**
 * Used inside an API function in order to set the debug level of control byte.
 *
 * <b>Example:</b>
 * \code
 * void module1Func() {
 *   if (error) {
 *     SYS_DEBUGF(SYS_DBG_SET_LEVEL(SYS_DBG_MODULE1, SYS_DBG_LEVEL_WARNING) , ("modulo 1 warning"));
 *   }
 */
#define SYS_DBG_SET_LEVEL(control_byte, level) (((control_byte) & (~SYS_DBG_MASK_LEVEL)) | (level))

/**
 * Print debug message only if debug message type is enabled...
 * AND is at least SYS_DBG_MIN_LEVEL.
 * If the SYS_DBG_HALT bit is specified in the debug control byte the system enter an infinity loop.
 */
#define _SYS_DEBUGF(debug, message) do { \
  if ( \
      ((debug) & SYS_DBG_ON) && \
      ((int8_t)((debug) & SYS_DBG_MASK_LEVEL) >= g_sys_dbg_min_level)) { \
    SYS_PLATFORM_DIAG(message); \
    if ((debug) & SYS_DBG_HALT) { \
      while(1); \
    } \
  } \
} while(0)

#define SYS_PLATFORM_DIAG(_x_) do { xSysDebugLockFn();xSysDebugPrintfFn _x_;xSysDebugUnlockFn(); } while (0)

//#define SYS_PLATFORM_ASSERT(x) do { assert(x); } while (0)

/**
 * Send a message to the debug channel. This macro should be used by system developers.
 * It takes three parameters:
 * - module: specifies the caller module.
 * - level: specifies the debug level.
 * - message: a text message in printf format.
 */
#define SYS_DEBUGF3(module, level, message) _SYS_DEBUGF(SYS_DBG_SET_LEVEL(module, level), message)

/**
 * Send a message to the debug channel. This macro should be used by application developers.
 * This message has a default debug level of SYS_DBG_LEVEL_DEFAULT
 * It takes two parameters:
 * - module: specifies the caller module.
 * - message: a text message in printf format.
 */
#define SYS_DEBUGF2(level, message) _SYS_DEBUGF(SYS_DBG_SET_LEVEL(SYS_DBG_APP, level), message)

#else  /* SYS_DEBUG */
#define SYS_DEBUGF3(debug, level, message)
#define SYS_DEBUGF2(level, message)
#endif /* SYS_DEBUG */

/**
 * Initialize the hardware of the debug communication port.
 *
 * @return 0 if success, an error code otherwise.
 */
int SysDebugHardwareInit(void);

/**
 * Initialize the debug support.
 *
 * @return 0 if success, an error code otherwise.
 */
int SysDebugInit(void);

/**
 * Enable the IRQ for the timer used as timebase in order to compute
 * the FreeRTOS statistics about the MCU usage on per task basis.
 */
void SysDebugStartRunTimeStatsTimer(void);

/**
 * Toggle a PIN used as test point.
 *
 * @param nLed [IN] specifies the PIN number (stating from 0).
 */
void SysDebugToggleLed(uint8_t nLed);

/**
 * Set high a PIN used as test point.
 *
 * @param nLed [IN] specifies the PIN number (stating from 0).
 */
void SysDebugLedOn(uint8_t nLed);

/**
 * Set low a PIN used as test point.
 *
 * @param nLed  [IN] specifies the PIN number (stating from 0).
 */
void SysDebugLedOff(uint8_t nLed);

int putChar( int ch );



#ifdef __cplusplus
}
#endif


#endif /* SYSDEBUG_H_ */

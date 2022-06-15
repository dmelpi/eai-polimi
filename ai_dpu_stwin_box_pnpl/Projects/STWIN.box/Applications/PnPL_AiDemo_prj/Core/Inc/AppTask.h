/**
 ******************************************************************************
 * @file    AppTask.h
 * @author  STMicroelectronics
 * @version 1.0.0
 * @date    Jun 14, 2022
 *
 * @brief Application Controller Task
 *
 *
 ******************************************************************************
 * @attention
 *
 * <h2><center>COPYRIGHT &copy; 2022 STMicroelectronics</center></h2>
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file in
 * the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
#ifndef APPTASK_H_
#define APPTASK_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "services/systp.h"
#include "services/syserror.h"
#include "services/AManagedTask.h"
#include "services/AManagedTask_vtbl.h"
#include "events/IProcessEventListener.h"
#include "events/IProcessEventListener_vtbl.h"
#include "drivers/IDriver.h"
#include "drivers/IDriver_vtbl.h"
#include "IUsbDelegate.h"
#include "IUsbDelegate_vtbl.h"
#include "IAiApplication.h"
#include "IAiApplication_vtbl.h"
#include "PnPLCompManager.h"

/**
 * Create  type name for _AppTask.
 */
typedef struct _AppTask AppTask;


#ifndef USB_CDC_RX_BUFFER_SIZE
#define USB_CDC_RX_BUFFER_SIZE       (2000)
#endif


/**
 *  AppTask internal structure.
 */
struct _AppTask {
  /**
   * Base class object.
   */
  AManagedTask super;

  // Task variables should be added here.
  /**
   * The driver used by the task. The actual type is ::PushButtonDriver_t.
   */
  IDriver *p_driver;

  /**
   * HAL driver configuration parameters.
   */
  const void *p_mx_drv_cfg;

  /**
   * Listener interface to observe the processing data coming from a DPU.
   */
  IProcessEventListener dpu_listener_if;

  /**
   * Index of first empty byte of rx_buffer.
   */
  uint32_t rx_buffer_idx;

  /**
   *  Buffer for received USB data.
   */
  uint8_t rx_buffer[USB_CDC_RX_BUFFER_SIZE];

  /**
   * Delegate interface to receive data coming from USB.
   */
  IUsbDelegate usb_delegate;

  PnPLCommand_t outPnPLCommand;

  /**
   * Demo Control interface (Start/Stop)
   */
  IAiApplication_t ai_application_controller;
};


// Public API declaration
//***********************

/**
 * Allocate an instance of AppTask.
 *
 * @return a pointer to the generic obejct ::AManagedTask if success,
 * or NULL if out of memory error occurs.
 */
AManagedTask *AppTaskAlloc(const void *p_mx_drv_cfg);

/**
 * Get a pointer to the driver used by the task.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return a pointer to the driver used by the task.
 */
inline IDriver *AppTaskGetDriver(AppTask *_this);

/**
 * Get a pointer to the ::IProcessEventListener IF of the task.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return a pointer to the ::IProcessEventListener IF of the task.
 */
static inline IProcessEventListener* AppTaskGetProcEvtListenerIF(AppTask *_this);

/**
 * Get a pointer to the ::IUsbDelegate IF of the task.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return a pointer to the ::IUsbDelegate IF of the task.
 */
static inline IUsbDelegate* AppTaskGetUsbDelegateIF(AppTask *_this);

/**
 * Get a pointer to the ::IAiApplication IF of the task.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return a pointer to the ::IAiApplication IF of the task.
 */
static inline IAiApplication_t* AppTaskGetIAiApplicationIF(AppTask *_this);

// Inline functions definition
// ***************************

inline IDriver *AppTaskGetDriver(AppTask *_this)
{
  assert_param(_this);

  return _this->p_driver;
}

static inline IProcessEventListener* AppTaskGetProcEvtListenerIF(AppTask *_this)
{
  assert_param(_this != NULL);

  return &_this->dpu_listener_if;
}

static inline IUsbDelegate* AppTaskGetUsbDelegateIF(AppTask *_this)
{
  assert_param(_this != NULL);

  return &_this->usb_delegate;
}

static inline IAiApplication_t* AppTaskGetIAiApplicationIF(AppTask *_this)
{
  assert_param(_this != NULL);

  return &_this->ai_application_controller;
}

#ifdef __cplusplus
}
#endif

#endif /* APPTASK_H_ */

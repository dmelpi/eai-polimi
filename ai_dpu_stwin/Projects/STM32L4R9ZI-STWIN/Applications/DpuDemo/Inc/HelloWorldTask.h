/**
 ******************************************************************************
 * @file    HelloWorldTask.h
 * @author  STMicroelectronics - AIS - MCD Team
 * @version V1.0.0
 * @date    15-September-2021
 *
 * @brief Blink the user LED of the NUCLEO board.
 *
 * This managed task is responsible to blink the user LED with a fixed period
 * of 1 second and a duty cycle of 50%.
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

#ifndef HELLOWORLDTASK_H_
#define HELLOWORLDTASK_H_

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

/**
 * Create  type name for _HelloWorldTask.
 */
typedef struct _HelloWorldTask HelloWorldTask;

/**
 *  HelloWorldTask internal structure.
 */
struct _HelloWorldTask {
  /**
   * Base class object.
   */
  AManagedTask super;

  // Task variables should be added here.

  /**
   * The driver used by the task. The actual type is ::PushButtonDriver_t.
   */
  IDriver *m_pxDriver;

  /**
   * HAL driver configuration parameters.
   */
  const void *p_mx_drv_cfg;

  /**
   * Listener interface to observe the processing data coming from a DPU.
   */
  IProcessEventListener m_xDpuListenerIF;
};


// Public API declaration
//***********************

/**
 * Allocate an instance of HelloWorldTask.
 *
 * @param p_mx_drv_cfg [IN] specifies a ::MX_GPIOParams_t instance declared in the mx.h file.
 *        It must be a GPIO connected to the user button and configured in EXTI mode.
 *        If it is NULL then the task ignore the button and no PM transaction will happen in the demo.
 * @return a pointer to the generic object ::AManagedTask if success,
 * or NULL if out of memory error occurs.
 */
AManagedTask *HelloWorldTaskAlloc(const void *p_mx_drv_cfg);

/**
 * Get a pointer to the driver used by the task.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return a pointer to the driver used by the task.
 */
inline IDriver *HelloWorldTaskGetDriver(HelloWorldTask *_this);

/**
 * Geta pointer to the ::IProcessEventListener IF of the task.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return a pointer to the ::IProcessEventListener IF of the task.
 */
static inline IProcessEventListener* HelloWorldTaskGetProcEvtListenerIF(HelloWorldTask *_this);


// Inline functions definition
// ***************************

SYS_DEFINE_INLINE
IDriver *HelloWorldTaskGetDriver(HelloWorldTask *_this) {
  assert_param(_this != NULL);

  return _this->m_pxDriver;
}

SYS_DEFINE_STATIC_INLINE
IProcessEventListener* HelloWorldTaskGetProcEvtListenerIF(HelloWorldTask *_this) {
  assert_param(_this != NULL);

  return &_this->m_xDpuListenerIF;
}

#ifdef __cplusplus
}
#endif

#endif /* HELLOWORLDTASK_H_ */

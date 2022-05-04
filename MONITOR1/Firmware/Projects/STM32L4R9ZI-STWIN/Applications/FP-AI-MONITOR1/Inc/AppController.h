/**
 ******************************************************************************
 * @file    AppController.h
 * @author  STMicroelectronics - AIS - MCD Team
 * @version V1.0.0
 * @date    15-September-2021
 *
 * @brief
 *
 * <DESCRIPTIOM>
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
#ifndef INC_APPCONTROLLER_H_
#define INC_APPCONTROLLER_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "services/systp.h"
#include "services/syserror.h"
#include "services/AManagedTaskEx.h"
#include "services/AManagedTaskEx_vtbl.h"
#include "events/IProcessEventListener.h"
#include "events/IProcessEventListener_vtbl.h"
#include "queue.h"
#include "timers.h"
#include "usbd_def.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_cdc.h"
#include "usbd_cdc_if.h"

#include "SensorManager.h"

#define CTRL_TASK_CFG_MAX_IN_LENGTH        50
#define CTRL_TASK_CFG_MAX_OUT_LENGTH       512

/**
 * Create  type name for struct _ACProcessEventListener_t
 */
typedef struct _ACProcessEventListener_t ACProcessEventListener_t;

struct _ACProcessEventListener_t
{
  IProcessEventListener super;

  void* p_owner;
};

/**
 * Create  type name for struct _AppController_t.
 */
typedef struct _AppController_t AppController_t;

/**
 *  AppController_t internal structure.
 */
struct _AppController_t {
  /**
   * Base class object.
   */
  AManagedTaskEx super;

  /* Task variables should be added here. */

  /**
   * Task input message queue. The task receives message of type struct CtrlMessage_t in this queue.
   * This is one of the way the task expose its services at application level.
   */
  QueueHandle_t in_queue;

  /**
   * Input buffer to process the commands from the console.
   */
  char in_string[CTRL_TASK_CFG_MAX_IN_LENGTH];

  /**
   * Output buffer to process assembly the command answer to sent to the console.
   */
  char out_string[CTRL_TASK_CFG_MAX_OUT_LENGTH];

  /**
   * index of the insertion point in the in_buffer.
   */
  uint16_t in_index;

  /**
   * Specifies the threshold to classify a signal as an anomaly based on the similarity percentage
   * returned by the NanoEdge AI detect algorithm.
   */
  uint8_t detect_threshold;

  /**
   * Specifies the number of signals to evaluate in the next phase.
   */
  uint32_t signals;

  /**
   * Used to count the evaluated signals during a detection or learning phase.
   */
  uint32_t signal_count;

  /**
   * Specifies the duration in ms of the next phase.
   */
  TickType_t timer_period_ms;

  /**
   * Sensor interface. It is set by the application during the startup.
   */
  ISourceObservable *p_ism330dhcx_acc;

  /**
   * Sensor interface. It is set by the application during the startup.
   */
  ISourceObservable *p_ism330dhcx_gyr;

  /**
   * Sensor interface. It is set by the application during the startup.
   */
  ISourceObservable *p_is3dwb_acc;

  /**
   * Input queue of the AITask. It is used by the controller to operate the task.
   */
  QueueHandle_t ai_in_queue;

  /**
   * Input queue of the AITask. It is used by the controller to operate the task.
   */
  QueueHandle_t neai_in_queue;

  /**
   * Input queue of the UtilTask. It is used by the controller to operate LED2.
   */
  QueueHandle_t util_in_queue;

  /**
   * Listener IF to listen the process events coming from the DPUs.
   */
  ACProcessEventListener_t listenetIF;

  /**
   * Specifies a software timer used to stop the current detection or evaluation phase.
   */
  TimerHandle_t stop_timer;

};


/* Public API declaration */
/**************************/

/**
 * Allocate an instance of AppController_t.
 *
 * @return a pointer to the generic object ::AManagedTaskEx if success,
 * or NULL if out of memory error occurs.
 */
AManagedTaskEx *AppControllerAlloc(void);

/**
 * Get the task input queue. The application can use the services exported by the task by sending message
 * of type struct CtrlMessage_t to this queue.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return an handle to the task input message queue.
 */
inline QueueHandle_t AppControllerGetInQueue(AppController_t *_this);

/**
 * Register the sensor interface with the application controller. They are used by the controller to operate the sensors.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @param p_ism330dhcx_acc [IN] specifies a sensor interface. The accelerometer of the combo sensor.
 * @param p_ism330dhcx_gyr [IN] specifies a sensor interface. The gyroscope of the combo sensor.
 * @param p_is3dwb [IN] specifies a sensor interface. The vibrometer.
 * @return SYS_NO_ERROR_CODE if success, an error code otherwise.
 */
sys_error_code_t AppControllerSetSensorsIF(AppController_t *_this, ISourceObservable *p_ism330dhcx_acc, ISourceObservable *p_ism330dhcx_gyr, ISourceObservable *p_is3dwb);

/**
 * Register the AI processes input queue with the application controller. They are used by the controller to operate the AI process.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @param ai_queue [IN] specifies the AITask input queue.
 * @param neai_queue [IN] specifies the NeaiTask input queue.
 * @param util_queue [IN] specifies the UtilTask input queue.
 * @return SYS_NO_ERROR_CODE if success, an error code otherwise.
 */
sys_error_code_t AppControllerSetAIProcessesInQueue(AppController_t *_this, QueueHandle_t ai_queue, QueueHandle_t neai_queue, QueueHandle_t util_queue);


/* Inline functions definition */
/*******************************/

SYS_DEFINE_INLINE
QueueHandle_t AppControllerGetInQueue(AppController_t *_this)
{
  assert_param(_this != NULL);
  return _this->in_queue;
}

#ifdef __cplusplus
}
#endif

#endif /* INC_APPCONTROLLER_H_ */

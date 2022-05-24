/**
 ******************************************************************************
 * @file    AppController.h
 * @author  STMicroelectronics - AIS - MCD Team
 * @version $Version$
 * @date    $Date$
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
#include "services/CircularBuffer.h"
#include "events/ISensorEventListener.h"
#include "events/ISensorEventListener_vtbl.h"
#include "queue.h"
#include "timers.h"
#include "usbd_def.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_cdc.h"
#include "usbd_cdc_if.h"

#include "SensorManager.h"

#define CTRL_TASK_CFG_MAX_IN_LENGTH        128
#define CTRL_TASK_CFG_MAX_OUT_LENGTH       512

/**
 * Create  type name for struct _CBHandle_t.
 */
typedef struct _CBHandle_t CBHandle_t;

/**
 * Describe the struct used to manage the Circular Buffer.
 */
struct _CBHandle_t
{
	/**
	 * Ponter to the CircularBuffer object.
	 */
  CircularBuffer *p_circular_buffer;

  /**
   * Point at the CBItem used by the data producer (a sensor).
   */
  CBItem *p_producer_data_buff;

  /**
   * Point at the CBItem used by the data consumer (teh AppController).
   */
  CBItem *pConsumerDataBuff;

  /**
   *
   */
  uint32_t data_idx;
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
   * Specifies the duration in ms of the next phase.
   */
  TickType_t timer_period_ms;

  /**
   * Sensor interface. This is or the sensor selected during the start of an execution phase,
   * or NULL.
   */
  ISourceObservable *p_selected_sensor;

  /**
   * Specifies the listener interface to receive the sensor event.
   */
  ISensorEventListener sensor_listener;

  /**
   * CircularBuffer handle used to decouple the sensor producing data from teh AppController
   * that consume the data.
   */
  CBHandle_t cb;

  /**
   * Input queue of the UtilTask. It is used by the controller to operate LED2.
   */
//  QueueHandle_t util_in_queue;

  /**
   * Specifies a software timer used to stop the streamer phase.
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

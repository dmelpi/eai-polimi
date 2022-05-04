/**
 ******************************************************************************
 * @file    NeaiTask.h
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
#ifndef SRC_NEAITASK_H_
#define SRC_NEAITASK_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "services/AManagedTaskEx.h"
#include "services/AManagedTaskEx_vtbl.h"
#include "NeaiDPU.h"
#include "NeaiDPU_vtbl.h"
#include "events/IProcessEventListener.h"
#include "events/IProcessEventListener_vtbl.h"
#include "queue.h"


/**
 * Create  type name for _NeaiTask_t.
 */
typedef struct _NeaiTask_t NeaiTask_t;

/**
 *  NeaiTask_t internal structure.
 */
struct _NeaiTask_t {
  /**
   * Base class object.
   */
  AManagedTaskEx super;

  /**
   * Task input message queue. The task receives message of type struct NeaiMessage_t in this queue.
   * This is one of the way the task expose its services at application level.
   */
  QueueHandle_t in_queue;

  /* Task variables should be added here. */

  /**
   * Digital processing Unit specialized for the NanoEdgeAI library.
   */
  NeaiDPU_t dpu;

  /**
   * Data buffer used by the DPU but allocated by the task. The size of the buffer depend
   * on many parameters like:
   * - the type of the data used as input by the DPU
   * - the length of the signal
   * - the number of signals to manage in a circular way in order to decouple the data producer from the data process.
   * The correct size in byte is computed by the DPU with the method NeaiDPUSetStreamsParam().
   */
  void *p_dpu_buff;
};


/* Public API declaration */
/**************************/

/**
 * Allocate an instance of NeaiTask_t.
 *
 * @return a pointer to the generic object ::AManagedTaskEx if success,
 * or NULL if out of memory error occurs.
 */
AManagedTaskEx *NeaiTaskAlloc(void);

/**
 * Connect a sensor to the task as data source. This is a simplified version of the NeaiTaskAttachToSensorEx() that use
 * the configuration information from the neai library for the following parameters:
 * - signal size
 * - number of axes
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @param p_sensor [IN] specifies a pointer to the sensor to be connected.
 * @param cb_items [IN] specifies the maximum number of signals stored in the task.
 * @return SYS_NO_ERROR_CODE if success, and error code otherwise.
 */
sys_error_code_t NeaiTaskAttachToSensor(NeaiTask_t *_this, ISourceObservable *p_sensor, uint8_t cb_items);

/**
 * Connect a sensor to the task as data source. Data are collected to form a signal of n axes and signal_size number of sample per axis,
 * and stored in a circular buffer of cb_items signals.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @param p_sensor [IN] specifies a pointer to the sensor to be connected.
 * @param signal_size [IN] specifies the size of the signal in number of sample.
 * @param axes [IN] specifies the number of axes of the signal. For example an accelerometer generate up to 3-axes data (x, y, z).
 * @param cb_items [IN] specifies the maximum number of signals stored in the task.
 * @return SYS_NO_ERROR_CODE if success, and error code otherwise.
 */
sys_error_code_t NeaiTaskAttachToSensorEx(NeaiTask_t *_this, ISourceObservable *p_sensor, uint16_t signal_size, uint8_t axes, uint8_t cb_items);

/**
 * Add a process listener to the NeaiDPU owned by the task.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @param p_listener [IN] specifies the ::IProcessListener object to add.
 * @return SYS_NO_ERROR_CODE if success, an error code otherwise.
 */
sys_error_code_t NeaiTaskAddDPUListener(NeaiTask_t *_this, IProcessEventListener *p_listener);

/**
 * Remove a process listener to the NeaiDPU owned by the task.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @param p_listener [IN] specifies the ::IProcessListener object to remove.
 * @return SYS_NO_ERROR_CODE if success, an error code otherwise.
 */
sys_error_code_t NeaiTaskRemoveDPUListener(NeaiTask_t *_this, IProcessEventListener *p_listener);

/**
 * Get the task input queue. The application can use the services exported by the task by sending message
 * of type struct NeaiMessage_t to this queue.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return an handle to the task input message queue.
 */
inline QueueHandle_t NeaiTaskGetInQueue(NeaiTask_t *_this);

/**
 * Get the DPU interface (::IDPU) of the the DPU used by the task.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return an handle to the task DPU interface.
 */
inline IDPU *NeaiTaskGetDPU(NeaiTask_t *_this);

/**
 * Sets the sensitivity of the model in detection mode in the NanoEdge AI library.
 * This command is asynchronous: it sends a message in the input queue of the task.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @param sensitivity [IN] specifies a new value for the sensitivity.
 *        A sensitivity value between [0, 1[ (1 excluded) decreases the sensitivity of the model,
 *        while a value in between [1, 100] increases it.
 *        We recommend increasing or decreasing sensitivity by steps of 0.1.
 * @return SYS_NO_ERROR_CODE if success, an error code otherwise.
 */
sys_error_code_t NeaiTaskSetSensitivity(NeaiTask_t *_this, float sensitivity);

/**
 * Check if the application has been linked with NanonoEdge library or with the standard stub.
 *
 * @return true if the application has been linked with the standard stub, false otherwise.
 */
bool NeaiTaskIsStubLibrary(void);

/* Inline functions definition */
/*******************************/

SYS_DEFINE_INLINE
QueueHandle_t NeaiTaskGetInQueue(NeaiTask_t *_this)
{
  assert_param(_this != NULL);
  return _this->in_queue;
}

SYS_DEFINE_INLINE
IDPU *NeaiTaskGetDPU(NeaiTask_t *_this)
{
  assert_param(_this != NULL);
  return (IDPU *) &_this->dpu;
}

#ifdef __cplusplus
}
#endif

#endif /* SRC_NEAITASK_H_ */

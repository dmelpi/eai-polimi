/**
 ******************************************************************************
 * @file    ProcesTask.h
 * @author  STMicroelectronics - AIS - MCD Team
 * @version M.m.b
 * @date    Mar 23, 2022
 *
 * @brief
 *
 *
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file in
 * the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 ******************************************************************************
 */
#ifndef PROCESTASK_H_
#define PROCESTASK_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "services/AManagedTaskEx.h"
#include "services/AManagedTaskEx_vtbl.h"
#include "Dummy_DPU.h"
#include "Dummy_DPU_vtbl.h"
#include "events/IProcessEventListener.h"
#include "events/IProcessEventListener_vtbl.h"
#include "queue.h"

/**
 * Create  type name for _ProcesTask_t.
 */
typedef struct _ProcesTask_t ProcesTask_t;

/**
 *  ProcessTask_t internal structure.
 */
struct _ProcesTask_t {
  /**
   * Base class object.
   */
  AManagedTaskEx super;

  /* Task variables should be added here. */

  /**
   * Task input message queue. The task receives message of type struct AIMessage_t in this queue.
   * This is one of the way the task expose its services at application level.
   */
  QueueHandle_t in_queue;

  /**
   * Digital processing Unit specialized for nothing.
   */
  Dummy_DPU *p_dpu;

  /**
   * Data buffer used by the DPU but allocated by the task. The size of the buffer depend
   * on many parameters like:
   * - the type of the data used as input by the DPU
   * - the length of the signal
   * - the number of signals to manage in a circular way in order to decouple the data producer from the data process.
   * The correct size in byte is computed by the DPU with the method Dummy_DPUSetInputParam().
   */
  void *p_dpu_buff;
};


/* Public API declaration */
/**************************/

/**
 * Allocate an instance of ProcessTask_t.
 *
 * @return a pointer to the generic object ::AManagedTaskEx if success,
 * or NULL if out of memory error occurs.
 */
AManagedTaskEx *ProcesTaskAlloc(void);

/**
 * Get the task input queue. The application can use the services exported by the task by sending message
 * of type struct ProcMessage_t to this queue. The command ID for all supported command are defined in
 * ProcMessagesDef.h
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return an handle to the task input message queue.
 */
static inline QueueHandle_t ProcesTaskGetInQueue(ProcesTask_t *_this);

/**
 * Connect a sensor to the task as data source.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @param p_sensor [IN] specifies a pointer to the sensor to be connected.
 * @param cb_items [IN] specifies the maximum number of signals stored in the task.
 * @return SYS_NO_ERROR_CODE if success, and error code otherwise.
 */
sys_error_code_t ProcesTaskAttachToSensor(ProcesTask_t *_this, ISourceObservable *p_sensor, uint8_t cb_items);

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
sys_error_code_t ProcesTaskAttachToSensorEx(ProcesTask_t *_this, ISourceObservable *p_sensor, uint16_t signal_size, uint8_t axes, uint8_t cb_items);

/**
 * Add a process listener to the AiDPU owned by the task.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @param p_listener [IN] specifies the ::IProcessListener object to add.
 * @return SYS_NO_ERROR_CODE if success, an error code otherwise.
 */
sys_error_code_t ProcesTaskAddDPUListener(ProcesTask_t *_this, IProcessEventListener *p_listener);

/**
 * Remove a process listener to the AiDPU owned by the task.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @param p_listener [IN] specifies the ::IProcessListener object to remove.
 * @return SYS_NO_ERROR_CODE if success, an error code otherwise.
 */
sys_error_code_t ProcesTaskRemoveDPUListener(ProcesTask_t *_this, IProcessEventListener *p_listener);


/* Inline functions definition */
/*******************************/

SYS_DEFINE_STATIC_INLINE
QueueHandle_t ProcesTaskGetInQueue(ProcesTask_t *_this)
{
  assert_param(_this != NULL);
  return _this->in_queue;
}


#ifdef __cplusplus
}
#endif

#endif /* PROCESTASK_H_ */

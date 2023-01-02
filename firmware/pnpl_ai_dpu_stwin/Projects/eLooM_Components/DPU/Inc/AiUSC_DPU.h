/**
 ******************************************************************************
 * @file    AiUSC_DPU.h
 * @author  STMicroelectronics - AIS - MCD Team
 * @version 1.0.0
 * @date    May 28, 2021
 *
 * @brief   Digital processing Unit specialized for the Cube.AI library
 *
 * This DPU process the data using Ai library generated by Cube.AI.
 *
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under ODE SOFTWARE LICENSE AGREEMENT
 * SLA0094, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0094
 *
 ******************************************************************************
 */
#ifndef DPU_INC_AI_USC_DPU_H_
#define DPU_INC_AI_USC_DPU_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "ADPU.h"
#include "ADPU_vtbl.h"
#include "aiApp.h"
#include "fft.h"
#include "feature_extraction.h"

/**
 * Specifies the execution mode for the DPU. The execution mode tells the DPU what to do when it is running
 * and new signals provided by the data source is ready to be processed by the AI library.
 */
typedef enum _EAiUSC_Mode {
  E_AI_USC_MODE_NONE = 0,
  E_AI_USC_DETECTION     
} EAiUSC_Mode_t;

#define AI_USC_DPU_NAME             "usc_network"

/**
 * Create  type name for _AiUSC_DPU_t.
 */
typedef struct _AiUSC_DPU_t AiUSC_DPU_t;

/**
 * AiUSC_DPU_t internal state.
 * It declares only the virtual table used to implement the inheritance.
 */
struct _AiUSC_DPU_t {
  /**
   * Base class object.
   */
  ADPU super;

  /**
   * Specifies if the in and out stream of teh DPU have been initialized.
   * To initialize the streams the application must use
   */
  boolean_t stream_ready;

  /**
   * Specifies the output data from Ai library. The output of the inference is:
   * ai_out[0] = index that specifies the HAR class
   * ai_out[1] = probability that the recognized activity belong to a given class.
   */
  float ai_out[2];

  /**
   * Specifies AI processing function to use in order to process the signals.
   */
  int (*ai_processing_f)(const char*, float *, float[2]);

};


/* Public API declaration */
/**************************/

/**
 * Allocate an instance of AiUSC_DPU_t in the heap.
 *
 * @return a pointer to the generic object ::IDPU if success,
 * or NULL if out of memory error occurs.
 */
IDPU *AiUSC_DPUAlloc(void);

/**
 * Allocate an instance of AiUSC_DPU_t in a memory block specified by the application.
 * The size of the memory block must be greater or equal to sizeof(AiUSC_DPU_t).
 * This allocator allows the application to avoid the dynamic allocation.
 *
 * \code
 * AiUSC_DPU_t dpu;
 * AiUSC_DPUStaticAlloc(&dpu);
 * \endcode
 *
 * @param p_mem_block [IN] specify a memory block allocated by the application.
 *        The size of the memory block must be greater or equal to sizeof(AiUSC_DPU_t).
 * @return a pointer to the generic object ::IDPU if success,
 * or NULL if out of memory error occurs.
 */
IDPU *AiUSC_DPUStaticAlloc(void *p_mem_block);

/**
 * Compute the size in byte of the buffer that the application need to allocate and pass
 * to the DPU when it is attached to a sensor or to another DPU.
 *
 * \anchor fig400 \image html 400_api_DPUSetInputParams.png "Fig.400 - API - AiUSC_DPUSetInputParam() " width=630px
 *
 * @param _this [IN] specifies a pointer to the object.
 * @param signal_size [IN] specifies the number of input elements that the DPU must collect before starting the processing phase.
 * @param axes [IN] specifies the number of axes (or dimension) of each element.
 * @param cb_items [IN] specifies the number of items for the ::CircularBuffer used by the DPU.
 *                      An item of the ::CirculaBuffer is used to store `size` elements from the input source.
 * @return The size in byte of the buffer need by the DPU to acquire and process input data if success, zero otherwise.
 */
uint16_t AiUSC_DPUSetStreamsParam(AiUSC_DPU_t *_this, uint16_t signal_size, uint8_t axes, uint8_t cb_items);

/**
 * Set the processing mode for the DPU. It specifies to the DPU if a new signal is used
 * to learn and improve the model, or to detect anomalies.
 *
 * @param _this [IN] specifies a pointer to the object.
 * @param mode [IN] specifies the processing mode. Valid value are:
 *  - E_AI_DETECTION
 * @return SYS_NO_ERROR_CODE if success, an error code otherwise.
 */
sys_error_code_t AiUSC_DPUSetProcessingMode(AiUSC_DPU_t *_this, EAiUSC_Mode_t mode);

/**
 * Get the actual processing mode for the DPU.
 *
 * @param _this [IN] specifies a pointer to the object.
 * @return the actual processing mode of the DPU.
 */
inline EAiUSC_Mode_t AiUSC_DPUGetProcessingMode(AiUSC_DPU_t *_this);

/**
 * Partial reset of the DPU internal state: all input and output buffers are re-initialized to prepare
 * the DPU to process a new stream of data.
 *
 * @param _this [IN] specifies a pointer to the object.
 * @return SYS_NO_ERROR_CODE if success, an error code otherwise.
 */
sys_error_code_t AiUSC_DPUPrepareToProcessData(AiUSC_DPU_t *_this);


/* Inline functions definition */
/*******************************/

SYS_DEFINE_INLINE
EAiUSC_Mode_t AiUSC_DPUGetProcessingMode(AiUSC_DPU_t *_this)
{
  assert_param(_this != NULL);
  EAiUSC_Mode_t res = E_AI_USC_MODE_NONE;

  return res;
}

#ifdef __cplusplus
}
#endif

#endif /* DPU_INC_AiUSC_DPU_H_ */
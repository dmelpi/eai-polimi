/**
 ******************************************************************************
 * @file    MfccDPU.h
 * @author  STMicroelectronics - AIS - MCD Team
 * @version 1.0.0
 * @date    May 28, 2021
 *
 * @brief
 *
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
#ifndef DPU_INC_MFCCDPU_H_
#define DPU_INC_MFCCDPU_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "ADPU.h"
#include "ADPU_vtbl.h"
#include "mfccDPU_tables.h"
#include "feature_extraction.h"

#define MFCCDPU_NB_AXIS          (1)
//#define MFCCDPU_NB_SAMPLE        (24)
//#define MFCCDPU_AI_PROC_IN_SIZE  (AI_NETWORK_IN_1_SIZE)
#define MFCCDPU_SAMPLE_RATE      (192000U)  /* Input signal sampling rate */
#define MFCCDPU_FFT_LEN          (4096U)     /* Number of FFT points. Must be greater or equal to FRAME_LEN */
#define MFCCDPU_FRAME_LEN        (MFCCDPU_FFT_LEN) /* Window length and then padded with zeros to match FFT_LEN */
#define MFCCDPU_NUM_MEL          (32U)       /* Number of mel bands */
#define MFCCDPU_NUM_MEL_COEFS    (3772U)     /* Number of mel filter weights. Returned by MelFilterbank_Init */
#define MFCCDPU_NUM_MEL_COL      (46U)       /* Number of MEL columns to return */
#define MFCCDPU_NUM_MFCC         (32U)     /* Number of MFCCs to return */
#define MFCCDPU_NUM_MFCC_COL     (46U)     /* Number of MFCC columns to return */


/**
 * Create  type name for _MfccDPU_t.
 */
typedef struct _MfccDPU_t MfccDPU_t;

/**
 * MfccDPU_t internal state.
 * It declares only the virtual table used to implement the inheritance.
 */
struct _MfccDPU_t {
  /**
   * Base class object.
   */
  ADPU super;

  /**
   * Specifies if the in and out stream of the DPU have been initialized.
   * To initialize the streams the application must use
   */
  boolean_t stream_ready;

  /**
   *
   */
  float mfcc_out[MFCCDPU_NUM_MFCC*MFCCDPU_NUM_MFCC_COL];

  arm_rfft_fast_instance_f32 S_Rfft;
  MelFilterTypeDef           S_MelFilter;
  SpectrogramTypeDef         S_Spectr;
  MelSpectrogramTypeDef      S_MelSpectr;
  LogMelSpectrogramTypeDef   S_LogMelSpectr;
  DCT_InstanceTypeDef        S_DCT;
  MfccTypeDef                S_Mfcc;

  float32_t pSpectrScratchBuffer[MFCCDPU_FFT_LEN];
  float32_t pDCTCoefsBuffer[MFCCDPU_NUM_MEL * MFCCDPU_NUM_MFCC];
  float32_t pMfccScratchBuffer[MFCCDPU_NUM_MEL];

  int32_t mfccColIdx;
};


/* Public API declaration */
/**************************/

/**
 * Allocate an instance of AiDPU_t in the heap.
 *
 * @return a pointer to the generic object ::IDPU if success,
 * or NULL if out of memory error occurs.
 */
IDPU *MfccDPUAlloc(void);

/**
 * Allocate an instance of MfccDPU_t in a memory block specified by the application.
 * The size of the memory block must be greater or equal to sizeof(MfccDPU_t).
 * This allocator allows the application to avoid the dynamic allocation.
 *
 * \code
 * MfccDPU_t dpu;
 * MfccDPUStaticAlloc(&dpu);
 * \endcode
 *
 * @param p_mem_block [IN] specify a memory block allocated by the application.
 *        The size of the memory block must be greater or equal to sizeof(MfccDPU_t).
 * @return a pointer to the generic object ::IDPU if success,
 * or NULL if out of memory error occurs.
 */
IDPU *MfccDPUStaticAlloc(void *p_mem_block);

/**
 * Compute the size in byte of the buffer that the application need to allocate and pass
 * to the DPU when it is attached to a sensor or to another DPU.
 *
 * \anchor fig400 \image html 400_api_DPUSetInputParams.png "Fig.400 - API - MfccDPUSetInputParam() " width=630px
 *
 * @param _this [IN] specifies a pointer to the object.
 * @param signal_size [IN] specifies the number of input elements that the DPU must collect before starting the processing phase.
 * @param cb_items [IN] specifies the number of items for the ::CircularBuffer used by the DPU.
 *                      An item of the ::CirculaBuffer is used to store `size` elements from the input source.
 * @return The size in byte of the buffer need by the DPU to acquire and process input data if success, zero otherwise.
 */
uint32_t MfccDPUSetStreamsParam(MfccDPU_t *_this, uint16_t signal_size,uint8_t cb_items);

/**
 * Partial reset of the DPU internal state: all input and output buffers are re-initialized to prepare
 * the DPU to process a new stream of data.
 *
 * @param _this [IN] specifies a pointer to the object.
 * @return SYS_NO_ERROR_CODE if success, an error code otherwise.
 */
sys_error_code_t MfccDPUPrepareToProcessData(MfccDPU_t *_this);


/* Inline functions definition */
/*******************************/

#ifdef __cplusplus
}
#endif

#endif /* DPU_INC_MFCCDPU_H_ */

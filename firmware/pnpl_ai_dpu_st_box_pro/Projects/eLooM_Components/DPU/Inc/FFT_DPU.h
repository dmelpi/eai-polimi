/**
  ******************************************************************************
  * @file    FFT_DPU.h
  * @author  SRA - MCD
  * @brief   
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
 
#ifndef FFT_DPU_H_
#define FFT_DPU_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "ADPU.h"
#include "ADPU_vtbl.h"


  /**
   * Create  type name for _FFT_DPUTask.
   */
  typedef struct _FFT_DPU_t FFT_DPU_t;

  // Public API declaration
  //***********************

  /**
   * Allocate an instance of FFT_DPUTask.
   *
   * @return a pointer to the interface IDPU
   * or NULL if out of memory error occurs.
   */
  IDPU *FFT_DPUAlloc(void);

  uint32_t FFT_DPUSetInputParam(FFT_DPU_t *_this, uint16_t fft_size, uint8_t nAxis, uint8_t nAverage, uint8_t cb_items);


#ifdef __cplusplus
}
#endif

#endif /* FFT_DPU_H_ */

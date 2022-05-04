/**
 ******************************************************************************
 * @file    FFT_DPU.h
 * @author  SRA - MCD
 * @version 1.0.0
 * @date    6-Sep-2021
 *
 * @brief   
 *
 * <DESCRIPTIOM>
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

/**
  ******************************************************************************
  * @file    MotionTD_DPU.h
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
  
#ifndef MOTIONTD_DPU_H_
#define MOTIONTD_DPU_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "ADPU.h"
#include "ADPU_vtbl.h"

#include "MotionTimeDomain.h"



  /**
   * Create  type name for _MotionTD_DPUTask.
   */
  typedef struct _MotionTD_DPU_t MotionTD_DPU_t;

  // Public API declaration
  //***********************

  /**
   * Allocate an instance of MotionTD_DPUTask.
   *
   * @return a pointer to the generic obejct ::AManagedTaskEx if success,
   * or NULL if out of memory error occurs.
   */
  IDPU *MotionTD_DPUAlloc(void);

  uint32_t MotionTD_DPUSetInputParam(MotionTD_DPU_t *_this, float odr, float sensitivity, MotionTD_AnalysisType_t analysis_type, uint16_t window_length, uint8_t n_axis, uint8_t cb_items);


#ifdef __cplusplus
}
#endif

#endif /* USER_INC_MotionTD_DPUTask_H_ */

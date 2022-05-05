/**
 ******************************************************************************
 * @file    MotionTD_DPU.h
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

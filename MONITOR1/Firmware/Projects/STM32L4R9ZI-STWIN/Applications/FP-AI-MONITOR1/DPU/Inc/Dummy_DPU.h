/**
 ******************************************************************************
 * @file    Dummy_DPU.h
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
#ifndef DUMMY_DPU_H_
#define DUMMY_DPU_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "services/systp.h"
#include "services/syserror.h"


#include "events/ProcessEventSrc.h"
#include "events/ProcessEventSrc_vtbl.h"
#include "events/ProcessEvent.h"

#include "IDPU.h"
#include "IDPUVtbl.h"
#include "ADPU.h"
#include "ADPU_vtbl.h"




  /**
   * Create  type name for _Dummy_DPUTask.
   */
  typedef struct _Dummy_DPU Dummy_DPU;

  // Public API declaration
  //***********************

  /**
   * Allocate an instance of Dummy_DPUTask.
   *
   * @return a pointer to the generic obejct ::AManagedTaskEx if success,
   * or NULL if out of memory error occurs.
   */
  IDPU *Dummy_DPUAlloc(void);

  uint32_t Dummy_DPUSetInputParam(Dummy_DPU *_this, uint16_t size, uint8_t nAxis, uint8_t cb_items);

  // Inline functions definition
  // ***************************

#ifdef __cplusplus
}
#endif

#endif /* USER_INC_Dummy_DPUTask_H_ */

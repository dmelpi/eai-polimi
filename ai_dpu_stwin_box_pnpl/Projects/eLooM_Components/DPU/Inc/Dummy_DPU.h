/**
  ******************************************************************************
  * @file    Dummy_DPU.h
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

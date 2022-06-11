/**
 ******************************************************************************
 * @file    MfccDPU_vtbl.h
 * @author  STMicroelectronics - AIS - MCD Team
 * @version 1.0.0
 * @date    May 28, 2021
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
#ifndef DPU_INC_MFCCDPU_VTBL_H_
#define DPU_INC_MFFCDPU_VTBL_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "services/systp.h"
#include "services/systypes.h"
#include "services/syserror.h"


/* IDPU virtual functions */
  sys_error_code_t MfccDPU_vtblProcess(IDPU *_this); /*!< @sa IDPU_Process */
  sys_error_code_t MfccDPU_vtblInit(IDPU *_this); /*!< @sa IDPU_Init */



#ifdef __cplusplus
}
#endif

#endif /* DPU_INC_AIDPU_VTBL_H_ */

/**
 ******************************************************************************
 * @file    Dummy_DPUTask_vtbl.h
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
#ifndef USER_INC_DUMMY_DPU_VTBL_H_
#define USER_INC_DUMMY_DPU_VTBL_H_

#ifdef __cplusplus
extern "C" {
#endif




// IDPU virtual functions
  sys_error_code_t Dummy_Process_vtbl(IDPU *_this);
  sys_error_code_t Dummy_DPUInit(IDPU *_this);


#ifdef __cplusplus
}
#endif

#endif /* USER_INC_Dummy_DPUTask_VTBL_H_ */

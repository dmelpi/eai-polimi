/**
 ******************************************************************************
 * @file    SMUtilTask.c
 * @author  SRA - MCD
 * @version 1.0.0
 * @date    6-Sep-2021
 *
 * @brief   Implementation of the SMUTIL task of the Sensor Manager module.
 *
 * This file define the SMUtilTask class.
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

#include "SMUtilTask.h"
#include "services/sysdebug.h"

#define SYS_DEBUGF(level, message)             SYS_DEBUGF3(SYS_DBG_SMUTIL, level, message)

/* Private member function declaration */
/***************************************/

/* Inline function forward declaration */
/***************************************/

#if defined (__GNUC__)
/* Inline function defined inline in the header file UtilTask.h must be declared here as extern function. */
#endif

/* Public API definition */
/*************************/

uint32_t SMUtilGetTimeStamp(void)
{
  uint32_t timestamp = 0;
  SMUtilityDriver_t *p_drv = GetSMUtilityDriver();
  if(p_drv != NULL)
  {
    timestamp = SMUtilityDrvGetTimeStamp(p_drv);
  }

  return timestamp;
}

__weak SMUtilityDriver_t* GetSMUtilityDriver(void)
{
  return NULL;
}

/* Private function definition */
/*******************************/


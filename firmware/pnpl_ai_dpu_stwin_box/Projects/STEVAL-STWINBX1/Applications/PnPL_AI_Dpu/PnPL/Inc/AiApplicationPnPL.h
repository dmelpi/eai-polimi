/**
  ******************************************************************************
  * @file    AiApplicationPnPL.h
  * @author  SRA
  * @version v1.0.0
  * @date    09-Jun-2022
  * @brief   AI Application PnPL Component Manager
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _PNPL_AI_APPLICATION_H_
#define _PNPL_AI_APPLICATION_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include "parson.h"
#include "IPnPLComponent.h"
#include "IPnPLComponent_vtbl.h"

#include "IAiApplication.h"
#include "IAiApplication_vtbl.h"

/**
  * Create a type name for _AiApplicationPnPL.
 */
typedef struct _AiApplicationPnPL AiApplicationPnPL;


// Public API declaration
//***********************

IPnPLComponent_t *AiApplicationPnPLAlloc(void);

/**
  * Initialize the default parameters.
  *
 */
uint8_t AiApplicationPnPLInit(IPnPLComponent_t *_this, IAiApplication_t *inf);

#ifdef __cplusplus
}
#endif

#endif /* _PNPL_AI_APPLICATION_H_ */

/******************* (C) COPYRIGHT 2016 STMicroelectronics *****END OF FILE****/

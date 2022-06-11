/**
  ******************************************************************************
  * @file    LogControllerPnPL.h
  * @author  SRA
  * @version v1.0.0
  * @date    27-Oct-2021
  * @brief   LogController PnP Component Manager
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
#ifndef _PNPL_LOG_CONTROLLER_H_
#define _PNPL_LOG_CONTROLLER_H_

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/

#include "parson.h"
#include "IPnPLComponent.h"
#include "IPnPLComponent_vtbl.h"

#include "ILogController.h"
#include "ILogController_vtbl.h"

 /**
  * Create a type name for _LogControllerPnPL.
  */
 typedef struct _LogControllerPnPL LogControllerPnPL;


 // Public API declaration
 //***********************

 IPnPLComponent_t *LogControllerPnPLAlloc(void);

 /**
  * Initialize the default parameters.
  *
  * TODO add comment
  */
 uint8_t LogControllerPnPLInit(IPnPLComponent_t *_this, ILogController_t *inf);

#ifdef __cplusplus
}
#endif

#endif /* _PNPL_LOG_CONTROLLER_H_ */

/******************* (C) COPYRIGHT 2016 STMicroelectronics *****END OF FILE****/

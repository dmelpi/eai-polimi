/**
  ******************************************************************************
  * @file    DeviceInfoPnPL.h
  * @author  SRA
  * @version v1.0.0
  * @date    27-Oct-2021
  * @brief   DeviceInfo PnP Component Manager
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
#ifndef _PNPL_DEVICE_INFO__H_
#define _PNPL_DEVICE_INFO__H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include "parson.h"
#include "IPnPLComponent.h"
#include "IPnPLComponent_vtbl.h"


/**
  * Create a type name for _DeviceInfoPnPL.
 */
typedef struct _DeviceInfoPnPL DeviceInfoPnPL;


// Public API declaration
//***********************

IPnPLComponent_t *DeviceInfoPnPLAlloc(void);

/**
  * Initialize the default parameters.
  *
  * @param _this [IN] specifies a pointer to a task object.
  * @return SYS_NO_ERROR_CODE if success, an error code otherwise
 */
uint8_t DeviceInfoPnPLInit(IPnPLComponent_t *_this);

/**
  * Get the IPnPLComponent interface for the component.
  * @param _this [IN] specifies a pointer to a PnPL component.
  * @return a pointer to the generic object ::IPnPLComponent if success,
  * or NULL if out of memory error occurs.
 */
IPnPLComponent_t *DeviceInfoPnPLGetComponentIF(DeviceInfoPnPL *_this);

#ifdef __cplusplus
}
#endif

#endif /* _PNPL_DEVICE_INFO_H_ */

/******************* (C) COPYRIGHT 2016 STMicroelectronics *****END OF FILE****/

/**
  ******************************************************************************
  * @file    PnPLCompManager.h
  * @author  SRA
  * @version v1.0.0
  * @date    27-Oct-2021
  * @brief   PnP-like Components Manager
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
#ifndef _PNPL_COMP_MANAGER_H_
#define _PNPL_COMP_MANAGER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "IPnPLComponent.h"
#include "IPnPLComponent_vtbl.h"
#include "PnPLDef.h"
#include "PnPLCompManager_Conf.h"
#include "parson.h"

/* Includes ------------------------------------------------------------------*/

/**
  * Create  type name for _PnPLCompManager_t.
  */
typedef struct _PnPLCompManager_t PnPLCompManager_t;

/**
  *  PnPLCompManager_t internal structure.
  */
struct _PnPLCompManager_t
{

  /**
    * Describes the sensor capabilities.
    */
  IPnPLComponent_t *Components[COM_MAX_PNPL_COMPONENTS];

  /**
    * Indicates the number of PnP components available.
    */
  uint16_t n_components;
};

/* Public API declaration */
/**************************/
void PnPLGenerateAcquisitionUUID(char *uuid);
uint16_t PnPLGetNComponents(void);
uint16_t PnPLGetComponentsNames(char **components_names);
uint8_t PnPLGetComponentValue(char *comp_name, char **SerializedJSON, uint32_t *size, uint8_t pretty);
uint8_t PnPLAddComponent(IPnPLComponent_t *pComponent);
uint8_t PnPLAddSensorComponent(IPnPLComponent_t *pComponent, uint8_t sensor_id, uint8_t sensor_type);
uint8_t PnPLRemoveComponent(uint8_t id); //char*
uint8_t PnPLGetPresentationJSON(char **SerializedJSON, uint32_t *size);
uint8_t PnPLGetDeviceStatusJSON(char **SerializedJSON, uint32_t *size, uint8_t pretty);
uint8_t PnPLGetFilteredDeviceStatusJSON(char **serializedJSON, uint32_t *size, char **namesList, uint32_t namesListSize, uint8_t pretty);
uint8_t PnPLUpdateDeviceStatusFromJSON(char *serializedJSON);
uint8_t PnPLParseCommand(char *commandString, PnPLCommand_t *command);
uint8_t PnPLSerializeResponse(PnPLCommand_t *command, char **SerializedJSON, uint32_t *size, uint8_t pretty);
uint8_t PnPLSerializeTelemetry(char *compName, PnPLTelemetry_t *telemetryValue, uint8_t telemetryNum, char **telemetryJSON, uint32_t *size, uint8_t pretty);


/* Inline functions definition */
/*******************************/

#ifdef __cplusplus
}
#endif

#endif /* _PNPL_COMP_MANAGER_H_ */

/******************* (C) COPYRIGHT 2016 STMicroelectronics *****END OF FILE****/

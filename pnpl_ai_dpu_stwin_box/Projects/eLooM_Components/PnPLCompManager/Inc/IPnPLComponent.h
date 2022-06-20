/**
  ******************************************************************************
  * @file    IPnPLComponent.h
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

#ifndef INCLUDE_IPNPLCOMPONENT_H_
#define INCLUDE_IPNPLCOMPONENT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
/**
  * Create  type name for IPnPLComponent.
  */
typedef struct _IPnPLComponent_t IPnPLComponent_t;

// Public API declaration
//***********************
/** Public interface **/
inline char *IPnPLComponentGetKey(IPnPLComponent_t *_this);
inline uint8_t IPnPLComponentGetNCommands(IPnPLComponent_t *_this);
inline char *IPnPLComponentGetCommandKey(IPnPLComponent_t *_this, uint8_t id);
inline uint8_t IPnPLComponentGetStatus(IPnPLComponent_t *_this, char **serializedJSON, uint32_t *size, uint8_t pretty);
inline uint8_t IPnPLComponentSetProperty(IPnPLComponent_t *_this, char *serializedJSON);
inline uint8_t IPnPLCommandExecuteFunction(IPnPLComponent_t *_this, char *serializedJSON);
#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_IPNPLCOMPONENT_H_ */

/**
  ******************************************************************************
  * @file    IPnPLComponent_vtbl.h
  * @author  SRA - MCD
  * @version 1.0.0
  * @date    27-Oct-2021
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

#ifndef INCLUDE_IPNPLCOMPONENT_VTBL_H_
#define INCLUDE_IPNPLCOMPONENT_VTBL_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
  * Create a type name for IPnPLComponent_vtbl.
  */
typedef struct _IPnPLComponent_vtbl IPnPLComponent_vtbl;

struct _IPnPLComponent_vtbl
{
  char *(*PnPLComponentGetKey)(IPnPLComponent_t *_this);
  uint8_t (*PnPLComponentGetNCommands)(IPnPLComponent_t *_this);
  char *(*PnPLComponentGetCommandsKey)(IPnPLComponent_t *_this, uint8_t id);
  uint8_t (*PnPLComponentGetStatus)(IPnPLComponent_t *_this, char **serializedJSON, uint32_t *size, uint8_t pretty);
  uint8_t (*PnPLComponentSetProperty)(IPnPLComponent_t *_this, char *serializedJSON);
  uint8_t (*PnPLCommandExecuteFunction)(IPnPLComponent_t *_this, char *serializedJSON);
};

struct _IPnPLComponent_t
{
  /**
    * Pointer to the virtual table for the class.
    */
  const IPnPLComponent_vtbl *vptr;
};

// Inline functions definition
// ***************************
inline char *IPnPLComponentGetKey(IPnPLComponent_t *_this)
{
  return _this->vptr->PnPLComponentGetKey(_this);
}

inline uint8_t IPnPLComponentGetNCommands(IPnPLComponent_t *_this)
{
  return _this->vptr->PnPLComponentGetNCommands(_this);
}

inline char *IPnPLComponentGetCommandKey(IPnPLComponent_t *_this, uint8_t id)
{
  return _this->vptr->PnPLComponentGetCommandsKey(_this, id);
}

inline uint8_t IPnPLComponentGetStatus(IPnPLComponent_t *_this, char **serializedJSON, uint32_t *size, uint8_t pretty)
{
  return _this->vptr->PnPLComponentGetStatus(_this, serializedJSON, size, pretty);
}

inline uint8_t IPnPLComponentSetProperty(IPnPLComponent_t *_this, char *serializedJSON)
{
  return _this->vptr->PnPLComponentSetProperty(_this, serializedJSON);
}

inline uint8_t IPnPLCommandExecuteFunction(IPnPLComponent_t *_this, char *serializedJSON)
{
  return _this->vptr->PnPLCommandExecuteFunction(_this, serializedJSON);
}

#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_IPNPLCOMPONENT_VTBL_H_ */

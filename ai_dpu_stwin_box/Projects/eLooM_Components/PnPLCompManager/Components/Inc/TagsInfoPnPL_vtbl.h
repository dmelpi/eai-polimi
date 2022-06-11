/**
 ******************************************************************************
 * @file    TagsInfoPnPL_vtbl.h
 * @author  SRA - MCD
 * @version 1.0.0
 * @date    202-Mar-2022
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
#ifndef TAGS_INFO_PNPL_VTBL_H_
#define TAGS_INFO_PNPL_VTBL_H_

#ifdef __cplusplus
extern "C" {
#endif

char* TagsInfoPnPL_vtblGetKey(IPnPLComponent_t *_this);
uint8_t TagsInfoPnPL_vtblGetNCommands(IPnPLComponent_t *_this);
char* TagsInfoPnPL_vtblGetCommandKey(IPnPLComponent_t *_this, uint8_t id);
uint8_t TagsInfoPnPL_vtblGetStatus(IPnPLComponent_t *_this, char** serializedJSON , uint32_t* size, uint8_t pretty);
uint8_t TagsInfoPnPL_vtblSetProperty(IPnPLComponent_t *_this, char* serializedJSON);
uint8_t TagsInfoPnPL_vtblExecuteFunction(IPnPLComponent_t *_this, char* serializedJSON);

#ifdef __cplusplus
}
#endif

#endif /* TAGS_INFO_PNPL_VTBL_H_ */

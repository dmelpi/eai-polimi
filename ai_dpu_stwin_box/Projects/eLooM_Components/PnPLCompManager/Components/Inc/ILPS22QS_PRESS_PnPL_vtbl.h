/**
 ******************************************************************************
 * @file    ILPS22QS_PRESS_PnPL_vtbl.h
 * @author  SRA - MCD
 * @version 1.0.0
 * @date    28-oct-2021
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
#ifndef ILPS22QS_PRESS_PNPL_VTBL_H_
#define ILPS22QS_PRESS_PNPL_VTBL_H_

#ifdef __cplusplus
extern "C" {
#endif

char* ILPS22QS_PRESS_PnPL_vtblGetKey(IPnPLComponent_t *_this);
uint8_t ILPS22QS_PRESS_PnPL_vtblGetNCommands(IPnPLComponent_t *_this);
char* ILPS22QS_PRESS_PnPL_vtblGetCommandKey(IPnPLComponent_t *_this, uint8_t id);
uint8_t ILPS22QS_PRESS_PnPL_vtblGetStatus(IPnPLComponent_t *_this, char** serializedJSON , uint32_t* size, uint8_t pretty);
uint8_t ILPS22QS_PRESS_PnPL_vtblSetProperty(IPnPLComponent_t *_this, char* serializedJSON);
uint8_t ILPS22QS_PRESS_PnPL_vtblExecuteFunction(IPnPLComponent_t *_this, char* serializedJSON);

#ifdef __cplusplus
}
#endif

#endif /* ILPS22QS_PRESS_PNPL_VTBL_H_ */

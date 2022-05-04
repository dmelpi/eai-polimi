/**
 ******************************************************************************
 * @file    IBus.h
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
#ifndef IBUS_H_
#define IBUS_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "ABusIF.h"


/**
 * Creates a type name for _IBus.
 */
typedef struct _IBus IBus;


inline sys_error_code_t IBusCtrl(IBus *_this, EBusCtrlCmd eCtrlCmd, uint32_t nParams);

/**
 * Connect a device to the bus using its interface.
 *
 * @param _this [IN] specifies a bus object.
 * @param pxBusIF [IN] specifies the device interface to connect.
 * @return SYS_NO_ERROR_CODE is success, SYS_INVALID_PARAMETER_ERROR_CODE if pxBuff is NULL.
 */
inline sys_error_code_t IBusConnectDevice(IBus *_this, ABusIF *pxBusIF);

/**
 * Disconnect a device from the bus using its interface.
 *
 * @param _this [IN] specifies a bus object.
 * @param pxBusIF [IN] specifies the device interface to connect.
 * @return SYS_NO_ERROR_CODE is success, SYS_INVALID_PARAMETER_ERROR_CODE if pxBuff is NULL.
 */
inline sys_error_code_t IBusDisconnectDevice(IBus *_this, ABusIF *pxBusIF);

#ifdef __cplusplus
}
#endif

#endif /* IBUS_H_ */

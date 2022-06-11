/**
 ******************************************************************************
 * @file    ISensorEventListener.h
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
#ifndef INCLUDE_EVENTS_ISENSOREVENTLISTENER_H_

#define INCLUDE_EVENTS_ISENSOREVENTLISTENER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ai_sp_dataformat.h"

#include "events/IEventListener.h"
#include "events/IEventListener_vtbl.h"
#include "events/SensorEvent.h"


/**
 * Create  type name for _ISensorEventListener.
 */
typedef struct _ISensorEventListener ISensorEventListener;


// Public API declaration
//***********************

/**
 * Called by the a sensor ::IEventSrc when new data are ready.
 *
 * @param _this [IN] specifies a pointer to the object.
 * @return SYS_NO_ERROR_CODE if success, an error code otherwise.
 */
inline sys_error_code_t ISensorEventListenerOnNewDataReady(IEventListener *_this, const SensorEvent *pxEvt);


// Inline functions definition
// ***************************


#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_EVENTS_ISENSOREVENTLISTENER_H_ */

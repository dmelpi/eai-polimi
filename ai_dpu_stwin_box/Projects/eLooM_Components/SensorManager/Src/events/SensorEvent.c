/**
 ******************************************************************************
 * @file    SensorEvent.c
 * @author  SRA - MCD
 * @version 1.0.0
 * @date    6-Sep-2021
 *
 * @brief  SensorEvent class definition.
 *
 * This file is needed only for GCC compiler in DEBUG configuration.
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

#include "events/SensorEvent.h"

// Public API implementation.
// **************************

// GCC requires one function forward declaration in only one .c source
// in order to manage the inline.
// See also http://stackoverflow.com/questions/26503235/c-inline-function-and-gcc
#if defined (__GNUC__) || defined (__ICCARM__)
extern sys_error_code_t SensorEventInit(IEvent *_this, const IEventSrc *pSource, const ai_logging_packet_t *stream, double fTimeStamp, uint16_t nSensorID);
#endif

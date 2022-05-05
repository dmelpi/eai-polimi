/**
 ******************************************************************************
 * @file    IEventSrc.c
 * @author  STMicroelectronics - ST-Korea - MCD Team
 * @version 3.0.0
 * @date    Apr 6, 2017
 *
 * @brief   External definition of the IEventSrc inline methods.
 *
 * This file is used by GCC only to to provide a compilation unit where to
 * put the methods implementation when the compiler can't expand them inline.
 *
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2017 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file in
 * the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 ******************************************************************************
 */

#include "events/IEventSrc.h"
#include "events/IEventSrc_vtbl.h"

#if defined (__GNUC__) || defined (__ICCARM__)

extern sys_error_code_t IEventSrcInit(IEventSrc *this);
extern sys_error_code_t IEventSrcAddEventListener(IEventSrc *this, IEventListener *pListener);
extern sys_error_code_t IEventSrcRemoveEventListener(IEventSrc *this, IEventListener *pListener);
extern uint32_t IEventSrcGetMaxListenerCount(const IEventSrc *this);
extern sys_error_code_t IEventSrcSendEvent(const IEventSrc *this, const IEvent *pxEvent, void *pvParams);

#endif

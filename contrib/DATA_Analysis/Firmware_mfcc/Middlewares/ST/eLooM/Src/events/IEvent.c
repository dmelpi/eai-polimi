/**
 ******************************************************************************
 * @file    IEvent.c
 * @author  STMicroelectronics - ST-Korea - MCD Team
 * @version 3.0.0
 * @date    Apr 17, 2017
 *
 * @brief
 *
 * <DESCRIPTIOM>
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

#include "events/IEvent.h"
#include "events/IEventSrc.h"
#include "events/IEventSrc_vtbl.h"

#if defined (__GNUC__) || defined (__ICCARM__)

extern sys_error_code_t IEventInit(IEvent *this, const IEventSrc *pSource);

#endif

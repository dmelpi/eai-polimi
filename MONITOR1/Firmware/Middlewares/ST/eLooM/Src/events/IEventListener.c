/**
 ******************************************************************************
 * @file    IEventListener.c
 * @author  STMicroelectronics - ST-Korea - MCD Team
 * @version 3.0.0
 * @date    Apr 13, 2017
 *
 * @brief External definition of the IEventListener inline methods.
 *
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


#include "events/IEventListener.h"
#include "events/IEventListener_vtbl.h"

#if defined (__GNUC__) || defined (__ICCARM__)

extern void IEventListenerSetOwner(IEventListener *this, void *pxOwner);
extern void *IEventListenerGetOwner(IEventListener *this);

#endif

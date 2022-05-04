/**
 ******************************************************************************
 * @file    IListener.c
 * @author  STMicroelectronics - ST-Korea - MCD Team
 * @version 3.0.0
 * @date    Mar 20, 2017
 *
 * @brief   External definition of the IListener inline methods.
 *
 * This file is used by GCC only to to provide a compilation unit where to
 * put the methods implementation when the compiler can't expand them inline.
 *
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2016 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file in
 * the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 ******************************************************************************
 */

#include "events/IListener.h"
#include "events/IListener_vtbl.h"

#if defined (__GNUC__) || defined (__ICCARM__)

extern sys_error_code_t IListenerOnStatusChange(IListener *this);

#endif

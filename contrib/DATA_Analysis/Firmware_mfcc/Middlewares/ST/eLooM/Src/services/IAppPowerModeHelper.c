/**
 ******************************************************************************
 * @file    IAppPowerModeHelper.c
 * @author  STMicroelectronics - ST-Korea - MCD Team
 * @version 3.0.0
 * @date    Oct 30, 2018
 *
 * @brief
 *
 * <DESCRIPTIOM>
 *
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2018 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file in
 * the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 ******************************************************************************
 */

#include "services/IAppPowerModeHelper.h"
#include "services/IAppPowerModeHelper_vtbl.h"


// GCC requires one function forward declaration in only one .c source
// in order to manage the inline.
// See also http://stackoverflow.com/questions/26503235/c-inline-function-and-gcc
#if defined (__GNUC__) || defined (__ICCARM__)
extern sys_error_code_t IapmhInit(IAppPowerModeHelper *this);
extern EPowerMode IapmhComputeNewPowerMode(IAppPowerModeHelper *this, const SysEvent xEvent);
extern boolean_t IapmhCheckPowerModeTransaction(IAppPowerModeHelper *this, const EPowerMode eActivePowerMode, const EPowerMode eNewPowerMode);
extern sys_error_code_t IapmhDidEnterPowerMode(IAppPowerModeHelper *this, EPowerMode ePowerMode);
extern EPowerMode IapmhGetActivePowerMode(IAppPowerModeHelper *this);
extern SysPowerStatus IapmhGetPowerStatus(IAppPowerModeHelper *this);
extern boolean_t IapmhIsLowPowerMode(IAppPowerModeHelper *this, const EPowerMode ePowerMode);
#endif


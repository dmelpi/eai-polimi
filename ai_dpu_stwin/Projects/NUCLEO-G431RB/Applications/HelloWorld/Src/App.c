/**
 ******************************************************************************
 * @file    App.c
 * @author  STMicroelectronics - AIS - MCD Team
 * @version 3.0.1
 * @date    21/01/2022 10:35:16
 *
 * @brief   Define the the main entry points for the eLooM framework.
 *
 *********************************************************************************
 * @attention
 *
 * Copyright (c) 2021 STMicroelectronics
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file in
 * the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *********************************************************************************
 */

#include "services/sysdebug.h"
#include "services/ApplicationContext.h"
#include "AppPowerModeHelper.h"
#include "HelloWorldTask.h"

#include "mx.h"

/**
 * HelloWorld task object.
 */
static AManagedTask *spHElloWorldObj = NULL;

/* eLooM framework entry points definition */
/*******************************************/

sys_error_code_t SysLoadApplicationContext(ApplicationContext *pAppContext)
{
  assert_param(pAppContext);
  sys_error_code_t xRes = SYS_NO_ERROR_CODE;

  // Allocate the task objects
  spHElloWorldObj = HelloWorldTaskAlloc(&MX_GPIO_UB1InitParams);

  // Add the task object to the context.
  xRes = ACAddTask(pAppContext, spHElloWorldObj);

  return xRes;
}

sys_error_code_t SysOnStartApplication(ApplicationContext *pAppContext) {
  UNUSED(pAppContext);

  return SYS_NO_ERROR_CODE;
}

/*IApplicationErrorDelegate *SysGetErrorDelegate(void)
 * {
  // Install the application error manager delegate.
  static IApplicationErrorDelegate *spErrDelegate = NULL;
  if (spErrDelegate == NULL)
  {
    spErrDelegate = AEMAlloc();
  }

  return spErrDelegate;
}*/

IAppPowerModeHelper *SysGetPowerModeHelper(void)
{
  // Install the application power mode helper.
  static IAppPowerModeHelper *spPowerModeHelper = NULL;
  if (spPowerModeHelper == NULL) {
    spPowerModeHelper = AppPowerModeHelperAlloc();
  }

  return spPowerModeHelper;
}

/* Private function definition */
/*******************************/


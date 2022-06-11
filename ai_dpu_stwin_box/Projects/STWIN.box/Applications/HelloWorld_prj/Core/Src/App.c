/**
 ******************************************************************************
 * @file    App.c
 * @author  STMicroelectronics - AIS - MCD Team
 * @version $Version$
 * @date    $Date$
 *
 * @brief   Define the Application main entry points
 *
 * The framework `weak` function are redefined in this file and they link
 * the application specific code with the framework.
 *
 *********************************************************************************
 * @attention
 *
 * <h2><center>COPYRIGHT &copy; 2021 STMicroelectronics</center></h2>
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
//#include "UtilTask.h"
#include "mx.h"

/**
 * Application managed task.
 */
static AManagedTask *spHelloWorldObj = NULL;

/**
 * Utility task object.
 */
//static AManagedTaskEx *spUtilObj = NULL;


/**
 * specifies the map (PM_APP, PM_SM). It re-map the state of teh application into the state of the Sensor Manager.
 */
//static EPowerMode spAppPMState2SMPMStateMap[] = {
//    E_POWER_MODE_STATE1,
//    E_POWER_MODE_SLEEP_1,
//    E_POWER_MODE_SENSORS_ACTIVE,
//    E_POWER_MODE_RESERVED,
//    E_POWER_MODE_SENSORS_ACTIVE,
//    E_POWER_MODE_SENSORS_ACTIVE
//};


/* Private functions declaration */
/*********************************/

/**
 * Re-map the PM State Machine of the Sensor Manager managed tasks used in the application according to the following map:
 *
 * | App State                      | Sensor Manager State         |
 * | :----------------------------- | ---------------------------: |
 * | E_POWER_MODE_STATE1            | E_POWER_MODE_STATE1          |
 * | E_POWER_MODE_SLEEP_1           | E_POWER_MODE_SLEEP_1         |
 * | E_POWER_MODE_X_CUBE_AI_ACTIVE  | E_POWER_MODE_SENSORS_ACTIVE  |
 * | E_POWER_MODE_NEAI_ACTIVE       | E_POWER_MODE_SENSORS_ACTIVE  |
 *
 * @param pPMState2PMStateMap [IN] specifies the map (PM_APP, PM_SM).
 * @return SYS_NO_ERROR_CODE if success, an error code otherwise.
 */
//static sys_error_code_t SensorManagerStateMachineRemap(EPowerMode *pPMState2PMStateMap);


/* eLooM framework entry points definition */
/*******************************************/

sys_error_code_t SysLoadApplicationContext(ApplicationContext *pAppContext)
{
  assert_param(pAppContext);
  sys_error_code_t xRes = SYS_NO_ERROR_CODE;

  /* Allocate the task objects */
  spHelloWorldObj = HelloWorldTaskAlloc();

//  spUtilObj = UtilTaskAlloc(&MX_TIM5InitParams);

  /* Add the task object to the context. */
  xRes = ACAddTask(pAppContext, spHelloWorldObj);
//  xRes = ACAddTask(pAppContext, (AManagedTask*)spUtilObj);

  return xRes;
}

sys_error_code_t SysOnStartApplication(ApplicationContext *pAppContext)
{
  UNUSED(pAppContext);

  return SYS_NO_ERROR_CODE;
}

/*IApplicationErrorDelegate *SysGetErrorDelegate(void)
 * {
  // Install the application error manager delegate.
  static IApplicationErrorDelegate *s_pxErrDelegate = NULL;
  if (s_pxErrDelegate == NULL)
  {
    s_pxErrDelegate = AEMAlloc();
  }

  return s_pxErrDelegate;
}*/

IAppPowerModeHelper *SysGetPowerModeHelper(void)
{
  // Install the application power mode helper.
  static IAppPowerModeHelper *s_pxPowerModeHelper = NULL;
  if (s_pxPowerModeHelper == NULL) {
    s_pxPowerModeHelper = AppPowerModeHelperAlloc();
  }

  return s_pxPowerModeHelper;
}




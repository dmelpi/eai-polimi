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
 * Copyright (c) 2022 STMicroelectronics
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file in
 * the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *********************************************************************************
 */

#include "services/sysdebug.h"
#include "services/ApplicationContext.h"
#include "events/SensorListener.h"
#include "events/SensorListener_vtbl.h"
#include "AppPowerModeHelper.h"
#include "HelloWorldTask.h"
#include "UtilTask.h"
#include "I2CBusTask.h"
#include "HTS221Task.h"

#include "mx.h"

/**
 * HelloWorld task object.
 */
static AManagedTask *spHelloWorldObj = NULL;

/**
 * Utility task object.
 */
static AManagedTaskEx *spUtilObj = NULL;

/**
 * I2C bus task object.
 */
static AManagedTaskEx *spI2CBusObj = NULL;

/**
 * HTS221 sensor task object.
 */
static AManagedTaskEx *spHTS221Obj = NULL;

/**
 * Sensor Listener used to print the data of teh active sensor.
 */
static SensorListener_t sSensorListener;

/**
 * specifies the map (PM_APP, PM_SM). It re-map the state of the application into the state of the Sensor Manager.
 */
static EPowerMode spAppPMState2SMPMStateMap[] = {
    E_POWER_MODE_STATE1,
    E_POWER_MODE_SLEEP_1,
    E_POWER_MODE_SENSORS_ACTIVE,
    E_POWER_MODE_SENSORS_ACTIVE
};


/* Private functions declaration */
/*********************************/

/**
 * Re-map the PM State Machine of the Sensor Manager managed tasks used in the application according to the following map:
 *
 * | App State                      | SensorManager State          |
 * | :----------------------------- | ---------------------------: |
 * | E_POWER_MODE_STATE1            | E_POWER_MODE_STATE1          |
 * | E_POWER_MODE_SLEEP_1           | E_POWER_MODE_SLEEP_1         |
 * | E_POWER_MODE_TEST              | E_POWER_MODE_SENSORS_ACTIVE  |
 *
 * @param pPMState2PMStateMap [IN] specifies the map (PM_APP, PM_SM).
 * @return SYS_NO_ERROR_CODE if success, an error code otherwise.
 */
static sys_error_code_t SensorManagerStateMachineRemap(EPowerMode *pPMState2PMStateMap);


/* eLooM framework entry points definition */
/*******************************************/

sys_error_code_t SysLoadApplicationContext(ApplicationContext *pAppContext)
{
  assert_param(pAppContext);
  sys_error_code_t xRes = SYS_NO_ERROR_CODE;

  // Allocate the task objects
  spHelloWorldObj = HelloWorldTaskAlloc(&MX_GPIO_UB1InitParams);
  spUtilObj = UtilTaskAlloc(&MX_TIM16InitParams, &MX_GPIO_LD2InitParams);
  spI2CBusObj = I2CBusTaskAlloc(&MX_I2C1InitParams);
  spHTS221Obj = HTS221TaskAlloc();

  // Add the task object to the context.
  xRes = ACAddTask(pAppContext, spHelloWorldObj);
  xRes = ACAddTask(pAppContext, (AManagedTask*)spUtilObj);
  xRes = ACAddTask(pAppContext, (AManagedTask*)spI2CBusObj);
  xRes = ACAddTask(pAppContext, (AManagedTask*)spHTS221Obj);

  // Allocate other application objects
  SensorListenerStaticAlloc(&sSensorListener);

  return xRes;
}

sys_error_code_t SysOnStartApplication(ApplicationContext *pAppContext) {
  UNUSED(pAppContext);

  /* Re-map the state machine of the Sensor Manager tasks */
  SensorManagerStateMachineRemap(spAppPMState2SMPMStateMap);

  /* Disable the automatic low power mode timer */
  UtilTaskSetAutoLowPowerModePeriod((UtilTask_t*)spUtilObj, 0);
  /* Set the sensor listener used by the Util task.*/
  UtilTaskSetSensorListener((UtilTask_t*)spUtilObj, &sSensorListener);

  /* connect the sensors task to the SPI bus. */
  I2CBusTaskConnectDevice((I2CBusTask*)spI2CBusObj, HTS221TaskGetSensorIF((HTS221Task*)spHTS221Obj));

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

static sys_error_code_t SensorManagerStateMachineRemap(EPowerMode *pPMState2PMStateMap)
{
  assert_param(pPMState2PMStateMap != NULL);

  AMTSetPMStateRemapFunc((AManagedTask*)spI2CBusObj, pPMState2PMStateMap);
  AMTSetPMStateRemapFunc((AManagedTask*)spHTS221Obj, pPMState2PMStateMap);
  AMTSetPMStateRemapFunc((AManagedTask*)spUtilObj, pPMState2PMStateMap);

  return SYS_NO_ERROR_CODE;
}


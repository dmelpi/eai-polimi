/**
 ******************************************************************************
 * @file    App.c
 * @author  STMicroelectronics - AIS - MCD Team
 * @version V1.0.0
 * @date    15-September-2021
 *
 * @brief   Define the Application main entry points
 *
 * The framework `weak` function are redefined in this file and they link
 * the application specific code with the framework.
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
#include "SPIBusTask.h"
#include "ISM330DHCXTask.h"
//#include "IIS3DWBTask.h"
#include "IMP23ABSUTask.h"
#include "NeaiTask.h"
#include "AITask.h"
#include "AppController.h"

#include "UtilTask.h"
#include "mx.h"


/**
 * SPI bus task object.
 */
static AManagedTaskEx *spSPIBusObj = NULL;

/**
 * Sensor task object.
 */
static AManagedTaskEx *spISM330DHCXObj = NULL;

/**
 * Sensor task object.
 */
//static AManagedTaskEx *spIIS3DWBObj = NULL;

/**
 * Sensor task object.
 */
static AManagedTaskEx *spIMP23ABSUObj = NULL;

/**
 * NanoedgeAI task object.
 */
static AManagedTaskEx *spNeaiObj = NULL;

/**
 * CubeAI task object.
 */
static AManagedTaskEx *spAIObj = NULL;
/**
 * Utility task object.
 */
static AManagedTaskEx *spUtilObj = NULL;

/**
 * Application controller object.
 */
static AManagedTaskEx *spControllerObj = NULL;


/**
 * specifies the map (PM_APP, PM_SM). It re-map the state of teh application into the state of the Sensor Manager.
 */
static EPowerMode spAppPMState2SMPMStateMap[] = {
    E_POWER_MODE_STATE1,
    E_POWER_MODE_SLEEP_1,
    E_POWER_MODE_SENSORS_ACTIVE,
    E_POWER_MODE_RESERVED,
    E_POWER_MODE_SENSORS_ACTIVE,
    E_POWER_MODE_SENSORS_ACTIVE
};


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
static sys_error_code_t SensorManagerStateMachineRemap(EPowerMode *pPMState2PMStateMap);


/* eLooM framework entry points definition */
/*******************************************/

sys_error_code_t SysLoadApplicationContext(ApplicationContext *pAppContext)
{
  assert_param(pAppContext);
  sys_error_code_t xRes = SYS_NO_ERROR_CODE;

  // Allocate the task objects
  spSPIBusObj = SPIBusTaskAlloc(&MX_SPI3InitParams);
  spISM330DHCXObj = ISM330DHCXTaskAlloc();
//  spIIS3DWBObj = IIS3DWBTaskAlloc();
  spIMP23ABSUObj = IMP23ABSUTaskAlloc(&MX_DFSDMCH0F1InitParams, &MX_ADC1InitParams);
  spUtilObj = UtilTaskAlloc(&MX_TIM5InitParams, &MX_TIM16InitParams, &MX_GPIO_PF6InitParams);
  spNeaiObj = NeaiTaskAlloc();
  spAIObj = AITaskAlloc();
  spControllerObj = AppControllerAlloc();

  // Add the task object to the context.
  xRes = ACAddTask(pAppContext, (AManagedTask*)spSPIBusObj);
  xRes = ACAddTask(pAppContext, (AManagedTask*)spISM330DHCXObj);
  xRes = ACAddTask(pAppContext, (AManagedTask*)spIMP23ABSUObj);
//  xRes = ACAddTask(pAppContext, (AManagedTask*)spIIS3DWBObj);
  xRes = ACAddTask(pAppContext, (AManagedTask*)spUtilObj);
  xRes = ACAddTask(pAppContext, (AManagedTask*)spNeaiObj);
  xRes = ACAddTask(pAppContext, (AManagedTask*)spAIObj);
  xRes = ACAddTask(pAppContext, (AManagedTask*)spControllerObj);

  return xRes;
}

sys_error_code_t SysOnStartApplication(ApplicationContext *pAppContext) {
  UNUSED(pAppContext);

  /* Re-map the state machine of the Sensor Manager tasks */
  SensorManagerStateMachineRemap(spAppPMState2SMPMStateMap);

  /* Disable the automatic low power mode timer */
  UtilTaskSetAutoLowPowerModePeriod((UtilTask_t*)spUtilObj, 0);

  /* connect the sensors task to the SPI bus. */
  SPIBusTaskConnectDevice((SPIBusTask*)spSPIBusObj, ISM330DHCXTaskGetSensorIF((ISM330DHCXTask*)spISM330DHCXObj));
//  SPIBusTaskConnectDevice((SPIBusTask*)spSPIBusObj, IIS3DWBTaskGetSensorIF((IIS3DWBTask*)spIIS3DWBObj));

  /* set the output queue for the USB CDC device. It is used to deliver all the incoming input */
  CDC_SetOutQueue(AppControllerGetInQueue((AppController_t*)spControllerObj));

  /* Connect the Util Task with the AppController task to propagate the push button event. */
  UtilTaskSetCtrlInQueue((UtilTask_t*)spUtilObj, AppControllerGetInQueue((AppController_t*)spControllerObj));

  /* register sensors and AI processing with the application controller. */
  ISourceObservable *p_sensor1 = ISM330DHCXTaskGetAccSensorIF((ISM330DHCXTask*)spISM330DHCXObj);
//  ISourceObservable *p_sensor2 = IIS3DWBTaskGetAccSensorIF((IIS3DWBTask*)spIIS3DWBObj);
  ISourceObservable *p_sensor2 = IMP23ABSUTaskGetMicSensorIF((IMP23ABSUTask*)spIMP23ABSUObj);
  ISourceObservable *p_sensor3 = ISM330DHCXTaskGetGyroSensorIF((ISM330DHCXTask*)spISM330DHCXObj);
  AppControllerSetSensorsIF((AppController_t*)spControllerObj, p_sensor1, p_sensor3, p_sensor2);
  QueueHandle_t queue1 = AITaskGetInQueue((AITask_t*)spAIObj);
  QueueHandle_t queue2 = NeaiTaskGetInQueue((NeaiTask_t*)spNeaiObj);
  QueueHandle_t queue3 = UtilTaskGetInQueue((UtilTask_t*)spUtilObj);
  AppControllerSetAIProcessesInQueue((AppController_t*)spControllerObj, queue1, queue2, queue3);

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


/* Private function definition */
/*******************************/

static sys_error_code_t SensorManagerStateMachineRemap(EPowerMode *pPMState2PMStateMap)
{
  assert_param(pPMState2PMStateMap != NULL);

  AMTSetPMStateRemapFunc((AManagedTask*)spSPIBusObj, pPMState2PMStateMap);
  AMTSetPMStateRemapFunc((AManagedTask*)spISM330DHCXObj, pPMState2PMStateMap);
//  AMTSetPMStateRemapFunc((AManagedTask*)spIIS3DWBObj, pPMState2PMStateMap);
  AMTSetPMStateRemapFunc((AManagedTask*)spIMP23ABSUObj, pPMState2PMStateMap);
  AMTSetPMStateRemapFunc((AManagedTask*)spUtilObj, pPMState2PMStateMap);

  return SYS_NO_ERROR_CODE;
}

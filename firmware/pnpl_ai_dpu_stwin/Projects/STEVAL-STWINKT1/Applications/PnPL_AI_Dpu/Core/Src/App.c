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

#include "services/ApplicationContext.h"
#include "AppPowerModeHelper.h"
#include "AppTask.h"
#include "ProcessTask.h"
#include "SPIBusTask.h"
#include "ISM330DHCXTask.h"
#include "IIS3DWBTask.h"
#include "SCLIUtilTask.h"
#include "mx.h"



/**
 * HelloWorld task object.
 */
static AManagedTask *spAppObj = NULL;

/**
 * Processing task. It process the data data coming from the sensor using a DPU.
 */
static AManagedTaskEx *spProcessTask = NULL;

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
static AManagedTaskEx *spIIS3DWBObj = NULL;

/**
 * Utility task object.
 */
static AManagedTaskEx *spUtilObj = NULL;


/* eLooM framework entry points definition */
/*******************************************/

sys_error_code_t SysLoadApplicationContext(ApplicationContext *pAppContext)
{
  assert_param(pAppContext);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  // Allocate the task objects
  spSPIBusObj = SPIBusTaskAlloc(&MX_SPI3InitParams);
  spISM330DHCXObj = ISM330DHCXTaskAlloc(&MX_GPIO_PE8InitParams, &MX_GPIO_PF13InitParams, SPI_IF);
  spIIS3DWBObj = IIS3DWBTaskAlloc(&MX_GPIO_PE14InitParams, &MX_GPIO_PF5InitParams, SPI_IF);
  spUtilObj = SCLIUtilTaskAlloc(&MX_GPIO_PE1InitParams, NULL);
  spProcessTask = ProcessTaskAlloc();
  spAppObj = AppTaskAlloc(&MX_GPIO_PE0InitParams);

  // Add the task object to the context.
  res = ACAddTask(pAppContext, (AManagedTask*)spSPIBusObj);
  res = ACAddTask(pAppContext, (AManagedTask*)spISM330DHCXObj);
  res = ACAddTask(pAppContext, (AManagedTask*)spIIS3DWBObj);
  res = ACAddTask(pAppContext, (AManagedTask*)spUtilObj);
  res = ACAddTask(pAppContext, (AManagedTask*)spProcessTask);
  res = ACAddTask(pAppContext, spAppObj);

  return res;
}

sys_error_code_t SysOnStartApplication(ApplicationContext *pAppContext) {
  UNUSED(pAppContext);

  /* Disable the automatic low power mode timer */
  SCLIUtilTaskSetAutoLowPowerModePeriod((SCLIUtilTask_t*)spUtilObj, 0);

  /* connect the sensors task to the SPI bus. */
  SPIBusTaskConnectDevice((SPIBusTask*)spSPIBusObj, (SPIBusIF*)ISM330DHCXTaskGetSensorIF((ISM330DHCXTask*)spISM330DHCXObj));
  SPIBusTaskConnectDevice((SPIBusTask*)spSPIBusObj, (SPIBusIF*)IIS3DWBTaskGetSensorIF((IIS3DWBTask*)spIIS3DWBObj));

  /* for simplicity, in this demo, the configuration of the sensors is done in the virtual function
   * ProcesTask::OnEnterTaskControlLoop() -> ProcesTask_vtblOnEnterTaskControlLoop()
   */

  /* connect the HelloWord task object to the Process task.*/
  ProcessTaskAddDPUListener((ProcessTask_t*)spProcessTask, AppTaskGetProcEvtListenerIF((AppTask*)spAppObj));

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


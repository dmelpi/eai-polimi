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
#include "SPIBusTask.h"
#include "ISM330DHCXTask.h"
#include "IIS3DWBTask.h"
#include "StreamerCLI.h"
#include "SCLIUtilTask.h"
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
static AManagedTaskEx *spIIS3DWBObj = NULL;

/**
 * Utility task object.
 */
static AManagedTaskEx *spUtilObj = NULL;

/**
 * Streamer CLI object.
 */
static AManagedTaskEx *spStremaerCLIObj = NULL;

/**
 * HelloWorld task object
 */
static AManagedTask *spHWObj = NULL;


/* Private functions declaration */
/*********************************/



/* eLooM framework entry points definition */
/*******************************************/

sys_error_code_t SysLoadApplicationContext(ApplicationContext *pAppContext)
{
  assert_param(pAppContext);
  sys_error_code_t xRes = SYS_NO_ERROR_CODE;

  // Allocate the task objects
  spSPIBusObj = SPIBusTaskAlloc(&MX_SPI3InitParams);
  spISM330DHCXObj = ISM330DHCXTaskAlloc(&MX_GPIO_PE8InitParams, &MX_GPIO_PF13InitParams, SPI_IF);
  spIIS3DWBObj = IIS3DWBTaskAlloc(&MX_GPIO_PE14InitParams, &MX_GPIO_PF5InitParams, SPI_IF);
  spUtilObj = SCLIUtilTaskAlloc(&MX_GPIO_PE1InitParams, NULL);
  spStremaerCLIObj = StreamerCLIAlloc();
  spHWObj = HelloWorldTaskAlloc(&MX_GPIO_PE0InitParams);

  // Add the task object to the context.
  xRes = ACAddTask(pAppContext, (AManagedTask*)spSPIBusObj);
  xRes = ACAddTask(pAppContext, (AManagedTask*)spISM330DHCXObj);
  xRes = ACAddTask(pAppContext, (AManagedTask*)spIIS3DWBObj);
  xRes = ACAddTask(pAppContext, (AManagedTask*)spUtilObj);
  xRes = ACAddTask(pAppContext, (AManagedTask*)spStremaerCLIObj);
  xRes = ACAddTask(pAppContext, spHWObj);

  return xRes;
}

sys_error_code_t SysOnStartApplication(ApplicationContext *pAppContext) {
  UNUSED(pAppContext);

  /* Disable the automatic low power mode timer */
  SCLIUtilTaskSetAutoLowPowerModePeriod((SCLIUtilTask_t*)spUtilObj, 0);

  /* connect the sensors task to the SPI bus. */
  SPIBusTaskConnectDevice((SPIBusTask*)spSPIBusObj, (SPIBusIF*)ISM330DHCXTaskGetSensorIF((ISM330DHCXTask*)spISM330DHCXObj));
  SPIBusTaskConnectDevice((SPIBusTask*)spSPIBusObj, (SPIBusIF*)IIS3DWBTaskGetSensorIF((IIS3DWBTask*)spIIS3DWBObj));

  /* set the output queue for the USB CDC device. It is used to deliver all the incoming input */
  CDC_SetOutQueue(StreamerCLIGetInQueue((StreamerCLI_t*)spStremaerCLIObj));

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


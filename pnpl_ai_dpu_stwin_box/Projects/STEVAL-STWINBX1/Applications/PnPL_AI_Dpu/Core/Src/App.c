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
#include "AppTask.h"
#include "SPIBusTask.h"
#include "IIS3DWBTask.h"
#include "ISM330DHCXTask.h"
#include "ProcessTask.h"
#include "UsbCdcTask.h"
#include "mx.h"

#include "PnPLCompManager.h"
#include "DeviceInfoPnPL.h"
#include "FwInfoPnPL.h"
#include "AiApplicationPnPL.h"
#include "ISM330DHCX_ACC_PnPL.h"

/**
 * Application managed task.
 */
static AManagedTask *sAppTaskObj = NULL;

/**
 * Processing task. It process the data data coming from the sensor using a DPU.
 */
static AManagedTaskEx *sProcessTaskObj = NULL;

/**
 * SPI bus task object.
 */
static AManagedTaskEx *sSPIBusObj = NULL;

/**
 * Sensor task object.
 */
static AManagedTaskEx *sISM330DHCXObj = NULL;

/**
 * Sensor task object.
 */
static AManagedTaskEx *sIIS3DWBObj = NULL;

/**
 * USB_CDC task object.
 */
static AManagedTaskEx *sUsbCdcObj = NULL;


static IPnPLComponent_t *pAiApplicationPnPLObj = NULL;
static IPnPLComponent_t *pFwInfoPnPLObj = NULL;
static IPnPLComponent_t *pDeviceInfoPnPLObj = NULL;
static IPnPLComponent_t *pISM330DHCX_ACC_PnPLObj = NULL;



/**
 * specifies the map (PM_APP, PM_SM). It re-map the state of the application into the state of the Sensor Manager.
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

  // Allocate the task objects
  sSPIBusObj = SPIBusTaskAlloc(&MX_SPI2InitParams);
  sISM330DHCXObj = ISM330DHCXTaskAlloc(&MX_GPIO_INT1_DHCXInitParams, &MX_GPIO_INT2_DHCXInitParams, &MX_GPIO_CS_DHCXInitParams);
  sIIS3DWBObj = IIS3DWBTaskAlloc(NULL, &MX_GPIO_CS_DWBInitParams);

  sUsbCdcObj = UsbCdcTaskAlloc();
  sProcessTaskObj = ProcessTaskAlloc();
  sAppTaskObj = AppTaskAlloc(&MX_GPIO_UBInitParams);

  // Add the task object to the context.
  xRes = ACAddTask(pAppContext, (AManagedTask*)sSPIBusObj);
  xRes = ACAddTask(pAppContext, (AManagedTask*)sISM330DHCXObj);
  xRes = ACAddTask(pAppContext, (AManagedTask*)sIIS3DWBObj);
  xRes = ACAddTask(pAppContext, (AManagedTask*)sUsbCdcObj);
  xRes = ACAddTask(pAppContext, (AManagedTask*)sProcessTaskObj);
  xRes = ACAddTask(pAppContext, sAppTaskObj);

  pDeviceInfoPnPLObj = DeviceInfoPnPLAlloc();
  pFwInfoPnPLObj = FwInfoPnPLAlloc();
  pISM330DHCX_ACC_PnPLObj = ISM330DHCX_ACC_PnPLAlloc();
  pAiApplicationPnPLObj = AiApplicationPnPLAlloc();

  /* Add the task object to the context. */
  return xRes;
}

sys_error_code_t SysOnStartApplication(ApplicationContext *pAppContext)
{
  UNUSED(pAppContext);

  /* connect the sensors task to the SPI bus. */
  SPIBusTaskConnectDevice((SPIBusTask*)sSPIBusObj, (SPIBusIF*)ISM330DHCXTaskGetSensorIF((ISM330DHCXTask*)sISM330DHCXObj));
  SPIBusTaskConnectDevice((SPIBusTask*)sSPIBusObj, (SPIBusIF*)IIS3DWBTaskGetSensorIF((IIS3DWBTask*)sIIS3DWBObj));

  /*
   * For simplicity, in this demo, the configuration of the sensors is done in the virtual function
   * ProcessTask::OnEnterTaskControlLoop() -> ProcessTask_vtblOnEnterTaskControlLoop()
   */

  /* connect the App task object to the Process task.*/
  ProcessTaskAddDPUListener((ProcessTask_t*)sProcessTaskObj, AppTaskGetProcEvtListenerIF((AppTask*)sAppTaskObj));

  /* set USB_CDC class delegate */
  UsbCdcTask_SetDelegate((UsbCdcTask_t*)sUsbCdcObj, AppTaskGetUsbDelegateIF((AppTask*)sAppTaskObj));

  ISM330DHCX_ACC_PnPLInit(pISM330DHCX_ACC_PnPLObj);
  DeviceInfoPnPLInit(pDeviceInfoPnPLObj);
  FwInfoPnPLInit(pFwInfoPnPLObj);
  AiApplicationPnPLInit(pAiApplicationPnPLObj, AppTaskGetIAiApplicationIF((AppTask *)sAppTaskObj));

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




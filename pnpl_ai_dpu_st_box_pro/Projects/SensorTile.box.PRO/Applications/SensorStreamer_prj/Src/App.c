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
#include "events/SensorListener.h"
#include "events/SensorListener_vtbl.h"
#include "AppPowerModeHelper.h"
#include "HelloWorldTask.h"
#include "UtilTask.h"
#include "I2CBusTask.h"
#include "SPIBusTask.h"
#include "HTS221Task.h"
#include "STTS22HTask.h"
#include "LPS22DFTask.h"
#include "LIS2MDLTask.h"
#include "LIS2DU12Task.h"
#include "LSM6DSV16XTask.h"
#include "IIS2ICLXTask.h"
#include "IIS2DLPCTask.h"
#include "ISM330DHCXTask.h"
#include "IIS3DWBTask.h"
#include "mx.h"

/**
 * Application managed task.
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
 * SPI bus task object.
 */
static AManagedTaskEx *spSPIBusObj = NULL;

/**
 * Sensor task object.
 */
//static AManagedTaskEx *spHTS221Obj = NULL;
//static AManagedTaskEx *spSTTS22HObj = NULL;
//static AManagedTaskEx *spLPS22DFObj = NULL;
//static AManagedTaskEx *spLIS2MDLObj = NULL;
//static AManagedTaskEx *spLIS2MDLObj = NULL;
//static AManagedTaskEx *spLIS2DU12Obj = NULL;
static AManagedTaskEx *spLSM6DSV16XObj = NULL;

//static AManagedTaskEx *spIIS2ICLXObj = NULL;
//static AManagedTaskEx *spIIS2DLPCObj = NULL;
//static AManagedTaskEx *spISM330DHCXObj = NULL;
//static AManagedTaskEx *spIIS3DWBObj = NULL;

/**
 * Sensor Listener used to print the data of the active sensor.
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
 * | E_POWER_MODE_SENSORS_ACTIVE    | E_POWER_MODE_SENSORS_ACTIVE  |
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

  MX_GPIO_PI0_Init();
  /* Allocate the task objects */
  spHelloWorldObj = HelloWorldTaskAlloc(&MX_GPIO_UBInitParams);
  spUtilObj = UtilTaskAlloc(&MX_TIM7InitParams, NULL);
//  spUtilObj = UtilTaskAlloc(&MX_TIM3InitParams, NULL);
//  spI2CBusObj = I2CBusTaskAlloc(&MX_I2C1InitParams);
  spSPIBusObj = SPIBusTaskAlloc(&MX_SPI2InitParams);
//  spHTS221Obj = HTS221TaskAlloc(&MX_GPIO_INT_HTSInitParams, NULL);
//  spSTTS22HObj = STTS22HTaskAlloc(&MX_GPIO_INT_STTInitParams, NULL);
//  spILPS22QSObj = ILPS22QSTaskAlloc(NULL, NULL);
//  spLPS22DFObj = LPS22DFTaskAlloc(&MX_GPIO_INT_LPSInitParams, NULL);
//  spLIS2MDLObj = LIS2MDLTaskAlloc(&MX_GPIO_INT_MAGInitParams, NULL);
//  spLIS2DU12Obj = LIS2DU12TaskAlloc(&MX_GPIO_INT1_ACCInitParams, &MX_GPIO_CS_ACCInitParams);
    spLSM6DSV16XObj = LSM6DSV16XTaskAlloc(&MX_GPIO_INT1_DSV16XInitParams, NULL, &MX_GPIO_CS_DSV16XInitParams);
//  spIIS2ICLXObj = IIS2ICLXTaskAlloc(&MX_GPIO_INT1_ICLXInitParams, &MX_GPIO_CS_ICLXInitParams);
//  spIIS2DLPCObj = IIS2DLPCTaskAlloc(&MX_GPIO_INT1_DLPCInitParams, &MX_GPIO_CS_DLPCInitParams);
//  spISM330DHCXObj = ISM330DHCXTaskAlloc(&MX_GPIO_INT1_DHCXInitParams, &MX_GPIO_INT2_DHCXInitParams, &MX_GPIO_CS_DHCXInitParams);
//  spIIS3DWBObj = IIS3DWBTaskAlloc(&MX_GPIO_INT1_DWBInitParams, &MX_GPIO_CS_DWBInitParams);

  /* Add the task object to the context. */
  xRes = ACAddTask(pAppContext, spHelloWorldObj);
  xRes = ACAddTask(pAppContext, (AManagedTask*)spUtilObj);
//  xRes = ACAddTask(pAppContext, (AManagedTask*)spI2CBusObj);
  xRes = ACAddTask(pAppContext, (AManagedTask*)spSPIBusObj);

//  xRes = ACAddTask(pAppContext, (AManagedTask*)spHTS221Obj);
//  xRes = ACAddTask(pAppContext, (AManagedTask*)spSTTS22HObj);
//  xRes = ACAddTask(pAppContext, (AManagedTask*)spILPS22QSObj);
//  xRes = ACAddTask(pAppContext, (AManagedTask*)spLPS22DFObj);
//  xRes = ACAddTask(pAppContext, (AManagedTask*)spLIS2MDLObj);
//  xRes = ACAddTask(pAppContext, (AManagedTask*)spLIS2DU12Obj);
    xRes = ACAddTask(pAppContext, (AManagedTask*)spLSM6DSV16XObj);
//  xRes = ACAddTask(pAppContext, (AManagedTask*)spIIS2ICLXObj);
//  xRes = ACAddTask(pAppContext, (AManagedTask*)spIIS2DLPCObj);
//  xRes = ACAddTask(pAppContext, (AManagedTask*)spISM330DHCXObj);
//  xRes = ACAddTask(pAppContext, (AManagedTask*)spIIS3DWBObj);

  // Allocate other application objects
  SensorListenerStaticAlloc(&sSensorListener);

  return xRes;
}

sys_error_code_t SysOnStartApplication(ApplicationContext *pAppContext)
{
  UNUSED(pAppContext);

  /* Re-map the state machine of the Sensor Manager tasks */
  SensorManagerStateMachineRemap(spAppPMState2SMPMStateMap);

  /* Disable the automatic low power mode timer */
  UtilTaskSetAutoLowPowerModePeriod((UtilTask_t*)spUtilObj, 0);
  /* Set the sensor listener used by the Util task.*/
  UtilTaskSetSensorListener((UtilTask_t*)spUtilObj, &sSensorListener);

  /* connect the sensors task to the I2C bus. */
//  I2CBusTaskConnectDevice((I2CBusTask*)spI2CBusObj, (I2CBusIF*)HTS221TaskGetSensorIF((HTS221Task*)spHTS221Obj));
//  I2CBusTaskConnectDevice((I2CBusTask*)spI2CBusObj, (I2CBusIF*)STTS22HTaskGetSensorIF((STTS22HTask*)spSTTS22HObj));
//  I2CBusTaskConnectDevice((I2CBusTask*)spI2CBusObj, (I2CBusIF*)ILPS22QSTaskGetSensorIF((ILPS22QSTask*)spILPS22QSObj));
//  I2CBusTaskConnectDevice((I2CBusTask*)spI2CBusObj, (I2CBusIF*)LPS22DFTaskGetSensorIF((LPS22DFTask*)spLPS22DFObj));
//  I2CBusTaskConnectDevice((I2CBusTask*)spI2CBusObj, (I2CBusIF*)LIS2MDLTaskGetSensorIF((LIS2MDLTask*)spLIS2MDLObj));
  /* connect the sensors task to the SPI bus. */
//  SPIBusTaskConnectDevice((SPIBusTask*)spSPIBusObj, (SPIBusIF*)LIS2DU12TaskGetSensorIF((LIS2DU12Task*)spLIS2DU12Obj));
  SPIBusTaskConnectDevice((SPIBusTask*)spSPIBusObj, (SPIBusIF*)LSM6DSV16XTaskGetSensorIF((LSM6DSV16XTask*)spLSM6DSV16XObj));
//  SPIBusTaskConnectDevice((SPIBusTask*)spSPIBusObj, (SPIBusIF*)IIS2DLPCTaskGetSensorIF((IIS2DLPCTask*)spIIS2DLPCObj));
//  SPIBusTaskConnectDevice((SPIBusTask*)spSPIBusObj, (SPIBusIF*)ISM330DHCXTaskGetSensorIF((ISM330DHCXTask*)spISM330DHCXObj));
//  SPIBusTaskConnectDevice((SPIBusTask*)spSPIBusObj, (SPIBusIF*)IIS3DWBTaskGetSensorIF((IIS3DWBTask*)spIIS3DWBObj));

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

static sys_error_code_t SensorManagerStateMachineRemap(EPowerMode *pPMState2PMStateMap)
{
  assert_param(pPMState2PMStateMap != NULL);

//  AMTSetPMStateRemapFunc((AManagedTask*)spI2CBusObj, pPMState2PMStateMap);
  AMTSetPMStateRemapFunc((AManagedTask*)spSPIBusObj, pPMState2PMStateMap);
//  AMTSetPMStateRemapFunc((AManagedTask*)spHTS221Obj, pPMState2PMStateMap);
//  AMTSetPMStateRemapFunc((AManagedTask*)spSTTS22HObj, pPMState2PMStateMap);
//  AMTSetPMStateRemapFunc((AManagedTask*)spILPS22QSObj, pPMState2PMStateMap);
//  AMTSetPMStateRemapFunc((AManagedTask*)spLPS22DFObj, pPMState2PMStateMap);
//  AMTSetPMStateRemapFunc((AManagedTask*)spLIS2MDLObj, pPMState2PMStateMap);
//  AMTSetPMStateRemapFunc((AManagedTask*)spLIS2DU12Obj, pPMState2PMStateMap);
  AMTSetPMStateRemapFunc((AManagedTask*)spLSM6DSV16XObj, pPMState2PMStateMap);
//  AMTSetPMStateRemapFunc((AManagedTask*)spIIS2DLPCObj, pPMState2PMStateMap);
//  AMTSetPMStateRemapFunc((AManagedTask*)spISM330DHCXObj, pPMState2PMStateMap);
//  AMTSetPMStateRemapFunc((AManagedTask*)spIIS3DWBObj, pPMState2PMStateMap);
  AMTSetPMStateRemapFunc((AManagedTask*)spUtilObj, pPMState2PMStateMap);

  return SYS_NO_ERROR_CODE;
}



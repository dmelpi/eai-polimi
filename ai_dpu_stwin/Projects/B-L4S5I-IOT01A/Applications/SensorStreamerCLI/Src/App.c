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
#include "I2CBusTask.h"
#include "LPS22HBTask.h"
#include "StreamerCLI.h"
#include "SCLIUtilTask.h"
#include "services/SQuery.h"
#include "mx.h"


/**
 * I2C bus task object.
 */
static AManagedTaskEx *spI2CBusObj = NULL;

/**
 * Sensor task object.
 */
static AManagedTaskEx *spLPS22HBObj = NULL;

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

static sys_error_code_t StreamerCLISensorsConfigF(SensorManager_t *p_sm, uint16_t *p_active_sensor_id)
{
  assert_param(p_sm != NULL);
  assert_param(p_active_sensor_id != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  uint16_t sensor_id;

  SIterator_t itarator;
  sensor_id = SI_NULL_SENSOR_ID;
  SIInit(&itarator, p_sm);
  while (SIHasNext(&itarator))
  {
    sensor_id = SINext(&itarator);
    SMSensorDisable(sensor_id);
  }

  SQuery_t query;
  SQInit(&query, p_sm);
  sensor_id = SQNextByNameAndType(&query, "lps22hb",  COM_TYPE_TEMP);
  if (sensor_id != SI_NULL_SENSOR_ID)
  {
    SMSensorEnable(sensor_id);
    SMSensorSetODR(sensor_id, 10);
    SMSensorSetFS(sensor_id, 1260.0);
  }

  *p_active_sensor_id = sensor_id;

  return res;
}

/* eLooM framework entry points definition */
/*******************************************/

sys_error_code_t SysLoadApplicationContext(ApplicationContext *pAppContext)
{
  assert_param(pAppContext);
  sys_error_code_t xRes = SYS_NO_ERROR_CODE;

  // Allocate the task objects
  spI2CBusObj = I2CBusTaskAlloc(&MX_I2C2InitParams);
  spLPS22HBObj = LPS22HBTaskAlloc(&MX_GPIO_PD10InitParams, NULL, I2C_IF);
  spUtilObj = SCLIUtilTaskAlloc(&MX_GPIO_PA5InitParams, NULL);
  spStremaerCLIObj = StreamerCLIAlloc();
  spHWObj = HelloWorldTaskAlloc(&MX_GPIO_PC13InitParams);

  // Add the task object to the context.
  xRes = ACAddTask(pAppContext, (AManagedTask*)spI2CBusObj);
  xRes = ACAddTask(pAppContext, (AManagedTask*)spLPS22HBObj);
  xRes = ACAddTask(pAppContext, (AManagedTask*)spUtilObj);
  xRes = ACAddTask(pAppContext, (AManagedTask*)spStremaerCLIObj);
  xRes = ACAddTask(pAppContext, spHWObj);

  return xRes;
}

sys_error_code_t SysOnStartApplication(ApplicationContext *pAppContext) {
  UNUSED(pAppContext);

  /* Disable the automatic low power mode timer */
  SCLIUtilTaskSetAutoLowPowerModePeriod((SCLIUtilTask_t*)spUtilObj, 0);

  /* connect the sensors task to the I2C bus. */
  I2CBusTaskConnectDevice((I2CBusTask*)spI2CBusObj, (I2CBusIF*)LPS22HBTaskGetSensorIF((LPS22HBTask*)spLPS22HBObj));

  StreamerCLISetDefSensorsConfig((StreamerCLI_t*)spStremaerCLIObj, StreamerCLISensorsConfigF);

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


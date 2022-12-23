/**
 ******************************************************************************
 * @file    app_model.c
 * @author  STMicroelectronics
 * @version $Version$
 * @date    $Date$
 *
 * @brief   Define the Application main entry points
 *
 * The framework `weak` function are redefined in this file and they link
 * the application specific code with the framework.
 *
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include "App_model.h"
#include "services/SQuery.h"
#include "parson.h"
#include <string.h>
#include <stdio.h>

AppModel_t app_model;

AppModel_t* getAppModel(void)
{
  return &app_model;
}

//AI_APPLICATION Component ==========================================================================================
//uint8_t <comp_name>_comp_init
uint8_t ai_application_comp_init(void)
{
  return 0;
}

char* ai_application_get_key(void)
{
  return "ai_application";
}

//uint8_t <comp_name>_<command_name>(I<Compname>_t * ifn, field1_type field1_name, field2_type field2_name, ..., fieldN_type fieldN_name); /ifn: Interface Functions
uint8_t ai_application_stop_log(IAiApplication_t *ifn)
{
  IAiApplication_stop(ifn);
  return 0;
}

uint8_t ai_application_start_log(IAiApplication_t *ifn)
{
  IAiApplication_start(ifn);
  return 0;
}

uint8_t ai_application_switch_bank(IAiApplication_t *ifn)
{
  IAiApplication_switch_bank(ifn);
  return 0;
}
////=================================================================================================================

//FWINFO Component ==================================================================================================

//////INIT Function
//uint8_t <comp_name>_comp_init
uint8_t fwinfo_comp_init(void)
{
  app_model.fwinfo_model.comp_name = fwinfo_get_key();
  //insert here your initialization code
  char default_alias[DEVICE_ALIAS_LENGTH] = "AI_Demo_001";
  fwinfo_set_alias(default_alias);
  return 0;
}

//////GET KEY Function
//uint8_t <comp_name>_get_key
char* fwinfo_get_key(void) //AUTOMATICALLY IMPLEMENTED in .c --> will return the DTDL Component name: "fwinfo"
{
  return "fwinfo";
}

//////GET Functions
//uint8_t <comp_name>_get_<prop_name>(prop_type *prop_value)
uint8_t fwinfo_get_alias(char **value)
{
  *value = app_model.fwinfo_model.alias;
  return 0;
}

uint8_t fwinfo_get_fw_name(char **value)
{
  *value = "PnPL_AI_Demo";
  return 0;
}

uint8_t fwinfo_get_fw_version(char **value)
{
  *value = "1.0.0";
  return 0;
}

uint8_t fwinfo_get_serial_number(char **value)
{
  *value = "STEVAL-STWINBX1";
  return 0;
}

uint8_t fwinfo_get_device_url(char **value)
{
  *value = "www.st.com/stwinbox";
  return 0;
}

uint8_t fwinfo_get_fw_url(char **value)
{
  *value = "www.st.com/dummy_place"; //TODO fix this value
  return 0;
}

//////SET Functions
//uint8_t <comp_name>_set_<prop_name>(prop_type prop_value)
uint8_t fwinfo_set_alias(const char *value)
{
  strcpy(app_model.fwinfo_model.alias, value);
  return 0;
}
////=================================================================================================================

//DEVICEINFO Component ==========================================================================================
//////INIT Function
//uint8_t <comp_name>_comp_init
uint8_t deviceinfo_comp_init(void)
{
  return 0;
}

//////GET KEY Function
//uint8_t <comp_name>_get_key
char* deviceinfo_get_key(void) //AUTOMATICALLY IMPLEMENTED in .c --> will return the DTDL Component name: "deviceinfo"
{
  return "deviceinfo";
}
//////GET Functions
//uint8_t <comp_name>_get_<prop_name>(prop_type *prop_value)
uint8_t deviceinfo_get_manufacturer(char **value)
{
  *value = "STMicroelectronics";
  return 0;
}
uint8_t deviceinfo_get_model(char **value)
{
  *value = "STEVAL-STWINBX1";
  return 0;
}
uint8_t deviceinfo_get_swVersion(char **value)
{
  *value = "1.0.0";
  return 0;
}
uint8_t deviceinfo_get_osName(char **value)
{
  *value = "AzureRTOS";
  return 0;
}

uint8_t deviceinfo_get_processorArchitecture(char **value)
{
  *value = "ARM Cortex-M33";
  return 0;
}

uint8_t deviceinfo_get_processorManufacturer(char **value)
{
  *value = "STMicroelectronics";
  return 0;
}

uint8_t deviceinfo_get_totalStorage(uint32_t *value)
{
  *value = 0;
  return 0;
}

uint8_t deviceinfo_get_totalMemory(uint32_t *value)
{
  *value = 0;
  return 0;
}
////=================================================================================================================

////Sensor Components
////ISM330DHCX APIs =================================================================================================
//ISM330DHCX ACC Component Model ====================================================================================
SensorModel_t ism330dhcx_acc_model;
//===================================================================================================================
//////INIT Function
//uint8_t <comp_name>_comp_init
uint8_t ism330dhcx_acc_comp_init(void)
{
  SQuery_t querySM;
  SQInit(&querySM, SMGetSensorManager());
  uint16_t id = SQNextByNameAndType(&querySM, "ism330dhcx", COM_TYPE_ACC);
  ism330dhcx_acc_model.id = id;
  ism330dhcx_acc_model.comp_name = ism330dhcx_acc_get_key();
  ism330dhcx_acc_model.sensorStatus = SMSensorGetStatus(id);
  ism330dhcx_acc_model.streamParams.stream_id = -1;
  ism330dhcx_acc_model.streamParams.usb_ep = -1;
  //User code

  app_model.s_models[id] = &ism330dhcx_acc_model;
  return 0;
}

//////GET KEY Function
//uint8_t <comp_name>_get_key
char* ism330dhcx_acc_get_key(void) //AUTOMATICALLY IMPLEMENTED in .c --> will return the DTDL Component name: "ism330dhcx_acc"
{
  return "ism330dhcx_acc";
}

//////GET Functions
//uint8_t <comp_name>_get_<prop_name>(prop_type *prop_value)
uint8_t ism330dhcx_acc_get_odr(float *value)
{
  //Status update to check if the value has been updated by the FW
  uint16_t id = ism330dhcx_acc_model.id;
  ism330dhcx_acc_model.sensorStatus = SMSensorGetStatus(id);
  *value = ism330dhcx_acc_model.sensorStatus.ODR;
  return 0;
}

uint8_t ism330dhcx_acc_get_fs(float *value)
{
  *value = ism330dhcx_acc_model.sensorStatus.FS;
  return 0;
}

uint8_t ism330dhcx_acc_get_enable(bool *value)
{
  //Status update to check if the value has been updated by the FW
  uint16_t id = ism330dhcx_acc_model.id;
  ism330dhcx_acc_model.sensorStatus = SMSensorGetStatus(id);
  *value = ism330dhcx_acc_model.sensorStatus.IsActive;
  return 0;
}

uint8_t ism330dhcx_acc_get_dim(uint32_t *value)
{
  *value = ism330dhcx_acc_model.sensorStatus.Dimensions;
  return 0;
}

uint8_t ism330dhcx_acc_get_data_type(char **value)
{
  *value = "int16";
  return 0;
}

uint8_t ism330dhcx_acc_get_sensitivity(float *value)
{
  //Status update to check if the value has been updated by the FW
  uint16_t id = ism330dhcx_acc_model.id;
  ism330dhcx_acc_model.sensorStatus = SMSensorGetStatus(id);
  *value = ism330dhcx_acc_model.sensorStatus.Sensitivity;
  return 0;
}

//////SET Functions
//uint8_t <comp_name>_set_<prop_name>(prop_type prop_value)
uint8_t ism330dhcx_acc_set_odr(float value)
{
  sys_error_code_t ret = SMSensorSetODR(ism330dhcx_acc_model.id, value);
  if(ret == SYS_NO_ERROR_CODE)
  {
    ism330dhcx_acc_model.sensorStatus.ODR = value;
    return ret;
  }
  return 1;
}

uint8_t ism330dhcx_acc_set_fs(float value)
{
  sys_error_code_t ret = SMSensorSetFS(ism330dhcx_acc_model.id, value);
  if(ret == SYS_NO_ERROR_CODE)
  {
    ism330dhcx_acc_model.sensorStatus.FS = value;
    return ret;
  }
  return 1;
}

uint8_t ism330dhcx_acc_set_enable(bool value)
{
  sys_error_code_t ret = 1;
  if(value)
  {
    ret = SMSensorEnable(ism330dhcx_acc_model.id);
  }
  else
  {
    ret = SMSensorDisable(ism330dhcx_acc_model.id);
  }
  if(ret == SYS_NO_ERROR_CODE)
  {
    ism330dhcx_acc_model.sensorStatus.IsActive = value;
  }
  return ret;
}
//=================================================================================================================


//COMP_NAME Component Model =========================================================================================
////if the component is a Sensor (look @ component schema field)
//////SensorModel_t <comp_name>_model
//===================================================================================================================
//COMP_NAME get_key function ========================================================================================
//Automatically generated
//===================================================================================================================
//COMP_NAME APIs ====================================================================================================
////INIT Function (filled by user)
//////uint8_t <comp_name>_comp_init()
////GET Functions (filled by user)
//////uint8_t <comp_name>_get_<prop_name>(prop_type *value)
////SET Functions (filled by user)
//////uint8_t <comp_name>_set_<prop_name>(prop_type value)
//===================================================================================================================

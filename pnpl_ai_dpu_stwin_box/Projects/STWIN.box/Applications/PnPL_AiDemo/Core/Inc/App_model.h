/**
 ******************************************************************************
 * @file    app_model.h
 * @author  STMicroelectronics - SRA Team
 * @version 1.0.0
 * @date    Feb 02, 2022
 *
 * @brief App Application Model and PnPL Components APIs
 *
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
#ifndef APP_MODEL_H_
#define APP_MODEL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "stdbool.h"
#include "parson.h"
#include "PnPLCompManager.h"
#include "SensorManager.h"
#include "IAiApplication.h"
#include "IAiApplication_vtbl.h"

#define COMP_TYPE_SENSOR			  0x00
#define COMP_TYPE_ALGORITHM			0x01
#define COMP_TYPE_OTHER				  0x02

#define LOG_CTRL_MODE_SD        0x00
#define LOG_CTRL_MODE_USB       0x01
#define LOG_CTRL_MODE_BLE       0x02

#define SENSOR_NUMBER				    1
#define ALGORITHM_NUMBER			  0
#define OTHER_COMP_NUMBER			  3

#define HSD_ACQ_NAME_LENGTH     64U
#define HSD_ACQ_DESC_LENGTH     100U

#define FW_VERSION_MAJOR	'1'
#define FW_VERSION_MINOR	'0'
#define FW_VERSION_PATCH	'0'

#define N_MAX_EP 5

typedef struct _StreamParams_t
{
  int8_t stream_id;
  int8_t usb_ep;
  uint16_t spts;
  uint32_t usb_dps;
  uint32_t sd_dps;
  float ioffset;
  float bandwidth;
} StreamParams_t;

typedef struct _SensorModel_t
{ //E.g. IIS3DWB Component is a sensor (look @ schema field) so, its model has the following structure
  uint8_t id;
  char *comp_name;
  StreamParams_t streamParams;
  SensorStatus_t sensorStatus;
} SensorModel_t;

typedef struct _AlgorithmModel_t
{ //E.g. FFT_Algo Component is an algorithm (look @ schema field) so, its model has the following structure
  uint8_t id;
  char *comp_name;
  StreamParams_t streamParams;
} AlgorithmModel_t;

#define DEVICE_ALIAS_LENGTH     16U
typedef struct _FwInfoModel_t
{
  char *comp_name;
  //Insert here your Firmware Info model code
  char alias[DEVICE_ALIAS_LENGTH];
} FwInfoModel_t;

typedef struct _AppModel_t
{
  SensorModel_t *s_models[SENSOR_NUMBER];
#if (ALGORITHM_NUMBER != 0)
  AlgorithmModel_t *a_models[ALGORITHM_NUMBER];       /* Generated only when algorithms are available */
#endif
  FwInfoModel_t fwinfo_model;
} AppModel_t;

AppModel_t* getAppModel(void);

//AI Application Component ==============================================================================================

//////INIT Function
//uint8_t <comp_name>_comp_init
uint8_t ai_application_comp_init(void);

//////GET KEY Function
//uint8_t <comp_name>_get_key
char* ai_application_get_key(void); //AUTOMATICALLY IMPLEMENTED in .c --> will return the DTDL Component name: "ai_application"

////COMMAND Reaction Functions
//uint8_t <comp_name>_<command_name>(I<Compname>_t * ifn, field1_type field1_name, field2_type field2_name, ..., fieldN_type fieldN_name); /ifn: Interface Functions
uint8_t ai_application_stop_log(IAiApplication_t *ifn);
uint8_t ai_application_start_log(IAiApplication_t *ifn);
uint8_t ai_application_switch_bank(IAiApplication_t *ifn);
//===================================================================================================================

//Fwinfo Component ==================================================================================================

//////INIT Function
//uint8_t <comp_name>_comp_init
uint8_t fwinfo_comp_init(void);

//////GET KEY Function
//uint8_t <comp_name>_get_key
char *fwinfo_get_key(void); //AUTOMATICALLY IMPLEMENTED in .c --> will return the DTDL Component name: "fwinfo"

//////GET Functions
//uint8_t <comp_name>_get_<prop_name>(prop_type *prop_value)
uint8_t fwinfo_get_alias(char **value);
uint8_t fwinfo_get_fw_name(char **value);
uint8_t fwinfo_get_fw_version(char **value);
uint8_t fwinfo_get_serial_number(char **value);
uint8_t fwinfo_get_device_url(char **value);
uint8_t fwinfo_get_fw_url(char **value);

//////SET Functions
//uint8_t <comp_name>_set_<prop_name>(prop_type prop_value)
uint8_t fwinfo_set_alias(const char *value);
//===================================================================================================================

//Deviceinfo Component ==============================================================================================

//////INIT Function
//uint8_t <comp_name>_comp_init
uint8_t deviceinfo_comp_init(void);

//////GET KEY Function
//uint8_t <comp_name>_get_key
char* deviceinfo_get_key(void); //AUTOMATICALLY IMPLEMENTED in .c --> will return the DTDL Component name: "deviceinfo"

//////GET Functions
//uint8_t <comp_name>_get_<prop_name>(prop_type *prop_value)
uint8_t deviceinfo_get_manufacturer(char **value);
uint8_t deviceinfo_get_model(char **value);
uint8_t deviceinfo_get_swVersion(char **value);
uint8_t deviceinfo_get_osName(char **value);
uint8_t deviceinfo_get_processorArchitecture(char **value);
uint8_t deviceinfo_get_processorManufacturer(char **value);
uint8_t deviceinfo_get_totalStorage(uint32_t *value);
uint8_t deviceinfo_get_totalMemory(uint32_t *value);

//////SET Functions
//uint8_t <comp_name>_set_<prop_name>(prop_type prop_value)
//===================================================================================================================

//ISM330DHCX ACC e GYRO Component ===================================================================================

//////INIT Function
//uint8_t <comp_name>_comp_init
uint8_t ism330dhcx_acc_comp_init(void);

//////GET KEY Function
//uint8_t <comp_name>_get_key
char* ism330dhcx_acc_get_key(void); //AUTOMATICALLY IMPLEMENTED in .c --> will return the DTDL Component name: "ism330dhcx_acc"

//////GET Functions
//uint8_t <comp_name>_get_<prop_name>(prop_type *prop_value)
uint8_t ism330dhcx_acc_get_odr(float *value);
uint8_t ism330dhcx_acc_get_fs(float *value);

//////SET Functions
//uint8_t <comp_name>_set_<prop_name>(prop_type prop_value)
uint8_t ism330dhcx_acc_set_odr(float value);
uint8_t ism330dhcx_acc_set_fs(float value);

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
////GET KEY Function
/////uint8_t <comp_name>_get_key
////GET Functions (filled by user)
//////uint8_t <comp_name>_get_<prop_name>(prop_type *value)
////SET Functions (filled by user)
//////uint8_t <comp_name>_set_<prop_name>(prop_type value)
////COMMAND Reaction Functions
//////uint8_t <comp_name>_<command_name>(I<Compname>_t * ifn, field1_type field1_name, field2_type field2_name, ..., fieldN_type fieldN_name); /ifn: Interface Functions
//===================================================================================================================

#ifdef __cplusplus
}
#endif

#endif /* APP_MODEL_H_ */

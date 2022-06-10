#include "IIS3DWB_ACC_PnPL.h"
#include "IIS3DWB_ACC_PnPL_vtbl.h"
#include "IPnPLComponent.h"
#include "IPnPLComponent_vtbl.h"
#include "PnPLCompManager.h"
#include <string.h>

#include "App_model.h"

static const IPnPLComponent_vtbl sIIS3DWB_ACC_PnPL_CompIF_vtbl =
{
    IIS3DWB_ACC_PnPL_vtblGetKey,
    IIS3DWB_ACC_PnPL_vtblGetNCommands,
    IIS3DWB_ACC_PnPL_vtblGetCommandKey,
    IIS3DWB_ACC_PnPL_vtblGetStatus,
    IIS3DWB_ACC_PnPL_vtblSetProperty,
    IIS3DWB_ACC_PnPL_vtblExecuteFunction };

/**
 *  IIS3DWB_ACC_PnPL internal structure.
 */
struct _IIS3DWB_ACC_PnPL
{

  /**
   * Implements the IPnPLComponent interface.
   */
  IPnPLComponent_t component_if;

};

/* Objects instance */
/********************/

static IIS3DWB_ACC_PnPL sIIS3DWB_ACC_bObj;

// Public API definition
// *********************

IPnPLComponent_t* IIS3DWB_ACC_PnPLAlloc()
{
  IPnPLComponent_t *pxObj = (IPnPLComponent_t*) &sIIS3DWB_ACC_bObj;
  if(pxObj != NULL)
  {
    pxObj->vptr = &sIIS3DWB_ACC_PnPL_CompIF_vtbl;
  }
  return pxObj;
}

uint8_t IIS3DWB_ACC_PnPLInit(IPnPLComponent_t *_this)
{
  IPnPLComponent_t *component_if = _this;
  PnPLAddComponent(component_if);
  iis3dwb_acc_comp_init();
  return 0;
}

// IPnPLComponent virtual functions definition
// *******************************************

char* IIS3DWB_ACC_PnPL_vtblGetKey(IPnPLComponent_t *_this)
{
  return iis3dwb_acc_get_key();
}

uint8_t IIS3DWB_ACC_PnPL_vtblGetNCommands(IPnPLComponent_t *_this)
{
  return 0;
}

char* IIS3DWB_ACC_PnPL_vtblGetCommandKey(IPnPLComponent_t *_this, uint8_t id)
{
  return "";
}

uint8_t IIS3DWB_ACC_PnPL_vtblGetStatus(IPnPLComponent_t *_this, char **serializedJSON, uint32_t *size, uint8_t pretty)
{
  JSON_Value *tempJSON;
  JSON_Object *JSON_Status;

  tempJSON = json_value_init_object();
  JSON_Status = json_value_get_object(tempJSON);

  float temp = 0;
  iis3dwb_acc_get_odr(&temp);
  json_object_dotset_number(JSON_Status, "iis3dwb_acc.odr", temp);
  iis3dwb_acc_get_fs(&temp);
  json_object_dotset_number(JSON_Status, "iis3dwb_acc.fs", temp);
  bool temp_b = 0;
  iis3dwb_acc_get_enable(&temp_b);
  json_object_dotset_boolean(JSON_Status, "iis3dwb_acc.enable", temp_b);
  uint32_t temp_i = 0;
  iis3dwb_acc_get_samples_per_ts(&temp_i);
  json_object_dotset_number(JSON_Status, "iis3dwb_acc.samples_per_ts", temp_i);
  iis3dwb_acc_get_dim(&temp_i);
  json_object_dotset_number(JSON_Status, "iis3dwb_acc.dim", temp_i);
  iis3dwb_acc_get_ioffset(&temp);
  json_object_dotset_number(JSON_Status, "iis3dwb_acc.ioffset", temp);
  iis3dwb_acc_get_measodr(&temp);
  json_object_dotset_number(JSON_Status, "iis3dwb_acc.measodr", temp);
  char *temp_s = "";
  iis3dwb_acc_get_data_type(&temp_s);
  json_object_dotset_string(JSON_Status, "iis3dwb_acc.data_type", temp_s);
  iis3dwb_acc_get_usb_dps(&temp_i);
  json_object_dotset_number(JSON_Status, "iis3dwb_acc.usb_dps", temp_i);
  iis3dwb_acc_get_sd_dps(&temp_i);
  json_object_dotset_number(JSON_Status, "iis3dwb_acc.sd_dps", temp_i);
  iis3dwb_acc_get_sensitivity(&temp);
  json_object_dotset_number(JSON_Status, "iis3dwb_acc.sensitivity", temp);
  json_object_dotset_number(JSON_Status, "iis3dwb_acc.c_type", COMP_TYPE_SENSOR); //Added looking @the component schema field (this is :sensor)
  int8_t temp_int8 = 0;
  iis3dwb_acc_get_stream_id(&temp_int8);
  json_object_dotset_number(JSON_Status, "iis3dwb_acc.stream_id", temp_int8); //Not in DTDL but Added looking @the component schema field (this is :sensor) ONLY for Sensors and Algorithms
  iis3dwb_acc_get_ep_id(&temp_int8);
  json_object_dotset_number(JSON_Status, "iis3dwb_acc.ep_id", temp_int8); //Not in DTDL but Added looking @the component schema field (this is :sensor) ONLY for Sensors and Algorithms

  if(pretty == 1)
  {
    *serializedJSON = json_serialize_to_string_pretty(tempJSON);
    *size = json_serialization_size_pretty(tempJSON);
  }
  else
  {
    *serializedJSON = json_serialize_to_string(tempJSON);
    *size = json_serialization_size(tempJSON);
  }

  json_value_free(tempJSON);

  return 0;
}

uint8_t IIS3DWB_ACC_PnPL_vtblSetProperty(IPnPLComponent_t *_this, char *serializedJSON)
{

  JSON_Value *tempJSON = json_parse_string(serializedJSON);
  JSON_Object *tempJSONObject = json_value_get_object(tempJSON);

  uint8_t ret = 0;

  //Only one ODR value supported! --> no SET ODR function
  if(json_object_dothas_value(tempJSONObject, "iis3dwb_acc.fs"))
  {
    float fs = (float) json_object_dotget_number(tempJSONObject, "iis3dwb_acc.fs"); //FS is an Enum --> value = float to contain all possible numeric values
    iis3dwb_acc_set_fs(fs);
  }
  if(json_object_dothas_value(tempJSONObject, "iis3dwb_acc.enable"))
  {
    bool enable = json_object_dotget_boolean(tempJSONObject, "iis3dwb_acc.enable");
    iis3dwb_acc_set_enable(enable);
  }
  if(json_object_dothas_value(tempJSONObject, "iis3dwb_acc.samples_per_ts"))
  {
    uint32_t samples_per_ts = (uint32_t) json_object_dotget_number(tempJSONObject, "iis3dwb_acc.samples_per_ts");
    iis3dwb_acc_set_samples_per_ts(samples_per_ts);
  }

  json_value_free(tempJSON);
  return ret;
}

uint8_t IIS3DWB_ACC_PnPL_vtblExecuteFunction(IPnPLComponent_t *_this, char *serializedJSON)
{
  return 1;
}

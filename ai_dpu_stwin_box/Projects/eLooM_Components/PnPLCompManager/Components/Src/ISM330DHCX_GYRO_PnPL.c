#include "ISM330DHCX_GYRO_PnPL.h"
#include "ISM330DHCX_GYRO_PnPL_vtbl.h"
#include "IPnPLComponent.h"
#include "IPnPLComponent_vtbl.h"
#include "PnPLCompManager.h"
#include <string.h>

#include "App_model.h"

static const IPnPLComponent_vtbl sISM330DHCX_GYRO_PnPL_CompIF_vtbl =
{
    ISM330DHCX_GYRO_PnPL_vtblGetKey,
    ISM330DHCX_GYRO_PnPL_vtblGetNCommands,
    ISM330DHCX_GYRO_PnPL_vtblGetCommandKey,
    ISM330DHCX_GYRO_PnPL_vtblGetStatus,
    ISM330DHCX_GYRO_PnPL_vtblSetProperty,
    ISM330DHCX_GYRO_PnPL_vtblExecuteFunction };

/**
 *  ISM330DHCX_GYRO_PnPL internal structure.
 */
struct _ISM330DHCX_GYRO_PnPL
{

  /**
   * Implements the IPnPLComponent interface.
   */
  IPnPLComponent_t component_if;

};

/* Objects instance */
/********************/
static ISM330DHCX_GYRO_PnPL sISM330DHCX_GYRO_bObj;

// Public API definition
// *********************

IPnPLComponent_t* ISM330DHCX_GYRO_PnPLAlloc()
{
  IPnPLComponent_t *pxObj = (IPnPLComponent_t*) &sISM330DHCX_GYRO_bObj;
  if(pxObj != NULL)
  {
    pxObj->vptr = &sISM330DHCX_GYRO_PnPL_CompIF_vtbl;
  }
  return pxObj;
}

uint8_t ISM330DHCX_GYRO_PnPLInit(IPnPLComponent_t *_this)
{
  IPnPLComponent_t *component_if = _this;
  PnPLAddComponent(component_if);
  ism330dhcx_gyro_comp_init();
  return 0;
}

// IPnPLComponent virtual functions definition
// *******************************************

char* ISM330DHCX_GYRO_PnPL_vtblGetKey(IPnPLComponent_t *_this)
{
  return ism330dhcx_gyro_get_key();
}

uint8_t ISM330DHCX_GYRO_PnPL_vtblGetNCommands(IPnPLComponent_t *_this)
{
  return 0;
}

char* ISM330DHCX_GYRO_PnPL_vtblGetCommandKey(IPnPLComponent_t *_this, uint8_t id)
{
  return "";
}

uint8_t ISM330DHCX_GYRO_PnPL_vtblGetStatus(IPnPLComponent_t *_this, char **serializedJSON, uint32_t *size, uint8_t pretty)
{
  JSON_Value *tempJSON;
  JSON_Object *JSON_Status;

  tempJSON = json_value_init_object();
  JSON_Status = json_value_get_object(tempJSON);

  float temp = 0;
  ism330dhcx_gyro_get_odr(&temp);
  json_object_dotset_number(JSON_Status, "ism330dhcx_gyro.odr", temp);
  ism330dhcx_gyro_get_fs(&temp);
  json_object_dotset_number(JSON_Status, "ism330dhcx_gyro.fs", temp);
  bool temp_b = 0;
  ism330dhcx_gyro_get_enable(&temp_b);
  json_object_dotset_boolean(JSON_Status, "ism330dhcx_gyro.enable", temp_b);
  uint32_t temp_i = 0;
  ism330dhcx_gyro_get_samples_per_ts(&temp_i);
  json_object_dotset_number(JSON_Status, "ism330dhcx_gyro.samples_per_ts", temp_i);
  ism330dhcx_gyro_get_dim(&temp_i);
  json_object_dotset_number(JSON_Status, "ism330dhcx_gyro.dim", temp_i);
  ism330dhcx_gyro_get_ioffset(&temp);
  json_object_dotset_number(JSON_Status, "ism330dhcx_gyro.ioffset", temp);
  ism330dhcx_gyro_get_measodr(&temp);
  json_object_dotset_number(JSON_Status, "ism330dhcx_gyro.measodr", temp);
  char *temp_s;
  ism330dhcx_gyro_get_data_type(&temp_s);
  json_object_dotset_string(JSON_Status, "ism330dhcx_gyro.data_type", temp_s);
  ism330dhcx_gyro_get_usb_dps(&temp_i);
  json_object_dotset_number(JSON_Status, "ism330dhcx_gyro.usb_dps", temp_i);
  ism330dhcx_gyro_get_sd_dps(&temp_i);
  json_object_dotset_number(JSON_Status, "ism330dhcx_gyro.sd_dps", temp_i);
  ism330dhcx_gyro_get_sensitivity(&temp);
  json_object_dotset_number(JSON_Status, "ism330dhcx_gyro.sensitivity", temp);
  json_object_dotset_number(JSON_Status, "ism330dhcx_gyro.c_type", COMP_TYPE_SENSOR); //Added looking @the component schema field (this is :sensor)
  int8_t temp_int8 = 0;
  ism330dhcx_gyro_get_stream_id(&temp_int8);
  json_object_dotset_number(JSON_Status, "ism330dhcx_gyro.stream_id", temp_int8); //Not in DTDL but Added looking @the component schema field (this is :sensor) ONLY for Sensors and Algorithms
  ism330dhcx_gyro_get_ep_id(&temp_int8);
  json_object_dotset_number(JSON_Status, "ism330dhcx_gyro.ep_id", temp_int8); //Not in DTDL but Added looking @the component schema field (this is :sensor) ONLY for Sensors and Algorithms

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

uint8_t ISM330DHCX_GYRO_PnPL_vtblSetProperty(IPnPLComponent_t *_this, char *serializedJSON)
{

  JSON_Value *tempJSON = json_parse_string(serializedJSON);
  JSON_Object *tempJSONObject = json_value_get_object(tempJSON);

  uint8_t ret = 0;

  if(json_object_dothas_value(tempJSONObject, "ism330dhcx_gyro.odr"))
  {
    float odr = (float) (json_object_dotget_number(tempJSONObject, "ism330dhcx_gyro.odr"));
    ism330dhcx_gyro_set_odr(odr);
  }
  if(json_object_dothas_value(tempJSONObject, "ism330dhcx_gyro.fs"))
  {
    float fs = (float) json_object_dotget_number(tempJSONObject, "ism330dhcx_gyro.fs"); //FS is an Enum --> value = float to contain all possible numeric values
    ism330dhcx_gyro_set_fs(fs);
  }
  if(json_object_dothas_value(tempJSONObject, "ism330dhcx_gyro.enable"))
  {
    bool enable = json_object_dotget_boolean(tempJSONObject, "ism330dhcx_gyro.enable");
    ism330dhcx_gyro_set_enable(enable);
  }
  if(json_object_dothas_value(tempJSONObject, "ism330dhcx_gyro.samples_per_ts"))
  {
    uint32_t samples_per_ts = (uint32_t) json_object_dotget_number(tempJSONObject, "ism330dhcx_gyro.samples_per_ts");
    ism330dhcx_gyro_set_samples_per_ts(samples_per_ts);
  }

  json_value_free(tempJSON);
  return ret;
}

uint8_t ISM330DHCX_GYRO_PnPL_vtblExecuteFunction(IPnPLComponent_t *_this, char *serializedJSON)
{
  return 1;
}
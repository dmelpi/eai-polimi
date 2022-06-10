#include "HTS221_TEMP_PnPL.h"
#include "HTS221_TEMP_PnPL_vtbl.h"
#include "IPnPLComponent.h"
#include "IPnPLComponent_vtbl.h"
#include "PnPLCompManager.h"
#include <string.h>

#include "App_model.h"

static const IPnPLComponent_vtbl sHTS221_TEMP_PnPL_CompIF_vtbl =
{
    HTS221_TEMP_PnPL_vtblGetKey,
    HTS221_TEMP_PnPL_vtblGetNCommands,
    HTS221_TEMP_PnPL_vtblGetCommandKey,
    HTS221_TEMP_PnPL_vtblGetStatus,
    HTS221_TEMP_PnPL_vtblSetProperty,
    HTS221_TEMP_PnPL_vtblExecuteFunction, };

/**
 *  HTS221_TEMP_PnPL internal structure.
 */
struct _HTS221_TEMP_PnPL
{

  /**
   * Implements the IPnPLComponent interface.
   */
  IPnPLComponent_t component_if;

};

/* Objects instance */
/********************/

static HTS221_TEMP_PnPL sHTS221_TEMP_bObj;

// Public API definition
// *********************

IPnPLComponent_t* HTS221_TEMP_PnPLAlloc()
{

  IPnPLComponent_t *pxObj = (IPnPLComponent_t*) &sHTS221_TEMP_bObj;
  if(pxObj != NULL)
  {
    pxObj->vptr = &sHTS221_TEMP_PnPL_CompIF_vtbl;
  }
  return pxObj;
}

uint8_t HTS221_TEMP_PnPLInit(IPnPLComponent_t *_this)
{
  IPnPLComponent_t *component_if = _this;
  PnPLAddComponent(component_if);
  hts221_temp_comp_init();

  return 0;
}

// IPnPLComponent virtual functions definition
// *******************************************

char* HTS221_TEMP_PnPL_vtblGetKey(IPnPLComponent_t *_this)
{
  return hts221_temp_get_key();
}

uint8_t HTS221_TEMP_PnPL_vtblGetNCommands(IPnPLComponent_t *_this)
{
  return 0;
}

char* HTS221_TEMP_PnPL_vtblGetCommandKey(IPnPLComponent_t *_this, uint8_t id)
{
  return "";
}

uint8_t HTS221_TEMP_PnPL_vtblGetStatus(IPnPLComponent_t *_this, char **serializedJSON, uint32_t *size, uint8_t pretty)
{
  JSON_Value *tempJSON;
  JSON_Object *JSON_Status;

  tempJSON = json_value_init_object();
  JSON_Status = json_value_get_object(tempJSON);

  float temp = 0;
  hts221_temp_get_odr(&temp);
  json_object_dotset_number(JSON_Status, "hts221_temp.odr", temp); //hum ODR is(must be) the same
  hts221_temp_get_fs(&temp);
  json_object_dotset_number(JSON_Status, "hts221_temp.fs", temp);
  bool temp_b = 0;
  hts221_temp_get_enable(&temp_b);
  json_object_dotset_boolean(JSON_Status, "hts221_temp.enable", temp_b);
  uint32_t temp_i = 0;
  hts221_temp_get_samples_per_ts(&temp_i);
  json_object_dotset_number(JSON_Status, "hts221_temp.samples_per_ts", temp_i);
  hts221_temp_get_dim(&temp_i);
  json_object_dotset_number(JSON_Status, "hts221_temp.dim", temp_i);
  hts221_temp_get_ioffset(&temp);
  json_object_dotset_number(JSON_Status, "hts221_temp.ioffset", temp);
  hts221_temp_get_measodr(&temp);
  json_object_dotset_number(JSON_Status, "hts221_temp.measodr", temp);
  char *temp_s = "";
  hts221_temp_get_data_type(&temp_s);
  json_object_dotset_string(JSON_Status, "hts221_temp.data_type", temp_s);
  hts221_temp_get_usb_dps(&temp_i);
  json_object_dotset_number(JSON_Status, "hts221_temp.usb_dps", temp_i);
  hts221_temp_get_sd_dps(&temp_i);
  json_object_dotset_number(JSON_Status, "hts221_temp.sd_dps", temp_i);
  hts221_temp_get_sensitivity(&temp);
  json_object_dotset_number(JSON_Status, "hts221_temp.sensitivity", temp);
  json_object_dotset_number(JSON_Status, "hts221_temp.c_type", COMP_TYPE_SENSOR); //Added looking @the component schema field (this is :sensor)
  int8_t temp_int8 = 0;
  hts221_temp_get_stream_id(&temp_int8);
  json_object_dotset_number(JSON_Status, "hts221_temp.stream_id", temp_int8); //Not in DTDL but Added looking @the component schema field (this is :sensor) ONLY for Sensors and Algorithms
  hts221_temp_get_ep_id(&temp_int8);
  json_object_dotset_number(JSON_Status, "hts221_temp.ep_id", temp_int8); //Not in DTDL but Added looking @the component schema field (this is :sensor) ONLY for Sensors and Algorithms

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

uint8_t HTS221_TEMP_PnPL_vtblSetProperty(IPnPLComponent_t *_this, char *serializedJSON)
{

  JSON_Value *tempJSON = json_parse_string(serializedJSON);
  JSON_Object *tempJSONObject = json_value_get_object(tempJSON);

  uint8_t ret = 0;

  if(json_object_dothas_value(tempJSONObject, "hts221_temp.odr"))
  {
    float odr = (float) (json_object_dotget_number(tempJSONObject, "hts221_temp.odr"));
    hts221_temp_set_odr(odr); //ODR is an Enum --> value = float to contain all possible numeric values
  }
  //Only one FS value supported! --> no SET FS function
  if(json_object_dothas_value(tempJSONObject, "hts221_temp.enable"))
  {
    bool enable = json_object_dotget_boolean(tempJSONObject, "hts221_temp.enable");
    hts221_temp_set_enable(enable);
  }
  if(json_object_dothas_value(tempJSONObject, "hts221_temp.samples_per_ts"))
  {
    uint32_t samples_per_ts = (uint32_t) json_object_dotget_number(tempJSONObject, "hts221_temp.samples_per_ts");
    hts221_temp_set_samples_per_ts(samples_per_ts);
  }

  json_value_free(tempJSON);
  return ret;
}

uint8_t HTS221_TEMP_PnPL_vtblExecuteFunction(IPnPLComponent_t *_this, char *serializedJSON)
{
  return 1;
}

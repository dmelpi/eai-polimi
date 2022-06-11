#include "IMP34DT05_MIC_PnPL.h"
#include "IMP34DT05_MIC_PnPL_vtbl.h"
#include "IPnPLComponent.h"
#include "IPnPLComponent_vtbl.h"
#include "PnPLCompManager.h"
#include <string.h>

#include "App_model.h"

static const IPnPLComponent_vtbl sIMP34DT05_MIC_PnPL_CompIF_vtbl =
{
    IMP34DT05_MIC_PnPL_vtblGetKey,
    IMP34DT05_MIC_PnPL_vtblGetNCommands,
    IMP34DT05_MIC_PnPL_vtblGetCommandKey,
    IMP34DT05_MIC_PnPL_vtblGetStatus,
    IMP34DT05_MIC_PnPL_vtblSetProperty,
    IMP34DT05_MIC_PnPL_vtblExecuteFunction, };

/**
 *  IMP34DT05_MIC_PnPL internal structure.
 */
struct _IMP34DT05_MIC_PnPL
{

  /**
   * Implements the IPnPLComponent interface.
   */
  IPnPLComponent_t component_if;

};

// Public API definition
// *********************
static IMP34DT05_MIC_PnPL sIMP34DT05_MIC_bObj;

IPnPLComponent_t* IMP34DT05_MIC_PnPLAlloc()
{
  IPnPLComponent_t *pxObj = (IPnPLComponent_t*) &sIMP34DT05_MIC_bObj;
  if(pxObj != NULL)
  {
    pxObj->vptr = &sIMP34DT05_MIC_PnPL_CompIF_vtbl;
  }
  return pxObj;
}

uint8_t IMP34DT05_MIC_PnPLInit(IPnPLComponent_t *_this/*TODO: pass here the SensorManager*/)
{
  IPnPLComponent_t *component_if = _this;
  PnPLAddComponent(component_if);
  imp34dt05_mic_comp_init();
  return 0;
}

// IPnPLComponent virtual functions definition
// *******************************************

char* IMP34DT05_MIC_PnPL_vtblGetKey(IPnPLComponent_t *_this)
{
  return imp34dt05_mic_get_key();
}

uint8_t IMP34DT05_MIC_PnPL_vtblGetNCommands(IPnPLComponent_t *_this)
{
  return 0;
}

char* IMP34DT05_MIC_PnPL_vtblGetCommandKey(IPnPLComponent_t *_this, uint8_t id)
{
  return "";
}

uint8_t IMP34DT05_MIC_PnPL_vtblGetStatus(IPnPLComponent_t *_this, char **serializedJSON, uint32_t *size, uint8_t pretty)
{
  JSON_Value *tempJSON;
  JSON_Object *JSON_Status;

  tempJSON = json_value_init_object();
  JSON_Status = json_value_get_object(tempJSON);

  float temp = 0;
  imp34dt05_mic_get_odr(&temp);
  json_object_dotset_number(JSON_Status, "imp34dt05_mic.odr", temp);
  imp34dt05_mic_get_aop(&temp);
  json_object_dotset_number(JSON_Status, "imp34dt05_mic.aop", temp);
  bool temp_b = 0;
  imp34dt05_mic_get_enable(&temp_b);
  json_object_dotset_boolean(JSON_Status, "imp34dt05_mic.enable", temp_b);
  uint32_t temp_i = 0;
  imp34dt05_mic_get_samples_per_ts(&temp_i);
  json_object_dotset_number(JSON_Status, "imp34dt05_mic.samples_per_ts", temp_i);
  imp34dt05_mic_get_dim(&temp_i);
  json_object_dotset_number(JSON_Status, "imp34dt05_mic.dim", temp_i);
  imp34dt05_mic_get_ioffset(&temp);
  json_object_dotset_number(JSON_Status, "imp34dt05_mic.ioffset", temp);
  imp34dt05_mic_get_measodr(&temp);
  json_object_dotset_number(JSON_Status, "imp34dt05_mic.measodr", temp);
  char *temp_s;
  imp34dt05_mic_get_data_type(&temp_s);
  json_object_dotset_string(JSON_Status, "imp34dt05_mic.data_type", temp_s);
  imp34dt05_mic_get_usb_dps(&temp_i);
  json_object_dotset_number(JSON_Status, "imp34dt05_mic.usb_dps", temp_i);
  imp34dt05_mic_get_sd_dps(&temp_i);
  json_object_dotset_number(JSON_Status, "imp34dt05_mic.sd_dps", temp_i);
  imp34dt05_mic_get_sensitivity(&temp);
  json_object_dotset_number(JSON_Status, "imp34dt05_mic.sensitivity", temp);
  json_object_dotset_number(JSON_Status, "imp34dt05_mic.c_type", COMP_TYPE_SENSOR); //Added looking @the component schema field (this is :sensor)
  int8_t temp_int8 = 0;
  imp34dt05_mic_get_stream_id(&temp_int8);
  json_object_dotset_number(JSON_Status, "imp34dt05_mic.stream_id", temp_int8); //Not in DTDL but Added looking @the component schema field (this is :sensor) ONLY for Sensors and Algorithms
  imp34dt05_mic_get_ep_id(&temp_int8);
  json_object_dotset_number(JSON_Status, "imp34dt05_mic.ep_id", temp_int8); //Not in DTDL but Added looking @the component schema field (this is :sensor) ONLY for Sensors and Algorithms

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

uint8_t IMP34DT05_MIC_PnPL_vtblSetProperty(IPnPLComponent_t *_this, char *serializedJSON)
{

  JSON_Value *tempJSON = json_parse_string(serializedJSON);
  JSON_Object *tempJSONObject = json_value_get_object(tempJSON);

  uint8_t ret = 0;

  //Only one AOP value supported! --> no SET AOP function
  if(json_object_dothas_value(tempJSONObject, "imp34dt05_mic.odr"))
  {
    float odr = (float) (json_object_dotget_number(tempJSONObject, "imp34dt05_mic.odr"));
    imp34dt05_mic_set_odr(odr);
  }
  if(json_object_dothas_value(tempJSONObject, "imp34dt05_mic.enable"))
  {
    bool enable = json_object_dotget_boolean(tempJSONObject, "imp34dt05_mic.enable");
    imp34dt05_mic_set_enable(enable);
  }
  if(json_object_dothas_value(tempJSONObject, "imp34dt05_mic.samples_per_ts"))
  {
    uint32_t samples_per_ts = (uint32_t) json_object_dotget_number(tempJSONObject, "imp34dt05_mic.samples_per_ts");
    imp34dt05_mic_set_samples_per_ts(samples_per_ts);
  }

  json_value_free(tempJSON);
  return ret;
}

uint8_t IMP34DT05_MIC_PnPL_vtblExecuteFunction(IPnPLComponent_t *_this, char *serializedJSON)
{
  return 1;
}

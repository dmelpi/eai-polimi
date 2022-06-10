#include "ILPS22QS_PRESS_PnPL.h"
#include "ILPS22QS_PRESS_PnPL_vtbl.h"
#include "IPnPLComponent.h"
#include "IPnPLComponent_vtbl.h"
#include "PnPLCompManager.h"
#include <string.h>

#include "App_model.h"

static const IPnPLComponent_vtbl sILPS22QS_PRESS_PnPL_CompIF_vtbl =
{
    ILPS22QS_PRESS_PnPL_vtblGetKey,
    ILPS22QS_PRESS_PnPL_vtblGetNCommands,
    ILPS22QS_PRESS_PnPL_vtblGetCommandKey,
    ILPS22QS_PRESS_PnPL_vtblGetStatus,
    ILPS22QS_PRESS_PnPL_vtblSetProperty,
    ILPS22QS_PRESS_PnPL_vtblExecuteFunction, };

/**
 *  ILPS22QS_PRESS_PnPL internal structure.
 */
struct _ILPS22QS_PRESS_PnPL
{

  /**
   * Implements the IPnPLComponent interface.
   */
  IPnPLComponent_t component_if;

};

/* Objects instance */
/********************/

static ILPS22QS_PRESS_PnPL sILPS22QS_PRESS_bObj;

// Public API definition
// *********************

IPnPLComponent_t* ILPS22QS_PRESS_PnPLAlloc()
{

  IPnPLComponent_t *pxObj = (IPnPLComponent_t*) &sILPS22QS_PRESS_bObj;
  if(pxObj != NULL)
  {
    pxObj->vptr = &sILPS22QS_PRESS_PnPL_CompIF_vtbl;
  }
  return pxObj;
}

uint8_t ILPS22QS_PRESS_PnPLInit(IPnPLComponent_t *_this)
{
  IPnPLComponent_t *component_if = _this;
  PnPLAddComponent(component_if);
  ilps22qs_press_comp_init();

  return 0;
}

// IPnPLComponent virtual functions definition
// *******************************************

char* ILPS22QS_PRESS_PnPL_vtblGetKey(IPnPLComponent_t *_this)
{
  return ilps22qs_press_get_key();
}

uint8_t ILPS22QS_PRESS_PnPL_vtblGetNCommands(IPnPLComponent_t *_this)
{
  return 0;
}

char* ILPS22QS_PRESS_PnPL_vtblGetCommandKey(IPnPLComponent_t *_this, uint8_t id)
{
  return "";
}

uint8_t ILPS22QS_PRESS_PnPL_vtblGetStatus(IPnPLComponent_t *_this, char **serializedJSON, uint32_t *size, uint8_t pretty)
{
  JSON_Value *tempJSON;
  JSON_Object *JSON_Status;

  tempJSON = json_value_init_object();
  JSON_Status = json_value_get_object(tempJSON);

  float press = 0;
  ilps22qs_press_get_odr(&press);
  json_object_dotset_number(JSON_Status, "ilps22qs_press.odr", press); //hum ODR is(must be) the same
  ilps22qs_press_get_fs(&press);
  json_object_dotset_number(JSON_Status, "ilps22qs_press.fs", press);
  bool press_b = 0;
  ilps22qs_press_get_enable(&press_b);
  json_object_dotset_boolean(JSON_Status, "ilps22qs_press.enable", press_b);
  uint32_t press_i = 0;
  ilps22qs_press_get_samples_per_ts(&press_i);
  json_object_dotset_number(JSON_Status, "ilps22qs_press.samples_per_ts", press_i);
  ilps22qs_press_get_dim(&press_i);
  json_object_dotset_number(JSON_Status, "ilps22qs_press.dim", press_i);
  ilps22qs_press_get_ioffset(&press);
  json_object_dotset_number(JSON_Status, "ilps22qs_press.ioffset", press);
  ilps22qs_press_get_measodr(&press);
  json_object_dotset_number(JSON_Status, "ilps22qs_press.measodr", press);
  char *press_s = "";
  ilps22qs_press_get_data_type(&press_s);
  json_object_dotset_string(JSON_Status, "ilps22qs_press.data_type", press_s);
  ilps22qs_press_get_usb_dps(&press_i);
  json_object_dotset_number(JSON_Status, "ilps22qs_press.usb_dps", press_i);
  ilps22qs_press_get_sd_dps(&press_i);
  json_object_dotset_number(JSON_Status, "ilps22qs_press.sd_dps", press_i);
  ilps22qs_press_get_sensitivity(&press);
  json_object_dotset_number(JSON_Status, "ilps22qs_press.sensitivity", press);
  json_object_dotset_number(JSON_Status, "ilps22qs_press.c_type", COMP_TYPE_SENSOR); //Added looking @the component schema field (this is :sensor)
  int8_t temp_int8 = 0;
  ilps22qs_press_get_stream_id(&temp_int8);
  json_object_dotset_number(JSON_Status, "ilps22qs_press.stream_id", temp_int8); //Not in DTDL but Added looking @the component schema field (this is :sensor) ONLY for Sensors and Algorithms
  ilps22qs_press_get_ep_id(&temp_int8);
  json_object_dotset_number(JSON_Status, "ilps22qs_press.ep_id", temp_int8); //Not in DTDL but Added looking @the component schema field (this is :sensor) ONLY for Sensors and Algorithms

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

uint8_t ILPS22QS_PRESS_PnPL_vtblSetProperty(IPnPLComponent_t *_this, char *serializedJSON)
{

  JSON_Value *tempJSON = json_parse_string(serializedJSON);
  JSON_Object *tempJSONObject = json_value_get_object(tempJSON);

  uint8_t ret = 0;

  if(json_object_dothas_value(tempJSONObject, "ilps22qs_press.odr"))
  {
    float odr = (float) (json_object_dotget_number(tempJSONObject, "ilps22qs_press.odr"));
    ilps22qs_press_set_odr(odr); //ODR is an Enum --> value = float to contain all possible numeric values
  }

  if(json_object_dothas_value(tempJSONObject, "ilps22qs_press.fs"))
  {
    float fs = (float) (json_object_dotget_number(tempJSONObject, "ilps22qs_press.fs"));
    ilps22qs_press_set_fs(fs); //FS is an Enum --> value = float to contain all possible numeric values
  }
  if(json_object_dothas_value(tempJSONObject, "ilps22qs_press.enable"))
  {
    bool enable = json_object_dotget_boolean(tempJSONObject, "ilps22qs_press.enable");
    ilps22qs_press_set_enable(enable);
  }
  if(json_object_dothas_value(tempJSONObject, "ilps22qs_press.samples_per_ts"))
  {
    uint32_t samples_per_ts = (uint32_t) json_object_dotget_number(tempJSONObject, "ilps22qs_press.samples_per_ts");
    ilps22qs_press_set_samples_per_ts(samples_per_ts);
  }

  json_value_free(tempJSON);
  return ret;
}

uint8_t ILPS22QS_PRESS_PnPL_vtblExecuteFunction(IPnPLComponent_t *_this, char *serializedJSON)
{
  return 1;
}

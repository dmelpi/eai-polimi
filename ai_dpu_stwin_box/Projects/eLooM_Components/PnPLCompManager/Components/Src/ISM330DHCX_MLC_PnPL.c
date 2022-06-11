#include "ISM330DHCX_MLC_PnPL.h"
#include "ISM330DHCX_MLC_PnPL_vtbl.h"
#include "IPnPLComponent.h"
#include "IPnPLComponent_vtbl.h"
#include "PnPLCompManager.h"
#include <string.h>

#include "App_model.h"

static const IPnPLComponent_vtbl sISM330DHCX_MLC_PnPL_CompIF_vtbl =
{
    ISM330DHCX_MLC_PnPL_vtblGetKey,
    ISM330DHCX_MLC_PnPL_vtblGetNCommands,
    ISM330DHCX_MLC_PnPL_vtblGetCommandKey,
    ISM330DHCX_MLC_PnPL_vtblGetStatus,
    ISM330DHCX_MLC_PnPL_vtblSetProperty,
    ISM330DHCX_MLC_PnPL_vtblExecuteFunction };

/**
 *  ISM330DHCX_MLC_PnPL internal structure.
 */
struct _ISM330DHCX_MLC_PnPL
{

  /**
   * Implements the IPnPLComponent interface.
   */
  IPnPLComponent_t component_if;

  /**
   * Contains ucf loading function pointer.
   */
  IMLCController_t *mlcControllerIF;

};

/* Objects instance */
/********************/

static ISM330DHCX_MLC_PnPL sISM330DHCX_MLC_bObj;

// Public API definition
// *********************

IPnPLComponent_t* ISM330DHCX_MLC_PnPLAlloc()
{
  IPnPLComponent_t *pxObj = (IPnPLComponent_t*) &sISM330DHCX_MLC_bObj;
  if(pxObj != NULL)
  {
    pxObj->vptr = &sISM330DHCX_MLC_PnPL_CompIF_vtbl;
  }
  return pxObj;
}

uint8_t ISM330DHCX_MLC_PnPLInit(IPnPLComponent_t *_this, IMLCController_t *inf)
{
  IPnPLComponent_t *component_if = _this;
  PnPLAddComponent(component_if);

  ISM330DHCX_MLC_PnPL *p_if_owner = (ISM330DHCX_MLC_PnPL*) _this;
  p_if_owner->mlcControllerIF = inf;

  ism330dhcx_mlc_comp_init();
  return 0;
}

// IPnPLComponent virtual functions definition
// *******************************************

char* ISM330DHCX_MLC_PnPL_vtblGetKey(IPnPLComponent_t *_this)
{
  return ism330dhcx_mlc_get_key();
}

uint8_t ISM330DHCX_MLC_PnPL_vtblGetNCommands(IPnPLComponent_t *_this)
{
  return 1;
}

char* ISM330DHCX_MLC_PnPL_vtblGetCommandKey(IPnPLComponent_t *_this, uint8_t id)
{
  return "ism330dhcx_mlc*load_conf";
}

uint8_t ISM330DHCX_MLC_PnPL_vtblGetStatus(IPnPLComponent_t *_this, char **serializedJSON, uint32_t *size, uint8_t pretty)
{
  JSON_Value *tempJSON;
  JSON_Object *JSON_Status;

  tempJSON = json_value_init_object();
  JSON_Status = json_value_get_object(tempJSON);

  bool temp_b = 0;
  ism330dhcx_mlc_get_enable(&temp_b);
  json_object_dotset_boolean(JSON_Status, "ism330dhcx_mlc.enable", temp_b);
  uint32_t temp_i = 0;
  ism330dhcx_mlc_get_samples_per_ts(&temp_i);
  json_object_dotset_number(JSON_Status, "ism330dhcx_mlc.samples_per_ts", temp_i);
  ism330dhcx_mlc_get_dim(&temp_i);
  json_object_dotset_number(JSON_Status, "ism330dhcx_mlc.dim", temp_i);
  float temp_f = 0;
  ism330dhcx_mlc_get_ioffset(&temp_f);
  json_object_dotset_number(JSON_Status, "ism330dhcx_mlc.ioffset", temp_f);
  char *temp_s;
  ism330dhcx_mlc_get_data_type(&temp_s);
  json_object_dotset_string(JSON_Status, "ism330dhcx_mlc.data_type", temp_s);
  ism330dhcx_mlc_get_usb_dps(&temp_i);
  json_object_dotset_number(JSON_Status, "ism330dhcx_mlc.usb_dps", temp_i);
  ism330dhcx_mlc_get_sd_dps(&temp_i);
  json_object_dotset_number(JSON_Status, "ism330dhcx_mlc.sd_dps", temp_i);
  ism330dhcx_mlc_get_sensitivity(&temp_f);
  json_object_dotset_number(JSON_Status, "ism330dhcx_mlc.sensitivity", temp_f);
  json_object_dotset_number(JSON_Status, "ism330dhcx_mlc.c_type", COMP_TYPE_SENSOR); //Added looking @the component schema field (this is :sensor)
  int8_t temp_int8 = 0;
  ism330dhcx_mlc_get_stream_id(&temp_int8);
  json_object_dotset_number(JSON_Status, "ism330dhcx_mlc.stream_id", temp_int8); //Not in DTDL but Added looking @the component schema field (this is :sensor) ONLY for Sensors and Algorithms
  ism330dhcx_mlc_get_ep_id(&temp_int8);
  json_object_dotset_number(JSON_Status, "ism330dhcx_mlc.ep_id", temp_int8); //Not in DTDL but Added looking @the component schema field (this is :sensor) ONLY for Sensors and Algorithms

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

uint8_t ISM330DHCX_MLC_PnPL_vtblSetProperty(IPnPLComponent_t *_this, char *serializedJSON)
{

  JSON_Value *tempJSON = json_parse_string(serializedJSON);
  JSON_Object *tempJSONObject = json_value_get_object(tempJSON);

  uint8_t ret = 0;

  if(json_object_dothas_value(tempJSONObject, "ism330dhcx_mlc.enable"))
  {
    bool enable = json_object_dotget_boolean(tempJSONObject, "ism330dhcx_mlc.enable");
    ism330dhcx_mlc_set_enable(enable);
  }
  if(json_object_dothas_value(tempJSONObject, "ism330dhcx_mlc.samples_per_ts"))
  {
    uint32_t samples_per_ts = (uint32_t) json_object_dotget_number(tempJSONObject, "ism330dhcx_mlc.samples_per_ts");
    ism330dhcx_mlc_set_samples_per_ts(samples_per_ts);
  }

  json_value_free(tempJSON);
  return ret;
}

uint8_t ISM330DHCX_MLC_PnPL_vtblExecuteFunction(IPnPLComponent_t *_this, char *serializedJSON)
{
  ISM330DHCX_MLC_PnPL *p_if_owner = (ISM330DHCX_MLC_PnPL*) _this;
  JSON_Value *tempJSON = json_parse_string(serializedJSON);
  JSON_Object *tempJSONObject = json_value_get_object(tempJSON);

  if(json_object_dothas_value(tempJSONObject, "ism330dhcx_mlc*load_conf.ucf_data"))
  {
    if(json_object_dothas_value(tempJSONObject, "ism330dhcx_mlc*load_conf.ucf_data.size"))
    {
      uint32_t ucf_size = (uint32_t) (json_object_dotget_number(tempJSONObject, "ism330dhcx_mlc*load_conf.ucf_data.size"));
      if(json_object_dothas_value(tempJSONObject, "ism330dhcx_mlc*load_conf.ucf_data.data"))
      {
        const char *ucf_data = json_object_dotget_string(tempJSONObject, "ism330dhcx_mlc*load_conf.ucf_data.data");
        ism330dhcx_mlc_load_ucf(p_if_owner->mlcControllerIF, ucf_size, (char*) ucf_data);
      }
    }
  }
  return 1;
}

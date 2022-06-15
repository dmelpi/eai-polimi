#include "FwInfoPnPL.h"
#include "FwInfoPnPL_vtbl.h"
#include "IPnPLComponent.h"
#include "IPnPLComponent_vtbl.h"
#include "PnPLCompManager.h"
#include <string.h>

#include "App_model.h"

static const IPnPLComponent_vtbl sFwInfoPnPL_CompIF_vtbl =
{
  FwInfoPnPL_vtblGetKey,
  FwInfoPnPL_vtblGetNCommands,
  FwInfoPnPL_vtblGetCommandKey,
  FwInfoPnPL_vtblGetStatus,
  FwInfoPnPL_vtblSetProperty,
  FwInfoPnPL_vtblExecuteFunction
};

/**
  *  FwInfoPnPL internal structure.
  */
struct _FwInfoPnPL
{
  /**
    * Implements the IPnPLComponent interface.
    */
  IPnPLComponent_t component_if;
};

/* Objects instance */
/********************/

static FwInfoPnPL sFwInfoPnPL;

// Public API definition
// *********************
IPnPLComponent_t *FwInfoPnPLAlloc()
{
  IPnPLComponent_t *pxObj = (IPnPLComponent_t *) &sFwInfoPnPL;
  if (pxObj != NULL)
  {
    pxObj->vptr = &sFwInfoPnPL_CompIF_vtbl;
  }
  return pxObj;
}

uint8_t FwInfoPnPLInit(IPnPLComponent_t *_this)
{
  IPnPLComponent_t *component_if = _this;
  PnPLAddComponent(component_if);
  fwinfo_comp_init();
  return 0;
}

// IPnPLComponent virtual functions definition
// *******************************************

char *FwInfoPnPL_vtblGetKey(IPnPLComponent_t *_this)
{
  return fwinfo_get_key();
}

uint8_t FwInfoPnPL_vtblGetNCommands(IPnPLComponent_t *_this)
{
  return 0;
}

char *FwInfoPnPL_vtblGetCommandKey(IPnPLComponent_t *_this, uint8_t id)
{
  return "";
}

uint8_t FwInfoPnPL_vtblGetStatus(IPnPLComponent_t *_this, char **serializedJSON, uint32_t *size, uint8_t pretty)
{
  JSON_Value *tempJSON;
  JSON_Object *JSON_Status;

  tempJSON = json_value_init_object();
  JSON_Status = json_value_get_object(tempJSON);

  char *temp = "";
  fwinfo_get_alias(&temp);
  json_object_dotset_string(JSON_Status, "fwinfo.alias", temp);
  fwinfo_get_fw_name(&temp);
  json_object_dotset_string(JSON_Status, "fwinfo.fw_name", temp);
  fwinfo_get_fw_version(&temp);
  json_object_dotset_string(JSON_Status, "fwinfo.fw_version", temp);
  fwinfo_get_serial_number(&temp);
  json_object_dotset_string(JSON_Status, "fwinfo.serial_number", temp);
  fwinfo_get_device_url(&temp);
  json_object_dotset_string(JSON_Status, "fwinfo.device_url", temp);
  fwinfo_get_fw_url(&temp);
  json_object_dotset_string(JSON_Status, "fwinfo.fw_url", temp);

  if (pretty == 1)
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

uint8_t FwInfoPnPL_vtblSetProperty(IPnPLComponent_t *_this, char *serializedJSON)
{
  JSON_Value *tempJSON = json_parse_string(serializedJSON);
  JSON_Object *tempJSONObject = json_value_get_object(tempJSON);

  uint8_t ret = 0;

  if (json_object_dothas_value(tempJSONObject, "fwinfo.alias"))
  {
    const char *alias = json_object_dotget_string(tempJSONObject, "fwinfo.alias");
    fwinfo_set_alias(alias);
  }

  json_value_free(tempJSON);
  return ret;
}

uint8_t FwInfoPnPL_vtblExecuteFunction(IPnPLComponent_t *_this, char *serializedJSON)
{
  return 1; //TODO fix error management
}

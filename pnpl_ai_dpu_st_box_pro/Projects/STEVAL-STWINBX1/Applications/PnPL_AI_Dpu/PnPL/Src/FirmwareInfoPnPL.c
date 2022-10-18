#include "FirmwareInfoPnPL.h"
#include "FirmwareInfoPnPL_vtbl.h"
#include <string.h>

#include "App_model.h"
#include "IPnPLComponent.h"
#include "IPnPLComponent_vtbl.h"
#include "PnPLCompManager.h"

static const IPnPLComponent_vtbl sFirmwareInfoPnPL_CompIF_vtbl =
{
  FirmwareInfoPnPL_vtblGetKey,
  FirmwareInfoPnPL_vtblGetNCommands,
  FirmwareInfoPnPL_vtblGetCommandKey,
  FirmwareInfoPnPL_vtblGetStatus,
  FirmwareInfoPnPL_vtblSetProperty,
  FirmwareInfoPnPL_vtblExecuteFunction
};

/**
  *  FirmwareInfoPnPL internal structure.
  */
struct _FirmwareInfoPnPL
{
  /**
    * Implements the IPnPLComponent interface.
    */
  IPnPLComponent_t component_if;
};

/* Objects instance */
/********************/

static FirmwareInfoPnPL sFirmwareInfoPnPL;

// Public API definition
// *********************
IPnPLComponent_t *FirmwareInfoPnPLAlloc()
{
  IPnPLComponent_t *pxObj = (IPnPLComponent_t *) &sFirmwareInfoPnPL;
  if (pxObj != NULL)
  {
    pxObj->vptr = &sFirmwareInfoPnPL_CompIF_vtbl;
  }
  return pxObj;
}

uint8_t FirmwareInfoPnPLInit(IPnPLComponent_t *_this)
{
  IPnPLComponent_t *component_if = _this;
  PnPLAddComponent(component_if);
  firmware_info_comp_init();
  return 0;
}

// IPnPLComponent virtual functions definition
// *******************************************

char *FirmwareInfoPnPL_vtblGetKey(IPnPLComponent_t *_this)
{
  return firmware_info_get_key();
}

uint8_t FirmwareInfoPnPL_vtblGetNCommands(IPnPLComponent_t *_this)
{
  return 0;
}

char *FirmwareInfoPnPL_vtblGetCommandKey(IPnPLComponent_t *_this, uint8_t id)
{
  return "";
}

uint8_t FirmwareInfoPnPL_vtblGetStatus(IPnPLComponent_t *_this, char **serializedJSON, uint32_t *size, uint8_t pretty)
{
  JSON_Value *tempJSON;
  JSON_Object *JSON_Status;

  tempJSON = json_value_init_object();
  JSON_Status = json_value_get_object(tempJSON);

  char *temp = "";
  firmware_info_get_alias(&temp);
  json_object_dotset_string(JSON_Status, "firmware_info.alias", temp);
  firmware_info_get_fw_name(&temp);
  json_object_dotset_string(JSON_Status, "firmware_info.fw_name", temp);
  firmware_info_get_fw_version(&temp);
  json_object_dotset_string(JSON_Status, "firmware_info.fw_version", temp);
  firmware_info_get_serial_number(&temp);
  json_object_dotset_string(JSON_Status, "firmware_info.serial_number", temp);
  firmware_info_get_device_url(&temp);
  json_object_dotset_string(JSON_Status, "firmware_info.device_url", temp);
  firmware_info_get_fw_url(&temp);
  json_object_dotset_string(JSON_Status, "firmware_info.fw_url", temp);

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

uint8_t FirmwareInfoPnPL_vtblSetProperty(IPnPLComponent_t *_this, char *serializedJSON)
{
  JSON_Value *tempJSON = json_parse_string(serializedJSON);
  JSON_Object *tempJSONObject = json_value_get_object(tempJSON);

  uint8_t ret = 0;

  if (json_object_dothas_value(tempJSONObject, "firmware_info.alias"))
  {
    const char *alias = json_object_dotget_string(tempJSONObject, "firmware_info.alias");
    firmware_info_set_alias(alias);
  }

  json_value_free(tempJSON);
  return ret;
}

uint8_t FirmwareInfoPnPL_vtblExecuteFunction(IPnPLComponent_t *_this, char *serializedJSON)
{
  return 1; //TODO fix error management
}

#include "DeviceInfoPnPL.h"
#include "DeviceInfoPnPL_vtbl.h"
#include "IPnPLComponent.h"
#include "IPnPLComponent_vtbl.h"
#include "PnPLCompManager.h"
#include <string.h>

#include "App_model.h"

static const IPnPLComponent_vtbl sDeviceInfoPnPL_CompIF_vtbl =
{
  DeviceInfoPnPL_vtblGetKey,
  DeviceInfoPnPL_vtblGetNCommands,
  DeviceInfoPnPL_vtblGetCommandKey,
  DeviceInfoPnPL_vtblGetStatus,
  DeviceInfoPnPL_vtblSetProperty,
  DeviceInfoPnPL_vtblExecuteFunction
};

/**
  *  DeviceInfoPnPL internal structure.
  */
struct _DeviceInfoPnPL
{
  /**
    * Implements the IPnPLComponent interface.
    */
  IPnPLComponent_t component_if;
};

/* Objects instance */
/********************/

static DeviceInfoPnPL sDeviceInfoPnPL;

// Public API definition
// *********************

IPnPLComponent_t *DeviceInfoPnPLAlloc()
{
  IPnPLComponent_t *pxObj = (IPnPLComponent_t *) &sDeviceInfoPnPL;
  if (pxObj != NULL)
  {
    pxObj->vptr = &sDeviceInfoPnPL_CompIF_vtbl;
  }
  return pxObj;
}

uint8_t DeviceInfoPnPLInit(IPnPLComponent_t *_this)
{
  IPnPLComponent_t *component_if = _this;
  PnPLAddComponent(component_if);
  deviceinfo_comp_init();
  return 0;
}

// IPnPLComponent virtual functions definition
// *******************************************

char *DeviceInfoPnPL_vtblGetKey(IPnPLComponent_t *_this)
{
  return deviceinfo_get_key();
}

uint8_t DeviceInfoPnPL_vtblGetNCommands(IPnPLComponent_t *_this)
{
  return 0;
}

char *DeviceInfoPnPL_vtblGetCommandKey(IPnPLComponent_t *_this, uint8_t id)
{
  return "";
}

uint8_t DeviceInfoPnPL_vtblGetStatus(IPnPLComponent_t *_this, char **serializedJSON, uint32_t *size, uint8_t pretty)
{
  JSON_Value *tempJSON;
  JSON_Object *JSON_Status;

  tempJSON = json_value_init_object();
  JSON_Status = json_value_get_object(tempJSON);

  char *temp = "";
  deviceinfo_get_manufacturer(&temp);
  json_object_dotset_string(JSON_Status, "deviceinfo.manufacturer", temp);
  deviceinfo_get_model(&temp);
  json_object_dotset_string(JSON_Status, "deviceinfo.model", temp);
  deviceinfo_get_swVersion(&temp);
  json_object_dotset_string(JSON_Status, "deviceinfo.swVersion", temp);
  deviceinfo_get_osName(&temp);
  json_object_dotset_string(JSON_Status, "deviceinfo.osName", temp);
  deviceinfo_get_processorArchitecture(&temp);
  json_object_dotset_string(JSON_Status, "deviceinfo.processorArchitecture", temp);
  deviceinfo_get_processorManufacturer(&temp);
  json_object_dotset_string(JSON_Status, "deviceinfo.processorManufacturer", temp);
  uint32_t temp_i = 0;
  deviceinfo_get_totalStorage(&temp_i);
  json_object_dotset_number(JSON_Status, "deviceinfo.totalStorage", temp_i);
  deviceinfo_get_totalMemory(&temp_i);
  json_object_dotset_number(JSON_Status, "deviceinfo.totalMemory", temp_i);

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

uint8_t DeviceInfoPnPL_vtblSetProperty(IPnPLComponent_t *_this, char *serializedJSON)
{
  return 0; //TODO fix error management
}

uint8_t DeviceInfoPnPL_vtblExecuteFunction(IPnPLComponent_t *_this, char *serializedJSON)
{
  return 1; //TODO fix error management
}

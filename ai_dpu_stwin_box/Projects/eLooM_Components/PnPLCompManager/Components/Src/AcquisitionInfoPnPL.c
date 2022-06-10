#include "AcquisitionInfoPnPL.h"
#include "AcquisitionInfoPnPL_vtbl.h"
#include "IPnPLComponent.h"
#include "IPnPLComponent_vtbl.h"
#include "PnPLCompManager.h"
#include <string.h>

#include "App_model.h"

static const IPnPLComponent_vtbl sAcquisitionInfoPnPL_CompIF_vtbl =
{
    AcquisitionInfoPnPL_vtblGetKey,
    AcquisitionInfoPnPL_vtblGetNCommands,
    AcquisitionInfoPnPL_vtblGetCommandKey,
    AcquisitionInfoPnPL_vtblGetStatus,
    AcquisitionInfoPnPL_vtblSetProperty,
    AcquisitionInfoPnPL_vtblExecuteFunction };

/**
 *  AcquisitionInfoPnPL internal structure.
 */
struct _AcquisitionInfoPnPL
{
  /**
   * Implements the IPnPLComponent interface.
   */
  IPnPLComponent_t component_if;

};

/* Objects instance */
/********************/

static AcquisitionInfoPnPL sAcquisitionInfoPnPL;

// Public API definition
// *********************
IPnPLComponent_t* AcquisitionInfoPnPLAlloc()
{
  IPnPLComponent_t *pxObj = (IPnPLComponent_t*) &sAcquisitionInfoPnPL;
  if(pxObj != NULL)
  {
    pxObj->vptr = &sAcquisitionInfoPnPL_CompIF_vtbl;
  }
  return pxObj;
}

uint8_t AcquisitionInfoPnPLInit(IPnPLComponent_t *_this)
{
  IPnPLComponent_t *component_if = _this;
  PnPLAddComponent(component_if);
  acquisition_info_comp_init();
  return 0;
}

// IPnPLComponent virtual functions definition
// *******************************************

char* AcquisitionInfoPnPL_vtblGetKey(IPnPLComponent_t *_this)
{
  return acquisition_info_get_key();
}

uint8_t AcquisitionInfoPnPL_vtblGetNCommands(IPnPLComponent_t *_this)
{
  return 0;
}

char* AcquisitionInfoPnPL_vtblGetCommandKey(IPnPLComponent_t *_this, uint8_t id)
{
  return "";
}

uint8_t AcquisitionInfoPnPL_vtblGetStatus(IPnPLComponent_t *_this, char **serializedJSON, uint32_t *size, uint8_t pretty)
{
  JSON_Value *tempJSON;
  JSON_Object *JSON_Status;

  tempJSON = json_value_init_object();
  JSON_Status = json_value_get_object(tempJSON);

  char *temp = "";
  acquisition_info_get_name(&temp);
  json_object_dotset_string(JSON_Status, "acquisition_info.name", temp);
  acquisition_info_get_description(&temp);
  json_object_dotset_string(JSON_Status, "acquisition_info.description", temp);
  acquisition_info_get_uuid(&temp);
  json_object_dotset_string(JSON_Status, "acquisition_info.uuid", temp);
  JSON_Value *temp_j = json_value_init_object();
  acquisition_info_get_tags(temp_j);
  json_object_dotset_value(JSON_Status, "acquisition_info.", temp_j);

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

  //no need to free temp_j as it is part of tempJSON
  json_value_free(tempJSON);

  return 0;
}

uint8_t AcquisitionInfoPnPL_vtblSetProperty(IPnPLComponent_t *_this, char *serializedJSON)
{
  JSON_Value *tempJSON = json_parse_string(serializedJSON);
  JSON_Object *tempJSONObject = json_value_get_object(tempJSON);

  uint8_t ret = 0;

  if(json_object_dothas_value(tempJSONObject, "acquisition_info.name"))
  {
    const char *name = json_object_dotget_string(tempJSONObject, "acquisition_info.name");
    acquisition_info_set_name(name);
  }
  if(json_object_dothas_value(tempJSONObject, "acquisition_info.description"))
  {
    const char *description = json_object_dotget_string(tempJSONObject, "acquisition_info.description");
    acquisition_info_set_description(description);
  }

  json_value_free(tempJSON);
  return ret;
}

uint8_t AcquisitionInfoPnPL_vtblExecuteFunction(IPnPLComponent_t *_this, char *serializedJSON)
{
  return 1; //TODO fix error management
}

#include "AiApplicationPnPL.h"
#include "AiApplicationPnPL_vtbl.h"
#include "IPnPLComponent.h"
#include "IPnPLComponent_vtbl.h"
#include "services/SQuery.h"
#include "SensorManager.h"
#include "PnPLCompManager.h"
#include <string.h>

#include "App_model.h"

static const IPnPLComponent_vtbl sAiApplicationPnPL_CompIF_vtbl =
{
  AiApplicationPnPL_vtblGetKey,
  AiApplicationPnPL_vtblGetNCommands,
  AiApplicationPnPL_vtblGetCommandKey,
  AiApplicationPnPL_vtblGetStatus,
  AiApplicationPnPL_vtblSetProperty,
  AiApplicationPnPL_vtblExecuteFunction
};

/**
  *  AiApplicationPnPL internal structure.
  */
struct _AiApplicationPnPL
{
  /**
    * Implements the IPnPLComponent interface.
    */
  IPnPLComponent_t component_if;

  /**
    * Contains start and stop log functions pointers.
    */
  IAiApplication_t *ai_applicationIF;
};

/* Objects instance */
/********************/

/**
  * The only instance of the task object.
  */
static AiApplicationPnPL sAiApplicationbObj;

// Public API definition
// *********************
IPnPLComponent_t *AiApplicationPnPLAlloc()
{
  IPnPLComponent_t *pxObj = (IPnPLComponent_t *) &sAiApplicationbObj;
  if (pxObj != NULL)
  {
    pxObj->vptr = &sAiApplicationPnPL_CompIF_vtbl;
  }
  return pxObj;
}

uint8_t AiApplicationPnPLInit(IPnPLComponent_t *_this, IAiApplication_t *inf)
{
  assert_param(_this);
  IPnPLComponent_t *component_if = _this;
  PnPLAddComponent(component_if);

  AiApplicationPnPL *p_if_owner = (AiApplicationPnPL *) _this;
  p_if_owner->ai_applicationIF = inf;

  ai_application_comp_init();
  return 0;
}

// IPnPLComponent virtual functions definition
// *******************************************

char *AiApplicationPnPL_vtblGetKey(IPnPLComponent_t *_this)
{
  return ai_application_get_key();
}

uint8_t AiApplicationPnPL_vtblGetNCommands(IPnPLComponent_t *_this)
{
  return 3;
}

char *AiApplicationPnPL_vtblGetCommandKey(IPnPLComponent_t *_this, uint8_t id)
{
  switch (id)
  {
    case 0:
      return "ai_application*start";
      break;
    case 1:
      return "ai_application*stop";
      break;
    case 2:
      return "ai_application*switch_bank";
      break;
  }
  return 0;
}

uint8_t AiApplicationPnPL_vtblGetStatus(IPnPLComponent_t *_this, char **serializedJSON, uint32_t *size, uint8_t pretty)
{
    JSON_Value *tempJSON;

    tempJSON = json_value_init_object();

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

uint8_t AiApplicationPnPL_vtblSetProperty(IPnPLComponent_t *_this, char *serializedJSON)
{
  return 0;
}

uint8_t AiApplicationPnPL_vtblExecuteFunction(IPnPLComponent_t *_this, char *serializedJSON)
{
  AiApplicationPnPL *p_if_owner = (AiApplicationPnPL *) _this;
  JSON_Value *tempJSON = json_parse_string(serializedJSON);
  JSON_Object *tempJSONObject = json_value_get_object(tempJSON);


  if (json_object_dothas_value(tempJSONObject, "ai_application*start"))
  {
    ai_application_start_log(p_if_owner->ai_applicationIF);
  }
  if (json_object_dothas_value(tempJSONObject, "ai_application*stop"))
  {
    ai_application_stop_log(p_if_owner->ai_applicationIF);
  }
  if (json_object_dothas_value(tempJSONObject, "ai_application*switch_bank"))
  {
    ai_application_switch_bank(p_if_owner->ai_applicationIF);
  }

  json_value_free(tempJSON);
  return 0;
}

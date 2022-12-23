#include "ApplicationPnPL.h"
#include "ApplicationPnPL_vtbl.h"
#include "IPnPLComponent.h"
#include "IPnPLComponent_vtbl.h"
#include "services/SQuery.h"
#include "SensorManager.h"
#include "PnPLCompManager.h"
#include <string.h>

#include "App_model.h"

static const IPnPLComponent_vtbl sApplicationPnPL_CompIF_vtbl =
{
  ApplicationPnPL_vtblGetKey,
  ApplicationPnPL_vtblGetNCommands,
  ApplicationPnPL_vtblGetCommandKey,
  ApplicationPnPL_vtblGetStatus,
  ApplicationPnPL_vtblSetProperty,
  ApplicationPnPL_vtblExecuteFunction
};

/**
  *  ApplicationPnPL internal structure.
  */
struct _ApplicationPnPL
{
  /**
    * Implements the IPnPLComponent interface.
    */
  IPnPLComponent_t component_if;

  /**
    * Contains start and stop log functions pointers.
    */
  IApplication_t *applicationIF;
};

/* Objects instance */
/********************/

/**
  * The only instance of the task object.
  */
static ApplicationPnPL sApplicationbObj;

// Public API definition
// *********************
IPnPLComponent_t *ApplicationPnPLAlloc()
{
  IPnPLComponent_t *pxObj = (IPnPLComponent_t *) &sApplicationbObj;
  if (pxObj != NULL)
  {
    pxObj->vptr = &sApplicationPnPL_CompIF_vtbl;
  }
  return pxObj;
}

uint8_t ApplicationPnPLInit(IPnPLComponent_t *_this, IApplication_t *inf)
{
  assert_param(_this);
  IPnPLComponent_t *component_if = _this;
  PnPLAddComponent(component_if);

  ApplicationPnPL *p_if_owner = (ApplicationPnPL *) _this;
  p_if_owner->applicationIF = inf;

  application_comp_init();
  return 0;
}

// IPnPLComponent virtual functions definition
// *******************************************

char *ApplicationPnPL_vtblGetKey(IPnPLComponent_t *_this)
{
  return application_get_key();
}

uint8_t ApplicationPnPL_vtblGetNCommands(IPnPLComponent_t *_this)
{
  return 2;
}

char *ApplicationPnPL_vtblGetCommandKey(IPnPLComponent_t *_this, uint8_t id)
{
  switch (id)
  {
    case 0:
      return "application*start";
      break;
    case 1:
      return "application*stop";
      break;
  }
  return 0;
}

uint8_t ApplicationPnPL_vtblGetStatus(IPnPLComponent_t *_this, char **serializedJSON, uint32_t *size, uint8_t pretty)
{
    return 0;
}

uint8_t ApplicationPnPL_vtblSetProperty(IPnPLComponent_t *_this, char *serializedJSON)
{
  return 0; //TODO fix error management
}

uint8_t ApplicationPnPL_vtblExecuteFunction(IPnPLComponent_t *_this, char *serializedJSON)
{
  ApplicationPnPL *p_if_owner = (ApplicationPnPL *) _this;
  JSON_Value *tempJSON = json_parse_string(serializedJSON);
  JSON_Object *tempJSONObject = json_value_get_object(tempJSON);


  if (json_object_dothas_value(tempJSONObject, "application*start"))
  {
    application_start_log(p_if_owner->applicationIF);
  }
  if (json_object_dothas_value(tempJSONObject, "application*stop"))
  {
    application_stop_log(p_if_owner->applicationIF);
  }

  json_value_free(tempJSON);
  return 0;
}

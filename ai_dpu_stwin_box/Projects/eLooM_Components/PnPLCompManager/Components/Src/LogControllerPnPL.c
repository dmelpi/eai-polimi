#include "LogControllerPnPL.h"
#include "LogControllerPnPL_vtbl.h"
#include "IPnPLComponent.h"
#include "IPnPLComponent_vtbl.h"
#include "services/SQuery.h"
#include "SensorManager.h"
#include "PnPLCompManager.h"
#include <string.h>

#include "App_model.h"

static const IPnPLComponent_vtbl sLogControllerPnPL_CompIF_vtbl =
{
    LogControllerPnPL_vtblGetKey,
    LogControllerPnPL_vtblGetNCommands,
    LogControllerPnPL_vtblGetCommandKey,
    LogControllerPnPL_vtblGetStatus,
    LogControllerPnPL_vtblSetProperty,
    LogControllerPnPL_vtblExecuteFunction };

/**
 *  LogControllerPnPL internal structure.
 */
struct _LogControllerPnPL
{
  /**
   * Implements the IPnPLComponent interface.
   */
  IPnPLComponent_t component_if;

  /**
   * Contains start and stop log functions pointers.
   */
  ILogController_t *loggerIF;
};

/* Objects instance */
/********************/

/**
 * The only instance of the task object.
 */
static LogControllerPnPL sLogControllerbObj;

// Public API definition
// *********************
IPnPLComponent_t* LogControllerPnPLAlloc()
{
  IPnPLComponent_t *pxObj = (IPnPLComponent_t*) &sLogControllerbObj;
  if(pxObj != NULL)
  {
    pxObj->vptr = &sLogControllerPnPL_CompIF_vtbl;
  }
  return pxObj;
}

uint8_t LogControllerPnPLInit(IPnPLComponent_t *_this, ILogController_t *inf)
{
  assert_param(_this);
  IPnPLComponent_t *component_if = _this;
  PnPLAddComponent(component_if);

  LogControllerPnPL *p_if_owner = (LogControllerPnPL*) _this;
  p_if_owner->loggerIF = inf;

  log_controller_comp_init();
  return 0;
}

// IPnPLComponent virtual functions definition
// *******************************************

char* LogControllerPnPL_vtblGetKey(IPnPLComponent_t *_this)
{
  return log_controller_get_key();
}

uint8_t LogControllerPnPL_vtblGetNCommands(IPnPLComponent_t *_this)
{
  return 5;
}

char* LogControllerPnPL_vtblGetCommandKey(IPnPLComponent_t *_this, uint8_t id)
{
  switch(id)
  {
    case 0:
      return "log_controller*log_control";
      break;
    case 1:
      return "log_controller*save_config";
      break;
    case 2:
      return "log_controller*set_usb_ep";
      break;
    case 3:
      return "log_controller*set_stream";
      break;
    case 4:
      return "log_controller*stream_control";
      break;
//		case 3:
//			return "log_controller*tag_control";
//		break;
//		case 4:
//		    return "log_controller*get_acq_info";
//		break;
  }
  return 0;
}

uint8_t LogControllerPnPL_vtblGetStatus(IPnPLComponent_t *_this, char **serializedJSON, uint32_t *size, uint8_t pretty)
{
  return 1; //TODO check error management
}

uint8_t LogControllerPnPL_vtblSetProperty(IPnPLComponent_t *_this, char *serializedJSON)
{
  return 0; //TODO fix error management
}

uint8_t LogControllerPnPL_vtblExecuteFunction(IPnPLComponent_t *_this, char *serializedJSON)
{
  LogControllerPnPL *p_if_owner = (LogControllerPnPL*) _this;
  JSON_Value *tempJSON = json_parse_string(serializedJSON);
  JSON_Object *tempJSONObject = json_value_get_object(tempJSON);

  if(json_object_dothas_value(tempJSONObject, "log_controller*log_control.start_log"))
  {
    bool status = json_object_dotget_boolean(tempJSONObject, "log_controller*log_control.start_log.status");
    uint32_t mode = (uint32_t) (json_object_dotget_number(tempJSONObject, "log_controller*log_control.start_log.mode"));
    log_controller_start_log(p_if_owner->loggerIF, status, mode); //status == true --> start_log, status == false --> stop_log
  }
  if(json_object_dothas_value(tempJSONObject, "log_controller*save_config"))
  {
    //TODO save config (overwrite default configuration)???
  }
  if(json_object_dothas_value(tempJSONObject, "log_controller*stream_control.compute_ids"))
  {
    bool status = json_object_dotget_boolean(tempJSONObject, "log_controller*stream_control.compute_ids");
//    const char *comp_name = json_object_dotget_string(tempJSONObject, "log_controller*set_stream.set_id.comp_name");
//    int32_t stream_id = json_object_dotget_number(tempJSONObject, "log_controller*set_stream.set_id.stream_id");
//    log_controller_set_stream(p_if_owner->loggerIF, comp_name, stream_id);
    log_controller_compute_stream_ids(p_if_owner->loggerIF, status); //status == true --> compute_ids, status == false --> reset_ids
  }
//  if(json_object_dothas_value(tempJSONObject, "log_controller*set_stream.set_id"))
//  {
//    const char *comp_name = json_object_dotget_string(tempJSONObject, "log_controller*set_stream.set_id.comp_name");
//    int32_t stream_id = json_object_dotget_number(tempJSONObject, "log_controller*set_stream.set_id.stream_id");
//    log_controller_set_stream(p_if_owner->loggerIF, comp_name, stream_id);
//  }
//  if(json_object_dothas_value(tempJSONObject, "log_controller*set_usb_ep.set_ep"))
//  {
//    const char *comp_name = json_object_dotget_string(tempJSONObject, "log_controller*set_usb_ep.set_ep.comp_name");
//    int32_t ep = json_object_dotget_number(tempJSONObject, "log_controller*set_usb_ep.set_ep.usb_ep");
//    log_controller_set_usb_ep(p_if_owner->loggerIF, comp_name, ep);
//  }

//	if (json_object_dothas_value(tempJSONObject, "log_controller*tag_control.tag_on_off")){
//		const char* label = json_object_dotget_string(tempJSONObject, "log_controller*tag_control.tag_on_off.label");
//		uint8_t status = (uint8_t)(json_object_dotget_boolean(tempJSONObject, "log_controller*tag_control.tag_on_off.status"));
//		log_control_comp.tagFuncPtr(label, status);
//	}
//  if (json_object_dothas_value(tempJSONObject, "log_controller*get_acq_info")){
//      log_control_comp.getAcqFuncPtr(serialized_json);
//  }


  json_value_free(tempJSON);
  return 0;
}

#include "TagsInfoPnPL.h"
#include "TagsInfoPnPL_vtbl.h"
#include "IPnPLComponent.h"
#include "IPnPLComponent_vtbl.h"
#include "PnPLCompManager.h"
#include <string.h>

#include "App_model.h"

static const IPnPLComponent_vtbl sTagsInfoPnPL_CompIF_vtbl =
{
    TagsInfoPnPL_vtblGetKey,
    TagsInfoPnPL_vtblGetNCommands,
    TagsInfoPnPL_vtblGetCommandKey,
    TagsInfoPnPL_vtblGetStatus,
    TagsInfoPnPL_vtblSetProperty,
    TagsInfoPnPL_vtblExecuteFunction };

/**
 *  TagsInfoPnPL internal structure.
 */
struct _TagsInfoPnPL
{
  /**
   * Implements the IPnPLComponent interface.
   */
  IPnPLComponent_t component_if;

};

/* Objects instance */
/********************/

static TagsInfoPnPL sTagsInfoPnPL;

// Public API definition
// *********************
IPnPLComponent_t* TagsInfoPnPLAlloc()
{
  IPnPLComponent_t *pxObj = (IPnPLComponent_t*) &sTagsInfoPnPL;
  if(pxObj != NULL)
  {
    pxObj->vptr = &sTagsInfoPnPL_CompIF_vtbl;
  }
  return pxObj;
}

uint8_t TagsInfoPnPLInit(IPnPLComponent_t *_this)
{
  IPnPLComponent_t *component_if = _this;
  PnPLAddComponent(component_if);
  tags_info_comp_init();
  return 0;
}

// IPnPLComponent virtual functions definition
// *******************************************

char* TagsInfoPnPL_vtblGetKey(IPnPLComponent_t *_this)
{
  return tags_info_get_key();
}

uint8_t TagsInfoPnPL_vtblGetNCommands(IPnPLComponent_t *_this)
{
  return 0;
}

char* TagsInfoPnPL_vtblGetCommandKey(IPnPLComponent_t *_this, uint8_t id)
{
  return "";
}

uint8_t TagsInfoPnPL_vtblGetStatus(IPnPLComponent_t *_this, char **serializedJSON, uint32_t *size, uint8_t pretty)
{
  JSON_Value *tempJSON;
  JSON_Object *JSON_Status;

  tempJSON = json_value_init_object();
  JSON_Status = json_value_get_object(tempJSON);

  uint32_t temp = 0;
  tags_info_get_max_tags_num(&temp);
  json_object_dotset_number(JSON_Status, "tags_info.max_tags_num", temp);
  char *temp_s = "";
  tags_info_get_sw_tag0_label(&temp_s);
  json_object_dotset_string(JSON_Status, "tags_info.sw_tag0.label", temp_s);
  bool temp_b = 0;
  tags_info_get_sw_tag0_enabled(&temp_b);
  json_object_dotset_boolean(JSON_Status, "tags_info.sw_tag0.enabled", temp_b);
  tags_info_get_sw_tag0_status(&temp_b);
  json_object_dotset_boolean(JSON_Status, "tags_info.sw_tag0.status", temp_b);
  tags_info_get_sw_tag1_label(&temp_s);
  json_object_dotset_string(JSON_Status, "tags_info.sw_tag1.label", temp_s);
  tags_info_get_sw_tag1_enabled(&temp_b);
  json_object_dotset_boolean(JSON_Status, "tags_info.sw_tag1.enabled", temp_b);
  tags_info_get_sw_tag1_status(&temp_b);
  json_object_dotset_boolean(JSON_Status, "tags_info.sw_tag1.status", temp_b);
  tags_info_get_sw_tag2_label(&temp_s);
  json_object_dotset_string(JSON_Status, "tags_info.sw_tag2.label", temp_s);
  tags_info_get_sw_tag2_enabled(&temp_b);
  json_object_dotset_boolean(JSON_Status, "tags_info.sw_tag2.enabled", temp_b);
  tags_info_get_sw_tag2_status(&temp_b);
  json_object_dotset_boolean(JSON_Status, "tags_info.sw_tag2.status", temp_b);
  tags_info_get_sw_tag3_label(&temp_s);
  json_object_dotset_string(JSON_Status, "tags_info.sw_tag3.label", temp_s);
  tags_info_get_sw_tag3_enabled(&temp_b);
  json_object_dotset_boolean(JSON_Status, "tags_info.sw_tag3.enabled", temp_b);
  tags_info_get_sw_tag3_status(&temp_b);
  json_object_dotset_boolean(JSON_Status, "tags_info.sw_tag3.status", temp_b);
  tags_info_get_sw_tag4_label(&temp_s);
  json_object_dotset_string(JSON_Status, "tags_info.sw_tag4.label", temp_s);
  tags_info_get_sw_tag4_enabled(&temp_b);
  json_object_dotset_boolean(JSON_Status, "tags_info.sw_tag4.enabled", temp_b);
  tags_info_get_sw_tag4_status(&temp_b);
  json_object_dotset_boolean(JSON_Status, "tags_info.sw_tag4.status", temp_b);

  tags_info_get_hw_tag0_label(&temp_s);
  json_object_dotset_string(JSON_Status, "tags_info.hw_tag0.label", temp_s);
  tags_info_get_hw_tag0_enabled(&temp_b);
  json_object_dotset_boolean(JSON_Status, "tags_info.hw_tag0.enabled", temp_b);
  tags_info_get_hw_tag1_label(&temp_s);
  json_object_dotset_string(JSON_Status, "tags_info.hw_tag1.label", temp_s);
  tags_info_get_hw_tag1_enabled(&temp_b);
  json_object_dotset_boolean(JSON_Status, "tags_info.hw_tag1.enabled", temp_b);
  tags_info_get_hw_tag2_label(&temp_s);
  json_object_dotset_string(JSON_Status, "tags_info.hw_tag2.label", temp_s);
  tags_info_get_hw_tag2_enabled(&temp_b);
  json_object_dotset_boolean(JSON_Status, "tags_info.hw_tag2.enabled", temp_b);
  tags_info_get_hw_tag3_label(&temp_s);
  json_object_dotset_string(JSON_Status, "tags_info.hw_tag3.label", temp_s);
  tags_info_get_hw_tag3_enabled(&temp_b);
  json_object_dotset_boolean(JSON_Status, "tags_info.hw_tag3.enabled", temp_b);
  tags_info_get_hw_tag4_label(&temp_s);
  json_object_dotset_string(JSON_Status, "tags_info.hw_tag4.label", temp_s);
  tags_info_get_hw_tag4_enabled(&temp_b);
  json_object_dotset_boolean(JSON_Status, "tags_info.hw_tag4.enabled", temp_b);


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

uint8_t TagsInfoPnPL_vtblSetProperty(IPnPLComponent_t *_this, char *serializedJSON)
{
  JSON_Value *tempJSON = json_parse_string(serializedJSON);
  JSON_Object *tempJSONObject = json_value_get_object(tempJSON);

  uint8_t ret = 0;

  if(json_object_dothas_value(tempJSONObject, "tags_info.sw_tag0.label"))
  {
    const char *sw_tag0_label = json_object_dotget_string(tempJSONObject, "tags_info.sw_tag0.label");
    tags_info_set_sw_tag0_label(sw_tag0_label);
  }
  if(json_object_dothas_value(tempJSONObject, "tags_info.sw_tag1.label"))
  {
    const char *sw_tag1_label = json_object_dotget_string(tempJSONObject, "tags_info.sw_tag1.label");
    tags_info_set_sw_tag1_label(sw_tag1_label);
  }
  if(json_object_dothas_value(tempJSONObject, "tags_info.sw_tag2.label"))
  {
    const char *sw_tag2_label = json_object_dotget_string(tempJSONObject, "tags_info.sw_tag2.label");
    tags_info_set_sw_tag2_label(sw_tag2_label);
  }
  if(json_object_dothas_value(tempJSONObject, "tags_info.sw_tag3.label"))
  {
    const char *sw_tag3_label = json_object_dotget_string(tempJSONObject, "tags_info.sw_tag3.label");
    tags_info_set_sw_tag3_label(sw_tag3_label);
  }
  if(json_object_dothas_value(tempJSONObject, "tags_info.sw_tag4.label"))
  {
    const char *sw_tag4_label = json_object_dotget_string(tempJSONObject, "tags_info.sw_tag4.label");
    tags_info_set_sw_tag4_label(sw_tag4_label);
  }
  if(json_object_dothas_value(tempJSONObject, "tags_info.sw_tag0.enabled"))
  {
    bool sw_tag0_enabled = json_object_dotget_boolean(tempJSONObject, "tags_info.sw_tag0.enabled");
    tags_info_set_sw_tag0_enabled(sw_tag0_enabled);
  }
  if(json_object_dothas_value(tempJSONObject, "tags_info.sw_tag1.enabled"))
  {
    bool sw_tag1_enabled = json_object_dotget_boolean(tempJSONObject, "tags_info.sw_tag1.enabled");
    tags_info_set_sw_tag1_enabled(sw_tag1_enabled);
  }
  if(json_object_dothas_value(tempJSONObject, "tags_info.sw_tag2.enabled"))
  {
    bool sw_tag2_enabled = json_object_dotget_boolean(tempJSONObject, "tags_info.sw_tag2.enabled");
    tags_info_set_sw_tag2_enabled(sw_tag2_enabled);
  }
  if(json_object_dothas_value(tempJSONObject, "tags_info.sw_tag3.enabled"))
  {
    bool sw_tag3_enabled = json_object_dotget_boolean(tempJSONObject, "tags_info.sw_tag3.enabled");
    tags_info_set_sw_tag3_enabled(sw_tag3_enabled);
  }
  if(json_object_dothas_value(tempJSONObject, "tags_info.sw_tag4.enabled"))
  {
    bool sw_tag4_enabled = json_object_dotget_boolean(tempJSONObject, "tags_info.sw_tag4.enabled");
    tags_info_set_sw_tag4_enabled(sw_tag4_enabled);
  }
  if(json_object_dothas_value(tempJSONObject, "tags_info.sw_tag0.status"))
  {
    bool sw_tag0_status = json_object_dotget_boolean(tempJSONObject, "tags_info.sw_tag0.status");
    tags_info_set_sw_tag0_status(sw_tag0_status);
  }
  if(json_object_dothas_value(tempJSONObject, "tags_info.sw_tag1.status"))
  {
    bool sw_tag1_status = json_object_dotget_boolean(tempJSONObject, "tags_info.sw_tag1.status");
    tags_info_set_sw_tag1_status(sw_tag1_status);
  }
  if(json_object_dothas_value(tempJSONObject, "tags_info.sw_tag2.status"))
  {
    bool sw_tag2_status = json_object_dotget_boolean(tempJSONObject, "tags_info.sw_tag2.status");
    tags_info_set_sw_tag2_status(sw_tag2_status);
  }
  if(json_object_dothas_value(tempJSONObject, "tags_info.sw_tag3.status"))
  {
    bool sw_tag3_status = json_object_dotget_boolean(tempJSONObject, "tags_info.sw_tag3.status");
    tags_info_set_sw_tag3_status(sw_tag3_status);
  }
  if(json_object_dothas_value(tempJSONObject, "tags_info.sw_tag4.status"))
  {
    bool sw_tag4_status = json_object_dotget_boolean(tempJSONObject, "tags_info.sw_tag4.status");
    tags_info_set_sw_tag4_status(sw_tag4_status);
  }
  if(json_object_dothas_value(tempJSONObject, "tags_info.hw_tag0.label"))
  {
    const char *hw_tag0_label = json_object_dotget_string(tempJSONObject, "tags_info.hw_tag0.label");
    tags_info_set_hw_tag0_label(hw_tag0_label);
  }
  if(json_object_dothas_value(tempJSONObject, "tags_info.hw_tag1.label"))
  {
    const char *hw_tag1_label = json_object_dotget_string(tempJSONObject, "tags_info.hw_tag1.label");
    tags_info_set_hw_tag1_label(hw_tag1_label);
  }
  if(json_object_dothas_value(tempJSONObject, "tags_info.hw_tag2.label"))
  {
    const char *hw_tag2_label = json_object_dotget_string(tempJSONObject, "tags_info.hw_tag2.label");
    tags_info_set_hw_tag2_label(hw_tag2_label);
  }
  if(json_object_dothas_value(tempJSONObject, "tags_info.hw_tag3.label"))
  {
    const char *hw_tag3_label = json_object_dotget_string(tempJSONObject, "tags_info.hw_tag3.label");
    tags_info_set_hw_tag3_label(hw_tag3_label);
  }
  if(json_object_dothas_value(tempJSONObject, "tags_info.hw_tag4.label"))
  {
    const char *hw_tag4_label = json_object_dotget_string(tempJSONObject, "tags_info.hw_tag4.label");
    tags_info_set_hw_tag4_label(hw_tag4_label);
  }
  if(json_object_dothas_value(tempJSONObject, "tags_info.hw_tag0.enabled"))
  {
    bool hw_tag0_enabled = json_object_dotget_boolean(tempJSONObject, "tags_info.hw_tag0.enabled");
    tags_info_set_hw_tag0_enabled(hw_tag0_enabled);
  }
  if(json_object_dothas_value(tempJSONObject, "tags_info.hw_tag1.enabled"))
  {
    bool hw_tag1_enabled = json_object_dotget_boolean(tempJSONObject, "tags_info.hw_tag1.enabled");
    tags_info_set_hw_tag1_enabled(hw_tag1_enabled);
  }
  if(json_object_dothas_value(tempJSONObject, "tags_info.hw_tag2.enabled"))
  {
    bool hw_tag2_enabled = json_object_dotget_boolean(tempJSONObject, "tags_info.hw_tag2.enabled");
    tags_info_set_hw_tag2_enabled(hw_tag2_enabled);
  }
  if(json_object_dothas_value(tempJSONObject, "tags_info.hw_tag3.enabled"))
  {
    bool hw_tag3_enabled = json_object_dotget_boolean(tempJSONObject, "tags_info.hw_tag3.enabled");
    tags_info_set_hw_tag3_enabled(hw_tag3_enabled);
  }
  if(json_object_dothas_value(tempJSONObject, "tags_info.hw_tag4.enabled"))
  {
    bool hw_tag4_enabled = json_object_dotget_boolean(tempJSONObject, "tags_info.hw_tag4.enabled");
    tags_info_set_hw_tag4_enabled(hw_tag4_enabled);
  }

  json_value_free(tempJSON);
  return ret;
}

uint8_t TagsInfoPnPL_vtblExecuteFunction(IPnPLComponent_t *_this, char *serializedJSON)
{
  return 1; //TODO fix error management
}

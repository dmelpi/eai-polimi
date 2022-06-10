#include "PnPLCompManager.h"
#include "SensorManager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static PnPLCompManager_t spPnPLObj =
{
    0 };

//TODO manage these fields
volatile static uint8_t boardId = 0x0E;
volatile static uint8_t fwId = 0xFF;
//TODO manage these fields

/* Unique ID is directly derived from STM32 UID and converted to string
string needs to be 25bytes 24+\0  */
static void PnPLGetUniqueID(char *id)
{
  uint32_t stm32_UID[3];

  stm32_UID[0] = (uint32_t)(READ_REG(*((uint32_t *)UID_BASE)));//LL_GetUID_Word0();
  stm32_UID[1] = (uint32_t)(READ_REG(*((uint32_t *)(UID_BASE + 4U))));//LL_GetUID_Word1();
  stm32_UID[2] = (uint32_t)(READ_REG(*((uint32_t *)(UID_BASE + 8U))));//LL_GetUID_Word2();

  sprintf(id, "%08lX%08lX%08lX", stm32_UID[0], stm32_UID[1], stm32_UID[2]);
}

/**
* @brief Generate and store the Acquisition UUID
* @param None
* @retval None
*/
void PnPLGenerateAcquisitionUUID(char *uuid)
{
  uint32_t UUID[4];
  uint8_t *p8 = (uint8_t*)UUID;

  UUID[0] = (uint32_t)rand();
  UUID[1] = (uint32_t)rand();
  UUID[2] = (uint32_t)rand();
  UUID[3] = (uint32_t)rand();

  /*
  * UUID format
  * xxxxxxxx-xxxx-Mxxx-Nxxx-xxxxxxxxxxxx
  * M: The four bits of digit M are the UUID version --> 4 is for random number
  * N: 1 to 3 most significant bits of digit N represent UUID variant --> using Variant 1 (10x)
  * */
  p8[5] = 0x40 | (p8[5] & 0xf);
  p8[11] = 0x80 | (p8[11] & 0x3f); /* in variant 1, 3rd bit can be ignored */

  uuid += sprintf(uuid, "%08lx-", UUID[0]);
  uuid += sprintf(uuid, "%04lx-%04lx-", (UUID[1]>>16)&0xFFFF, UUID[1]&0xFFFF);
  uuid += sprintf(uuid, "%04lx-%04lx", (UUID[2]>>16)&0xFFFF, UUID[2]&0xFFFF);
  uuid += sprintf(uuid, "%08lx", UUID[3]);
}

uint8_t PnPLAddComponent(IPnPLComponent_t *pComponent)
{
  assert_param(pComponent);
  uint8_t id = 0;
  uint16_t ii = 0;
  boolean_t add_ok = FALSE;

  for(ii = 0; ii < spPnPLObj.n_components + 1; ii++)
  {
    if(spPnPLObj.Components[ii] == NULL)
    {
      spPnPLObj.Components[ii] = pComponent;
      add_ok = TRUE;
      id = ii;
    }
  }

  if(add_ok)
  {
    spPnPLObj.n_components++;
  }

  return id;
}

//TODO No! remove direct dependency from SensorManager
uint16_t PnPLGetNSensors(void)
{
  return SMGetSensorManager()->n_sensors;
}

uint16_t PnPLGetNComponents(void)
{
  return spPnPLObj.n_components;
}

uint8_t PnPLGetPresentationJSON(char **serializedJSON, uint32_t *size)
{
  JSON_Value *tempJSON;
  JSON_Object *JSON_Status;

  tempJSON = json_value_init_object();
  JSON_Status = json_value_get_object(tempJSON);

  json_object_dotset_number(JSON_Status, "board_id", boardId);
  json_object_dotset_number(JSON_Status, "fw_id", fwId);

  *serializedJSON = json_serialize_to_string(tempJSON);
  *size = json_serialization_size(tempJSON);

  json_value_free(tempJSON);
  return PNPL_CMD_NO_ERROR_CODE;
}

uint8_t PnPLGetDeviceStatusJSON(char **serializedJSON, uint32_t *size, uint8_t pretty)
{

  char serial_number[25];
//  char uuid[37];

  JSON_Value *tempJSON;
  JSON_Value *tempJSON1;
  JSON_Object *JSON_Device;
  JSON_Array *JSON_ComponentArray;

  uint32_t sz_comp = 0;
  char *ser_comp = NULL;

  tempJSON = json_value_init_object();
  JSON_Device = json_value_get_object(tempJSON);

  /*
  "board_id":0,
  "fw_id":0,
  "sn":"3270366B3139",
  "uuid":"62d7d7da-9030-11ec-a78f-1098c361c94d"
  */
  json_object_dotset_number(JSON_Device, "board_id", boardId);
  json_object_dotset_number(JSON_Device, "fw_id", fwId);
  PnPLGetUniqueID(serial_number);
  json_object_dotset_string(JSON_Device, "sn", serial_number);
//  PnPLGenerateAcquisitionUUID(uuid);
//  json_object_dotset_string(JSON_Device, "uuid", uuid);

//  json_object_set_value(JSON_Device, "device", json_value_init_array());
//  JSON_ComponentArray = json_object_dotget_array(JSON_Device, "device");
  json_object_set_value(JSON_Device, "components", json_value_init_array());
  JSON_ComponentArray = json_object_dotget_array(JSON_Device, "components");

  for(uint8_t i = 0; i < PnPLGetNComponents(); i++)
  {
    IPnPLComponent_t *p_obj = spPnPLObj.Components[i];
    sz_comp = 0;
    ser_comp = NULL;
    IPnPLComponentGetStatus(p_obj, &ser_comp, &sz_comp, 0);
    tempJSON1 = json_parse_string(ser_comp);
    json_array_append_value(JSON_ComponentArray, tempJSON1);
    json_free_serialized_string(ser_comp);
  }

  /* convert to a json string and write to file */
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
  return PNPL_CMD_NO_ERROR_CODE;
}

uint8_t PnPLUpdateDeviceStatusFromJSON(char *serializedJSON)
{
  char componentName[COMP_KEY_MAX_LENGTH];

  JSON_Array *JSON_components;
  JSON_Object *component;
  JSON_Value *component_value;

  JSON_Value *tempJSON = json_parse_string(serializedJSON);
  JSON_Object *JSON_ParseHandler = json_value_get_object(tempJSON);

  if(json_object_has_value(JSON_ParseHandler, "components") == 1)
  {
    JSON_components = json_object_get_array(JSON_ParseHandler, "components");
  }
  else
  { //old status version (without "device" key)
    JSON_components = json_object_get_array(JSON_ParseHandler, "");
  }

  for(uint8_t i = 0; i < json_array_get_count(JSON_components); i++)
  {
    component = json_array_get_object(JSON_components, i);
    strcpy(componentName, json_object_get_name(component, 0));
    for(uint8_t j = 0; j < PnPLGetNComponents(); j++)
    {
      IPnPLComponent_t *p_obj = (IPnPLComponent_t*) (spPnPLObj.Components[j]);
      if(strcmp(componentName, IPnPLComponentGetKey(p_obj)) == 0)
      {
        component_value = json_object_get_value(component, componentName);
        char *comp_string = json_serialize_to_string(json_value_get_parent(component_value));
        IPnPLComponentSetProperty(p_obj, comp_string);
        json_value_free(component_value);
        json_free_serialized_string(comp_string);
        break;
      }
    }
  }

  json_value_free(tempJSON);
  return PNPL_CMD_NO_ERROR_CODE;
}

static uint8_t extract_PnPL_cmd_data(char *commandString, uint8_t *commandType, char *componentName)
{
  JSON_Value *tempJSON = json_parse_string(commandString);
  if(tempJSON != 0x0)
  {
    JSON_Object *tempJSONObject = json_value_get_object(tempJSON);
    strcpy(componentName, json_object_get_name(tempJSONObject, 0));
    for(uint8_t i = 0; i < PnPLGetNComponents(); i++)
    {
      IPnPLComponent_t *p_obj = (IPnPLComponent_t*) (spPnPLObj.Components[i]);
      if(strcmp(IPnPLComponentGetKey(p_obj), componentName) == 0)
      { //Check if extracted string is a component added to the current FW
        *commandType = PNPL_CMD_SET;
        json_value_free(tempJSON);
        return PNPL_CMD_NO_ERROR_CODE;
      }
      //in case of control commands
      uint8_t nOfCommands = IPnPLComponentGetNCommands(p_obj);
      if(nOfCommands > 0)
      {
        for(uint8_t j = 0; j < nOfCommands; j++)
        {
          if(strcmp(IPnPLComponentGetCommandKey(p_obj, j), componentName) == 0)
          {
            *commandType = PNPL_CMD_SET;
            json_value_free(tempJSON);
            return PNPL_CMD_NO_ERROR_CODE;
          }
        }
      }
    }

    if(strcmp(componentName, "get_status") == 0)
    {
      *commandType = PNPL_CMD_GET;
      strcpy(componentName, json_object_get_string(tempJSONObject, "get_status"));
      json_value_free(tempJSON);
      return PNPL_CMD_NO_ERROR_CODE;
    }
    else if(strcmp(componentName, "update_device_status") == 0)
    {
      *commandType = PNPL_CMD_UPDATE_DEVICE;
      JSON_Value *tempJSONValue = json_object_get_value(tempJSONObject, "update_device_status");
      strcpy(commandString, json_serialize_to_string(tempJSONValue));
      json_value_free(tempJSONValue);
      return PNPL_CMD_NO_ERROR_CODE;
    }
//    else if(strcmp(componentName, "get_acq_info") == 0)
//    {
//      *commandType = PNPL_CMD_GET_ACQ_INFO;
//      strcpy(componentName, json_object_get_string(tempJSONObject, "get_acq_info"));
//      json_value_free(tempJSON);
//      return SYS_NO_ERROR_CODE;
//    }
    else if(strcmp(componentName, "system_config") == 0)
    {
      *commandType = PNPL_CMD_SYSTEM_CONFIG;
      JSON_Object *tempJSONObject2 = json_object_get_object(tempJSONObject, "system_config");
      strcpy(componentName, json_object_get_string(tempJSONObject2, "comp_name"));
      json_value_free(tempJSON);
      return PNPL_CMD_NO_ERROR_CODE;
    }
    else if(strcmp(componentName, "system_info") == 0)
    {
      *commandType = PNPL_CMD_SYSTEM_INFO;
      strcpy(componentName, "");
      json_value_free(tempJSON);
      return PNPL_CMD_NO_ERROR_CODE;
    }
  }
  json_value_free(tempJSON);
  //Not JSON command!
  return PNPL_CMD_ERROR_CODE;
}

uint8_t PnPLParseCommand(char *commandString, PnPLCommand_t *command)
{
  uint8_t commandType = 0;
  char componentName[2 * COMP_KEY_MAX_LENGTH]; //2* bacause this could be a comp or a comm key. If comm_key this is in the form (comp_key*comm_key)
  uint8_t ret = extract_PnPL_cmd_data(commandString, &commandType, componentName);

  command->comm_type = commandType;
  strcpy(command->comp_name, componentName);

  if(ret == PNPL_CMD_NO_ERROR_CODE)
  {
    if(commandType == PNPL_CMD_SET)
    {
      //Select right parse/update function
      for(uint8_t i = 0; i < PnPLGetNComponents(); i++)
      {
        IPnPLComponent_t *p_obj = (IPnPLComponent_t*) (spPnPLObj.Components[i]);
        if(strcmp(componentName, IPnPLComponentGetKey(p_obj)) == 0)
        {
          IPnPLComponentSetProperty(p_obj, commandString);
        }
        uint8_t nOfCommands = IPnPLComponentGetNCommands(p_obj); // 0 if none
        if(nOfCommands > 0)
        {
          for(uint8_t j = 0; j < nOfCommands; j++)
          {
            if(strcmp(IPnPLComponentGetCommandKey(p_obj, j), componentName) == 0)
            {
              IPnPLCommandExecuteFunction(p_obj, commandString);
            }
          }
        }
      }
    }
    if(commandType == PNPL_CMD_SYSTEM_CONFIG)
    {
      //TODO merge this logic with SET?? or make a function!
      for(uint8_t i = 0; i < PnPLGetNComponents(); i++)
      {
        IPnPLComponent_t *p_obj = (IPnPLComponent_t*) (spPnPLObj.Components[i]);
        if(strcmp(componentName, IPnPLComponentGetKey(p_obj)) == 0)
        {
          IPnPLComponentSetProperty(p_obj, commandString);
        }
      }
    }
    if(commandType == PNPL_CMD_UPDATE_DEVICE)
    {
      PnPLUpdateDeviceStatusFromJSON(commandString);
    }
  }
  else
  {
    command->comm_type = PNPL_CMD_ERROR;
  }
  return ret;
}

uint8_t PnPLSerializeResponse(PnPLCommand_t *command, char **SerializedJSON, uint32_t *size, uint8_t pretty)
{
  if(command->comm_type == PNPL_CMD_SYSTEM_INFO)
  {
    PnPLGetPresentationJSON(SerializedJSON, size);
    return PNPL_CMD_NO_ERROR_CODE;
  }
//TODO
//	  else if(command->comm_type == PNPL_CMD_GET_ACQ_INFO){
//	      *size_in = PnPL_GetAcquisitionInfo(serialized_json, pretty);
//
//	      return 0;
//	  }
  else if(command->comm_type == PNPL_CMD_GET)
  {
    uint8_t comp_found = 0;
    if(strcmp(command->comp_name, "all") == 0)
    {
      PnPLGetDeviceStatusJSON(SerializedJSON, size, pretty);
      comp_found = PnPLGetNComponents();
    }
    else
    {
      for(uint8_t i = 0; i < PnPLGetNComponents(); i++)
      {
        IPnPLComponent_t *p_obj = (IPnPLComponent_t*) (spPnPLObj.Components[i]);
        if(strcmp(command->comp_name, IPnPLComponentGetKey(p_obj)) == 0)
        {
          uint8_t ret = IPnPLComponentGetStatus(p_obj, SerializedJSON, size, pretty);
          if(ret == 0)
          {
            comp_found = 1;
          }
          break;
        }
      }
    }
    if(comp_found > 0)
    {
      return PNPL_CMD_NO_ERROR_CODE;
    }
  }
  else if(command->comm_type == PNPL_CMD_ERROR)
  {
    strcpy(*SerializedJSON,"PnPL_Error");
    *size = 10;
    return PNPL_CMD_NO_ERROR_CODE;
  }
  return PNPL_CMD_ERROR_CODE;
}

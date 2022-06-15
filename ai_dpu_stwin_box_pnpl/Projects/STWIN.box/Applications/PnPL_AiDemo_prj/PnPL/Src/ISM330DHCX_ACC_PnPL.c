#include "ISM330DHCX_ACC_PnPL.h"
#include "ISM330DHCX_ACC_PnPL_vtbl.h"
#include "IPnPLComponent.h"
#include "IPnPLComponent_vtbl.h"
#include "PnPLCompManager.h"
#include <string.h>

#include "App_model.h"

#define hz12_5 12.5
#define hz26 26
#define hz52 52
#define hz104 104
#define hz208 208
#define hz416 416
#define hz833 833
#define hz1666 1666
#define hz3332 3332
#define hz6667 6667

#define g2 2
#define g4 4
#define g8 8
#define g16 16

static const IPnPLComponent_vtbl sISM330DHCX_ACC_PnPL_CompIF_vtbl =
{
  ISM330DHCX_ACC_PnPL_vtblGetKey,
  ISM330DHCX_ACC_PnPL_vtblGetNCommands,
  ISM330DHCX_ACC_PnPL_vtblGetCommandKey,
  ISM330DHCX_ACC_PnPL_vtblGetStatus,
  ISM330DHCX_ACC_PnPL_vtblSetProperty,
  ISM330DHCX_ACC_PnPL_vtblExecuteFunction
};

/**
  *  ISM330DHCX_ACC_PnPL internal structure.
  */
struct _ISM330DHCX_ACC_PnPL
{

  /**
    * Implements the IPnPLComponent interface.
    */
  IPnPLComponent_t component_if;

};

/* Objects instance */
/********************/

static ISM330DHCX_ACC_PnPL sISM330DHCX_ACC_bObj;

// Public API definition
// *********************

IPnPLComponent_t *ISM330DHCX_ACC_PnPLAlloc()
{
  IPnPLComponent_t *pxObj = (IPnPLComponent_t *) &sISM330DHCX_ACC_bObj;
  if (pxObj != NULL)
  {
    pxObj->vptr = &sISM330DHCX_ACC_PnPL_CompIF_vtbl;
  }
  return pxObj;
}

uint8_t ISM330DHCX_ACC_PnPLInit(IPnPLComponent_t *_this)
{
  IPnPLComponent_t *component_if = _this;
  PnPLAddComponent(component_if);
  ism330dhcx_acc_comp_init();
  return 0;
}

// IPnPLComponent virtual functions definition
// *******************************************

char *ISM330DHCX_ACC_PnPL_vtblGetKey(IPnPLComponent_t *_this)
{
  return ism330dhcx_acc_get_key();
}

uint8_t ISM330DHCX_ACC_PnPL_vtblGetNCommands(IPnPLComponent_t *_this)
{
  return 0;
}

char *ISM330DHCX_ACC_PnPL_vtblGetCommandKey(IPnPLComponent_t *_this, uint8_t id)
{
  return "";
}

uint8_t ISM330DHCX_ACC_PnPL_vtblGetStatus(IPnPLComponent_t *_this, char **serializedJSON, uint32_t *size,
                                          uint8_t pretty)
{
  JSON_Value *tempJSON;
  JSON_Object *JSON_Status;

  tempJSON = json_value_init_object();
  JSON_Status = json_value_get_object(tempJSON);

  float temp = 0;
  ism330dhcx_acc_get_odr(&temp);
  uint8_t enum_id = 0;
  if(temp == hz12_5)
  {
    enum_id = 0;
  }
  else if(temp == hz26)
  {
    enum_id = 1;
  }
  if(temp == hz52)
  {
    enum_id = 2;
  }
  if(temp == hz104)
  {
    enum_id = 3;
  }
  if(temp == hz208)
  {
    enum_id = 4;
  }
  if(temp == hz416)
  {
    enum_id = 5;
  }
  if(temp == hz833)
  {
    enum_id = 6;
  }
  if(temp == hz1666)
  {
    enum_id = 7;
  }
  if(temp == hz3332)
  {
    enum_id = 8;
  }
  if(temp == hz6667)
  {
    enum_id = 9;
  }

  json_object_dotset_number(JSON_Status, "ism330dhcx_acc.odr", enum_id);
  ism330dhcx_acc_get_fs(&temp);
  enum_id = 0;
  if(temp == g2)
  {
    enum_id = 0;
  }
  else if(temp == g4)
  {
    enum_id = 1;
  }
  else if(temp == g8)
  {
    enum_id = 2;
  }
  else if(temp == g16)
  {
    enum_id = 3;
  }
  json_object_dotset_number(JSON_Status, "ism330dhcx_acc.fs", enum_id);

  json_object_dotset_number(JSON_Status, "ism330dhcx_acc.c_type", COMP_TYPE_SENSOR); //Added looking @the component schema field (this is :sensors)

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

uint8_t ISM330DHCX_ACC_PnPL_vtblSetProperty(IPnPLComponent_t *_this, char *serializedJSON)
{

  JSON_Value *tempJSON = json_parse_string(serializedJSON);
  JSON_Object *tempJSONObject = json_value_get_object(tempJSON);

  uint8_t ret = 0;

  if(json_object_dothas_value(tempJSONObject, "ism330dhcx_acc.odr"))
  {
    int odr = json_object_dotget_number(tempJSONObject, "ism330dhcx_acc.odr"); //ODR is an Enum --> value = float to contain all possible numeric values
    switch(odr)
    {
      case 0:
        ism330dhcx_acc_set_odr(hz12_5);
        break;
      case 1:
        ism330dhcx_acc_set_odr(hz26);
        break;
      case 2:
        ism330dhcx_acc_set_odr(hz52);
        break;
      case 3:
        ism330dhcx_acc_set_odr(hz104);
        break;
      case 4:
        ism330dhcx_acc_set_odr(hz208);
        break;
      case 5:
        ism330dhcx_acc_set_odr(hz416);
        break;
      case 6:
        ism330dhcx_acc_set_odr(hz833);
        break;
      case 7:
        ism330dhcx_acc_set_odr(hz1666);
        break;
      case 8:
        ism330dhcx_acc_set_odr(hz3332);
        break;
      case 9:
        ism330dhcx_acc_set_odr(hz6667);
        break;
    }
  }
  if(json_object_dothas_value(tempJSONObject, "ism330dhcx_acc.fs"))
  {
    int fs = json_object_dotget_number(tempJSONObject, "ism330dhcx_acc.fs"); //FS is an Enum --> value = float to contain all possible numeric values
    switch(fs)
    {
      case 0:
        ism330dhcx_acc_set_fs(g2);
        break;
      case 1:
        ism330dhcx_acc_set_fs(g4);
        break;
      case 2:
        ism330dhcx_acc_set_fs(g8);
        break;
      case 3:
        ism330dhcx_acc_set_fs(g16);
        break;
    }
  }

  json_value_free(tempJSON);
  return ret;
}

uint8_t ISM330DHCX_ACC_PnPL_vtblExecuteFunction(IPnPLComponent_t *_this, char *serializedJSON)
{
  return 1;
}

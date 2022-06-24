#
#                   Copyright (c) 2020 STMicroelectronics.
#                             All rights reserved
#
#   This software component is licensed by ST under BSD-3-Clause license,
#   the "License"; You may not use this file except in compliance with the
#   License. You may obtain a copy of the License at:
#                        https://opensource.org/licenses/BSD-3-Clause

import json
import HSD_utils.logger as logger
from HSD_utils.exceptions import CommunicationEngineOpenError, CommunicationEngineCloseError, EmptyCommandResponse, PnPLCommandError, \
    SETCommandError, PnPLSETDeviceStatusCommandError

from HSD_link.communication.PnPL_HSD.hsd_dll import HSD_Dll

log = logger.get_logger(__name__)

class PnPLHSD_CommandManager:

    def __init__(self, cmd_set):
        
        self.cmd_set = None
        self.hsd_dll = HSD_Dll()
        if(not self.hsd_dll.hs_datalog_open()):
            log.error("Error in Communication Engine opening")
            raise CommunicationEngineOpenError
        else:
            log.info("Communication Engine UP (DLL/so)")
    
    def __del__(self):
        if(not self.hsd_dll.hs_datalog_close()):
            log.error("Error in Communication Engine closure")
            raise CommunicationEngineCloseError
        else:
            log.info("Communication Engine DOWN (DLL/so)")

    def open(self):
        return self.hsd_dll.hs_datalog_open()
    
    def close(self):
        return self.hsd_dll.hs_datalog_close()

    def get_nof_devices(self):
        res = self.hsd_dll.hs_datalog_get_device_number()
        if res[0]:
            return res[1]
        log.error("Empty response from get_nof_devices(...).")
        raise EmptyCommandResponse("get_nof_devices")

    def get_version(self):
        res = self.hsd_dll.hs_datalog_get_version()
        return res[0]

    def get_device_info(self, d_id:int):
        return self.get_component_status(d_id, "fwinfo")

    def __send_message(self, d_id: int, message):
        res = self.hsd_dll.hs_datalog_send_message(d_id,message,len(message))
        if res[0]:
            return res[2]
        return None

    def send_command(self, d_id: int, message):
        return self.__send_message(d_id,message)
    
    def send_data(self):
        #TODO
        log.error("Not implemented function")
        raise EmptyCommandResponse("send_data")

    def get_sensor_data(self, d_id: int, comp_name:str):
        size = self.hsd_dll.hs_datalog_get_available_data_size(d_id, comp_name)
        if size[1] > 0:
            data = self.hsd_dll.hs_datalog_get_data(d_id, comp_name, size[1])
            if data[0]:
                return [size[1], data[1]]
        return None
    
    def get_cmd_set_presentation_string(self):
        return "Vespucci PnPL Commands"

    def get_device_presentation_string(self, d_id: int):
        if d_id is not None:
            res = self.hsd_dll.hs_datalog_get_presentation(d_id)
            if res[0]:
                return {"board_id" : res[1], "fw_id": res[2]}
            log.error("Empty response from get_device_presentation_string(d_id={})".format(d_id))
            raise EmptyCommandResponse("get_device_presentation_string")
        log.error("Missing Device id")
        raise PnPLCommandError("get_device_presentation_string")
    
    def get_device_alias(self, d_id: int):
        return self.get_device_info(d_id)["fwinfo"]["alias"]

    def __get_sub_sensor_components(self, d_id:int):
        sub_sensor_components = dict()
        components = self.get_device_status(d_id)["components"]
        for c in components:
            comp_name = list(c.keys())[0]
            if "c_type" in c[comp_name] and c[comp_name]["c_type"] == 0:
                sub_sensor_components[comp_name] = c[comp_name]
        return sub_sensor_components

    def get_sensors_count(self, d_id:int):
        sub_sensor_keys = self.__get_sub_sensor_components(d_id).keys()
        sensor_keys = [s.split("_")[0] for s in sub_sensor_keys]
        return len(set(sensor_keys))
    
    def get_sub_sensors_count(self, d_id:int, comp_name: str):
        sub_sensor_keys = self.__get_sub_sensor_components(d_id).keys()
        res_sub_sensor_keys = [s for s in sub_sensor_keys if s.startswith(comp_name)]
        return len(res_sub_sensor_keys)
    
    def set_device_alias(self, d_id: int, alias: str):
        res = self.hsd_dll.hs_datalog_set_device_alias(d_id, alias)
        if res is not None:
            log.info("Device Alias set to {}".format(alias))
            return True
        log.error("Error Device Alias update.")
        raise SETCommandError("set_device_alias")

    def start_log(self,d_id: int, mode: int = 1):
        return self.hsd_dll.hs_datalog_start_log(d_id, mode)
    
    def stop_log(self,d_id: int, mode: int = 1):
        return self.hsd_dll.hs_datalog_stop_log(d_id, mode)

    def get_devices(self):
        nof_devices = self.get_nof_devices()
        if nof_devices is not None:
            dev_list=[]
            for i in range(0,nof_devices):
                device = self.get_device_status(i)
                dev_list.append(device)
            return dev_list
        return None

    def get_device_status(self, d_id: int):
        res = self.hsd_dll.hs_datalog_get_device_status(d_id)
        if res[0]:
            device_dict = json.loads(res[1])
            return device_dict
        log.error("No Device Status[d_id:{}] returned.".format(d_id))
        raise EmptyCommandResponse("get_device_status")

    def set_device_status(self, d_id: int, device_status_json_file_path):
        with open(device_status_json_file_path) as f:
            device_json = json.load(f)
            f.close()
        res = self.hsd_dll.hs_datalog_set_device_status(d_id, device_json)
        if res:
            log.info("Device Status [{}] correctly updated".format())
        log.error("Error in Device Status update.")
        raise PnPLSETDeviceStatusCommandError("set_device_status")

    def __get_components_status(self, d_id: int, c_type:int):
        res = dict()
        device_status = self.get_device_status(d_id)
        for comp in device_status["components"]:
            comp_name = list(comp.keys())[0]
            comp_status = comp[comp_name]
            if "c_type" in comp_status and comp_status["c_type"] == c_type:
                res[comp_name] = comp_status
        return res

    def get_sensors_status(self, d_id: int):
        return self.__get_components_status(d_id,0)
    
    def get_algorithms_status(self, d_id: int):
        return self.__get_components_status(d_id,1)

    def get_component_status(self, d_id:int, component_name:str):
        res = self.hsd_dll.hs_datalog_get_component_status(d_id, component_name)
        if res[0] and len(res[1]) > 0:
            try:
                sensor_dict = json.loads(res[1])
                return sensor_dict
            except json.decoder.JSONDecodeError:
                log.error("Component Status[d_id:{},{}] parsing error.".format(d_id,component_name))
                raise EmptyCommandResponse("get_component_status")
        log.error("No Component Status[d_id:{},{}] returned.".format(d_id,component_name))
        raise EmptyCommandResponse("get_component_status")
    
    def get_components_count(self, d_id: int):
        res = self.hsd_dll.hs_datalog_get_components_number(d_id)
        if res[0]:
            return res[1]
        log.error("Empty response from get_components_count.".format(d_id))
        raise EmptyCommandResponse("get_components_count")

    def get_sensor_components_count(self, d_id: int, only_active: bool):
        res = self.hsd_dll.hs_datalog_get_sensor_components_number(d_id, only_active)
        if res[0]:
            return res[1]
        log.error("Empty response from get_sensor_components_count.".format(d_id))
        raise EmptyCommandResponse("get_sensor_components_count")

    def get_algorithm_components_count(self, d_id: int, only_active: bool):
        res = self.hsd_dll.hs_datalog_get_algorithm_components_number(d_id, only_active)
        if res[0]:
            return res[1]
        log.error("Empty response from get_algorithm_components_count.".format(d_id))
        raise EmptyCommandResponse("get_algorithm_components_count")

    def get_sensor_components_names(self, d_id: int, only_active: bool):
        res = self.get_sensors_status(d_id)
        if res is not None:
            return list(res.keys())
        log.error("Empty response from get_sensor_components_names.".format(d_id))
        raise EmptyCommandResponse("get_sensor_components_names")

    def get_algorithm_components_names(self, d_id: int, only_active: bool):
        res = self.get_algorithms_status(d_id)
        if res is not None:
            return list(res.keys())
        log.error("Empty response from get_algorithm_components_names.".format(d_id))
        raise EmptyCommandResponse("get_algorithm_components_names")

    def get_boolean_property(self, d_id: int, comp_name: str, prop_name: str, sub_prop_name: str = None):
        res = self.hsd_dll.hs_datalog_get_boolean_property(d_id, comp_name, prop_name, sub_prop_name)
        if res[0]:
            return res[1]
        if sub_prop_name is None:
            log.error("Empty response from get_boolean_property(d_id={}, comp_name={}, prop_name={})".format(d_id, comp_name, prop_name))
        else:
            log.error("Empty response from get_boolean_property(d_id={}, comp_name={}, prop_name={}, sub_prop_name={})".format(d_id, comp_name, prop_name, sub_prop_name))
        raise EmptyCommandResponse("get_boolean_property")
       
    def get_integer_property(self, d_id: int, comp_name: str, prop_name: str, sub_prop_name: str = None):
        res = self.hsd_dll.hs_datalog_get_integer_property(d_id, comp_name, prop_name, sub_prop_name)
        if res[0]:
            return res[1]
        if sub_prop_name is None:
            log.error("Empty response from get_integer_property(d_id={}, comp_name={}, prop_name={})".format(d_id, comp_name, prop_name))
        else:
            log.error("Empty response from get_integer_property(d_id={}, comp_name={}, prop_name={}, sub_prop_name={})".format(d_id, comp_name, prop_name, sub_prop_name))
        raise EmptyCommandResponse("get_integer_property")

    def get_float_property(self, d_id: int, comp_name: str, prop_name: str, sub_prop_name: str = None):
        res = self.hsd_dll.hs_datalog_get_float_property(d_id, comp_name, prop_name, sub_prop_name)
        if res[0]:
            return res[1]
        if sub_prop_name is None:
            log.error("Empty response from get_float_property(d_id={}, comp_name={}, prop_name={})".format(d_id, comp_name, prop_name))
        else:
            log.error("Empty response from get_float_property(d_id={}, comp_name={}, prop_name={}, sub_prop_name={})".format(d_id, comp_name, prop_name, sub_prop_name))
        raise EmptyCommandResponse("get_float_property")

    def get_string_property(self, d_id: int, comp_name: str, prop_name: str, sub_prop_name: str = None):
        # if sub_prop_name is None:
        #     res = self.hsd_dll.hs_datalog_get_string_property(d_id, comp_name, prop_name)
        # else:
        #     res = self.hsd_dll.hs_datalog_get_string_sub_property(d_id, comp_name, prop_name, sub_prop_name)
        res = self.hsd_dll.hs_datalog_get_string_property(d_id, comp_name, prop_name, sub_prop_name)
        if res[0]:
            return res[1]
        if sub_prop_name is None:
            log.error("Empty response from get_string_property(d_id={}, comp_name={}, prop_name={})".format(d_id, comp_name, prop_name))
        else:
            log.error("Empty response from get_string_property(d_id={}, comp_name={}, prop_name={}, sub_prop_name={})".format(d_id, comp_name, prop_name, sub_prop_name))
        raise EmptyCommandResponse("get_string_property")

    def set_property(self, d_id: int, value, comp_name: str, prop_name: str, sub_prop_name: str = None):
        if type(value) == bool:
            res = self.hsd_dll.hs_datalog_set_boolean_property(d_id, value, comp_name, prop_name, sub_prop_name)
        elif type(value) == int:
            res = self.hsd_dll.hs_datalog_set_integer_property(d_id, value, comp_name, prop_name, sub_prop_name)
        elif type(value) == float:
            res = self.hsd_dll.hs_datalog_set_float_property(d_id, value, comp_name, prop_name, sub_prop_name)
        elif type(value) == str:
            res = self.hsd_dll.hs_datalog_set_string_property(d_id, value, comp_name, prop_name, sub_prop_name)
        if res:
            return res
        if sub_prop_name is None:
            log.error("Empty response from set_property(d_id={}, comp_name={}, prop_name={})".format(d_id, comp_name, prop_name))
        else:
            log.error("Empty response from set_property(d_id={}, comp_name={}, prop_name={}, sub_prop_name={})".format(d_id, comp_name, prop_name, sub_prop_name))
        raise EmptyCommandResponse("set_property")

    def set_data_ready_callback(self):
        #TODO
        log.error("Not implemented function")
        raise EmptyCommandResponse("set_data_ready_callback")
    
    #WP2 OK
    # def __set_property_value(self, d_id: int, comp_name: str, prop_name: str, prop_value):
    #     message = {
    #         comp_name: {
    #             prop_name: prop_value
    #         }
    #     }
    #     json_string = json.dumps(message)
    #     self.__send_message(0,json_string)
    #     print(json_string)
    
    #WP2 OK
    # def __set_component_values(self, d_id: int, comp, dtdl_comp):
    #     comp_name = list(comp.keys())[0]
    #     for content in dtdl_comp.contents:
    #         if content.type == DTM.ContentType.PROPERTY and content.writable == True:
    #             if content.name in comp[comp_name].keys():
    #                 #check property type (Enum, Object or others)
    #                 if type(content.schema) == DTM.SchemaClass:
    #                     if content.schema.type == 'Enum':
    #                         for enum_value in content.schema.enum_values:
    #                             ev_str = enum_value.display_name if isinstance(enum_value.display_name, str) else enum_value.display_name.en
    #                             if str(comp[comp_name][content.name]) in ev_str:
    #                                 self.__set_property_value(d_id, comp_name, content.name, enum_value.enum_value)
    #                     if content.schema.type == 'Object':
    #                         print("Object")
    #                         for field in content.schema.fields:
    #                             if field.schema == 'string':
    #                                 print(field.name)
    #                                 prop_value = { 
    #                                     field.name : comp[comp_name][content.name][field.name]
    #                                 }
    #                                 self.__set_property_value(d_id, comp_name, content.name, prop_value)                    
    #                 else:
    #                     self.__set_property_value(d_id, comp_name, content.name, comp[comp_name][content.name])
    #             else:
    #                 print("[WARNING] - wrong property name in your DeviceStatus --> Component: {}".format(comp_name))

    #WP2 OK
    # def update_device(self, d_id: int, device_status_json_file_path, dtdl_components):
    #     with open(device_status_json_file_path) as f:
    #         device_dict = json.load(f)
    #         f.close()
    #     for component in device_dict["components"]:
    #         for key in component.keys():
    #             if key in dtdl_components:
    #                 self.__set_component_values(d_id, component, dtdl_components[key])

    #WP2 TODO
    #using hsd_dll API (currently used)
    # def upload_mlc_ucf_file(self, d_id: int, s_id: int, ucf_file_path):
    #     with open(ucf_file_path, 'rb') as f:
    #         ucf_buffer = np.fromfile(f, dtype='uint8')
    #         f.close()
    #         res = self.hsd_dll.hs_datalog_send_UCF_to_MLC(d_id, s_id, ucf_buffer, len(ucf_buffer))
    #         if res:
    #             click.secho("--> ucf configuration file sent successfully! MLC sensor id: {}".format(s_id),fg='green')
    #             return True
    #     return None

class PnPLHSD_Creator:
    def __create_cmd_set(self): return None
    def create_cmd_manager(self):
        cmd_set = self.__create_cmd_set()
        return PnPLHSD_CommandManager(cmd_set)
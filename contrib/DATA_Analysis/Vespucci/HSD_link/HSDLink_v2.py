# *****************************************************************************
#  * @file    HSDLink.py
#  * @author  SRA
#  * @version 1.3.0
#  * @date    5-Nov-2021
# *****************************************************************************
#                   Copyright (c) 2020 STMicroelectronics.
#                             All rights reserved
#
#   This software component is licensed by ST under BSD-3-Clause license,
#   the "License"; You may not use this file except in compliance with the
#   License. You may obtain a copy of the License at:
#                        https://opensource.org/licenses/BSD-3-Clause

from xmlrpc.client import boolean
from HSD_link.communication.DTDL.device_template_model import ContentType
import os
import shutil
import json
from datetime import datetime

from HSD_utils.exceptions import InvalidCommandSetError, NoDeviceConnectedError
import HSD_utils.logger as logger
from HSD_link.communication.PnPL_HSD.PnPLHSD_com_manager import PnPLHSD_CommandManager, PnPLHSD_Creator
from HSD_link.communication.DTDL.device_template_manager import DeviceTemplateManager

log = logger.get_logger(__name__)

class HSDLink_v2:

    __com_manager = None
    __acquisition_folder = "."
    #__dt_manager = None
    acq_start_time = 0
    
    def __init__(self, dev_com_type: str = 'pnpl', acquisition_folder = None):
        self.__create_com_manager(dev_com_type)
        #check that there is at least one connected device
        if self.get_nof_devices() <= 0:
            log.error("No device connected. Connect a device and retry.")
            raise NoDeviceConnectedError

        #acquisition folder creation
        if acquisition_folder is not None:
            if not os.path.exists(acquisition_folder):
                os.makedirs(acquisition_folder)
            self.__acquisition_folder = acquisition_folder
        else:
            self.__acquisition_folder = os.path.join(self.__acquisition_folder, "{}_{}".format("Vespucci",datetime.today().strftime('%Y%m%d_%H_%M_%S')))
            os.makedirs(self.__acquisition_folder)

    def set_device_template(self, dev_template_json: str):
        self.__dt_manager = DeviceTemplateManager(dev_template_json)

    def open(self):
        return self.__com_manager.open()
    
    def close(self):
        return self.__com_manager.close()

    def __create_com_manager(self,dev_com_type):
        if dev_com_type == 'pnpl':
            factory = PnPLHSD_Creator()
        else:
            log.error("Invalid Command Set selected: {}".format(dev_com_type))
            raise InvalidCommandSetError(dev_com_type)
        self.__com_manager:PnPLHSD_CommandManager = factory.create_cmd_manager()
        #Command set presentation
        log.info(self.get_cmd_set_presentation_string())

    def get_device_presentation_string(self, d_id:int):
        return self.__com_manager.get_device_presentation_string(d_id)
    
    def get_cmd_set_presentation_string(self):
        return self.__com_manager.get_cmd_set_presentation_string()

    def get_acquisition_folder(self):
        return self.__acquisition_folder

    def get_nof_devices(self):
        return self.__com_manager.get_nof_devices()

    def get_version(self):
        return self.__com_manager.get_version()
    
    def get_fw_info(self, d_id:int):
        return self.__com_manager.get_component_status(d_id, "fwinfo")

    def get_device_info(self, d_id:int):
        return self.__com_manager.get_component_status(d_id, "deviceinfo") 

    def get_devices(self):
        return self.__com_manager.get_devices()
    
    def get_device_status(self, d_id:int):
        return self.__com_manager.get_device_status(d_id)
    
    def get_component_status(self, d_id:int, component_name:str):
        return self.__com_manager.get_component_status(d_id, component_name)

    def get_device_alias(self, d_id:int):
        return self.__com_manager.get_device_alias(d_id)
    
    def get_sensors_count(self, d_id:int, only_active:bool = None):
        return self.__com_manager.get_sensor_components_count(d_id, only_active)
    
    def get_sensors_names(self, d_id:int, only_active:bool = None):
        return self.__com_manager.get_sensor_components_names(d_id, only_active)

    def get_algorithms_count(self, d_id:int, only_active:bool = None):
        return self.__com_manager.get_algorithm_components_count(d_id, only_active)

    def get_algorithms_names(self, d_id:int, only_active:bool = None):
        return self.__com_manager.get_algorithm_components_names(d_id, only_active)

    def get_sensor_enable(self, d_id:int, sensor_name:str):
        return self.__com_manager.get_boolean_property(d_id, sensor_name, "enable")

    def get_sensor_odr(self, d_id:int, sensor_name:str):
        return self.__com_manager.get_float_property(d_id, sensor_name, "odr")
    
    def get_sensor_measured_odr(self, d_id:int, sensor_name:str):
        return self.__com_manager.get_float_property(d_id, sensor_name, "measodr")

    def get_sensor_fs(self, d_id:int, sensor_name:str):
        return self.__com_manager.get_float_property(d_id, sensor_name, "fs")

    def get_sensor_samples_per_ts(self, d_id:int, sensor_name:str):
        return self.__com_manager.get_integer_property(d_id, sensor_name, "samples_per_ts")
    
    def get_sensor_initial_offset(self, d_id:int, sensor_name:str):
        return self.__com_manager.get_float_property(d_id, sensor_name, "ioffset")
    
    def get_sensors_status(self, d_id:int):
        return self.__com_manager.get_sensors_status(d_id)

    def get_algorithms_status(self, d_id:int):
        return self.__com_manager.get_algorithms_status(d_id)

    def get_acquisition_info(self, d_id:int):
        return self.__com_manager.get_component_status(d_id, "acquisition_info")

    def get_tags_info(self, d_id:int):
        return self.__com_manager.get_component_status(d_id, "tags_info")

    def get_tag_list(self, d_id:int):
        tags_info = self.get_tags_info(d_id)
        if "tags" in tags_info:
            return tags_info["tags"]
        return list()

    def get_max_tags_per_acq(self, d_id: int):
        tags_info = self.get_tags_info(d_id)
        if "max_tags_num" in tags_info["tags_info"]:
            return tags_info["tags_info"]["max_tags_num"]
        return None

    def get_sw_tag_classes(self, d_id: int):
        sw_tags = dict()
        res = self.get_tags_info(d_id)
        for t in res["tags_info"]:
            if "sw" in t:
                sw_tags[t] = res["tags_info"][t]
        return sw_tags

    def get_sw_tag_class(self, d_id: int, tag_class_id: int):
        ret = self.__com_manager.get_string_property(d_id, "tags_info", "sw_tag{}".format(tag_class_id))
        return ret
    
    def get_sw_tag_class_label(self, d_id: int, tag_class_id: int):
        ret = self.__com_manager.get_string_property(d_id, "tags_info", "sw_tag{}".format(tag_class_id), "label")
        return ret
    
    def get_sw_tag_class_enabled(self, d_id: int, tag_class_id: int):
        ret = self.__com_manager.get_boolean_property(d_id, "tags_info", "sw_tag{}".format(tag_class_id), "enabled")
        return ret

    def get_hw_tag_classes(self, d_id: int):
        hw_tags = dict()
        res = self.get_tags_info(d_id)
        for t in res["tags_info"]:
            if "hw" in t:
                hw_tags[t] = res["tags_info"][t]
        return hw_tags

    def get_hw_tag_class(self, d_id: int, tag_class_id: int):
        ret = self.__com_manager.get_string_property(d_id, "tags_info", "hw_tag{}".format(tag_class_id))
        return ret
    
    def get_hw_tag_class_label(self, d_id: int, tag_class_id: int):
        ret = self.__com_manager.get_string_property(d_id, "tags_info", "hw_tag{}".format(tag_class_id), "label")
        return ret
    
    def get_hw_tag_class_enabled(self, d_id: int, tag_class_id: int):
        ret = self.__com_manager.get_boolean_property(d_id, "tags_info", "hw_tag{}".format(tag_class_id), "enabled")
        return ret

    def send_command(self, d_id:int, message):
        return self.__com_manager.send_command(d_id, message)
    
    def get_boolean_property(self, d_id: int, comp_name: str, prop_name: str):
        return self.__com_manager.get_boolean_property(d_id, comp_name, prop_name)
    
    def get_integer_property(self, d_id: int, comp_name: str, prop_name: str):
        return self.__com_manager.get_integer_property(d_id, comp_name, prop_name)

    def get_string_property(self, d_id: int, comp_name: str, prop_name: str):
        return self.__com_manager.get_string_property(d_id, comp_name, prop_name)

    def get_float_property(self, d_id: int, comp_name: str, prop_name: str):
        return self.__com_manager.get_float_property(d_id, comp_name, prop_name)

    def set_acquisition_name(self, d_id:int, name:str):
        return self.__com_manager.set_property(d_id, name, "acquisition_info", "name")

    def set_acquisition_description(self, d_id:int, description:str):
        return self.__com_manager.set_property(d_id, description, "acquisition_info", "description")

    def set_sensor_enable(self, d_id: int, new_status: bool, comp_name: str):        
        return self.__com_manager.set_property(d_id, new_status, comp_name, "enable")
    
    def set_sensor_odr(self, d_id: int, new_odr: float, comp_name: str):
        return self.__com_manager.set_property(d_id, new_odr, comp_name, "odr")

    def set_sensor_fs(self, d_id: int, new_fs: float, comp_name: str):
        return self.__com_manager.set_property(d_id, new_fs, comp_name, "fs")

    def set_sensor_samples_per_ts(self, d_id: int, new_spts: int, comp_name: str):
        return self.__com_manager.set_property(d_id, new_spts, comp_name, "samples_per_ts")

    def set_property(self, d_id: int, new_value: bool, comp_name: str, prop_name: str, sub_prop_name: str = None):        
        return self.__com_manager.set_property(d_id, new_value, comp_name, prop_name, sub_prop_name)

    def set_sw_tag_class_enabled(self, d_id:int, tag_class_id:int, new_status: boolean):
        return self.__com_manager.set_property(d_id, new_status, "tags_info", "sw_tag{}".format(tag_class_id), "enabled")
    
    def set_sw_tag_class_label(self, d_id:int, tag_class_id:int, new_label: str):
        return self.__com_manager.set_property(d_id, new_label, "tags_info", "sw_tag{}".format(tag_class_id), "label")

    def set_sw_tag_on_off(self, d_id:int, tag_class_id:int, new_status: boolean):
        return self.__com_manager.set_property(d_id, new_status, "tags_info", "sw_tag{}".format(tag_class_id), "status")

    def set_hw_tag_class_enabled(self, d_id:int, tag_class_id:int, new_status: boolean):
        return self.__com_manager.set_property(d_id, new_status, "tags_info", "hw_tag{}".format(tag_class_id), "enabled")
    
    def set_hw_tag_class_label(self, d_id:int, tag_class_id:int, new_label: str):
        return self.__com_manager.set_property(d_id, new_label, "tags_info", "hw_tag{}".format(tag_class_id), "label")

    
    def get_sensor_data(self, d_id:int, comp_name:str):
        return self.__com_manager.get_sensor_data(d_id, comp_name)
    
    def start_log(self, d_id:int, mode:int = 1):
        log.info("Log Started")
        self.acq_start_time = datetime.now().isoformat()
        return self.__com_manager.start_log(d_id, mode)

    def stop_log(self, d_id:int, mode:int = 1):
        log.info("Log Stopped")
        return self.__com_manager.stop_log(d_id, mode)
    
    def save_json_device_status_file(self, d_id: int, out_acq_path = None):
        json_save_path = self.__acquisition_folder
        if out_acq_path is not None:
            if not os.path.exists(out_acq_path):
                os.makedirs(out_acq_path)
            json_save_path = out_acq_path
        try:
            res = self.get_device_status(d_id)
            if res is not None:
                if "acquisition_info" in res:
                    res["components"]["acquisition_info"].pop()
                device_status_filename = os.path.join(json_save_path, "device_config.json")
                sensor_data_file = open(device_status_filename, "w+")
                sensor_data_file.write(json.dumps(res, indent = 4))
                sensor_data_file.close()
                log.info("device_config.json Configuration file correctly saved")
                return True
        except:
            raise
    
    def save_json_acq_info_file(self, d_id: int, out_acq_path = None):
        json_save_path = self.__acquisition_folder
        if out_acq_path is not None:
            if not os.path.exists(out_acq_path):
                os.makedirs(out_acq_path)
            json_save_path = out_acq_path
        try:
            res = self.get_acquisition_info(d_id)
            if res is not None:
                acq_info_filename = os.path.join(json_save_path,"acquisition_info.json")
                acq_info_file = open(acq_info_filename, "w+")

                st_split = self.acq_start_time.split('.')
                acq_s_time = st_split[0] + '.' + st_split[-1][:3] + 'Z'
                res["start_time"] = acq_s_time

                et_split = datetime.now().isoformat().split('.')
                acq_e_time = et_split[0] + '.' + et_split[-1][:3] + 'Z'
                res["end_time"] = acq_e_time

                acq_info_file.write(json.dumps(res, indent = 4))
                acq_info_file.close()
                log.info("acquisition_info.json file correctly saved")
                return True
        except:
            raise
    
    
    
    
    
    
    
    
    #WP2 ONGOING
 
    # def __get_sensor_prop_value_enum(self, new_value, p_name, s_name: str, s_type: str = None):
    #     if self.__dt_manager is not None:
    #         sensor_component = self.__dt_manager.get_component(s_name)
    #         if sensor_component is not None:
    #             for content in sensor_component.contents:
    #                 if content.type == ContentType.PROPERTY:
    #                     p_complete_name = p_name
    #                     if s_type is not None:
    #                         p_complete_name = s_type + "_" + p_name
    #                     if content.name == p_complete_name or content.name == p_name:
    #                         if content.schema.type == 'Enum':
    #                             for i, enum_value in enumerate(content.schema.enum_values):
    #                                 ev_str = enum_value.display_name if isinstance(enum_value.display_name, str) else enum_value.display_name.en
    #                                 if str(new_value) == ''.join(c for c in ev_str if c.isdigit()):
    #                                     return i
    #     return False

    #WP2 OK

    #WP2 OK
    def update_device(self, d_id:int, device_json_file_path):
        if self.__dt_manager is not None:
            return self.__com_manager.update_device(d_id, device_json_file_path, self.__dt_manager.get_components())
        else:
            print("HSD_link does not have an associated Device Template")

    #WP2 TODO
    def upload_mlc_ucf_file(self, d_id:int, s_id:int, ucf_file_path):
        res = self.__com_manager.upload_mlc_ucf_file(d_id, s_id, ucf_file_path)
        if res is not None:
            #if correctly loaded, save ucf file in acquisition folder
            shutil.copy(ucf_file_path, self.__acquisition_folder)  
        return res

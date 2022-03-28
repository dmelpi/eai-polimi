#
#                   Copyright (c) 2020 STMicroelectronics.
#                             All rights reserved
#
#   This software component is licensed by ST under BSD-3-Clause license,
#   the "License"; You may not use this file except in compliance with the
#   License. You may obtain a copy of the License at:
#                        https://opensource.org/licenses/BSD-3-Clause

from HSD.model.DeviceConfig import Device, DeviceInfo, Sensor, HwTag, SwTag
from HSD.model.AcquisitionInfo import Tag
import HSD_utils.logger as logger

log = logger.setup_applevel_logger(is_debug = False, file_name= "app_debug.log")

class CLIInteraction:

    @staticmethod
    def select_item(what: str, a_list: list):
        choice_not_in_range = True
        choice = 0
        item_id = ""
        while choice_not_in_range:
            for i, c in enumerate(a_list):
                if isinstance(c , Device):
                    item_id = c.device_info.alias + " [" + c.device_info.part_number + "]"
                elif isinstance(c , Sensor):
                    item_id = c.name
                elif isinstance(c, Tag):
                    item_id = c.label
                else:
                    item_id = str(c)
                print(str(i) + ' - ' + item_id)

            if len(a_list) == 0:
                log.error('==> No {w} in list'.format(w=what))
                return None

            print('q - Quit')
            choice = input('Select one {} (''q'' to quit) ==> '.format(what))
            try:
                choice_not_in_range = int(choice) not in range(len(a_list))
                if choice_not_in_range:
                    print('please input a correct value')
            except ValueError:
                if choice != 'q':
                    print('please input a correct value')
                else:
                    #choice_not_in_range = False
                    break
        if choice == 'q':
            print('Bye!')
            return None
        else:
            return a_list[int(choice)]

    @staticmethod
    def present_items(item_list: list):
        for item in item_list:
            if isinstance(item , Sensor):
                print("{}) {}".format(item.id, item.name))
                sd_list = item.sensor_descriptor.sub_sensor_descriptor
                ss_list = item.sensor_status.sub_sensor_status
                for i, ssd in enumerate(sd_list):
                    if ss_list[i].is_active:
                        print("    {}) {}".format(ssd.id, ssd.sensor_type))
                    else:
                        print("    {}) {} --> [INACTIVE]".format(ssd.id, ssd.sensor_type))
            elif isinstance(item , HwTag):
                print("- {} [{}]".format(item.label, "ENABLED" if item.enabled else "DISABLED"))
            elif isinstance(item , SwTag):
                print("- {}".format(item.label))

    @staticmethod
    def present_item(item):
        if isinstance(item , DeviceInfo):
            print("Serial number: {}".format(item.serial_number))
            print("Device alias: {}".format(item.alias))
            print("Part number: {}".format(item.part_number))
            print("Website: {}".format(item.url))
            print("FW name: {}".format(item.fw_name))
            print("FW version: {}".format(item.fw_version))
            print("Data file extension: {}".format(item.data_file_ext))
            print("Data file format: {}".format(item.data_file_format))
        elif isinstance(item , Sensor):
            s_descriptor_list = item.sensor_descriptor.sub_sensor_descriptor
            s_status_list = item.sensor_status.sub_sensor_status
            for i, ssd in enumerate(s_descriptor_list):
                print("\n[{}] - ID: {}".format(item.name, ssd.id))
                print("- DESCRIPTOR:")
                print(" -- Sensor Type: {}".format(ssd.sensor_type))
                print(" -- Dimensions: {}".format(ssd.dimensions))
                print(" -- Dimensions Labels: {}".format(ssd.dimensions_label))
                print(" -- Unit: {}".format(ssd.unit))
                print(" -- Data Type: {}".format(ssd.data_type))
                print(" -- Supported FS: {}".format(ssd.fs))
                print(" -- Supported ODR: {}".format(ssd.odr))
                print(" -- Samples per Timestamp info:")
                print("   - Min: {}".format(ssd.samples_per_ts.min))
                print("   - Max: {}".format(ssd.samples_per_ts.max))
                print("   - Data Type: {}".format(ssd.samples_per_ts.data_type))
                print("- STATUS:")
                print(" -- isActive: {}".format(s_status_list[i].is_active))
                print(" -- ODR: {}".format(s_status_list[i].odr))
                print(" -- FS: {}".format(s_status_list[i].fs))
                print(" -- samples per timestamp: {}".format(s_status_list[i].samples_per_ts))
                print(" -- sensitivity: {}".format(s_status_list[i].samples_per_ts))
                print(" -- initial_offset: {}".format(s_status_list[i].initial_offset))
#
#                   Copyright (c) 2020 STMicroelectronics.
#                             All rights reserved
#
#   This software component is licensed by ST under BSD-3-Clause license,
#   the "License"; You may not use this file except in compliance with the
#   License. You may obtain a copy of the License at:
#                        https://opensource.org/licenses/BSD-3-Clause

import os

class FileManager:

    @staticmethod
    def decode_file_name(file_name):
        [sensor_name, sub] = file_name.split(".")[0].split("_")
        return sensor_name, sub

    @staticmethod
    def encode_file_name(sensor_name, sub_sensor_type, ext = '.dat'):
        file_name = sensor_name + '_' + sub_sensor_type + ext
        return file_name

    @staticmethod
    def get_file_names_from_model(device):
        """
        list of files that are supposed to be in the datalog based on DeviceConfig.json
        """
        data_files_list = []
        for s in device.sensor:
            sss_list = s.sensor_status.sub_sensor_status
            for i, ssd in enumerate(s.sensor_descriptor.sub_sensor_descriptor):
                if sss_list[i].is_active:
                    file_name = FileManager.encode_file_name(s.name,ssd.sensor_type)
                    data_files_list.append(file_name)
        return data_files_list

    @staticmethod
    def get_dat_files_from_folder(acq_folder_path):
        file_names = []
        with os.scandir(acq_folder_path) as listOfEntries:
            for entry in listOfEntries:
                # print all entries that are files
                if entry.is_file():
                    if entry.name.endswith('.dat'):
                        file_names.append(entry.name)
        return file_names
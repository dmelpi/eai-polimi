#
#                   Copyright (c) 2020 STMicroelectronics.
#                             All rights reserved
#
#   This software component is licensed by ST under BSD-3-Clause license,
#   the "License"; You may not use this file except in compliance with the
#   License. You may obtain a copy of the License at:
#                        https://opensource.org/licenses/BSD-3-Clause

import os
import sys
import click
from .file_manager import FileManager
from HSD.model.DeviceConfig import Device, DeviceInfo, Sensor, HwTag, SwTag
from HSD.model.AcquisitionInfo import Tag
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
                click.secho(str(i) + ' - ' + item_id)

            if len(a_list) == 0:
                click.secho('==> No {w} in list'.format(w=what), fg='red')
                return None

            print('q - Quit')
            choice = input('Select one {} (''q'' to quit) ==> '.format(what))
            try:
                choice_not_in_range = int(choice) not in range(len(a_list))
                if choice_not_in_range:
                    click.secho('please input a correct value',fg='red')
            except ValueError:
                if choice != 'q':
                    click.secho('please input a correct value',fg='red')
                else:
                    #choice_not_in_range = False
                    break
        if choice == 'q':
            click.echo('Bye!')
            return None
        else:
            return a_list[int(choice)]

    @staticmethod
    def present_items(item_list: list):
        for item in item_list:
            if isinstance(item , Sensor):
                click.secho("{}) {}".format(item.id, item.name))
                sd_list = item.sensor_descriptor.sub_sensor_descriptor
                ss_list = item.sensor_status.sub_sensor_status
                for i, ssd in enumerate(sd_list):
                    if ss_list[i].is_active:
                        click.secho("    {}) {}".format(ssd.id, ssd.sensor_type))
                    else:
                        click.secho("    {}) {} --> [INACTIVE]".format(ssd.id, ssd.sensor_type),fg='yellow')
            elif isinstance(item , HwTag):
                click.secho("- {} [{}]".format(item.label, "ENABLED" if item.enabled else "DISABLED"))
            elif isinstance(item , SwTag):
                click.secho("- {}".format(item.label))

    @staticmethod
    def present_item(item):
        if isinstance(item , DeviceInfo):
            click.secho("Serial number: {}".format(item.serial_number))
            click.secho("Device alias: {}".format(item.alias))
            click.secho("Part number: {}".format(item.part_number))
            click.secho("Website: {}".format(item.url))
            click.secho("FW name: {}".format(item.fw_name))
            click.secho("FW version: {}".format(item.fw_version))
            click.secho("Data file extension: {}".format(item.data_file_ext))
            click.secho("Data file format: {}".format(item.data_file_format))
        elif isinstance(item , Sensor):
            s_descriptor_list = item.sensor_descriptor.sub_sensor_descriptor
            s_status_list = item.sensor_status.sub_sensor_status
            for i, ssd in enumerate(s_descriptor_list):
                click.secho("\n[{}] - ID: {}".format(item.name, ssd.id))
                click.secho("- DESCRIPTOR:")
                click.secho(" -- Sensor Type: {}".format(ssd.sensor_type))
                click.secho(" -- Dimensions: {}".format(ssd.dimensions))
                click.secho(" -- Dimensions Labels: {}".format(ssd.dimensions_label))
                click.secho(" -- Unit: {}".format(ssd.unit))
                click.secho(" -- Data Type: {}".format(ssd.data_type))
                click.secho(" -- Supported FS: {}".format(ssd.fs))
                click.secho(" -- Supported ODR: {}".format(ssd.odr))
                click.secho(" -- Samples per Timestamp info:")
                click.secho("   - Min: {}".format(ssd.samples_per_ts.min))
                click.secho("   - Max: {}".format(ssd.samples_per_ts.max))
                click.secho("   - Data Type: {}".format(ssd.samples_per_ts.data_type))
                click.secho("- STATUS:")
                click.secho(" -- isActive: {}".format(s_status_list[i].is_active))
                click.secho(" -- ODR: {}".format(s_status_list[i].odr))
                click.secho(" -- FS: {}".format(s_status_list[i].fs))
                click.secho(" -- samples per timestamp: {}".format(s_status_list[i].samples_per_ts))
                click.secho(" -- sensitivity: {}".format(s_status_list[i].samples_per_ts))
                click.secho(" -- initial_offset: {}".format(s_status_list[i].initial_offset))
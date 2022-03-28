#!/usr/bin/env python
# coding: utf-8 
# *****************************************************************************
#  * @file    hsdatalog_cli.py
#  * @author  SRA
#  * @version 1.3.0
#  * @date    5-Nov-2021
# *****************************************************************************
#
#                   Copyright (c) 2020 STMicroelectronics.
#                             All rights reserved
#
#   This software component is licensed by ST under BSD-3-Clause license,
#   the "License"; You may not use this file except in compliance with the
#   License. You may obtain a copy of the License at:
#                        https://opensource.org/licenses/BSD-3-Clause


from asciimatics.scene import Scene
from asciimatics.screen import Screen
from asciimatics.exceptions import ResizeScreenError
import sys
import os
import click
import time
import shutil
from threading import Thread, Event
import HSD_utils.logger as logger
from HSD_link.HSDLink import HSDLink
from Views.cli_views import HSDMainView, HSDLoggingView

log = logger.setup_applevel_logger(is_debug = False, file_name= "app_debug.log")

class HSDInfo():

    class CLIFlags():
        
        def __init__(self, output_folder, acq_name, acq_desc, file_config, ucf_file, time_sec, interactive_mode):
            self.output_folder = output_folder
            self.acq_name = acq_name
            self.acq_desc = acq_desc
            self.file_config = file_config
            self.ucf_file = ucf_file
            self.time_sec = time_sec
            self.interactive_mode = interactive_mode
    
    cli_flags = None
    version = None
    device_list = None
    sensor_list = None
    tag_list = []
    highlighted_device_id = None
    selected_device_id = None
    selected_device_info = None
    mlc_sensor_list = None
    selected_mlc_id = None
    is_log_started = False
    is_log_manually_stopped = False
    output_acquisition_path = None
    threads_stop_flags = []
    sensor_data_counts = []
    tag_status_list = []
    start_time = None

    class SensorAcquisitionThread(Thread):
        def __init__(self, event, hsd_info, s_id, ss_id, sensor_data_file):
            Thread.__init__(self)
            self.stopped = event
            self.hsd_info = hsd_info
            self.s_id = s_id
            self.ss_id = ss_id
            self.sensor_data_file = sensor_data_file

        def run(self):
            while not self.stopped.wait(0.5):
                res = self.hsd_info.hsd_link.get_sensor_data(self.hsd_info.selected_device_id, self.s_id, self.ss_id)
                if res is not None:
                    size, sensor_data = res
                    ## data size increment
                    self.hsd_info.increment_sensor_data_counter(size, self.s_id, self.ss_id)
                    # NOTE here you can queue sensor data (to process or display them)
                    # NOTE: e.g. -> self.hsd_info.queue_sensor_data([sensor_data])
                    ## file saving!
                    res = self.sensor_data_file.write(sensor_data)

    def __init__(self, cli_flags):
        self.cli_flags = cli_flags

        #Initialize the HSD_PythonSDK HSD_link module
        self.hsd_link = HSDLink("stwin_hsd")

        #Get current HSDatalog USB dll version
        self.version = self.hsd_link.get_version()

        #Get the connected device list
        self.device_list = self.hsd_link.get_devices()

        #Updates the output folder field
        if self.cli_flags.output_folder is None:
            self.cli_flags.output_folder = self.hsd_link.get_acquisition_folder()
                
        #Multiple connected devices management
        if self.device_list is None or len(self.device_list) >= 1:
            self.selected_device_id = None
        else:
            self.selected_device_id = 0
    
    def update_device_info(self):
        if self.selected_device_id is not None:
            self.selected_device_info = self.hsd_link.get_device_info(self.selected_device_id)

    def check_output_folder(self):
         if self.output_acquisition_path is None:
            if self.cli_flags.output_folder is not None:
                if not os.path.exists(self.cli_flags.output_folder):
                    os.makedirs(self.cli_flags.output_folder)
                self.output_acquisition_path = self.cli_flags.output_folder
            else:
                self.output_acquisition_path = self.hsd_link.get_acquisition_folder()

    def save_device_in_output_folder(self):
        return self.hsd_link.save_json_device_file(self.selected_device_id, self.output_acquisition_path)

    def update_acq_params(self):
        # Set acquisition name and description parameters to the device
        if self.cli_flags.acq_name is None:
            self.cli_flags.acq_name = "STWIN_Acq"
        if self.cli_flags.acq_desc is None:
            self.cli_flags.acq_desc = ""
        self.hsd_link.set_acquisition_info(self.selected_device_id, self.cli_flags.acq_name, self.cli_flags.acq_desc)

    def upload_device_conf_file(self):
        # Device configuration file [DeviceConfig.json]
        if self.cli_flags.file_config != '' and os.path.exists(self.cli_flags.file_config):
            res = self.hsd_link.update_device(self.selected_device_id, self.cli_flags.file_config)
            if not res:
                log.warning("Error in Device configuration update. The default configuration will be used!")
            else:
                shutil.copy(self.cli_flags.file_config, self.output_acquisition_path)
    
    def update_sensor_list(self):
        if self.selected_device_id is not None:
            self.sensor_list = self.hsd_link.get_sub_sensors(self.selected_device_id, only_active=True)

    def init_sensor_data_counters(self):
        all_sensor_list = self.hsd_link.get_sub_sensors(self.selected_device_id, only_active=False)
        self.sensor_data_counts = [None] * len(all_sensor_list)
        for i, s in enumerate(all_sensor_list):
            self.sensor_data_counts[i] = [0] * len(s.sensor_descriptor.sub_sensor_descriptor)

    def increment_sensor_data_counter(self, size, s_id, ss_id):
        self.sensor_data_counts[s_id][ss_id] += size

    def update_mlc_sensor_list(self):
        if self.selected_device_id is not None and self.mlc_sensor_list is None:
            self.mlc_sensor_list = self.hsd_link.get_sub_sensors(self.selected_device_id, type_filter="MLC", only_active=False)
            #NOTE WIP - @ the moment only ISM330DHCX_MLC sensor is supported
            for mlc_s in self.mlc_sensor_list:
                if mlc_s.name == 'ISM330DHCX':
                    self.selected_mlc_id = [mlc_s.id, mlc_s.sensor_descriptor.sub_sensor_descriptor[0].id]
                    break

    def upload_mlc_ucf_file(self):
        # MLC configuration file [<a_mlc_configuration_file>.ufc]
        if self.cli_flags.ucf_file != '' and os.path.exists(self.cli_flags.ucf_file):
            if self.mlc_sensor_list is not None and self.selected_mlc_id is not None:
                res = self.hsd_link.upload_mlc_ucf_file(self.selected_device_id,self.selected_mlc_id[0],self.cli_flags.ucf_file)
                if not res:
                    log.warning("Error in MLC configuration update!")
                #Activate MLC sensor
                time.sleep(0.05) #TODO remove it when this issue will be fixed in FW
                res = self.hsd_link.set_sub_sensor_active(self.selected_device_id, self.selected_mlc_id[0], self.selected_mlc_id[1], True)
                if not res:
                    log.warning("Error in MLC enable!")
                #update Sensor list with new MLC sensor active status
                self.update_sensor_list()

    def update_tag_list(self):
        if self.selected_device_id is not None:
            self.tag_list = self.hsd_link.get_sw_tag_classes(self.selected_device_id)

    def init_tag_status_list(self):
        self.tag_status_list = [False] * len(self.tag_list)

    def do_tag(self, t_id):
        self.tag_status_list[t_id] = not self.tag_status_list[t_id]
        self.hsd_link.set_sw_tag_on(self.selected_device_id, t_id) if self.tag_status_list[t_id] else self.hsd_link.set_sw_tag_off(self.selected_device_id, t_id)
        # self.hsd_link.set_sw_tag(self.selected_device_id, t_id, self.tag_status_list[t_id])

    def start_log(self):
        self.is_log_started = self.hsd_link.start_log(self.selected_device_id)
        self.threads_stop_flags = []
        self.sensor_data_files = []
        for s in self.sensor_list:
            for sd in s.sensor_descriptor.sub_sensor_descriptor:
                sensor_data_file_path = os.path.join(self.output_acquisition_path,(str(s.name) + "_"  + str(sd.sensor_type) + ".dat"))
                sensor_data_file = open(sensor_data_file_path, "wb+")
                self.sensor_data_files.append(sensor_data_file)
                stopFlag = Event()
                self.threads_stop_flags.append(stopFlag)
                thread = HSDInfo.SensorAcquisitionThread(stopFlag, self, s.id, sd.id, sensor_data_file)
                thread.start()

    def stop_log(self):
        for sf in self.threads_stop_flags:
            sf.set()
        for f in self.sensor_data_files:
            f.close()
        self.hsd_link.stop_log(self.selected_device_id)
        self.is_log_started = False
        self.hsd_link.save_json_device_file(self.selected_device_id, self.output_acquisition_path)
        self.hsd_link.save_json_acq_info_file(self.selected_device_id, self.output_acquisition_path)

def show_help(ctx, param, value):
    if value and not ctx.resilient_parsing:
        click.secho(ctx.get_help(), color=ctx.color)
        click.secho("\n-> Script execution examples:")
        click.secho("   python hsdatalog_cli.py", fg='cyan')
        click.secho("   python hsdatalog_cli.py -o .\your_out_folder", fg='cyan')
        click.secho("   python hsdatalog_cli.py -t 10", fg='cyan')
        click.secho("   python hsdatalog_cli.py -i", fg='cyan')
        click.secho("   python hsdatalog_cli.py -t 20 -an your_acq_name -ad your_acq_descr", fg='cyan')
        click.secho("   python hsdatalog_cli.py -f ..\\STWIN_config_examples\\DeviceConfig.json -u ..\\STWIN_config_examples\\UCF_examples\\ism330dhcx_six_d_position.ucf", fg='cyan')
        ctx.exit()

def validate_duration(ctx, param, value):
    if value < -1 or value == 0:
        raise click.BadParameter('\'{d}\'. Please retry'.format(d=value))
    return value

@click.command()
@click.option('-o', '--output_folder', help="Output folder (this will be created if it doesn't exist)")
@click.option('-an','--acq_name', help="Acquisition name", type=str)
@click.option('-ad','--acq_desc', help="Acquisition description", type=str)
@click.option('-f', '--file_config', help="Device configuration file (JSON)", default='')
@click.option('-u', '--ucf_file', help="UCF Configuration file for MLC", default='')
@click.option('-t', '--time_sec', help="Duration of the current acquisition [seconds]", callback=validate_duration, type=int, default=-1)
@click.option('-i', '--interactive_mode', help="Interactive mode. It allows to select a connected device, get info and start the acquisition process",  is_flag=True, default=False)
@click.option("-h", "--help", is_flag=True, is_eager=True, expose_value=False, callback=show_help, help="Show this message and exit.",)

def hsd_CLI(output_folder, acq_name, acq_desc, file_config, ucf_file, time_sec, interactive_mode):
    last_scene = None

    cli_flags = HSDInfo.CLIFlags(output_folder, acq_name, acq_desc, file_config, ucf_file, time_sec, interactive_mode)
    hsd_info = HSDInfo(cli_flags)

    while True:
        try:
            Screen.wrapper(demo, catch_interrupt=True, arguments=[last_scene, hsd_info])

            sys.exit(0)
        except ResizeScreenError as e:
            last_scene = e.scene

def demo(screen, scene, hsd_info):
    if hsd_info.cli_flags.interactive_mode or len(hsd_info.device_list) == 0:
        scenes = [
            Scene([HSDMainView(screen, hsd_info)], -1, name="Main"),
            Scene([HSDLoggingView(screen, hsd_info)], -1, name="Logger")
        ]
    else:
        hsd_info.selected_device_id = 0
        scenes = [
            Scene([HSDLoggingView(screen, hsd_info)], -1, name="Logger")
        ]

    screen.play(scenes, stop_on_resize=True, start_scene=scene, allow_int=True)

if __name__ == '__main__':          
    hsd_CLI()
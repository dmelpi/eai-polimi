#!/usr/bin/env python
# coding: utf-8 
# *****************************************************************************
#  * @file    hsdatalog_data_export_by_tags.py
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

import os
import click
import HSD_utils.logger as logger
from HSD_utils.converters import HSDatalogConverter
from HSD.HSDatalog import HSDatalog

log = logger.setup_applevel_logger(is_debug = False, file_name= "app_debug.log")

def show_help(ctx, param, value):
    if value and not ctx.resilient_parsing:
        click.secho(ctx.get_help(), color=ctx.color)
        click.secho("\n-> Script execution examples:")
        click.secho("   python hsdatalog_data_export_by_tags.py ..\\STWIN_acquisition_examples\\STWIN_00003 -t -f CSV -s all -o .\\expData", fg='cyan')
        click.secho("   python hsdatalog_data_export_by_tags.py ..\\STWIN_acquisition_examples\\STWIN_00003 -t -f CSV -s HTS221 -o .\\expData", fg='cyan')
        ctx.exit()

@click.command()
@click.argument('acq_folder', type=click.Path(exists=True))
@click.option('-o', '--output_folder', help="Output folder (this will be created if it doesn't exist)")
# @click.option('-s', '--sensor_name', help="Sensor name - (currently only the ISM330DHCX is supported)", default="ISM330DHCX")
@click.option('-s', '--sensor_name', help="Sensor Name - use \"all\" to extract all active sensors data, otherwise select a specific sensor by name", default='')
@click.option('-ss','--sample_start', help="Sample Start - Data conversion will start from this sample", type=int, default=0)
@click.option('-se','--sample_end', help="Sample End - Data conversion will end up in this sample", type=int, default=-1)
@click.option('-t', '--use_datalog_tags', is_flag=True, help="Enable this flag to include information about annotations taken during acquisition (if any) in the exported data", default=False)
@click.option('-f', '--out_format', help="Select exported data format", type=click.Choice(['TXT', 'CSV', 'TSV'], case_sensitive=False))
@click.option('-d', '--debug', is_flag=True, help="[DEBUG] Check for corrupted data and timestamps", default=False)
@click.option("-h", "--help", is_flag=True, is_eager=True, expose_value=False, callback=show_help, help="Show this message and exit.",)

def hsd_toUnico(acq_folder, output_folder, sensor_name, sample_start, sample_end, use_datalog_tags, out_format, debug):
    hsd = HSDatalog(acq_folder)
    
    output_folder = os.path.join("." if output_folder is None else output_folder, os.path.basename(os.path.dirname(acq_folder)) + "_Exported")    

    def get_df_object(hsd, sensor, use_datalog_tags, out_format):
        hsd_dfs = []
        data_tags = None
        for i, ssd in enumerate(sensor.sensor_descriptor.sub_sensor_descriptor):
            if ssd.sensor_type != 'MLC' and ssd.sensor_type != 'STREDL' and sensor.sensor_status.sub_sensor_status[i].is_active:
                df = hsd.get_dataframe(sensor.name,ssd.sensor_type, sample_start, sample_end, labeled = use_datalog_tags)
                if use_datalog_tags:
                    data_tags = hsd.get_data_stream_tags(sensor.name, ssd.sensor_type, sample_start, sample_end)
                    use_datalog_tags = False
                if not (df is None or df.empty):
                    hsd_dfs.append(df)
        HSDatalogConverter.to_unico(output_folder, sensor.name + "_" + os.path.basename(acq_folder), hsd_dfs, data_tags, out_format, mode = 'w')
        log.info("--> {} ST format conversion completed successfully".format(sensor.name))
        
    hsd.enable_timestamp_recovery(debug)

    df_flag = True
    while df_flag:
        if sensor_name == '':
            sensor = hsd.prompt_sensor_select_CLI(hsd.get_sensor_list(only_active=True))
            if sensor is not None:
                get_df_object(hsd, sensor ,use_datalog_tags, out_format)
            else:
                break

        elif sensor_name == 'all':
            for sensor in hsd.get_sensor_list(only_active=True):
                get_df_object(hsd, sensor ,use_datalog_tags, out_format)
            df_flag = False
        
        else:
            sensor = hsd.get_sensor(sensor_name)
            if sensor is not None:
                get_df_object(hsd, sensor ,use_datalog_tags, out_format)
            else:
                log.error("No \"{}\" sensor found in your Device Configuration.".format(sensor_name))
            df_flag = False
 
if __name__ == '__main__':
    hsd_toUnico()


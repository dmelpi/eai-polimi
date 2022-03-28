#!/usr/bin/env python
# coding: utf-8 
# *****************************************************************************
#  * @file    hsdatalog_to_cartesiam.py
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
from HSD_utils.exceptions import CartesiamConversionError
from HSD_utils.converters import HSDatalogConverter
from HSD.HSDatalog import HSDatalog

log = logger.setup_applevel_logger(is_debug = False, file_name= "app_debug.log")

def show_help(ctx, param, value):
    if value and not ctx.resilient_parsing:
        click.secho(ctx.get_help(), color=ctx.color)
        click.secho("\n-> Script execution examples:")
        click.secho("   python hsdatalog_to_cartesiam.py ..\\STWIN_acquisition_examples\\STWIN_00001 -s IIS3DWB -sl 1000 -si 750", fg='cyan')
        click.secho("   python hsdatalog_to_cartesiam.py ..\\STWIN_acquisition_examples\\STWIN_00001 -o .\\ToCartesiam -s all -sl 32 -si 96 -r", fg='cyan')
        ctx.exit()

def validate_signal_increment(ctx, param, value):
    if value < 0:
        raise click.BadParameter('\'{d}\'. It must be a positive value. Please retry'.format(d=value))
    return value

@click.command()
@click.argument('acq_folder', type=click.Path(exists=True))
@click.option('-o', '--output_folder', help="Output folder (this will be created if it doesn't exist)")
@click.option('-s', '--sensor_name', help="Sensor Name - use \"all\" to convert all active sensors data, otherwise select a specific sensor by name", default='')
@click.option('-sl','--signal_length', help="The length of each segment when performing segmentation", type=int, required=True)
@click.option('-si','--signal_increment', help="Parameter to control the overlap, signal_increment = None, is equal to no overlap, signal_increment = frameLength/2 is fifty percent overlap", type=int, callback=validate_signal_increment, default=0)
@click.option('-ss','--sample_start', help="Sample Start - Data conversion will start from this sample", type=int)
@click.option('-se','--sample_end', help="Sample End - Data conversion will end up in this sample", type=int)
@click.option('-r', '--raw_data', is_flag=True, help="Uses Raw data (not multiplied by sensitivity)", default=False)
@click.option('-d', '--debug', is_flag=True, help="[DEBUG] Check for corrupted data and timestamps", default=False)
@click.option("-h", "--help", is_flag=True, is_eager=True, expose_value=False, callback=show_help, help="Show this message and exit.",)

def hsd_dataframe(acq_folder, output_folder, sensor_name, signal_length, signal_increment, sample_start, sample_end, raw_data, debug):
    hsd = HSDatalog(acq_folder)

    output_folder = os.path.join("." if output_folder is None else output_folder, os.path.basename(os.path.dirname(acq_folder)) + "_Exported")

    def get_df_object(hsd, sensor, sample_start, sample_end):
        for ssd in sensor.sensor_descriptor.sub_sensor_descriptor:
            chunk_size = 10000000 #Feel free to change it!
            isLastChunk = False
            sample_offset = 0 if sample_start is None else sample_start
            while isLastChunk == 0:
                if sample_end is not None and sample_offset + chunk_size > sample_end:
                    #read exactly the missing samples up to sample_end
                    chunk_size = sample_end - sample_offset

                sensor_name = "{}_{}".format(sensor.name, ssd.sensor_type)
                df = hsd.get_dataframe(sensor.name, ssd.sensor_type, sample_start = sample_offset, sample_end = sample_offset+chunk_size, raw_flag = raw_data)
                if df is not None:
                    try:
                        HSDatalogConverter.to_cartesiam_format(output_folder, sensor_name, df, signal_length, signal_increment)
                    except CartesiamConversionError as err:
                        log.exception(err)
                        quit()
                    if len(df) == 0 or len(df) < chunk_size:
                        isLastChunk = 1
                        log.info("--> {} Cartesiam conversion completed successfully".format(sensor_name))
                    else:
                        sample_offset = sample_offset + chunk_size
                else:
                    isLastChunk = 1
                    log.info("--> {} Cartesiam conversion completed successfully".format(sensor_name))

    hsd.enable_timestamp_recovery(debug)

    df_flag = True
    while df_flag:
        if sensor_name == '':
            sensor = hsd.prompt_sensor_select_CLI(hsd.get_sensor_list(only_active=True))
            if sensor is not None:
                get_df_object(hsd, sensor ,sample_start, sample_end)
            else:
                break

        elif sensor_name == 'all':
            for sensor in hsd.get_sensor_list(only_active=True):
                get_df_object(hsd, sensor, sample_start, sample_end)
            df_flag = False
        
        else:
            sensor = hsd.get_sensor(sensor_name)
            if sensor is not None:
                get_df_object(hsd, sensor, sample_start, sample_end)
            else:
                log.exception("No \"{}\" sensor found in your Device Configuration.".format(sensor_name))
            df_flag = False
 
if __name__ == '__main__':
    hsd_dataframe()



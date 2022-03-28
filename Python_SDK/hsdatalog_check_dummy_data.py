#!/usr/bin/env python
# coding: utf-8 
# *****************************************************************************
#  * @file    hsdatalog_check_dummy_data.py
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
import sys
import json
import click
import numpy as np
import HSD_utils.logger as logger
from HSD.HSDatalog import HSDatalog

log = logger.setup_applevel_logger(is_debug = False, file_name= "app_debug.log")

def show_help(ctx, param, value):
    if value and not ctx.resilient_parsing:
        click.secho(ctx.get_help(), color=ctx.color)
        click.secho("\n-> Script execution examples:")
        click.secho("   python hsdatalog_check_dummy_data.py ..\\STWIN_acquisition_examples\\DummyData", fg='cyan')
        click.secho("   python hsdatalog_check_dummy_data.py ..\\STWIN_acquisition_examples\\DummyData -s IMP34DT05 \n", fg='cyan')
        ctx.exit()

@click.command()
@click.argument('acq_folder', type=click.Path(exists=True))
@click.option('-s', '--sensor_name', help="Sensor name - specify a sensor name to limit the analysis. ", default='all')
@click.option('-ss','--sample_start', help="Sample Start - Data analysis will start from this sample", type=int)
@click.option('-se','--sample_end', help="Sample End - Data analysis will end up in this sample", type=int)
@click.option("-h", "--help", is_flag=True, is_eager=True, expose_value=False, callback=show_help, help="Show this message and exit.",)

def hsd_dataframe(acq_folder, sensor_name, sample_start, sample_end):
    
    hsd = HSDatalog(acq_folder)
    
    def get_hsd_object(hsd, sensor):
        for ssd in sensor.sensor_descriptor.sub_sensor_descriptor:
            chunk_size = 10000000 #Feel free to change it!
            isFirstChunk = True
            isLastChunk = False
            chunkWithErrors = 0
            sample_offset = 0
            lastData = 0
            sample_offset = 0 if sample_start is None else sample_start
            while (isLastChunk == 0):

                if sample_end is not None and sample_offset + chunk_size > sample_end:
                    #read exactly the missing samples up to sample_end
                    chunk_size = sample_end - sample_offset
                
                data = hsd.get_data_and_timestamps(sensor.name, ssd.sensor_type, sample_start = sample_offset, sample_end = sample_offset + chunk_size, raw_flag = True)[0]
                if data is not None and len(data) != 0:
                    if len(data) < chunk_size:
                        isLastChunk = 1
                    else:
                        sample_offset = sample_offset + chunk_size

                    data = data.astype(np.int16).reshape(-1)

                    # check that first data of the current chunk is last data of previous chunk + 1
                    if (data[0] != lastData+1) and (isFirstChunk == False):
                        chunkWithErrors = chunkWithErrors + 1

                    lastData = data[len(data)-1]
                    isFirstChunk = False

                    x = data[0] + np.array([i for i in range(len(data))]).astype(np.int16)

                    if not (data == x).all():
                        chunkWithErrors = chunkWithErrors + 1
                else:
                    isLastChunk = 1
                    chunkWithErrors = chunkWithErrors + 1
            
            log.info("--> Data check completed for sensor {}".format(sensor.name))
            if (chunkWithErrors == 0):
                log.info('{:>20} ({:<6}) : OK'.format(sensor.name, ssd.sensor_type))
            else:
                log.error('{:>20} ({:<6}) : ### ERRORS FOUND ###'.format(sensor.name, ssd.sensor_type))

    df_flag = True
    while df_flag:
        if sensor_name == 'all':
            for sensor in hsd.get_sensor_list(only_active=True):
                get_hsd_object(hsd, sensor)
            df_flag = False
        else:
            sensor = hsd.get_sensor(sensor_name)
            if sensor is not None:
                get_hsd_object(hsd, sensor)
            else:
                log.error("No \"{}\" sensor found in your Device Configuration.".format(sensor_name))
            df_flag = False
 
if __name__ == '__main__':
    hsd_dataframe()


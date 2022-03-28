#!/usr/bin/env python
# coding: utf-8 
# *****************************************************************************
#  * @file    hsdatalog_to_wav.py
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
from datetime import datetime
import HSD_utils.logger as logger
from HSD.HSDatalog import HSDatalog
from HSD_utils.converters import HSDatalogConverter

log = logger.setup_applevel_logger(is_debug = False, file_name= "app_debug.log")

def show_help(ctx, param, value):
    if value and not ctx.resilient_parsing:
        click.secho(ctx.get_help(), color=ctx.color)
        click.secho("\n-> Script execution examples:")
        click.secho("   python hsdatalog_to_wav.py ..\\STWIN_acquisition_examples\\STWIN_20210205_15_47_16", fg='cyan')
        click.secho("   python hsdatalog_to_wav.py ..\\STWIN_acquisition_examples\\STWIN_20210205_15_47_16 -o .\\toWav -s IMP34DT05 -ss 77777 -se 300000", fg='cyan')
        ctx.exit()

@click.command()
@click.argument('acq_folder', type=click.Path(exists=True))
@click.option('-o', '--output_folder', help="Output folder (this will be created if it doesn't exist)")
@click.option('-s', '--sensor_name', help="Sensor Name - use \"all\" to convert all active sensors data, otherwise select a specific sensor by name", default='all')
@click.option('-ss', '--sample_start', help="Sample Start - Data conversion will start from this sample", type=int)
@click.option('-se', '--sample_end', help="Sample End - Data conversion will end up in this sample", type=int)
@click.option('-h', '--help', is_flag=True, is_eager=True, expose_value=False, callback=show_help, help="Show this message and exit.",)

def hsd_toWav(acq_folder, output_folder, sensor_name, sample_start, sample_end):
    hsd = HSDatalog(acq_folder)
    
    output_folder = os.path.join("." if output_folder is None else output_folder, os.path.basename(os.path.dirname(acq_folder)) + "_Exported")
    if not os.path.exists(output_folder): os.makedirs(output_folder)

    def sensor_to_wav(hsd, sensor, sample_start = None, sample_end = None):
        try:
            for i, ssd in enumerate(sensor.sensor_descriptor.sub_sensor_descriptor):         
                nchannels = sensor.sensor_descriptor.sub_sensor_descriptor[i].dimensions;
                odr = sensor.sensor_status.sub_sensor_status[i].odr
                log.info("Sensor: {}_{} (ODR: {})".format(sensor.name, ssd.sensor_type, odr))
                
                wav_file_name = os.path.join(output_folder, "{}_{}.wav".format(sensor.name, ssd.sensor_type))
                if odr > 0:
                    wav_file = HSDatalogConverter.wav_create(wav_file_name, odr, nchannels)
                else:
                    log.exception("Bad framerate selected for this sensor")
                    break

                chunk_size = 1000000 #Feel free to change it!
                isLastChunk = False
                sample_offset = 0 if sample_start is None else sample_start
                while isLastChunk == 0:
                    if sample_end is not None and sample_offset + chunk_size > sample_end:
                        #read exactly the missing samples up to sample_end
                        chunk_size = sample_end - sample_offset

                    res = hsd.get_data_and_timestamps(sensor.name, ssd.sensor_type, sample_start = sample_offset, sample_end = sample_offset + chunk_size, raw_flag = True)
                    if res is None:
                        HSDatalogConverter.wav_close(wav_file)   
                        log.exception("No data from selected sensor")
                        break
                    data = res[0]
                    # time = res[1]
                    pcm_data = data.astype(np.int16)
                    if len(data) == 0 or len(data) < chunk_size:
                        isLastChunk = 1
                    else:
                        sample_offset = sample_offset + chunk_size
                    HSDatalogConverter.wav_append(wav_file,pcm_data)
                HSDatalogConverter.wav_close(wav_file)
        except Exception as err:
            log.exception(err)

    hsd.enable_timestamp_recovery(True)

    df_flag = True
    while df_flag:
        if sensor_name is None:
            sensor = hsd.prompt_sensor_select_CLI(hsd.get_sensor_list(only_active=True))
            if sensor is not None:
                sensor_to_wav(hsd, sensor, sample_start, sample_end)
            else:
                break
        elif sensor_name == 'all':
            for sensor in hsd.get_sensor_list(only_active=True):
                sensor_to_wav(hsd, sensor, sample_start, sample_end)
            df_flag = False
        else:
            sensor = hsd.get_sensor(sensor_name)
            if sensor is not None:
                sensor_to_wav(hsd, sensor, sample_start, sample_end)
            else:
                log.error("No \"{}\" sensor found in your Device Configuration.".format(sensor_name))
            df_flag = False

if __name__ == '__main__':
    hsd_toWav()


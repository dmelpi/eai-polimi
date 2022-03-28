#!/usr/bin/env python
# coding: utf-8 
# *****************************************************************************
#  * @file    hsdatalog_dataframes.py
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

import click
import HSD_utils.logger as logger
from HSD.HSDatalog import HSDatalog

log = logger.setup_applevel_logger(is_debug = False, file_name= "app_debug.log")

def show_help(ctx, param, value):
    if value and not ctx.resilient_parsing:
        click.secho(ctx.get_help(), color=ctx.color)
        click.secho("\n-> Script execution examples:")
        click.secho("   python hsdatalog_dataframes.py ..\\STWIN_acquisition_examples\\STWIN_00002", fg='cyan')
        click.secho("   python hsdatalog_dataframes.py ..\\STWIN_acquisition_examples\\STWIN_00002 -s all", fg='cyan')
        click.secho("   python hsdatalog_dataframes.py ..\\STWIN_acquisition_examples\\STWIN_00002 -s IIS3DWB -l -r", fg='cyan')
        ctx.exit()

@click.command()
@click.argument('acq_folder', type=click.Path(exists=True))
@click.option('-s', '--sensor_name', help="Sensor Name - use \"all\" to obtain all active sensors dataframes, otherwise select a specific sensor by name", default='')
@click.option('-ss','--sample_start', help="Sample Start - Data extraction will start from this sample", type=int, default=0)
@click.option('-se','--sample_end', help="Sample End - Data conversion will end up in this sample", type=int, default=-1)
@click.option('-r', '--raw_data', is_flag=True, help="Uses Raw data (not multiplied by sensitivity)", default=False)
@click.option('-l', '--labeled', is_flag=True, help="Includes annotations taken during acquisition (if any) in the extracted dataframe", default=False)
@click.option('-d', '--debug', is_flag=True, help="[DEBUG] Check for corrupted data and timestamps", default=False)
@click.option("-h", "--help", is_flag=True, is_eager=True, expose_value=False, callback=show_help, help="Show this message and exit.",)

def hsd_dataframe(acq_folder, sensor_name, sample_start, sample_end, raw_data, labeled, debug):
    hsd = HSDatalog(acq_folder)

    def get_df_object(hsd, sensor, labeled):
        for ssd in sensor.sensor_descriptor.sub_sensor_descriptor:
            try:
                df = hsd.get_dataframe(sensor.name,ssd.sensor_type, sample_start, sample_end, labeled = labeled, raw_flag= raw_data)
                if not (df is None or df.empty):
                    log.info("\nDataFrame - Start sample: {}, End sample: {}\n{}".format(sample_start, len(df)-1 ,df))
            except Exception as err:
                log.exception(err)

    hsd.enable_timestamp_recovery(debug)

    df_flag = True
    while df_flag:
        if sensor_name == '':
            sensor = hsd.prompt_sensor_select_CLI(hsd.get_sensor_list(only_active=True))
            if sensor is not None:
                get_df_object(hsd, sensor, labeled)
            else:
                break

        elif sensor_name == 'all':
            for sensor in hsd.get_sensor_list(only_active=True):
                get_df_object(hsd, sensor, labeled)
            df_flag = False
        
        else:
            sensor = hsd.get_sensor(sensor_name)
            if sensor is not None:
                get_df_object(hsd, sensor, labeled)
            else:
                log.error("No \"{}\" sensor found in your Device Configuration.".format(sensor_name))
            df_flag = False
 
if __name__ == '__main__':
    hsd_dataframe()


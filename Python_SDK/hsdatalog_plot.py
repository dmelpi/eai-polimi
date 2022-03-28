#!/usr/bin/env python
# coding: utf-8 
# *****************************************************************************
#  * @file    hsdatalog_plot.py
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
import matplotlib.pyplot as plt
import HSD_utils.logger as logger
from HSD.HSDatalog import HSDatalog

log = logger.setup_applevel_logger(is_debug = False, file_name= "app_debug.log")

def show_help(ctx, param, value):
    if value and not ctx.resilient_parsing:
        click.secho(ctx.get_help(), color=ctx.color)
        click.secho("\n-> Script execution examples:")
        click.secho("   python hsdatalog_plot.py ..\\STWIN_acquisition_examples\\STWIN_00001", fg='cyan')
        click.secho("   python hsdatalog_plot.py ..\\STWIN_acquisition_examples\\STWIN_00001 -s all", fg='cyan')
        click.secho("   python hsdatalog_plot.py ..\\STWIN_acquisition_examples\\STWIN_00002 -s IIS3DWB -ss 7 -se 13579", fg='cyan')
        click.secho("   python hsdatalog_plot.py ..\\STWIN_acquisition_examples\\STWIN_00002 -s all -l", fg='cyan')
        click.secho("   python hsdatalog_plot.py ..\\STWIN_acquisition_examples\\STWIN_00002 -l -p -r \n", fg='cyan')
        ctx.exit()

@click.command()
@click.argument('acq_folder', type=click.Path(exists=True))
@click.option('-s', '--sensor_name', help="Sensor Name - use \"all\" to plot all active sensors data, otherwise select a specific sensor by name", default='')
@click.option('-ss','--sample_start', help="Sample Start - Data plot will start from this sample", type=int, default=0)
@click.option('-se','--sample_end', help="Sample End - Data plot will end up in this sample", type=int, default=-1)
@click.option('-r', '--raw_data', is_flag=True, help="Uses Raw data (not multiplied by sensitivity)", default=False)
@click.option('-l', '--labeled', is_flag=True, help="Plot data including information about annotations taken during acquisition (if any)", default=False)
@click.option('-p', '--subplots', is_flag=True, help="Multiple subplot for multi-dimensional sensors", default=False)
@click.option('-d', '--debug', is_flag=True, help="[DEBUG] Check for corrupted data and timestamps", default=False)
@click.option("-h"," --help", is_flag=True, is_eager=True, expose_value=False, callback=show_help, help="Show this message and exit.",)

def hsd_plot(acq_folder, sensor_name, sample_start, sample_end, raw_data, labeled, subplots, debug):
    hsd = HSDatalog(acq_folder)

    def get_plot_object(hsd, sensor, label, subplots):
        for ssd in sensor.sensor_descriptor.sub_sensor_descriptor:
            hsd.get_sensor_plot(sensor.name, ssd.sensor_type, sample_start, sample_end, label = label, subplots = subplots, raw_flag = raw_data)

    hsd.enable_timestamp_recovery(debug)

    label = None
    if labeled:
        label_list = hsd.get_acquisition_label_classes()
        label = hsd.prompt_label_select_CLI(label_list)
        if label is not None:
            log.info(label)
        else:
            quit()

    plot_flag = True
    while plot_flag:
        if sensor_name == '':
            sensor = hsd.prompt_sensor_select_CLI(hsd.get_sensor_list(only_active=True))
            if sensor is not None:
                get_plot_object(hsd,sensor,label,subplots)
            else:
                break

        elif sensor_name == 'all':
            for sensor in hsd.get_sensor_list(only_active=True):
                get_plot_object(hsd,sensor,label,subplots)
            plot_flag = False
        
        else:
            sensor = hsd.get_sensor(sensor_name)
            if sensor is not None:
                get_plot_object(hsd,sensor,label,subplots)
            else:
                log.error("No \"{}\" sensor found in your Device Configuration.".format(sensor_name))
                quit()
            plot_flag = False
        
        plt.show()
 
if __name__ == '__main__':
    hsd_plot()


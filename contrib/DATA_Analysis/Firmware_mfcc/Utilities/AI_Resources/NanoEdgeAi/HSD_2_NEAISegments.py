#!/usr/bin/env python
# coding: utf-8 

#
#                   Copyright (c) 2020 STMicroelectronics.
#                             All rights reserved
#
#   This software component is licensed by ST under BSD-3-Clause license,
#   the "License"; You may not use this file except in compliance with the
#   License. You may obtain a copy of the License at:
#                        https://opensource.org/licenses/BSD-3-Clause
  
'''
Helper code for converting a high speed datalog acquisition to NanoEdge AI studio complying segment file.

'''

import sys, argparse, numpy as np
sys.path.append('../../Datalog/Python_SDK/')
from HSD.HSDatalog import HSDatalog
from HSD_utils.converters import HSDatalogConverter

# parsing inputs and assigning default values to the parameters which are not provided by the user
parser = argparse.ArgumentParser(description='Data parser for high speed data logs from STWIN.' +
 											'This function parse the data from .dat files recoded' +
 											'with STWIN and converts the files into .csv files which' +
 											'are in NanoEdge AI studio format. ' 
 											+ 'It takes as an input the path to the directory containing high speed datalogs from STWIN.')
parser.add_argument('acqFolder', type = str, 
					help = 'Path of the directory containing .dat, AcquisitionInfo.json, and DeviceConfig.json files.' )
parser.add_argument('-raw', '--raw', default=True, type=bool, help='if to keep raw sensor data, i.e. with out multiplying the sensor sensitivity.' +
		' (default: True)\n In this case the register values of the sensors are returned except the audio.')
parser.add_argument('-sensorName', '--sensorName', default='ISM330DHCX', type=str, help='The name of the sensor to be parsed (default: ISM330DHCX).')
parser.add_argument('-sensorType', '--sensorType', default='ACC', type=str, help='The type of the sensor to be parsed (default: ACC).')
parser.add_argument('-winLen', '--winLen', default='1024', type=int, help='The size of the window/segment (default: 1024).')
parser.add_argument('-stride', '--stride', default='1024', type=int, help='The stride for taking the next segment (default: 1024).')
parser.add_argument('-offset', '--offset', default='512', type=int, help='The samples to skip at the start (default: 512).')

# entry point of the script 

'''
Parsing the provided values
'''
args = parser.parse_args()

# sensor information
acqDir = args.acqFolder
sensor_name = args.sensorName
sensor_type = args.sensorType
raw = args.raw # if the raw samples are required (True) or sensitivity to be applied to get physical values (False)

# framing related variables
winLen = args.winLen
stride = args.stride
offset = args.offset

# creating a high-speed datalog object for the given acquisition folder
hsd = HSDatalog( acquisition_folder = acqDir )
        
# reading the data as dataframe
dataFrame = hsd.get_dataframe( sensor_name, sensor_type, sample_start = offset, raw_flag = raw )
        
# converting to cartesiam complient frames
HSDatalogConverter.to_cartesiam_format( output_folder = acqDir, sensor_name = sensor_name,
                                        df = dataFrame, signal_length = winLen, signal_increment = stride )
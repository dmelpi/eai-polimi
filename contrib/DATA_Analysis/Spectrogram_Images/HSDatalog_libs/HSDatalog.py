from HSDatalog_libs import ClassFromDeviceConfig as cdev, ClassFromAcquisitionInfo as ctag
from typing import List
import json
import os
import sys
import ntpath
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

sns.set()


#### General purpose utility functions

# utilities for data type conversion from Datalog to Python


def checkType(check_type):
    switcher = {
        'uint8_t': 'uint8',
        'uint16_t': 'uint16',
        'uint32_t': 'uint32',
        'int8_t': 'int8',
        'int16_t': 'int16',
        'int32_t': 'int32',
        'float': 'float32',
        'double': 'double',
    }
    return switcher.get(check_type, "error")


def checkTypeLength(check_type):
    switcher = {
        'uint8_t': 1,
        'int8_t': 1,
        'uint16_t': 2,
        'int16_t': 2,
        'uint32_t': 4,
        'int32_t': 4,
        'float': 4,
        'double': 8,
    }
    return switcher.get(check_type, "error")


def get_JsonFile(fp, jf_name="DeviceConfig.json"):
    """Opens a JSON file ( by default the Device Config ) from path
    """
    cfn = os.path.join(fp, jf_name)
    with open(cfn) as jf:
        cJ = json.load(jf)
    return cJ


def CLI_selectDatalogFolder(path=None):
    """prompt user to select Datalog folder"""
    if path is None:
        cwd = os.getcwd()
    else:
        cwd = path
    files = get_listOfDatalogSubFolders(cwd)
    baseNames = [b[1] for b in files]
    s = CLI_select('Folder', baseNames)
    return [b[0] for b in files if b[1] == s][0]


def CLI_select(what: str, fromList: list):
    choiceNotInRange = True
    while choiceNotInRange:
        if len(fromList) == 1:
            print(' single {} in dataset: '.format(what), fromList[0])
            choice = 0
            choiceNotInRange = False
        else:
            print('\n'.join([str(i) + ' - ' + c for i, c in enumerate(fromList)]))
            print('q - Quit')
            choice = input('Select one {} (''q'' to quit) ==> '.format(what))
            print('')
            try:
                choiceNotInRange = int(choice) not in range(len(fromList))
            except ValueError:
                if choice != 'q':
                    print('please input a correct value')
                else:
                    choiceNotInRange = False

    if choice == 'q':
        print('Bye!')
        sys.exit(0)
    else:
        return fromList[int(choice)]


def get_listOfDatalogSubFolders(path=None):
    """
    get_listOfDatalogSubFolders(path=None)
    path: optional parameter. default is current path (as from os.getcwd()
    returns an np.array [full path, basename]
    full path = full path of folders with a valid Datalog
    basename = basename of folders (useful foe GUIs)
    """
    if path is None:
        cwd = os.getcwd()
    else:
        cwd = path
    folders = next(os.walk(cwd))[1]
    fps = [os.path.join(cwd, f) for f in folders]
    fps_ok = []

    for fp in fps:
        if check_HSDFolderStructure(fp, verbose=False)[0]:
            fps_ok = np.append(fps_ok, fp)
    ret = [[pa, ntpath.basename(pa)] for pa in fps_ok]
    return ret


class JsonControlUtils:
    @staticmethod
    def activeSensorsFromJson(cJ):
        activeDev = [t['sensorDescriptor']['name'] for t in cJ['device']['sensor'] if t['sensorStatus']['isActive']]
        return activeDev

    @staticmethod
    def filesFromJson(cJ):
        """
        list of files that are supposed to be in the datalog based on DeviceConfig.json
        """
        dataFilesList = []
        for d in cJ['device']['sensor']:
            sensorName = d['name']
            for idx, s in enumerate(d['sensorStatus']['subSensorStatus']):
                if s['isActive']:
                    subName = d['sensorDescriptor']['subSensorDescriptor'][idx]['sensorType']
                    fileName = sensorName + '_' + subName + '.dat'
                    dataFilesList.append(fileName)
        return dataFilesList


def check_HSDFolderStructure(fp, verbose=True):
    """
    returns [isConfigured, isLabelled, dataFilesExistReport]
    isConfigured is a boolean => folder and Config JSON file exist
    isLabelled is a boolean => labels JSON file exist in folder
    dataFilesExistReport is a list of tuples (filename, boolean => data file exist in folder)
    """
    isConfigured = False
    isLabelled = False
    dataFilesExistReport = [False]

    tagFName = os.path.join(fp, Datalog.labels_JsonFileName)
    jsonFName = os.path.join(fp, Datalog.config_JsonFileName)

    if os.path.exists(fp) and os.path.exists(jsonFName):
        cJ = get_JsonFile(fp, jf_name=Datalog.config_JsonFileName)
        dataFiles = JsonControlUtils.filesFromJson(cJ)
        dataFilesPath = [os.path.join(fp, f) for f in dataFiles]
        dataFilesExist = [os.path.exists(f) for f in dataFilesPath]
        dataFilesExistReport = [(f, os.path.exists(os.path.join(fp, f))) for f in dataFiles]

        isConfigured = all(dataFilesExist)
        if os.path.exists(tagFName):
            isLabelled = True
    elif verbose:
        print('Failed existence check in datalog folder:')
        print('path: ', os.path.exists(fp))
        print(Datalog.labels_JsonFileName, 'file:', os.path.exists(tagFName))
        print(Datalog.config_JsonFileName, 'file:', os.path.exists(jsonFName))
    return [isConfigured, isLabelled, dataFilesExistReport]


def get_subSensorsInfo(selectedSensor: cdev.Sensor, onlyActive=False):
    info = []
    for kk in range(len(selectedSensor.sensor_descriptor.sub_sensor_descriptor)):
        des = selectedSensor.sensor_descriptor.sub_sensor_descriptor[kk]
        st = selectedSensor.sensor_status.sub_sensor_status[kk]
        d = {}
        d['index'] = des.id
        d['samplesPerTs'] = st.samples_per_ts
        d['dataPerSample'] = des.dimensions
        d['dimensionsLabel'] = des.dimensions_label
        d['dataType'] = checkType(des.data_type)
        d['dataType_size'] = checkTypeLength(des.data_type)
        d['sensorType'] = des.sensor_type
        d['units'] = des.unit
        d['odr'] = st.odr
        d['initialOffset'] = st.initial_offset
        d['fs'] = st.fs
        d['isActive'] = st.is_active
        d['sensitivity'] = st.sensitivity
        d['timeStamp_size'] = checkTypeLength('double')
        d['dataFrameSize'] = int(st.samples_per_ts * des.dimensions * checkTypeLength(des.data_type))
        if (not onlyActive) or (onlyActive and st.is_active):
            info.append(d)
    return info


def get_subSensorInfo(selectedSensor: cdev.Sensor, subSensorType: str):
    for d in get_subSensorsInfo(selectedSensor):
        if d['sensorType'] == subSensorType:
            return d


def decode_fileName(fileName):
    [sensorName, sub] = fileName.split(".")[0].split("_")
    return sensorName, sub


def encode_fileName(sensorName, subSensorType):
    fileName = sensorName + '_' + subSensorType + '.dat'
    return fileName


class DatalogTags:
    def __init__(self, tags: List[ctag.Tag]):
        self.labels = sorted(set(dic.label for dic in tags))
        self.timeTags = self.get_timeTags(tags)

    def get_timeTags(self, tags: List[ctag.Tag]):
        # for each label and for each time segment:
        # time_labels: array of tag
        #   = {'label': lbl, 'time_start': t_start, 'time_end': xxx, }
        time_labels = []
        for lbl in self.labels:

            # start_time, end_time are vectors with the corresponding 't' entries in DataTag-json
            start_time = np.array([t.t for t in tags if t.label == lbl and t.enable])
            end_time = np.array([t.t for t in tags if t.label == lbl and not t.enable])

            # now must associate at each start tag the appropriate end tag
            # (some may be missing because of errors in the tagging process)
            for tstart in start_time:
                tag = {}
                jj = [i for (i, n) in enumerate(end_time) if n >= tstart]
                if jj:
                    tend = end_time[min(jj)]
                else:
                    tend = float(-1)  # if no 'end tag' found the end is eof
                tag['Label'] = lbl
                tag['time_start'] = tstart
                tag['time_end'] = tend
                time_labels.append(tag)
        return time_labels


class Datalogger:

    def __init__(self, json_config):
        acquisition = cdev.welcome_from_dict(json_config)
        self.device = acquisition.device
        self.sensorsMap = dict([(x.name, x.id) for x in self.device.sensor])
        self.sensorsNames = [x.name for x in self.device.sensor]
        self.activeSensors = [x.name for x in self.device.sensor if
                              any([y.is_active for y in x.sensor_status.sub_sensor_status])]

    def get_sensor(self, name: str = None, index: int = None) -> cdev.Sensor:
        """
        returns a Sensor from acquisition.device selected by name or index
        """
        if not (name or index):
            print("ERROR get_sensor(): at least name or index must be provided")
            return None
        if name:
            if name in self.sensorsNames:
                return self.device.sensor[self.sensorsMap[name]]
            else:
                print('ERROR get_sensor(): sensor {} is not in this datalog'.format(name))
                return None
        if index:
            if index in range(self.device.device_info.n_sensor):
                return self.device.sensor[index]
            else:
                print('ERROR get_sensorParams(): index {} very out of range'.format(index))
                return None


class Datalog:
    """
    main Class of this module. Access to datalog data and realtime control of the datalogger
    ----

    Attributes
    ==========
    acquisitioninfo: class tree generated from DeviceConfig.json by https://app.quicktype.io/
    config_JsonFileName: class attribute
    labels_JsonFileName: class attribute
    json_config: deserialization of file DeviceCOnfig.json
    json_labels: deserialization of file AcquisitionInfo.json

    datalogIsLabelled: boolean
    datalogTags: instance of Class DatalogTags - all labels and their timing and sample limits
    datalogger: instance of Class Datalogger - the board that originated the Datalog
    path: path of the Datalog

    Methods
    =======
    get_dataStream(sensorName: str, subSensorType: str)
    get_dataStreamTags(sensorName: str, subSensorType: str)
    get_sensorDataFrame(sensorName: str <sensor name>, labelled=False <if True adds the Label columns to just the data>)
    get_subSensorDataFrame(sensor: class Sensor, subSensorType: str, labelled=False)
    --
    plot_Datalog(sensorName, label=False, subplot=False)
    toUnico(sensorName='ISM330DHCX', outputFolderName='UnicoFiles', useDatalogTags=True)
    print_AcquisitionInfo() - utility to pretty print the json file
    print_DeviceConfig() - utility to pretty print the json file
    """

    # Class Attributes
    config_JsonFileName = "DeviceConfig.json"
    labels_JsonFileName = "AcquisitionInfo.json"

    def __init__(self, datalog_path=''):
        self.checkTimestamps = False
        if datalog_path != '':
            self.path = datalog_path
        else:
            self.path = CLI_selectDatalogFolder()
        datalogIsOK, self.datalogIsLabelled, dataFilesExistReport = check_HSDFolderStructure(self.path)
        if not datalogIsOK:
            if all(e[1] for e in dataFilesExistReport):
                print("Bad Datalog in path: ", self.path)
            else:
                print('Check Failed: missing data files of active sensors:')
                [print('{} {}'.format(e[0], e[1])) for e in dataFilesExistReport]
            sys.exit(0)
        self.json_config = get_JsonFile(self.path, jf_name=Datalog.config_JsonFileName)
        if self.datalogIsLabelled:
            self.json_labels = get_JsonFile(self.path, jf_name=Datalog.labels_JsonFileName)
            acquisitionInfo = ctag.welcome_from_dict(self.json_labels)
            self.datalogTags = DatalogTags(acquisitionInfo.tags)
        else:
            print('Warning: Datalog not labeled (file ' + Datalog.labels_JsonFileName + ' not found)')
            self.json_labels = None
            self.datalogTags = None
        self.datalogger = Datalogger(self.json_config)
        self.acquisitionFiles = JsonControlUtils.filesFromJson(self.json_config)
        self.filesInfo = self.get_filesInfo()

    def get_filesInfo(self):
        info = [
            {'size': os.path.getsize(os.path.join(self.path, f)),
             'name': f,
             'length': float(self.get_dataStream('', '', fileName=f)[1][-1]) - float(
                 self.get_dataStream('', '', fileName=f)[1][0]),
             } for f in self.acquisitionFiles]
        return info

    def print_AcquisitionInfo(self):
        print(json.dumps(self.json_labels, indent=4, sort_keys=True))

    def print_DeviceConfig(self):
        print(json.dumps(self.json_config, indent=4, sort_keys=True))

    def summary(self):
        # TODO: decide on contents to expose here. e.g. acquisition length, what sensors, etc...
        pass

    def get_dataType(self, sensorName, subsensortype):
        s = self.datalogger.get_sensor(sensorName)
        i = get_subSensorInfo(s, subSensorType=subsensortype)
        return i['dataType']

    def get_sensorDataFrame(self, sensorName: str, labelled: bool = False):
        """
        get_sensorDataFrame(sensorName, labelled)
        returns a pandas DataFrame with Time and Data columns from sensorname
        if sensorName is not specified user is prompted to select via CLI
        labelled = bool: if datalog is labelled adds one boolean column per data tag
                        True = label on; False = label off
        """
        if sensorName == '':
            sensorName = CLI_select('sensor', self.datalogger.activeSensors)
        elif sensorName not in self.datalogger.sensorsNames:
            print('Sensor {} does not exist in current datalog'.format(sensorName))
            return None

        sensor = self.datalogger.get_sensor(name=sensorName)
        subi = get_subSensorsInfo(sensor, onlyActive=True)
        num_activesub = len(subi)

        if num_activesub == 2 and subi[0]['odr'] != subi[1]['odr']:
            print('get_sensorDataFrame ERROR: ACC and GYRO must have the same ODR')
            exit(0)

        df_sub = []
        for idx, sub in enumerate(subi):
            # tag columns are after data columns
            if idx == num_activesub - 1:
                label_flag = labelled
            else:
                label_flag = False
            df_sub.append(self.get_subSensorDataFrame(sensor, subSensorType=sub['sensorType'], labelled=label_flag))
        df = pd.concat(df_sub, axis=1).drop_duplicates()
        return df.loc[:, ~df.columns.duplicated()]

    def get_subSensorDataFrame(self, sensor: cdev.Sensor, subSensorType: str = 'default',
                               labelled=False) -> pd.DataFrame:
        """
        get_subSensorDataFrame(sensor, subSensorType, labelled)
        returns a pandas DataFrame with Time and Data columns from sensor's selected subSebsor
        sensor: a Sensor instance
        subSensorType: str as in subsensorDescriptor.sensorType
        labelled = bool: if datalog is labelled adds one boolean column per data tag
                        True = label on; False = label off
        """
        if labelled and not self.datalogIsLabelled:
            print('get_subSensorDataFrame: Datalog is not Labelled')
            return None

        if subSensorType == 'default':
            subSensorType = sensor.sensor_descriptor.sub_sensor_descriptor[0].sensor_type

        subP = get_subSensorInfo(sensor, subSensorType)

        s, time = self.get_dataStream(sensor.name, subSensorType)

        cols = []
        numAxes = int(subP['dataPerSample'])
        if numAxes == 3:
            cc = subP['dimensionsLabel']
            col_prefix = subP['sensorType'][0] + '_'
            col_postfix = ' [' + subP['units'] + ']'
            c = [col_prefix + s + col_postfix for s in cc]
        elif numAxes == 1:
            c = [subP['sensorType']]
        else:
            print('get_subSensorDataFrame() ERROR: wring number of sensor axes ({})'.format(numAxes))
        cols = np.append(cols, c, axis=0)

        cols = np.append(["Time"], cols, axis=0)
        val = np.append(time, s, axis=1)

        df = pd.DataFrame(data=val, columns=cols)

        if labelled:
            tags = self.get_dataStreamTags(sensor.name, subSensorType)
            for lbl in self.datalogTags.labels:
                lbl_col = np.zeros(time.shape, dtype=bool)
                lbl_tags = [x for x in tags if x['Label'] == lbl]
                for lt in lbl_tags:
                    lbl_col[lt['sample_start']:lt['sample_end']] = True
                df[lbl] = lbl_col

        return df

    def toUnico(self, sensorName='ISM330DHCX', outputFolderName='UnicoFiles', useDatalogTags=False):
        """
        if 2 subSensors are active they are supposed to have the same odr
        MLC channel is excluded
        """
        if sensorName != 'ISM330DHCX':
            print('toUnico: current version supports only ISM330DHCX sensor')
            return

        sens = self.datalogger.get_sensor(sensorName)
        subs = get_subSensorsInfo(sens, onlyActive=True)

        if len(subs) >= 2 and subs[0]['odr'] != subs[1]['odr']:
            print('toUnico: {} and {} must have the same odr'.format(subs[0]['sensorType'], subs[1]['sensorType']))
            print('{} ODR = {}'.format(subs[0]['sensorType'], subs[0]['odr']))
            print('{} ODR = {}'.format(subs[1]['sensorType'], subs[1]['odr']))
            exit(0)

        if sens.sensor_status.sub_sensor_status[0].is_active:
            axCol = ['A_' + s.upper() + ' [' + subs[0]['units'] + ']' for s in subs[0]['dimensionsLabel']]
            axData, time = self.get_dataStream(sensorName, 'ACC')
            data = axData
            colHeader = axCol
            num_streams = subs[0]['dataPerSample']
        if sens.sensor_status.sub_sensor_status[1].is_active:
            gyCol = ['G_' + s.upper() + ' [' + subs[1]['units'] + ']' for s in subs[1]['dimensionsLabel']]
            gyData, time = self.get_dataStream(sensorName, 'GYRO')
            data = gyData
            colHeader = gyCol
            num_streams = subs[1]['dataPerSample']
        if sens.sensor_status.sub_sensor_status[0].is_active and sens.sensor_status.sub_sensor_status[1].is_active:
            data = np.concatenate((axData, gyData), axis=1)
            colHeader = axCol + gyCol
            num_streams = subs[0]['dataPerSample'] + subs[1]['dataPerSample']

        def write_UnicoFile(filename, columns, data: np.array):
            file = open(filename, "w")
            file.write('STEVAL-STWINKT1 (ISM330DHCX) \n\n\n')
            [file.write(c + '\t') for c in columns]
            file.write('\n')
            for r in range(data.shape[0]):
                for c in range(data.shape[1]):
                    print('{:14.9f}'.format(data[r, c]), end='\t', file=file)
                print('', file=file)
            file.close()

        # Datalog subfolder where the output files (folders) are written
        outFolder = os.path.join(self.path, outputFolderName)
        if not os.path.exists(outFolder):
            try:
                os.mkdir(outFolder)
            except OSError:
                print("Creation of folder %s failed" % outFolder)
            else:
                print("Successfully created output folder %s " % outFolder)

        if useDatalogTags:
            for f in self.datalogTags.labels:
                outSubFolder = os.path.join(self.path, outputFolderName, f)
                if not os.path.exists(outSubFolder):
                    try:
                        os.mkdir(outSubFolder)
                        print("Created output sub-folder %s " % outSubFolder)
                    except OSError:
                        print("Creation of the directory %s failed" % outSubFolder)

            tags = self.get_dataStreamTags(sensorName, 'ACC')
            for idx, tag in enumerate(tags):
                labelFileName = tag['Label'] + '_' + sensorName + '_dataLog_' + str(idx) + '.txt'
                labelFile = os.path.join(self.path, outputFolderName, tag['Label'], labelFileName)
                write_UnicoFile(labelFile, colHeader, data[tag['sample_start']:tag['sample_end'], :])
        else:
            filename = os.path.join(outFolder, sensorName + '.txt')
            write_UnicoFile(filename, colHeader, data)

    def get_dataStream(self, sensorName, subSensorType, fileName=None):
        """
        stream_data, stream_time = get_dataStream(self, sensorName, subSensorType, fileName=None)
        sensorName is sensor p/n
        subSensorType is in ('ACC', 'GYRO', 'PRESS', 'TEMP', 'HUM')
        stream_time: numpy array of 1 clock value per each data sample
        stream_data = array of sample data with all "dimensions" of subSensor
        returns

        """

        def get_dataAndTimeStream(rawData, print_debug=False):
            """ gets data from a file .dat
                np array with one column for each axis of each active subSensor
                np array with sample times
            """

            if subSensorType != 'MLC':
                checkTimeStamps = self.checkTimestamps
                framePeriod = samplesPerTs / subInfo['odr']
            else:
                checkTimeStamps = False
                framePeriod = 0

            # rndDataBuffer = rawData rounded to an integer # of frames
            rndDataBuffer = rawData[0:int(frame_size * num_frames)]

            # data = array of zeros of same length as rndDataBuffer but without ts
            data = np.zeros((samplesPerFrame * num_frames, 1), dtype=dataType)

            timeStamp = []
            for ii in range(num_frames):  # For each Frame:
                startFrame = ii * frame_size
                # segmentData = data in the current frame
                segmentData = rndDataBuffer[startFrame: startFrame + dataFrame_size]
                # segmentTS = ts is at the end of each frame
                segmentTS = rndDataBuffer[startFrame + dataFrame_size: startFrame + frame_size]

                # timestamp of current frame
                timeStamp.append(np.frombuffer(segmentTS, dtype='double'))
                # Data of current frame
                data[ii * samplesPerFrame:(ii + 1) * samplesPerFrame, 0] = \
                    np.frombuffer(segmentData, dtype=dataType)

                # Check Timestamp consistency
                if checkTimeStamps and (ii > 0):
                    deltaTS = timeStamp[ii] - timeStamp[ii - 1]
                    if abs(deltaTS) < 0.66 * framePeriod or abs(deltaTS) > 1.33 * framePeriod or np.isnan(
                            timeStamp[ii]) or np.isnan(timeStamp[ii - 1]):
                        data[ii * samplesPerFrame:(ii + 1) * samplesPerFrame, 0] = 0
                        timeStamp[ii] = timeStamp[ii - 1] + framePeriod
                        print('WARNING Sensor {} - {}: corrupted data at'.format(sensorName, subSensorType),
                              timeStamp[ii], 'sec')

            timestamp_first = subInfo['initialOffset']
            timeStamp = np.append(timestamp_first, timeStamp)

            # when you have only 1 frame, framePeriod = last timeStamp
            if num_frames == 1:
                timeStamp = np.append(timeStamp, framePeriod)

            sData = np.reshape(data, (-1, n_axis)).astype(dtype=float)

            if print_debug:
                right_adjust = 20
                print('# of frames: '.rjust(right_adjust), num_frames)
                print('samplesPerFrame = '.rjust(right_adjust), samplesPerFrame)
                print('dataFrame_size = '.rjust(right_adjust), dataFrame_size)
                print('timeStamp_size = '.rjust(right_adjust), timeStamp_size)
                print('frame_size = dF + ts = '.rjust(right_adjust), frame_size)
                print('len data = '.rjust(right_adjust), len(data))
                print('len rawData = '.rjust(right_adjust), len(rawData))
                print('len rndDataBuffer = '.rjust(right_adjust), len(rndDataBuffer))

            sensitivity = float(subInfo['sensitivity'])
            for kk in range(subInfo['dataPerSample']):
                sData[:, int(kk)] = sData[:, int(kk)] * sensitivity

            # samples_time: numpy array of 1 clock value per each data sample
            samples_time = np.zeros((num_frames * samplesPerTs, 1))
            # sample times between timestamps are linearly interpolated
            for ii in range(num_frames):  # For each Frame:
                temp = np.linspace(timeStamp[ii], timeStamp[ii + 1], samplesPerTs, endpoint=False)
                samples_time[ii * samplesPerTs:(ii + 1) * samplesPerTs, 0] = temp

            return sData, samples_time

        if fileName is None:
            filename = os.path.join(self.path, sensorName + '_' + subSensorType + '.dat')
        else:
            sensorName, subSensorType = decode_fileName(fileName)
            filename = os.path.join(self.path, fileName)

        with open(filename, 'rb') as fid:
            raw_data = np.fromfile(fid, dtype='uint8')

        selected_sensor = self.datalogger.get_sensor(sensorName)
        subInfo = get_subSensorInfo(selected_sensor, subSensorType)

        n_axis = subInfo['dataPerSample']
        dataType = subInfo['dataType']
        dataType_size = subInfo['dataType_size']
        samplesPerTs = subInfo['samplesPerTs']
        
        if samplesPerTs == 0:
            dataFrame_size = len(raw_data)   
            timeStamp_size = 0
        else:
            dataFrame_size = subInfo['dataFrameSize']
            timeStamp_size = subInfo['timeStamp_size']
            
        # frame is data + ts
        frame_size = dataFrame_size + timeStamp_size

        # number of frames = round down (//) len datalog // frame_size
        num_frames = len(raw_data) // frame_size
    
        # samplesPerFrame = # of data samples in 1 frame
        # must be the same as samplePerTs * number of active subsensors * number o axes !!!
        samplesPerFrame = int(dataFrame_size / dataType_size)
        
        if samplesPerTs == 0:
            samplesPerTs = samplesPerFrame

        if num_frames == 0:
            # todo throw a warning
            print('file {}: Empty data stream'.format(filename))
            print('WARNING: sensor {} dataStream is empty'.format(sensorName))

        stream_data, stream_time = get_dataAndTimeStream(raw_data)

        return stream_data, stream_time

    def get_dataStreamTags(self, sensorName: str, subSensorType: str):
        """
        returns an array of dict:
        {'Label': <Label>, 'time_start': <time start: float>, 'time_end': <time end: float>,
                             'sample_start': <sample index start: int>, 'sample_end': <sample index end: int>}
        """

        if not self.datalogIsLabelled:
            print('get_dataStreamTags: current Datalog is not labelled')
            return

        stream_time = self.get_dataStream(sensorName, subSensorType)[1]

        st_tim = np.reshape(stream_time, -1)
        ind_sel = np.array(range(len(st_tim)))
        sensor_labels = []

        for tag in self.datalogTags.timeTags:
            sampleTag = {}
            tend = float(tag['time_end'])
            tend = tend if tend >= 0 else st_tim[-1]
            tstart = float(tag['time_start'])

            ind_inf = st_tim <= tend
            ind_sup = st_tim >= tstart
            ind_both = np.logical_and(ind_inf, ind_sup)
            jj = ind_sel[ind_both]

            if len(jj) > 0:
                s_start = min(jj)
                s_end = max(jj)
                sampleTag = {'Label': tag["Label"], 'time_start': tag['time_start'], 'time_end': tag['time_end'],
                             'sample_start': s_start, 'sample_end': s_end}
            else:
                # happens only if start and end tags are less than one streamTime sample apart
                # for example with very low odr (e.g. temp, humidity)
                # Must decide how to handle it if it happens
                print('Tag Error: #### no data samples correspond to time tags')
                print('label: ', tag['Label'], '; start_time: ', tstart, '; end_time: ', tend, 'stream limits: ',
                      str(stream_time[0]), ' ', str(stream_time[-1]))
                print(stream_time.shape)

            sensor_labels.append(sampleTag)

        return sensor_labels

    def plot_dataFile(self, fileName, label=None, subplot=False, multi=False):
        [sensorName, sub] = fileName.split(".")[0].split("_")
        self.plot_Datalog(sensorName, label=label, subplot=subplot, multi=multi, subSensor=sub)

    def plot_Datalog(self, sensorName: str, label=None, subplot=False, multi=False, subSensor=None):
        """
        plot_Datalog(sensorName: str, label=False, subplot=False, multi=False)
        if multi is set to True then plt.show() must be called by the caller
        """
        if label and not self.datalogIsLabelled:
            print('Datalog.plot_Datalog(): attempt to use flag "label" on a non labelled Datalog ')
            exit(0)

        sens = self.datalogger.get_sensor(sensorName)
        if not subSensor:
            subi = get_subSensorsInfo(sens, onlyActive=True)
            dfl = [self.get_subSensorDataFrame(sens, subSensorType=s['sensorType'], labelled=label) for s in subi]
        else:
            dfl = [self.get_subSensorDataFrame(sens, subSensorType=subSensor, labelled=label)]
            subi = [get_subSensorInfo(sens, subSensor)]
        nsubs = len(subi)

        if label and subplot:
            for i in range(nsubs):
                df = dfl[i]
                cols = ['']
                if subi[i]['sensorType'] in ('ACC', 'GYRO', 'MAG'):
                    cols = ['X', 'Y', 'Z']
                dps = subi[i]['dataPerSample']
                fig, axs = plt.subplots(dps)
                if dps == 1:
                    axs = (axs,)
                tit = 'Label: ' + label + '-' + sensorName + '-' + subi[i]['sensorType']
                fig.suptitle(tit)
                for idx, p in enumerate(axs):
                    not_TAG = np.where(df[label], df.iloc[:, idx + 1].min(), df.iloc[:, idx + 1])
                    TAG = np.where(df[label], df.iloc[:, idx + 1], df.iloc[:, idx + 1].min())
                    p.plot(df[['Time']], TAG, color='blue', label=label)
                    p.plot(df[['Time']], not_TAG, color='red', label='Not Tagged')
                    p.set(title=cols[idx])
                    # p.grid()
                if dps > 1:
                    for ax in axs.flat:
                        ax.set(xlabel='Time')
                    for ax in fig.get_axes():
                        ax.label_outer()
                axs[0].legend(loc='upper left')
            plt.draw()

        elif not label and not subplot:
            for kk in range(nsubs):
                df = dfl[kk]
                dps = int(subi[kk]['dataPerSample'])
                cols = ['']
                if subi[kk]['sensorType'] in ('ACC', 'GYRO', 'MAG'):
                    cols = ['X', 'Y', 'Z']
                plt.figure()
                for k in range(dps):
                    plt.plot(df[['Time']], df.iloc[:, k + 1], color=np.random.rand(3, ), label=cols[k])
                plt.ylabel(subi[kk]['units'])
                plt.title(sensorName + '-' + subi[kk]['sensorType'])
                plt.xlabel('Time (s)')
                # plt.grid()
                if subi[kk]['sensorType'] in ('ACC', 'GYRO', 'MAG'):
                    plt.legend(loc='upper left')
            plt.draw()

        elif label and not subplot:
            for kk in range(nsubs):
                df = dfl[kk]
                dps = subi[kk]['dataPerSample']
                cols = ['']
                tags = ['Tagged', 'Not Tagged']
                if subi[kk]['sensorType'] in ('ACC', 'GYRO', 'MAG'):
                    cols = ['X', 'Y', 'Z']
                floor = min([min(df.iloc[:, k + 1]) for k in range(dps)])
                fig = plt.figure()
                ax = fig.add_subplot(111)
                for k in range(dps):
                    not_TAG = np.where(df[label], floor, df.iloc[:, k + 1])
                    TAG = np.where(df[label], df.iloc[:, k + 1], floor)
                    ptag, = ax.plot(df[['Time']], TAG, color=np.random.rand(3, ), label=cols[k])
                    ntag, = ax.plot(df[['Time']], not_TAG, color=np.random.rand(3, ))
                leg_tag = ax.legend([ptag, ntag], [label, 'not tagged'], loc='upper right')
                if subi[kk]['sensorType'] in ('ACC', 'GYRO', 'MAG'):
                    leg_col = ax.legend(loc='upper left')
                    ax.add_artist(leg_tag)
                plt.ylabel(subi[kk]['units'])
                plt.title(sensorName + '-' + subi[kk]['sensorType'])
                plt.xlabel('Time (s)')
                # plt.grid()
            plt.draw()
        else:  # not label and subplot
            for i in range(nsubs):
                df = dfl[i]
                cols = ['']
                if subi[i]['sensorType'] in ('ACC', 'GYRO', 'MAG'):
                    cols = ['X', 'Y', 'Z']
                dps = subi[i]['dataPerSample']
                fig, axs = plt.subplots(dps)
                if dps == 1:
                    axs = (axs,)
                tit = sensorName + '-' + subi[i]['sensorType']
                fig.suptitle(tit)
                for idx, p in enumerate(axs):
                    p.plot(df[['Time']], df.iloc[:, idx + 1], color=np.random.rand(3, ), label=cols[idx])
                    p.set(title=cols[idx])
                    # p.grid()
                if dps > 1:
                    for ax in axs.flat:
                        ax.set(xlabel='Time (s)')
                        ax.set(ylabel=subi[i]['units'])
                    for ax in fig.get_axes():
                        ax.label_outer()
                else:
                    axs[0].set(xlabel='Time (s)')
                    axs[0].set(ylabel=subi[i]['units'])
            plt.draw()
        if not multi:
            plt.show()

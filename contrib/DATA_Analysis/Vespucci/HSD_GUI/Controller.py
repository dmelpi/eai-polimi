from enum import Enum
import os
from threading import Thread, Event

from PySide6.QtCore import QObject, Signal
from HSD_GUI.PlotWidget import PlotWidget

# from HSD_utils import logger
# from HSD_link.HSDLink import HSDLink
from HSD_link.HSDLink import HSDLink
from HSD_link.communication.DTDL.device_template_manager import DeviceTemplateManager
from HSD_link.communication.DTDL.device_template_model import InterfaceElement

from .DataClass import PlotParams, RawDataClass, DataClass
from .DataReader import DataReader
# log = logger.setup_applevel_logger(is_debug = False, file_name= "app_debug.log")

class ComponentType(Enum):
    SENSOR = 0
    ALGORITHM = 1
    OTHER = 2
    NONE = -1

class Controller(QObject):
    
    # Signals
    sig_device_connected = Signal(bool)
    sig_device_template_loaded = Signal(bool)
    sig_logging = Signal(bool)
    
    sig_sensor_component_found = Signal(str, InterfaceElement)
    sig_algorithm_component_found = Signal(str, InterfaceElement)
    sig_component_found = Signal(str, InterfaceElement)

    sig_sensor_component_updated = Signal(str, dict)
    
    sig_component_removed = Signal(str)
    
    sig_component_updated = Signal(int, str, dict)
    
    # sig_plottable_component_added = Signal(str, dict)
    sig_dataReady = Signal(DataClass)
    sig_rawDataAvailable = Signal(RawDataClass)
    sig_plot_window_time_updated = Signal(float)

    class SensorAcquisitionThread(Thread):

        def __init__(self, event, hsd_link, data_reader, sig_rawDataAvailable, d_id, comp_name, sensor_data_file):            
            Thread.__init__(self)
            self.stopped = event
            self.hsd_link = hsd_link
            self.data_reader = data_reader
            self.sig_rawDataAvailable = sig_rawDataAvailable
            self.d_id = d_id
            self.comp_name = comp_name
            self.sensor_data_file = sensor_data_file
            self.data_counter = 0
            self.qt_app = None

        def run(self):
            while not self.stopped.wait(0.2):
            # while not self.stopped.wait(1): # TEST SLOW Update longer packets
                sensor_data = self.hsd_link.get_sensor_data(self.d_id, self.comp_name)
                if sensor_data is not None:
                    self.data_reader.feed_data(DataClass(self.comp_name, sensor_data[1]))
                    self.sensor_data_file.write(sensor_data[1])
                    #self.data_counter += sensor_data[0]
                    #print("{} Bytes written in {}\n".format(self.data_counter, self.sensor_data_file.name))

    def __init__(self, parent=None):
        super().__init__(parent)
        try:
            hsd_link_factory = HSDLink()
            self.hsd_link = hsd_link_factory.create_hsd_link()
        except Exception as err:
            print("Error: {}".format(err))
        self.device_id = 0 #default device id
        self.sensors_threads = []
        self.threads_stop_flags = []
        self.sensor_data_files = []
        
        self.api = None
        self.id_channels = [] # Store the list of different channel based on their ID (COMMAND + DATA)
        self.type_channels = [] # Store the list of channels based on their types
        self.dimension_channels = []

        self._must_check_value = 10 # Counter to know how many samples have to be tested to updated channel list

        self.data_readers = []
        self.components_status = dict() #From FW
        
        self.cconfig_widgets = dict() #{comp_name:CConfigWidget}
        self.c_cnt = 0

        self.plot_widgets = dict() #{comp_name:PlotWidget}
        self.p_cnt = 0
        self.p_ids = dict() #{plot_id:comp_name}
        
        self.__dt_manager = None

    def set_Qt_app(self, qt_app):
        self.qt_app = qt_app
    
    def get_device_list(self):
        devices = self.hsd_link.get_devices() 
        return devices
    
    def get_device_presentation_string(self, d_id = 0):
        return self.hsd_link.get_device_presentation_string(d_id)

    def get_device_info(self, d_id = 0):
        return self.hsd_link.get_device_info(d_id)

    def load_local_device_template(self, input_dt_file_path):
        self.hsd_link.set_device_template(input_dt_file_path)
        # One emit foreach Component (in this way it will be easy to catch every event in the MainWindow and popolate the UI)
        self.__dt_manager = DeviceTemplateManager(input_dt_file_path)
        components = self.__dt_manager.get_components()
        for comp_name in components.keys():
            if components[comp_name].id.split(":")[-2] == "sensors":
                self.sig_sensor_component_found.emit(comp_name, components[comp_name])
            elif components[comp_name].id.split(":")[-2] == "algorithms":
                self.sig_algorithm_component_found.emit(comp_name, components[comp_name])
            else:
                self.sig_component_found.emit(comp_name, components[comp_name])

    def is_sensor_enabled(self, comp_name, d_id = 0):
        return self.hsd_link.get_sensor_enable(d_id, comp_name)

    def get_plot_params(self, comp_name, comp_type):
        comp_status = self.hsd_link.get_component_status(self.device_id, comp_name)
        if comp_status is not None and comp_name in comp_status:
            if comp_type == "sensor":
                comp_status_value = comp_status[comp_name]
                enabled = comp_status_value["enable"]
                odr = comp_status_value["odr"]
                dimension = comp_status_value["dim"]
                return PlotParams(comp_name, enabled, odr, dimension)
        return None

    def fill_component_status(self, comp_name):
        comp_status = self.hsd_link.get_component_status(self.device_id, comp_name)
        if comp_status is not None and comp_name in comp_status:
            self.sig_component_updated.emit(self.c_cnt, comp_name, comp_status[comp_name])
            
        else:
            print("[WARNING] - The component [{}] defined in DeviceTemplate has not a Twin in Device Status from the FW".format(comp_name))
            self.sig_component_updated.emit(self.c_cnt, comp_name, None)
            self.c_cnt += 1
            self.remove_component_config_widget(comp_name)
            # self.sig_component_removed.emit(comp_name)

    def update_component_status(self, comp_name, comp_type = "other"):
        comp_status = self.hsd_link.get_component_status(self.device_id, comp_name)
        if comp_status is not None and comp_name in comp_status:
            if comp_type == "sensor":
                comp_status_value = comp_status[comp_name]
                self.components_status[comp_name] = comp_status_value
                enabled = comp_status_value["enable"]
                odr = comp_status_value["odr"]
                dimension = comp_status_value["dim"]
                plot_params = PlotParams(comp_name, enabled, odr, dimension)
                self.sig_sensor_component_updated.emit(comp_name, plot_params)

            self.sig_component_updated.emit(self.c_cnt, comp_name, comp_status[comp_name])
            self.c_cnt += 1
            
        else:
            print("[WARINING] - The component [{}] defined in DeviceTemplate has not a Twin in Device Status from the FW".format(comp_name))
            self.sig_component_updated.emit(self.c_cnt, comp_name, None)
            self.c_cnt += 1

    def start_log(self):
        res = self.hsd_link.start_log(self.device_id)
        # print("start: {}".format(res))
        if res:
            self.sig_logging.emit(True)

    def sample_size_from_data_type(self, data_type):
        if data_type == "uint8":
            return 1
        elif data_type == "int8":
            return 1
        elif data_type == "uint16":
            return 2
        elif data_type == "int16":
            return 2
        elif data_type == "uint32":
            return 4
        elif data_type == "int32":
            return 4
        elif data_type == "float":
            return 4
        
    def data_format_from_data_type(self, data_type):
        if data_type == "uint8":
            return 'B'
        elif data_type == "int8":
            return 'b'
        elif data_type == "uint16":
            return 'H'
        elif data_type == "int16":
            return 'h'
        elif data_type == "uint32":
            return 'I'
        elif data_type == "int32":
            return 'i'
        elif data_type == "float":
            return 'f'

    def start_plots(self):
         for s in self.plot_widgets:
            s_plot = self.plot_widgets[s]
            c_status = self.hsd_link.get_component_status(self.device_id, s_plot.comp_name)

            sensor_data_file_path = os.path.join(self.hsd_link.get_acquisition_folder(),(str(s_plot.comp_name) + ".dat"))
            sensor_data_file = open(sensor_data_file_path, "wb+")
            self.sensor_data_files.append(sensor_data_file)
            stopFlag = Event()
            self.threads_stop_flags.append(stopFlag)

            c_status_value = c_status[s_plot.comp_name]
            dimensions = c_status_value["dim"]
            sample_size = self.sample_size_from_data_type(c_status_value["data_type"])
            spts = c_status_value["samples_per_ts"]
            data_format = self.data_format_from_data_type(c_status_value["data_type"])

            dr = DataReader(self, s_plot.comp_name, spts, dimensions, sample_size, data_format)
            self.data_readers.append(dr)
            
            thread = self.SensorAcquisitionThread(stopFlag, self.hsd_link, dr, self.sig_rawDataAvailable, self.device_id, s_plot.comp_name, sensor_data_file)
            thread.start()
            self.sensors_threads.append(thread)

    def stop_log(self):
        self.sig_logging.emit(False)

        self.hsd_link.stop_log(self.device_id)
        self.hsd_link.save_json_device_status_file(self.device_id)
        self.hsd_link.save_json_acq_info_file(self.device_id)
    
    def stop_plots(self):
        for sf in self.threads_stop_flags:
            sf.set()
        for f in self.sensor_data_files:
            f.close()

    def plot_window_changed(self, plot_window_time):
        self.sig_plot_window_time_updated.emit(plot_window_time)

    def add_component_config_widget(self, cconfig_widget):
        self.cconfig_widgets[cconfig_widget.comp_name] = (cconfig_widget)

    def remove_component_config_widget(self, comp_name):
        self.cconfig_widgets.pop(comp_name)
    
    def add_plot_widget(self, plot_widget:PlotWidget):
        self.plot_widgets[plot_widget.comp_name] = (plot_widget)

    def remove_plot_widget(self, comp_name) -> PlotWidget:
        if comp_name in self.plot_widgets:
            return self.plot_widgets.pop(comp_name)
        else:
            print(comp_name, "is not in plot widget list yet")

    def add_data_to_a_plot(self, data:DataClass):
        self.plot_widgets[data.comp_name].add_data(data.data)

    def connectTo(self, device_id:int):  
        # log.debug("Connecting...") #TODO
        self.sig_device_connected.emit(True)
        self.device_id = device_id
        # log.debug("Connected") #TODO

    def disconnect(self):
        self.sig_device_connected.emit(False)
        # log.debug("Disconnected") #TODO

    def send_command(self, json_string):
        self.hsd_link.send_command(self.device_id, json_string)
        print(json_string)
    
    def get_device_status(self):
        return self.hsd_link.get_device(self.device_id)
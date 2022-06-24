from PySide6.QtWidgets import QFrame, QWidget
from PySide6.QtCore import Slot
from HSD_GUI.ComponentConfigWidget import ComponentConfigWidget
from HSD_GUI.LogControlWidget import LogControlWidget
from HSD_GUI.DataClass import PlotParams
from HSD_GUI.PlotWidget import PlotWidget

class DeviceConfigPage():
    def __init__(self, page_widget, controller):
        self.controller = controller
        
        self.controller.sig_sensor_component_found.connect(self.s_sensor_component_found)
        self.controller.sig_algorithm_component_found.connect(self.s_algorithm_component_found)
        self.controller.sig_component_found.connect(self.s_component_found)

        self.controller.sig_sensor_component_updated.connect(self.sig_sensor_component_updated)

        self.controller.sig_component_removed.connect(self.s_component_removed)

        self.page_widget = page_widget
        self.comp_widget_dict = dict()
        self.plot_widget_dict = dict()

        # layout_device_config
        self.main_layout = page_widget.findChild(QFrame, "frame_device_config")
        self.widget_log_control = page_widget.findChild(QWidget, "widget_log_control")

        # Log Control Widget (TODO it must be retrieved from the Device Template)
        log_control_widget = LogControlWidget(self.controller)
        self.widget_log_control.layout().addWidget(log_control_widget)
        
        self.device_config_widget = self.main_layout.findChild(QWidget,"widget_device_config")
        self.plots_widget = self.main_layout.findChild(QWidget,"widget_plots")

        self.threads_stop_flags = []
        self.sensor_data_files = []

    def remove_comp_widget(self, name):
        widget = self.comp_widget_dict[name]
        widget.setParent(None)
        self.device_config_widget.layout().removeWidget(widget)
        self.comp_widget_dict.pop(name)
    
    @Slot(int, str, dict)
    def s_component_found(self, comp_name, comp_interface):
        #create a ComponentConfigWidget
        comp_config_widget = ComponentConfigWidget(self.controller, comp_name, "", comp_interface.contents, self.device_config_widget)
        self.comp_widget_dict[comp_name] = comp_config_widget
        self.controller.add_component_config_widget(comp_config_widget)
        self.device_config_widget.layout().addWidget(comp_config_widget)
        self.controller.fill_component_status(comp_name)
    
    @Slot(int, str, dict)
    def s_sensor_component_found(self, comp_name, comp_interface):
        #create a ComponentConfigWidget
        sensor_config_widget = ComponentConfigWidget(self.controller, comp_name, "sensor", comp_interface.contents, self.device_config_widget)
        self.comp_widget_dict[comp_name] = sensor_config_widget
        self.controller.add_component_config_widget(sensor_config_widget)
        self.device_config_widget.layout().addWidget(sensor_config_widget)
        
        enabled = self.controller.is_sensor_enabled(comp_name)
        sensor_plot_params:PlotParams = self.controller.get_plot_params(comp_name, "sensor")
        if sensor_plot_params is not None:
            sensor_plot_widget = PlotWidget(self.controller, comp_name, sensor_plot_params.odr, 30, sensor_plot_params.dimension, sensor_config_widget, sensor_config_widget.c_id)
            self.plot_widget_dict[comp_name] = sensor_plot_widget
            self.controller.add_plot_widget(sensor_plot_widget)
            self.plots_widget.layout().addWidget(sensor_plot_widget)
            print("comp_name:",comp_name,"- status:", enabled)
            sensor_plot_widget.setVisible(enabled)
        
        self.controller.fill_component_status(comp_name)


    @Slot(int, str, dict)
    def s_algorithm_component_found(self, comp_name, comp_interface):
        print("Algorithm Components WIP!")

    @Slot(str)
    def s_component_removed(self, comp_name):
        self.remove_comp_widget(comp_name)

    
    @Slot(str, PlotParams)
    def sig_sensor_component_updated(self, comp_name, plot_params:PlotParams):
        enabled = plot_params.enabled
        odr = plot_params.odr
        dimension = plot_params.dimension

        self.plot_widget_dict[comp_name].update_plots_ui(odr, 30, dimension)
        # Debug log
        # print("comp_name:",comp_name,"- status:",enabled)
        self.plot_widget_dict[comp_name].setVisible(enabled)
    
from PySide6.QtCore import Slot
from PySide6.QtWidgets import QFileDialog, QFrame, QLineEdit, QVBoxLayout, QWidget, QPushButton
from PySide6.QtUiTools import QUiLoader
from PySide6.QtDesigner import QPyDesignerCustomWidgetCollection

class DeviceTemplateLoadingWidget(QWidget):
    def __init__(self, controller, parent=None):
        super().__init__(parent)
        self.controller = controller
        
        self.s_is_dt_loaded = False
        self.setWindowTitle("Device Template")

        QPyDesignerCustomWidgetCollection.registerCustomWidget(DeviceTemplateLoadingWidget, module="DeviceTemplateLoadingWidget")
        loader = QUiLoader()
        dt_load_widget = loader.load("HSD_GUI\GUI\device_template_load_widget.ui", parent)
        contents_widget = dt_load_widget.frame_dt_load.findChild(QFrame,"frame_contents")
        self.dt_value = contents_widget.findChild(QLineEdit,"lineEdit_dt_path_value")
        self.browse_dt_button = contents_widget.findChild(QPushButton,"pushButton_browse_in_file")
        self.browse_dt_button.clicked.connect(self.clicked_browse_dt_button)
        
        #Main layout
        main_layout = QVBoxLayout()
        self.setLayout(main_layout)
        main_layout.addWidget(dt_load_widget)

    @Slot()
    def clicked_browse_dt_button(self):
        json_filter = "JSON Device Template files (*.json *.JSON)"
        filepath = QFileDialog.getOpenFileName(filter=json_filter)
        self.input_file_path = filepath[0]
        self.dt_value.setText(self.input_file_path)
        self.controller.load_local_device_template(self.input_file_path)

    def saveSettings(self, settings):
        #  settings.setValue("logcontrolwidget/timewindow", self.time_spinbox.value())
        pass

    def restoreSettings(self, settings):
        pass
        #  try:
        #      self.time_spinbox.setValue(int(settings.value("logcontrolwidget/timewindow")))
        #  except:
        #      pass
    
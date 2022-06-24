from PySide6.QtCore import Slot
from PySide6.QtWidgets import QFrame, QSpinBox, QVBoxLayout, QWidget, QPushButton
from PySide6.QtUiTools import QUiLoader
from PySide6.QtDesigner import QPyDesignerCustomWidgetCollection

class LogControlWidget(QWidget):
    def __init__(self, controller, parent=None):
        super().__init__(parent)
        self.controller = controller
        self.controller.sig_logging.connect(self.s_is_logging)
        
        self.is_logging = False
        self.setWindowTitle("Log Control")

        QPyDesignerCustomWidgetCollection.registerCustomWidget(LogControlWidget, module="LogControlWidget")
        loader = QUiLoader()
        log_control_widget = loader.load("HSD_GUI\GUI\log_control_widget.ui", parent)
        contents_widget = log_control_widget.frame_connection.findChild(QFrame,"frame_contents")
        self.time_spinbox = contents_widget.findChild(QSpinBox,"spinBox_time_window")
        self.time_spinbox.valueChanged.connect(self.plot_window_time_change)
        self.time_spinbox.setValue(30)
        self.log_start_button = contents_widget.findChild(QPushButton,"start_button")
        self.log_start_button.clicked.connect(self.clicked_start_log_button)

        #Main layout
        main_layout = QVBoxLayout()
        self.setLayout(main_layout)
        main_layout.addWidget(log_control_widget)

    @Slot()
    def clicked_start_log_button(self):
        if not self.is_logging:
            self.controller.start_log()
        else:
            self.controller.stop_log()

    @Slot(bool)
    def s_is_logging(self, state:bool):
        if state:
            self.log_start_button.setText("Stop Log")
            self.is_logging = True
            self.controller.start_plots()
        else:
            self.log_start_button.setText("Start Log")
            self.is_logging = False
            self.controller.stop_plots()
    
    @Slot()
    def plot_window_time_change(self):
        self.controller.plot_window_changed(self.time_spinbox.value())


    def saveSettings(self, settings):
         settings.setValue("logcontrolwidget/timewindow", self.time_spinbox.value())

    def restoreSettings(self, settings):
         try:
             self.time_spinbox.setValue(int(settings.value("logcontrolwidget/timewindow")))
         except:
             pass
    
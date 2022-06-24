
import sys

from PySide6.QtWidgets import QMainWindow, QStackedWidget, QDockWidget, QPushButton, QVBoxLayout, QWidget
from PySide6.QtCore import Qt, Slot, QSettings
from HSD_GUI.DeviceConfigPage import DeviceConfigPage

from HSD_GUI.GUI.HSD_MainWindow import Ui_MainWindow

from HSD_GUI.styles import HSD_MenuButton
from .Controller import Controller
from .ConnectionWidget import ConnectionWidget
from .DeviceTemplateLoadingWidget import DeviceTemplateLoadingWidget

# import HSD_utils.logger as logger

# log = logger.setup_applevel_logger(is_debug = False, file_name= "app_debug.log")

class MainWindow(QMainWindow):
    
    def __init__(self, app, parent=None):
        super(MainWindow, self).__init__(parent)
        
        self.app = app        
        self.DEFAULT_TIME_WINDOW = 30

        self.controller = Controller(parent)
        self.controller.sig_device_connected.connect(self.s_device_connected)

        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)
        
        self.setWindowTitle("HSDatalog")

        # Find the widgets in the xml file
        # Main stacked widget (Application page manager)
        self.page_manager = self.findChild(QStackedWidget, "stacked_widget")
        
        # Connection page
        self.connection_page = self.findChild(QWidget, "page_connection")
        # self.connection_layout = self.findChild(QVBoxLayout, "page_connection_vlayout")
        self.connection_widget = ConnectionWidget(self.controller,self)
        self.connection_page.layout().addWidget(self.connection_widget)
        
        # Local Device Template Loading Widget
        self.dt_load_widget = DeviceTemplateLoadingWidget(self.controller)
        self.custom_dev_template_layout = QVBoxLayout()
        self.connection_page.layout().addLayout(self.custom_dev_template_layout)

        self.connection_page.layout().addStretch()
        
        # Device Components Configuration page
        configuration_widget = self.findChild(QWidget, "page_device_config") #change name in .ui file
        # for plots processEvent #TODO to be checked
        self.controller.set_Qt_app(self.app)
        self.device_conf_page = DeviceConfigPage(configuration_widget, self.controller) #TODO

        # Set the first displayed [Connection] Page
        self.page_manager.setCurrentWidget(self.connection_page)

        # Left Menu Items (page navigation menu)
        self.menu_btn_connection = self.findChild(QPushButton, "menu_btn_connection")
        self.menu_btn_connection.clicked.connect(self.clicked_menu_connection)
        self.menu_btn_device_conf = self.findChild(QPushButton, "menu_btn_device_conf")
        self.menu_btn_device_conf.clicked.connect(self.clicked_menu_device_conf)
        self.menu_btn_about = self.findChild(QPushButton, "menu_btn_about")
        self.menu_btn_about.clicked.connect(self.clicked_menu_about)

        # Hide Configuration menu button (it will be unhided when a device will be connected) 
        self.menu_btn_device_conf.setVisible(False)

        # Reload a presaved UI file, if any
        self.loadUI()        

    def quit(self):
        sys.exit(0)

    def addDockWidgetToUI(self, widget, name, title, position):
        position = None
        if position == 'top':
            position = Qt.TopDockWidgetArea
        elif position == 'bottom':
            position = Qt.BottomDockWidgetArea
        elif position == 'left':
            position = Qt.LeftDockWidgetArea
        elif position == 'right':
            position = Qt.RightDockWidgetArea
        else:
            position = Qt.LeftDockWidgetArea
                
        dock_widget = self.createDockWidgetFromWidget(widget)
        dock_widget.setObjectName(name)
        dock_widget.setWindowTitle(title)
        self.addDockWidget(position, dock_widget)

        self.widget_dict[name] = {
            'name':name,
            'widget': dock_widget,
            'position':position,
            'state':'displayed'
        }

    def clicked_menu_connection(self):
        self.page_manager.setCurrentWidget(self.connection_page)
        self.menu_btn_device_conf.setStyleSheet(HSD_MenuButton.unselected_device_conf_stylesheet)
    
    def clicked_menu_device_conf(self):
        self.page_manager.setCurrentWidget(self.device_conf_page.page_widget)
        self.menu_btn_device_conf.setStyleSheet(HSD_MenuButton.selected_device_conf_stylesheet)

    def clicked_menu_about(self):
        #TODO
        #dlg = AboutDialog(self)
        #dlg.exec_()
        print("ABOUT")
        pass

    def createDockWidgetFromWidget(self, child_widget):
        dockWidget = QDockWidget(self)
        dockWidget.setWidget(child_widget)
        return dockWidget

    def saveUI(self):
        settings = QSettings(".\\HSD_GUI\\HSD_GUI.ini", QSettings.IniFormat)
        settings.setValue("mainwindow/geometry", self.saveGeometry())
        settings.setValue("mainwindow/windowState", self.saveState())
        # self.log_control_widget.saveSettings(settings)

    def loadUI(self):
        settings = QSettings(".\\HSD_GUI\\HSD_GUI.ini", QSettings.IniFormat)
        self.restoreGeometry(settings.value("mainwindow/geometry"))
        self.restoreState(settings.value("mainwindow/windowState"))
        # self.log_control_widget.restoreSettings(settings)

    def closeEvent(self, event):
        self.saveUI()
        event.accept()

    @Slot(bool)
    def s_device_connected(self, status):
        if status:
            self.menu_btn_device_conf.setVisible(True)
            self.menu_btn_connection.setStyleSheet(HSD_MenuButton.connected_stylesheet)
            self.custom_dev_template_layout.addWidget(self.dt_load_widget)
        else:
            self.menu_btn_device_conf.setVisible(False)
            self.menu_btn_connection.setStyleSheet(HSD_MenuButton.unconnected_stylesheet)
            self.dt_load_widget.setParent(None)
            self.custom_dev_template_layout.removeWidget(self.dt_load_widget)
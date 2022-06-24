from PySide6.QtWidgets import QLineEdit, QVBoxLayout, QWidget, QLabel, QComboBox, QHBoxLayout, QPushButton, QFrame
from PySide6.QtCore import Slot
from PySide6.QtUiTools import QUiLoader
from PySide6.QtDesigner import QPyDesignerCustomWidgetCollection

class ConnectionWidget(QWidget):
    def __init__(self, controller, parent=None):
        super().__init__(parent)
        self.controller = controller
        self.controller.sig_device_connected.connect(self.s_isConnected)
        self.controller.sig_logging.connect(self.s_is_logging)
        
        self.is_connected = False
        self.setWindowTitle("Connection")
        
        QPyDesignerCustomWidgetCollection.registerCustomWidget(ConnectionWidget, module="ConnectionWidget")
        loader = QUiLoader()
        connection_widget = loader.load("HSD_GUI\GUI\connection_widget.ui", parent)
        contents_widget = connection_widget.frame_connection.findChild(QFrame,"frame_contents")
        self.COM_combo_box = contents_widget.findChild(QComboBox,"comboBox_COM_list")
        self.COM_connect_button = contents_widget.findChild(QPushButton,"pushButton_COM_connect")
        self.COM_connect_button.clicked.connect(self.clicked_COM_button)

        #Main layout
        main_layout = QVBoxLayout()
        self.setLayout(main_layout)
        main_layout.addWidget(connection_widget)

        self.presentation_widget = QWidget()
        presentation_layout = QVBoxLayout()
        self.presentation_widget.setLayout(presentation_layout)
        self.presentation_widget.setVisible(False)
        board_id_layout = QHBoxLayout()
        board_id_label = QLabel("Board Id: ")
        board_id_label.setFixedWidth(100)
        self.board_id_value = QLineEdit()
        self.board_id_value.setReadOnly(True)
        board_id_layout.addWidget(board_id_label)
        board_id_layout.addWidget(self.board_id_value)
        fw_id_layout = QHBoxLayout()
        fw_id_label = QLabel("FW Id: ")
        fw_id_label.setFixedWidth(100)
        self.fw_id_value = QLineEdit()
        self.fw_id_value.setReadOnly(True)
        fw_id_layout.addWidget(fw_id_label)
        fw_id_layout.addWidget(self.fw_id_value)
        presentation_layout.addLayout(board_id_layout)
        presentation_layout.addLayout(fw_id_layout)
        contents_widget.layout().addWidget(self.presentation_widget)

        self.fill_COM_combo_box()

    def fill_COM_combo_box(self):
        devices = self.controller.get_device_list()
        self.COM_connect_button.setEnabled(not devices == [])
        # TODO FIX THIS
        #for d in devices:
        #    print(d.device_info.alias)
        #    self.COM_combo_box.addItem("[" + d.device_info.part_number + "] - " + d.device_info.fw_name + "v" + d.device_info.fw_version)
        #self.COM_combo_box.setCurrentIndex(0)
        self.COM_combo_box.addItem("[ STWIN.BOX] - TEST_000")
        self.COM_combo_box.setCurrentIndex(0)

    @Slot()
    def clicked_COM_button(self):
        print(self.COM_combo_box.currentText())
        print(self.COM_combo_box.currentIndex())
        if self.is_connected:
            self.controller.disconnect()
            self.COM_connect_button.setText("Connect")
            self.presentation_widget.setVisible(False)
            self.board_id_value.setText("")
            self.fw_id_value.setText("")
        else:
            self.controller.connectTo(self.COM_combo_box.currentIndex())
            pres_res = self.controller.get_device_presentation_string(self.COM_combo_box.currentIndex())
            print(self.controller.get_device_info())
            self.presentation_widget.setVisible(True)
            self.board_id_value.setText(str(hex(pres_res["board_id"])))
            self.fw_id_value.setText(str(hex(pres_res["fw_id"])))
        

    @Slot(bool)
    def s_isConnected(self, state:bool):
        if state:
            self.COM_connect_button.setText("Disconnect")
            self.is_connected = True
        else:
            self.COM_connect_button.setText("Connect")
            self.is_connected = False

    @Slot(bool)
    def s_is_logging(self, state:bool):
        if state:
            self.setEnabled(False)
        else:
            self.setEnabled(True)            

    @Slot(str)
    def s_connectionTypeChanged(self, value):
        if value == "Serial":
            self.baudrate.setEnabled(True)
        else:
            self.baudrate.setEnabled(False)
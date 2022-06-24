from enum import Enum
import json
import re
from functools import partial

from PySide6.QtCore import Qt, Slot
from PySide6.QtGui import QValidator, QScreen
from PySide6.QtWidgets import QApplication, QFileDialog, QLabel, QRadioButton, QPushButton, QLineEdit, QVBoxLayout, QWidget, QHBoxLayout, QComboBox, QFrame, QGridLayout
from PySide6.QtGui import QDoubleValidator, QIntValidator
from PySide6.QtUiTools import QUiLoader
from PySide6.QtDesigner import QPyDesignerCustomWidgetCollection
from HSD_GUI.styles import HSD_EditLine
from HSD_GUI.Controller import ComponentType

# import device_model as DM

class PropertyTypeEnum(Enum):
    STRING = "string"
    INTEGER = "integer"
    DOUBLE = "double"
    BOOLEAN = "boolean"
    ENUM = "enum"
    OBJECT = "object"

class SubComponentConfigWidget(QWidget):
    def __init__(self, comp_name, object_name, prop_name, fields, is_writable, parent=None):
        super().__init__(parent)
        QPyDesignerCustomWidgetCollection.registerCustomWidget(SubComponentConfigWidget, module="SubComponentConfigWidget")
        loader = QUiLoader()
        self.comp_name = comp_name
        self.object_name = object_name
        comp_config_widget = loader.load("HSD_GUI\GUI\component_config_widget.ui", parent)
        title_frame = comp_config_widget.frame_component_config.findChild(QFrame,"frame_title")

        title_label = title_frame.findChild(QLabel,"label_title")
        title_label.setText(prop_name.upper())
        pushButton_show = title_frame.findChild(QPushButton, "pushButton_show")
        pushButton_show.setVisible(False)
        pushButton_pop_out = title_frame.findChild(QPushButton, "pushButton_pop_out")
        pushButton_pop_out.setVisible(False)
        self.contents_widget = comp_config_widget.frame_component_config.findChild(QFrame,"frame_contents")

        main_layout = QVBoxLayout()
        self.setLayout(main_layout)
        main_layout.addWidget(comp_config_widget)

        component_props_frame = QFrame()
        component_props_layout = QGridLayout()

        sub_widgets = []
        for i,f in enumerate(fields):
            schema_type = f.schema.value
            field_name = f.name
            field_dname = f.display_name if isinstance(f.display_name,str) else f.display_name.en
            sub_widget = PropertyWidget(comp_name, self.object_name, field_dname, "", schema_type, is_writable, field_name, parent)
            sub_widgets.append(sub_widget)
            component_props_layout.addWidget(sub_widget, i, 0)
        self.widget = MultiPropertyWidget(sub_widgets)

        component_props_frame.setLayout(component_props_layout)
        self.contents_widget.layout().addWidget(component_props_frame)


class PropertyWidget(QWidget):
    def __init__(self, comp_name, comp_sem_type, prop_name, label, value, prop_type, is_writable, field_name=None, parent=None):
        super().__init__(parent)
        self.prop_type = prop_type
        self.comp_name = comp_name
        self.comp_sem_type = comp_sem_type
        self.prop_name = prop_name
        self.field_name = field_name #for (SubProperties) DTDL Object fields
        self.label = QLabel(label)
        self.label.setFixedWidth(150)
        if self.prop_type == PropertyTypeEnum.STRING.value:
            self.value = QLineEdit(value)
        elif self.prop_type == PropertyTypeEnum.DOUBLE.value:
            self.validator = QDoubleValidator()
            self.value = QLineEdit(value)
            self.value.setValidator(self.validator)
        elif self.prop_type == PropertyTypeEnum.INTEGER.value:
            self.validator = QIntValidator(0,1000,self)
            self.value = QLineEdit(value)
            self.value.setValidator(self.validator)
        elif self.prop_type == PropertyTypeEnum.BOOLEAN.value:
            self.value = QRadioButton(value)
        elif self.prop_type == PropertyTypeEnum.ENUM.value:
            self.value = QComboBox()
            for v in value:
                self.value.addItem(v.display_name if isinstance(v.display_name,str) else v.display_name.en)
        elif self.prop_type == PropertyTypeEnum.OBJECT.value:
            self.value = SubComponentConfigWidget(comp_name, self.prop_name, label, value, is_writable, self)
        else:
            self.value = QLineEdit("UNKNOWN")
        
        self.setContentsMargins(20, 3, 0, 0)
        layout = QHBoxLayout(self)
        
        layout.setContentsMargins(20, 0, 20, 0)
        if(self.prop_type != PropertyTypeEnum.OBJECT.value):
            layout.addWidget(self.label)
        if not is_writable: #if writable is None or False --> property is read-only
            self.value.setEnabled(False)
        layout.addWidget(self.value)

class MultiPropertyWidget():
    def __init__(self, widget_list) -> None:
        self.sub_widgets = widget_list

class CommandField():
    def __init__(self, f_name, f_type, f_label, f_value):
        self.f_name = f_name
        self.f_type = f_type
        self.f_label = f_label
        self.f_value = f_value

class CommandWidget(QWidget):
    def __init__(self, controller, comp_name, request_name, command_name, command_fields, command_label=None, parent=None):
        super().__init__(parent)
        self.controller = controller
        self.comp_name = comp_name
        self.request_name = request_name
        self.command_name = command_name
        self.command_fields = command_fields
        self.values = dict()
        self.labels = dict()

        QPyDesignerCustomWidgetCollection.registerCustomWidget(CommandWidget, module="CommandWidget")
        loader = QUiLoader()
        command_widget = loader.load("HSD_GUI\GUI\send_command_widget.ui", parent)
        command_title_frame = command_widget.frame_component_config.findChild(QFrame,"frame_title")
        command_fields_widget = command_widget.frame_component_config.findChild(QFrame,"frame_contents")
        command_send_button = command_widget.findChild(QPushButton,"pushButton")
        command_send_button.clicked.connect(partial(self.clicked_send_command_button, self))

        if command_label is not None:
            command_title_label = command_title_frame.findChild(QLabel,"label_title")
            command_title_label.setText(command_label)

        command_fields_frame = QFrame()
        for f in self.command_fields:
            self.labels[f.f_name] = QLabel(f.f_label)
            self.labels[f.f_name].setFixedWidth(150)
            if f.f_type == PropertyTypeEnum.STRING.value:
                self.values[f.f_name] = QLineEdit(f.f_value)
            elif f.f_type == PropertyTypeEnum.DOUBLE.value:
                self.validator = QDoubleValidator()
                self.values[f.f_name] = QLineEdit(f.f_value)
                self.values[f.f_name].setValidator(self.validator)
            elif f.f_type == PropertyTypeEnum.INTEGER.value:
                self.validator = QIntValidator(0,1000,self)
                self.values[f.f_name] = QLineEdit(f.f_value)
                self.values[f.f_name].setValidator(self.validator)
            elif f.f_type == PropertyTypeEnum.BOOLEAN.value:
                self.values[f.f_name] = QRadioButton(f.f_value)
            elif f.f_type == PropertyTypeEnum.ENUM.value:
                self.values[f.f_name] = QComboBox()
                for v in f.f_value:
                    self.values[f.f_name].addItem(v.display_name if isinstance(v.display_name,str) else v.display_name.en)
            else:
                self.values[f.f_name] = QLineEdit("UNKNOWN")

        self.values[f.f_name].setFixedWidth(200)

        layout = QVBoxLayout(self)    
        self.setContentsMargins(20, 3, 0, 0)
    
        for l in self.labels:
            in_layout = QHBoxLayout()
            in_layout.addWidget(self.labels[l])
            in_layout.addWidget(self.values[l])
            layout.addLayout(in_layout)

        command_fields_frame.setLayout(layout)

        command_fields_widget.layout().addWidget(command_fields_frame)
        main_layout = QVBoxLayout()
        self.setLayout(main_layout)
        main_layout.addWidget(command_widget)
    
    def clicked_send_command_button(self, widget):
        message_fields = dict()
        for f in widget.command_fields:
            if f.f_type == PropertyTypeEnum.STRING.value:
                message_fields[f.f_name] = widget.values[f.f_name].text()
            elif f.f_type == PropertyTypeEnum.DOUBLE.value:
                message_fields[f.f_name] = float(widget.values[f.f_name].text())
            elif  f.f_type == PropertyTypeEnum.INTEGER.value:
                message_fields[f.f_name] = int(widget.values[f.f_name].text())
            elif f.f_type == PropertyTypeEnum.BOOLEAN.value:
                message_fields[f.f_name] = widget.values[f.f_name].isChecked()
            elif f.f_type == PropertyTypeEnum.ENUM.value:
                if type(f.f_value[0].enum_value) == int:
                    message_fields[f.f_name] = widget.values[f.f_name].currentIndex()#float(re.sub("[^0123456789\.]","", widget.values[f.f_name].itemText(widget.values[f.f_name].currentIndex())))
                else:
                    message_fields[f.f_name] = widget.values[f.f_name].itemText(widget.values[f.f_name].currentIndex())

            
        print(message_fields)
        message = {
            widget.comp_name + '*' + widget.request_name: {
              self.command_name : message_fields
            }
        }
        json_string = json.dumps(message)
        self.controller.send_command(json_string)

class ComponentConfigWidget(QWidget):
    def __init__(self, controller, comp_name, comp_sem_type, comp_contents, parent=None, c_id = 0):
        super().__init__(parent)
        self.parent = parent
        self.controller = controller
        self.controller.sig_component_updated.connect(self.s_component_updated)
        
        self.is_docked = True
        self.is_packed = True

        self.c_id = c_id
        self.comp_name = comp_name
        self.comp_sem_type = comp_sem_type
        self.comp_contents = comp_contents

        self.setWindowTitle("Connection")
        
        QPyDesignerCustomWidgetCollection.registerCustomWidget(ComponentConfigWidget, module="ComponentConfigWidget")
        loader = QUiLoader()
        comp_config_widget = loader.load("HSD_GUI\GUI\component_config_widget.ui", parent)
        title_frame = comp_config_widget.frame_component_config.findChild(QFrame,"frame_title")
        title_label = title_frame.findChild(QLabel,"label_title")
        title_label.setText(comp_name.upper())
        pushButton_show = title_frame.findChild(QPushButton, "pushButton_show")
        pushButton_show.clicked.connect(self.clicked_show_button)
        pushButton_pop_out = title_frame.findChild(QPushButton, "pushButton_pop_out")
        pushButton_pop_out.clicked.connect(self.clicked_pop_out_button)
        self.radioButton_enable = title_frame.findChild(QRadioButton, "radioButton_enable")
        self.radioButton_enable.setVisible(False)
        self.contents_widget = comp_config_widget.frame_component_config.findChild(QFrame,"frame_contents")
        self.contents_widget.setVisible(False)

        #Main layout
        main_layout = QVBoxLayout()
        self.setLayout(main_layout)
        main_layout.addWidget(comp_config_widget)

        self.property_widgets = dict()
        self.command_widgets = dict()

        #Frame Properties
        component_props_frame = QFrame()
        component_props_layout = QGridLayout()
        component_props_layout.setVerticalSpacing(6)
        for i, p in enumerate(self.comp_contents):
            pc_display_name = p.display_name if isinstance(p.display_name, str) else p.display_name.en
            if p.type.name == 'PROPERTY':
                try: #complex object schema
                    if p.schema.type.value == "Enum":
                        schema_type = p.schema.type.value.lower()
                        widget = PropertyWidget(comp_name, comp_sem_type, p.name, pc_display_name, p.schema.enum_values, p.schema.type.value.lower(), p.writable)
                    elif p.schema.type.value == "Object":
                        schema_type = p.schema.type.value.lower()
                        widget = PropertyWidget(comp_name, comp_sem_type, p.name, pc_display_name, p.schema.fields, p.schema.type.value.lower(), p.writable)
                except AttributeError: #primitive type schema
                    schema_type = p.schema
                    widget = PropertyWidget(comp_name, comp_sem_type, p.name, pc_display_name , "", p.schema, p.writable)

                self.assign_callbacks(widget, schema_type, p.schema.value_schema if schema_type == PropertyTypeEnum.ENUM.value else None)
                
                if (comp_sem_type == "sensor" or comp_sem_type == "algorithm") and p.name == "enable":
                    self.radioButton_enable.setVisible(True)
                    self.radioButton_enable.toggled.connect(partial(self.sensor_component_enabled, widget))

                component_props_layout.addWidget(widget, i,0)
                #add widget to the Property widget dictionary
                self.property_widgets[p.name] = widget
            
            elif p.type.name == 'COMMAND':
                fields = []
                try: #complex object schema
                    if p.request.schema.type.value == "Object":
                        if "fields" in p.request.schema.to_dict(): #more than one field in command
                            for f in p.request.schema.fields:
                                field_label = f.display_name if isinstance(f.display_name, str) else f.display_name.en
                                if f.schema is not None:
                                    fields.append(CommandField(f.name, f.schema, field_label, ""))
                                else:
                                    try:
                                        schema_type = f.dtmi_dtdl_property_schema_2.type.value.lower()
                                        fields.append(CommandField(f.name, schema_type, field_label, f.dtmi_dtdl_property_schema_2.enum_values))
                                    except AttributeError:
                                        print("ERROR - malformed commmand field: {}".format(field_label))
                            widget = CommandWidget(self.controller, comp_name, p.name, p.request.name, fields, pc_display_name)
                        else:
                            fields.append(CommandField(p.request.name, p.request.schema, pc_display_name, ""))
                            widget = CommandWidget(self.controller, comp_name, p.name, None, fields, pc_display_name)
                    else:
                        print("HERE!")
                except AttributeError:
                    fields.append(CommandField(p.request.name, p.request.schema, pc_display_name, ""))
                    widget = CommandWidget(self.controller, comp_name, p.name, None, fields, pc_display_name)

                component_props_layout.addWidget(widget, i,0)
                #add widget to the Property widget dictionary
                self.command_widgets[p.name] = widget               

        component_props_frame.setLayout(component_props_layout)
        component_props_frame.setFixedHeight(component_props_layout.sizeHint().height()+60)
        self.contents_widget.layout().addWidget(component_props_frame)

    def assign_callbacks(self, widget, schema_type, schema_value=None):
        if schema_type == PropertyTypeEnum.STRING.value:
            widget.value.editingFinished.connect(partial(self.send_string_command, widget))
        elif schema_type == PropertyTypeEnum.DOUBLE.value:
            widget.value.textChanged.connect(partial(self.validate_value, widget))
            widget.value.editingFinished.connect(partial(self.send_double_command, widget))
        elif  schema_type == PropertyTypeEnum.INTEGER.value:
            widget.value.textChanged.connect(partial(self.validate_value, widget))
            widget.value.editingFinished.connect(partial(self.send_int_command, widget))
        elif schema_type == PropertyTypeEnum.BOOLEAN.value:
            widget.value.toggled.connect(partial(self.boolean_prop_triggered, widget))
        elif schema_type == PropertyTypeEnum.ENUM.value:
            if schema_value == 'integer':
                widget.value.activated.connect(partial(self.send_enum_number_command, widget))
            if schema_value == 'string':
                widget.value.activated.connect(partial(self.send_enum_string_command, widget))
        elif schema_type == PropertyTypeEnum.OBJECT.value:
            for w in widget.value.widget.sub_widgets:
                self.assign_callbacks(w, w.prop_type)

    @Slot()
    def clicked_browse_dt_button(self):
        json_filter = "JSON Device Template files (*.json *.JSON)"
        filepath = QFileDialog.getOpenFileName(filter=json_filter)
        self.input_file_path = filepath[0]
        self.dt_value.setText(self.input_file_path)
        self.controller.load_local_device_template(self.input_file_path)

    @Slot()
    def clicked_show_button(self):
        if self.is_packed:
            self.unpack_contents_widget()
            self.is_packed = False
        else:
            self.pack_contents_widget()
            self.is_packed = True
    
    @Slot()
    def clicked_pop_out_button(self):
        if self.is_docked:
            self.pop_out_widget()
            self.is_docked = False
        else:
            self.pop_in_widget()
            self.is_docked = True

    @Slot(int, str, dict)
    def s_component_updated(self, comp_id: str, comp_name: str, comp_status: dict):
        if comp_name == self.comp_name:
            print("Component:", comp_name)
            self.c_id = comp_id
            if comp_status is not None:
                if "c_type" in comp_status:
                    comp_sem_type = ComponentType(comp_status["c_type"])
                else:
                    comp_sem_type = ComponentType(-1)
                # self.update_property_widget(comp_name, comp_sem_type, cont_name, cont_value)
                for cont_name, cont_value in comp_status.items():
                    if type(cont_value) is dict:
                        print("Content:", cont_name)
                        for key in cont_value:
                            print('- ' + key + ':', cont_value[key])
                            self.update_property_widget(comp_name, comp_sem_type, cont_name, cont_value[key])
                    else:
                        print('- {}: {}'.format(cont_name, cont_value))
                        self.update_property_widget(comp_name, comp_sem_type, cont_name, cont_value)
                print("{} UPDATED!".format(comp_name))
            else:
                print("No status to update!")
    
    def update_property_widget(self, comp_name, comp_sem_type, prop_name, prop_value):
        if prop_name in self.property_widgets.keys():
            w = self.property_widgets[prop_name]
            if type(w) is PropertyWidget:
                if {w.comp_name, w.prop_name} == {comp_name, prop_name}:
                    if w.prop_type == PropertyTypeEnum.STRING.value:
                        w.value.setText(prop_value)
                    elif w.prop_type == PropertyTypeEnum.INTEGER.value or w.prop_type == PropertyTypeEnum.DOUBLE.value:
                        w.value.setText(str(prop_value))
                    elif w.prop_type == PropertyTypeEnum.ENUM.value:
                        itemsTexts = [w.value.itemText(i) for i in range(w.value.count())]
                        index = 0
                        if len(itemsTexts) > 1:
                            index = 0
                            for idx, s in enumerate(itemsTexts):
                                if str(prop_value) in s:
                                    index = idx
                                    break
                        w.value.setCurrentIndex(index)
                    elif w.prop_type == PropertyTypeEnum.BOOLEAN.value:
                        w.value.blockSignals(True)
                        w.value.setChecked(prop_value)
                        w.value.blockSignals(False)
                        if prop_name == "enable" and (comp_sem_type == ComponentType.SENSOR or comp_sem_type == ComponentType.ALGORITHM):
                            self.radioButton_enable.blockSignals(True)
                            self.radioButton_enable.setChecked(prop_value)
                            self.radioButton_enable.blockSignals(False)
                    else:
                        print("[WARNING] - Unrecognized Property Type")
        else:
            print("key {} in status is not defined in your Device Template".format(prop_name))

    def send_string_command(self, widget: PropertyWidget):
        message = {
            widget.comp_name: {
                widget.prop_name: widget.value.text() if widget.field_name is None else { widget.field_name: widget.value.text() }
            }
        }
        json_string = json.dumps(message)
        self.controller.send_command(json_string)
    
    def send_int_command(self, widget: PropertyWidget):
        message = {
            widget.comp_name: {
                widget.prop_name: int(widget.value.text()) if widget.field_name is None else { widget.field_name: int(widget.value.text()) }
            }
        }
        json_string = json.dumps(message)
        self.controller.send_command(json_string)

    def validate_value(self, widget, text_value):
        print("res: {}".format(widget.validator.validate(text_value,0)[0]))
        if widget.validator.validate(text_value,0)[0] == QValidator.State.Acceptable:
            widget.value.setStyleSheet(HSD_EditLine.valid)
        else:
            widget.value.setStyleSheet(HSD_EditLine.invalid)
    
    def send_double_command(self, widget: PropertyWidget):
        message = {
            widget.comp_name: {
                widget.prop_name: float(widget.value.text()) if widget.field_name is None else { widget.field_name: float(widget.value.text()) }
            }
        }
        json_string = json.dumps(message)
        self.controller.send_command(json_string)

    def sensor_component_enabled(self, widget: PropertyWidget, status):
        widget.value.setChecked(status)

    def boolean_prop_triggered(self, widget: PropertyWidget, status):
        self.send_bool_command(widget, status)
        self.controller.update_component_status(widget.comp_name, widget.comp_sem_type)
        if widget.prop_name == "enable" and self.radioButton_enable.isVisible():
            self.radioButton_enable.blockSignals(True)
            self.radioButton_enable.setChecked(status);
            self.radioButton_enable.blockSignals(False)


    def send_bool_command(self, widget: PropertyWidget, status):
        message = {
            widget.comp_name: {
                widget.prop_name: status if widget.field_name is None else { widget.field_name: status }
            }
        }
        json_string = json.dumps(message)
        self.controller.send_command(json_string)

    def send_enum_number_command(self, widget: PropertyWidget, index):
        message = {
            widget.comp_name: {
                widget.prop_name: float(re.sub("[^0123456789\.]","",widget.value.itemText(index)))
            }
        }
        json_string = json.dumps(message)
        self.controller.send_command(json_string)
    
    def send_enum_string_command(self, widget: PropertyWidget, index):
        message = {
            widget.comp_name: {
                widget.prop_name: widget.value.itemText(index)
            }
        }
        json_string = json.dumps(message)
        self.controller.send_command(json_string)
    
    def closeEvent(self, event):
        self.pop_in_widget()
        self.is_docked = True

    def pop_out_widget(self):
        self.setWindowFlags(Qt.Dialog)
        center = QScreen.availableGeometry(QApplication.primaryScreen()).center()
        geo = self.frameGeometry()
        geo.moveCenter(center)
        self.move(geo.topLeft())
        self.show()

    def pop_in_widget(self):
        self.setWindowFlags(Qt.Widget)
        self.parent.layout().insertWidget(self.c_id, self)

    def unpack_contents_widget(self):
        self.contents_widget.setVisible(True)

    def pack_contents_widget(self):
        self.contents_widget.setVisible(False)

    def saveSettings(self, settings):
        #  settings.setValue("logcontrolwidget/timewindow", self.time_spinbox.value())
        pass

    def restoreSettings(self, settings):
        pass
        #  try:
        #      self.time_spinbox.setValue(int(settings.value("logcontrolwidget/timewindow")))
        #  except:
        #      pass
    
class HSD_MenuButton():

    connected_stylesheet = '''
            QPushButton {	
            background-image: url(:/icons/icons/outline_settings_input_com_white_18dp.png);
            background-position: center;
            background-repeat: no-repeat;
            border: none;
            border-right: 5px solid rgb(32, 133, 25);
            background-color: rgb(27, 29, 35);
            text-align: left;
        }
        QPushButton:hover {
            background-color: rgb(33, 37, 43);
        }
        QPushButton:pressed {	
            background-color: rgb(32, 178, 170);
        }
        '''
    
    unconnected_stylesheet = '''
        QPushButton {	
        background-image: url(:/icons/icons/outline_settings_input_com_white_18dp.png);
        background-position: center;
        background-repeat: no-repeat;
        border: none;
        border-right: 5px solid rgb(134, 26, 34);
        background-color: rgb(27, 29, 35);
        text-align: left;
        }
        QPushButton:hover {
            background-color: rgb(33, 37, 43);
        }
        QPushButton:pressed {	
            background-color: rgb(32, 178, 170);
        }
        '''
    
    selected_device_conf_stylesheet = '''
            QPushButton {	
            background-image: url(:/icons/icons/baseline_tune_white_18dp.png);
            background-position: center;
            background-repeat: no-repeat;
            border: none;
            border-right: 5px solid rgb(44, 49, 60);
            background-color: rgb(27, 29, 35);
            text-align: left;
        }
        QPushButton:hover {
            background-color: rgb(33, 37, 43);
        }
        QPushButton:pressed {	
            background-color: rgb(32, 178, 170);
        }
        '''
    
    unselected_device_conf_stylesheet = '''
        QPushButton {	
        background-image: url(:/icons/icons/baseline_tune_white_18dp.png);
        background-position: center;
        background-repeat: no-repeat;
        border: none;
        background-color: rgb(27, 29, 35);
        text-align: left;
        }
        QPushButton:hover {
            background-color: rgb(33, 37, 43);
        }
        QPushButton:pressed {	
            background-color: rgb(32, 178, 170);
        }
        '''

class HSD_EditLine():

    valid = '''
        QLineEdit {
            border: 2px solid rgb(27, 29, 35);
        }
        QLineEdit:hover {
            border: 2px solid rgb(64, 71, 88);
        }
        QLineEdit:focus {
            border: 2px solid rgb(91, 101, 124);
        }
        QLineEdit:disabled {
            border: 2px solid rgb(32, 32, 32);
        }
    '''

    invalid = '''
        QLineEdit {
            border: 2px solid rgb(128, 30, 30);
        }
        QLineEdit:hover {
            border: 2px solid rgb(148, 30, 30);
        }
        QLineEdit:focus {
            border: 2px solid rgb(168, 30, 30);
        }
        QLineEdit:disabled {
            border: 2px solid rgb(32, 32, 32);
        }
    '''
#
#                   Copyright (c) 2020 STMicroelectronics.
#                             All rights reserved
#
#   This software component is licensed by ST under BSD-3-Clause license,
#   the "License"; You may not use this file except in compliance with the
#   License. You may obtain a copy of the License at:
#                        https://opensource.org/licenses/BSD-3-Clause

class TypeConversion:

    @staticmethod
    def check_type(check_type):
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

    @staticmethod
    def check_type_length(check_type):
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

# *****************************************************************************
#  * @file    logger.py
#  * @author  SRA
#  * @version 1.3.0
#  * @date    5-Nov-2021
# *****************************************************************************
#                   Copyright (c) 2020 STMicroelectronics.
#                             All rights reserved
#
#   This software component is licensed by ST under BSD-3-Clause license,
#   the "License"; You may not use this file except in compliance with the
#   License. You may obtain a copy of the License at:
#                https://opensource.org/licenses/BSD-3-Clause

import logging
import sys

LOGGER_NAME = "HSDatalogApp"

class CustomFormatter(logging.Formatter):

    grey = "\x1b[38;21m"
    yellow = "\x1b[33;21m"
    red = "\x1b[31;21m"
    bold_red = "\x1b[31;1m"
    reset = "\x1b[0m"
    format = "%(asctime)s - %(name)s - %(levelname)s - %(message)s (%(filename)s:%(lineno)d)"

    FORMATS = {
        logging.DEBUG: grey + format + reset,
        logging.INFO: grey + format + reset,
        logging.WARNING: yellow + format + reset,
        logging.ERROR: red + format + reset,
        logging.CRITICAL: bold_red + format + reset
    }

    def format(self, record):
        log_fmt = self.FORMATS.get(record.levelno)
        formatter = logging.Formatter(log_fmt)
        return formatter.format(record)

def setup_applevel_logger( is_debug=True, 
                           file_name=None,
                           stream_handler=sys.stdout):
    logger = logging.getLogger(LOGGER_NAME)
    logger.setLevel(logging.DEBUG if is_debug else logging.INFO)

    formatter = CustomFormatter()

    sh = logging.StreamHandler(stream_handler)
    sh.setFormatter(formatter)
    logger.handlers.clear()
    logger.addHandler(sh)

    if file_name:
        fh = logging.FileHandler(file_name)
        fh.setFormatter(formatter)
        logger.addHandler(fh)

    return logger


def get_logger(module_name):
    return logging.getLogger(LOGGER_NAME).getChild(module_name)
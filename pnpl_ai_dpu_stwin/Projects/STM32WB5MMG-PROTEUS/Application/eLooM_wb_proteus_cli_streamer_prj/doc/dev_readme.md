## Components

### SensorManager

- Requires
    - ai_data_format
    - mx support (SPI, DMA, GPIO, TIM)
- Provides at app level
    - SMPinConfig.h
- Contributes at app level
    - App error
    - MX: contribution to stm32wbxx_it.c, SystemClock_Config()

### SensorStreamerCLI

- Requires
    - App Messages
    - (?? ai_data_format ?? Is it used directly by the AppController?)
    - SensorManager
    - FreeRTOSCLI
    - ST USB Library
    - mx support (GPIO, TIM)
- Provides at app level
- Contributes at app level
    - App error
    - App Messages (utilMessage, ctrlMessage)
    - MX: contribution to stm32wbxx_it.c

## Other SW Unit

### ST USB Library

- Requires
    - App Messages (app_messages_parser.h/c)
- Provides at app level
    - usb_device.h/c
    - usb_cdc_if.h/c
    - usb_conf.h/c
    - usb_desc.h/c
- Contributes at app level
    - MX: contribution to stm32wbxx_it.c, SystemClock_Config()

### FreeRTOSCLI

- Provides at components level
    - FreeRTOSCLI.h/c

### App Message

- Provide at app level
    - app_messages_parser.h/c

### ai_data_format

- Provide at app level
    - ai_logging.h/c
    - ai_sp_dataformat.h
    - features_extraction_if.h

## Known limitations and issues

### Hard Fault from USB IRQ on PROTEUS board

- How to reproduce: enable ISM330DHCX ACC with ODR set to 208 Hz, and "start streamer".

### AppController::StreamData(AppController_t *_this, CBItem *p_item) format issue

The method to display the raw data in the console doesn't take into account the shape of the data.
This means that it works only with a 3 axis data.

### Doxygen documentation is a placeholder

In most projects (maybe all) the doxygen is only a copy of the FP-AI-MONITOR1 to provide the documentation structure.
The documentation for the demo project must be done.

## Note

### Timebase IRQ callback

Must be moved in the file stm32xxxx_hal_timebase_tim.c (CubeMX generate the callback in the main)

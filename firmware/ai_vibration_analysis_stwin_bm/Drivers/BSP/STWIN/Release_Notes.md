---
pagetitle: Release Notes for STEVAL-STWINKT1B Board Drivers 
lang: en
---

::: {.row}
::: {.col-sm-12 .col-lg-4}

::: {.card .fluid}

::: {.sectione .dark}
<center>
# <small>Release Notes for</small> **STEVAL-STWINKT1B Board Drivers**
Copyright &copy; 2020  STMicroelectronics\
    
[![ST logo](_htmresc/st_logo.png)](https://www.st.com){.logo}
</center>
:::
:::

# License

Licensed by ST under BSD 3-Clause license (the "License"). You may not use this package except in compliance with the License. You may obtain a copy of the License at: [BSD-3-Clause](https://opensource.org/licenses/BSD-3-Clause)

# Purpose

The BSP (Board Specific Package) STEVAL-STWINKT1B Board Drivers are based on the HAL drivers and provide a set of high level APIs relative to the hardware components and features in the evaluation board for a given STM32 processor

The BSP drivers allow a quick access to the boards’ services using high level APIs and without any specific configuration as the link with the HAL and the external components is done in intrinsic within the drivers. 

Here is the list of references to user documents:

- [UM2621](https://www.st.com/resource/en/user_manual/dm00643532.pdf) : How to use the software package for the SensorTile Wireless Industrial Node based on STM32Cube
- [STM32Cube](https://www.st.com/stm32cube) : STM32Cube
:::

::: {.col-sm-12 .col-lg-8}
# Update History

::: {.collapse}
<input type="checkbox" id="collapse-section12" checked aria-hidden="true">
<label for="collapse-section12" aria-hidden="true">__v1.4.4 / 18-Dec-2020__</label>
<div>			

## Main Changes

### Patch release

- Updated STWIN_conf_template.h
- Typo solved in BSP_SD_Detect_Init

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section11" checked aria-hidden="true">
<label for="collapse-section11" aria-hidden="true">__v1.4.3 / 02-Dec-2020__</label>
<div>			

## Main Changes

### Patch release

- Bug fixed in BC_CmdMng function in STWIN_bc
- Minor in STWIN_wifi

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section10" checked aria-hidden="true">
<label for="collapse-section10" aria-hidden="true">__v1.4.2 / 13-Nov-2020__</label>
<div>			

## Main Changes

### Patch release

- Support for ISM330DHCX: added event-related functions and Set_INT1_FIFO_Full
- Solved some MISRA compliancy issues

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section9" checked aria-hidden="true">
<label for="collapse-section9" aria-hidden="true">__v1.4.1 / 09-Oct-2020__</label>
<div>			

## Main Changes

### Patch release

- Warning solved for I2C3 in STWIN_bus when USE_HAL_I2C_REGISTER_CALLBACKS is 1
- Enabled ISM330DHCX INT2
- Made STBC02 Timer Interrupt Priority available in STWIN_conf

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section8" checked aria-hidden="true">
<label for="collapse-section8" aria-hidden="true">__v1.4.0 / 11-Sep-2020__</label>
<div>			

## Main Changes

### Maintenance release and product update

- Added support for I2C3 in STWIN_bus
- Updated STWIN and STWIN_bc to avoid including "arm_math.h"
- Minor in hci_tl_interface_template

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section7" aria-hidden="true">
<label for="collapse-section7" aria-hidden="true">__v1.3.2 / 19-Jun-2020__</label>
<div>			

## Main Changes

### Patch release

- Added Chip Select Init all function in STWIN.c 

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section6" aria-hidden="true">
<label for="collapse-section6" aria-hidden="true">__v1.3.1 / 03-Apr-2020__</label>
<div>			

## Main Changes

### Maintenance and patch release

- Solved MISRA compliancy issues in STWIN_audio and STWIN_bc
- General code cleaning

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section5" aria-hidden="true">
<label for="collapse-section5" aria-hidden="true">__v1.3.0 / 17-Jan-2020__</label>
<div>			

## Main Changes

### Maintenance release and product update

- Issue solved in hci_tl_interface_template.c
- Added new defines for POWER_BUTTON and code cleaning in STWIN.c/.h
- General code cleaning in STWIN_audio.c
- Added support for IIS3DWB and ISM330DHCX in some FIFO related functions in STWIN_motion_sensor_ex.c

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section4" aria-hidden="true">
<label for="collapse-section4" aria-hidden="true">__v1.2.1 / 21-Nov-2019__</label>
<div>			

## Main Changes

### Patch release

-  Bug fixed in STWIN_bus

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section3"  aria-hidden="true">
<label for="collapse-section3" aria-hidden="true">__v1.2.0 / 31-Oct-2019__</label>
<div>			

## Main Changes

### Maintenance release

-  Bug fixed in STWIN_bc, STWIN_bus and hci_tl_interface_template
-  Added STWIN_debug_pins to easily use STMOD+ connector as debug pins

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section2"  aria-hidden="true">
<label for="collapse-section2" aria-hidden="true">__v1.1.0 / 12-Sep-2019__</label>
<div>			

## Main Changes

### Maintenance release

-  Updated STWIN_audio and STWIN_conf_template

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section1"  aria-hidden="true">
<label for="collapse-section1" aria-hidden="true">__v1.0.0 / 05-Jul-2019__</label>
<div>			

## Main Changes

### First official release

- First official release

</div>
:::

:::
:::

<footer class="sticky">
For complete documentation on **STEVAL-STWINKT1B** ,
visit: [www.st.com](https://www.st.com/content/st_com/en/products/evaluation-tools/solution-evaluation-tools/sensor-solution-eval-boards/steval-stwinkt1b.html)

This release note uses up to date web standards and, for this reason, should not
be opened with Internet Explorer but preferably with popular browsers such as
Google Chrome, Mozilla Firefox, Opera or Microsoft Edge.
</footer>

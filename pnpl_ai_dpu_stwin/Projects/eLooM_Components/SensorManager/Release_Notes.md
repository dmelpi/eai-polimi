---
pagetitle: Release Notes for Sensor Manager FW module
lang: en
header-includes: <link rel="icon" type="image/x-icon" href="_htmresc/favicon.png" />
---

::: {.row}
::: {.col-sm-12 .col-lg-4}

<center>
# Release Notes for <mark>Sensor Manager FW module</mark>
Copyright &copy; 2022  STMicroelectronics\
    
[![ST logo](_htmresc/st_logo_2020.png)](https://www.st.com){.logo}
</center>

# Purpose

The **Sensor Manager FW module** is an eLooM-based application-level FW module.

It retrieves sensor data and sets the sensors parameters.
It is implemented as an acquisition engine that:

- Orchestrates multiple tasks accesses to sensor bus data as follows:
  - One or more sensor for each task
  - Read/write requests via queue to handle concurrency on common buses
- Defines interfaces to avoid implementation dependencies 
- Dispatches events to notify when data ready

:::

::: {.col-sm-12 .col-lg-8}
# Update History

::: {.collapse}
<input type="checkbox" id="collapse-section4" checked aria-hidden="true">
<label for="collapse-section4" aria-hidden="true">v1.1.2 / 18-Mar-2022</label>
<div>	

## Main Changes

### Maintenance release

- Aligned IIS3DWB task to use the latest drivers available
- Aligned refs to TraceAlyzer v4.6.1
- Code cleaning, typos and minor fixes
- Updated Release Notes and License

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section3" aria-hidden="true">
<label for="collapse-section3" aria-hidden="true">v1.1.1 / 26-Jan-2022</label>
<div>	

## Main Changes

### Maintenance release and patch release

- Added new sensor types in SensorDef
- Fixed some warning and issue after modifying eLooM for MISRA
- Updated Sensitivity field when FS is changed in IIS3DWB and ISM330DHCX tasks
- Added new queries (SQNextByType and SQNextByStatusEnable)

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section2" aria-hidden="true">
<label for="collapse-section2" aria-hidden="true">v1.1.0 / 10-Dec-2021</label>
<div>	

## Main Changes

### Maintenance release and product update

- Redesigned Sensor Descriptor and Status, updated SMSensorGetDescription and added public API SMSensorGetStatus
- Alloc descriptor in sTheClass and link into the task object in the TaskAlloc function
- Removed GetIstance method and extend the sensor allocator to handle a board with multiple instances of a sensor
- AddSensor and RemoveSensor methods fixed and moved from SensorManager to SensorRegister
- measureODR parameter reset when ODR changed
- Solved inefficient multiple SPI read in ISM330DHCX and IIS3DWB
- Code cleaning, typos and minor fixes

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section1" aria-hidden="true">
<label for="collapse-section1" aria-hidden="true">v1.0.0 / 6-Sep-2021</label>
<div>	

## Main Changes

### First official release

It contains specific implementations for the following sensor and peripheral tasks:

- HTS221
- IIS3DWB
- IMP23ABSU 
- ISM330DHCX
- LPS22HH
- Env Task (HTS221+LPS22HH merged in a unique task)
- I2C
- SPI

</div>

:::
:::
:::

<footer class="sticky">
::: {.columns}
::: {.column width="95%"}
For complete documentation on **STM32**
microcontrollers please visit: [www.st.com](https://www.st.com/en/microcontrollers-microprocessors.html)
:::
::: {.column width="5%"}
<abbr title="Based on template cx566953 version 2.1">Info</abbr>
:::
:::
</footer>

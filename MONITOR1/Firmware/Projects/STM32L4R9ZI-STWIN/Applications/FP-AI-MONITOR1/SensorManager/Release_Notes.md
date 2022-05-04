---
pagetitle: Release Notes for Sensor Manager FW module
lang: en
header-includes: <link rel="icon" type="image/x-icon" href="_htmresc/favicon.png" />
---

::: {.row}
::: {.col-sm-12 .col-lg-4}

<center>
# Release Notes for <mark>Sensor Manager FW module</mark>
Copyright &copy; 2021  STMicroelectronics\
    
[![ST logo](_htmresc/st_logo.png)](https://www.st.com){.logo}
</center>


# License

This software package is licensed by ST under ST license SLA0094, the
"License"; You may not use this component except in compliance with the
License. You may obtain a copy of the License at:
[SLA0094](https://www.st.com/SLA0094)

# Purpose

The **Sensor Manager FW module** is an eLooM-based application-level FW module.

It retrieves sensor data and sets the sensors parameters.
It is implemented as an acquisition engine that:

- Orchestrates multiple tasks accesses to sensor bus data as follows:
-- One or more sensor for each task
-- Read/write requests via queue to handle concurrency on common buses
- Defines interfaces to avoid implementation dependencies 
- Dispatches events to notify when data ready

:::

::: {.col-sm-12 .col-lg-8}
# Update History

::: {.collapse}
<input type="checkbox" id="collapse-section1" checked aria-hidden="true">
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
<abbr title="Based on template cx566953 version 2.0">Info</abbr>
:::
:::
</footer>

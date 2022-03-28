---
pagetitle: Release Notes for HSDatalog Python SDK 
lang: en
header-includes: <link rel="icon" type="image/x-icon" href="_htmresc/favicon.png" />
---

::: {.row}
::: {.col-sm-12 .col-lg-4}

<center> 
# Release Notes for <mark>HSDatalog Python SDK</mark> 
Copyright &copy; 2021 STMicroelectronics
    
[![ST logo](_htmresc/st_logo_2020.png)](https://www.st.com){.logo}
</center>

# License

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

- Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

- Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

- Neither the name nor trademarks of STMicroelectronics International N.V. nor any other STMicroelectronics company nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

# Purpose

The High-Speed DataLog firmware provides an easy way to save data from any combination 
of sensors and microphones configured at their maximum sampling rate.
Sensor data can be either stored on a micro SD Card, SDHC (Secure Digital High Capacity) 
formatted with the FAT32 file system, or streamed to a PC via USB (WinUSB class).

This folder contains different Python scripts and classes that can be used to handle
the datasets obtained by the HSDatalog firmware.

:::

::: {.col-sm-12 .col-lg-8}
# Update History

::: {.collapse}
<input type="checkbox" id="collapse-section6" checked aria-hidden="true">
<label for="collapse-section6" aria-hidden="true">v1.3.0 / 5-Nov-2021</label>
<div>

## Main Changes

### Maintenance release and product update

- **Updated requirements.txt for Linux and Raspberry pi 3B compatibility**
- **Bug fixed in plot function: avoid time shifting for sensors whose samples_per_ts is 1**
- Reviewed Error/Log management
- Automatically setup wav file channels number in hsdatalog_to_wav.py according to the n dimensions of the considered subsensor
- AcquisitionInfo Model Updated (start/end acquisition json keys updated)
- Updated dll (for Windows, Linux and Raspberry pi 3B)

## Dependencies

- All the python modules required by the SDK are shown in **requirements.txt** file.
  - If you are on a Linux or Raspberry machine: 
    - Install manually the following libraries through the **sudo apt-get install** command:
      - libjpeg-dev
      - libbz2-dev
      - libatlas-base-dev
      - python3-tk
    - Then launch the **sudo pip install -r requirements.txt** command  
  - On a Windows machine, just launch the **pip install -r requirements.txt** command
- In Linux or Raspberry environment, launch the scripts as root through **sudo** command

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section5" aria-hidden="true">
<label for="collapse-section5" aria-hidden="true">v1.2.0 / 25-Jun-2021</label>
<div>

## Main Changes

### Maintenance release and product update

- **Updated requirements.txt - import missing**
- **Hotfix in nb_hsdatalog_communication notebook**
- **Removed MIC sensor_type constraints in hsdatalog_to_wav.py (conversion to .wav is available for all sensors)**
- **Redesigned HSDLink com_manager definition and code cleaning**
- Added missing commands in HSDLink and fixed missing import
- Fixed wrong condition for warning message in cartesiam converter function in HSD_utils
- Updated file_manager and DeviceConfig model in HSD
- Bug fixed in HSD plot function 


</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section4" aria-hidden="true">
<label for="collapse-section4" aria-hidden="true">v1.1.0 / 29-Jan-2021</label>
<div>

## Main Changes

### Maintenance release and product update

- **Updated core module and code cleaning**
- Add BSD 3 Clause license disclaimer
- Fixed update device function in command manager (add tagConfig params)
- Issue solved in set label HSDCmd
- **Added new wav and to_unico converters**
- **Updated hsdatalog_check_fake_data.py and hsdatalog_cli.py**


</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section3" aria-hidden="true">
<label for="collapse-section3" aria-hidden="true">v1.0.0 / 13-Nov-2020</label>
<div>

## Main Changes

### First official release


</div>
:::

:::
:::

<footer class="sticky">
::: {.columns}
::: {.column width="95%"}
For complete documentation on **STEVAL-STWINKT1B** ,
visit: [www.st.com](https://www.st.com/en/evaluation-tools/steval-stwinkt1b.html)
:::
::: {.column width="5%"}
<abbr title="Based on template cx566953 version 2.0">Info</abbr>
:::
:::
</footer>

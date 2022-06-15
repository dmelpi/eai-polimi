# PnPL_AiDemo Firmware Example

The PnPL_AiDemo Firmware example runs on the STWIN.box platform (STEVAL-STWINBX1).
The Firmware initializes the ISM330DHCX to stream accelerometer data at the selected ODR and Full Scale, the SensorManager reads the incoming data stream and passes it to the AiDPU that executes the selected algorithm. The output is sent via USB-CDC (Virtual COM Port) by means of Telemetry PnPL messages.
The application can also be controlled using PnPL messages through the same USB-CDC interface.

**PnPL_AiDemo software features**:

- eLooM Framework with ThreadX RTOS

- Sensor Manager

- AiDPU (v1) aligned with X-CUBE-AI V7.1.0

- PnPL Component Manager supporting different messages: command, property and telemetry

- USB-CDC communication interface (implemented through USBX middleware).

- Free user-friendly license terms

- Example implementation available STWIN.box

**List of PnPL commands**:

- GET
  - ``{"get_status": "all"}``
  - ``{"get_status":"ism330dhcx_acc"}``
- CONTROL
  - ``{"ai_application*start":""}``
  - ``{"ai_application*stop":""}``
  - ``{"ai_application*switch_bank":""}``
- Telemetry
  - ``{"ai_application":{"accuracy":1.5}}``

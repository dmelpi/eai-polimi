/**
  ******************************************************************************
  * @file    readme.txt  
  * @author  SRA
  * @version v2.2.1
  * @date    25-Jun-2021
  * @brief   HSDatalog Utilities
  ******************************************************************************
  *
  * Copyright (c) 2021 STMicroelectronics. All rights reserved.
  *
  * The STMicroelectronics corporate logo is a trademark of STMicroelectronics
  *                          
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *           
  * - Redistributions of source code must retain the above copyright notice, this 
  * list of conditions and the following disclaimer.
  * - Redistributions in binary form must reproduce the above copyright notice, 
  * this list of conditions and the following disclaimer in the documentation and/or
  * other materials provided with the distribution.
  * - Neither the name nor trademarks of STMicroelectronics International N.V. nor 
  * any other STMicroelectronics company nor the names of its contributors may be 
  * used to endorse or promote products derived from this software without specific 
  * prior written permission.
  *                          
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
  * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
  * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
  * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
  * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
  * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
  * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
  * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED 
  * OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

HSDatalog Utilities 

These utitlities equip user to perform the High-Speed DataLogging on STEVAL-STWINKTXX board
without requiring to download the full datalog package. The folder includes a pre-built 
binary file "HSDatalog.bin" to program the sensor board to save the data from any combination 
of sensors and microphones configured up-to their maximum sampling rate.

Sensor data can be streamed to a PC via USB (WinUSB class).

To save data via USB, a command line interface example is available in “/Utilities/HSDatalog/cli_example”.
USB_DataLog_Run.bat script provide a ready to use example.

A Python package is also provided with simple examples to read, export and plot the data saved by the application. 
The scripts have been successfully tested with Python 3.7. 

Python_SDK folder contains also a "how to use" Jupyter Notebook, with easy-to-use examples
to show the main functionalities of our HSDatalog Python SDK.

SensorBuffer.xlsx is an Excel file you can use to optimize SD buffer size for each sensor,
as suggested in the User Manual UM2688.

=========================
         Note
=========================
That these utilities and binary are coming from our function pack FP-SNS-DATALOG1 V2.0.
For downloading the full package to exploit all the features and code project please visit
https://www.st.com/en/embedded-software/fp-sns-datalog1.html. The readers are also invited 
to refer to the complete user manual to fully exploit the datalogger functionalities at
 the following link.
https://www.st.com/resource/en/user_manual/um2688-getting-started-with-the-stm32cube-high-speed-datalog-function-pack-for-stwin-evaluation-kits-stmicroelectronics.pdf


/*
  ******************************************************************************
*/
  ******************************************************************************
  *
  * Copyright (c) 2021 STMicroelectronics. All rights reserved.
  *
  *
  ******************************************************************************
  This directory contains the scripts to create input files for NanoEdge AI Studio.
  The contents of the folder are

  * HSD_2_NEAISegments.py : A simple function that will create the NanoEdge AI Studio compliant input segment files. This expects as input the name of the data acquisition made with HSDatalog.bin and, sensorName, sensorType, winlen, stride, and offset. All the other parameters are also available in default for ISM330DHCX sensor but the path of the acquisition directory is to be provided.

  * NanoEdgeAI_Utilities.ipynb : A detailed and step by step notebook to create a normalSegments and cloggedSegments for generating the NanoEdge AI libraries from a provided sample dataset present at /FP-AI-MONITOR1/Utilities/AI_Resources/Datasets/HSD_Logged_Data/Fan12CM/. The directory contains the data measured using the IIS3DWB as well as ISM330DHCX sensor.

  * readme.txt
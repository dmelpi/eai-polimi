  ******************************************************************************
  *
  * Copyright (c) 2021 STMicroelectronics. All rights reserved.
  *
  *
  ******************************************************************************

  This directory contains sample datasets and place holder for the WISDM dataset used in the scripts.

  There are following three sub-directories in this directory.

  * AST : An ST propritery dataset. The full dataset is not provided but a subset of the dataset is provided for code-completion purpose.
  	There are two files in the dataset named as "train.pkl", and "test.pkl" for train and test data respectively.

  * WISDM : This directory is to serve as the place holder for the WISDM dataset and contains only the instructions for downloading and placing the wisdm dataset txt file in the right place with the right name.

  * HSD_Logged_Data : This directory contains the datalogs generated using the HSDatalog.bin.
  This directory contains two sub directories.
  	> Fan12CM: This directory contains two sets of sample data created on the 12CM fan running in normal and clogged settings at three different speed (1000,1250,1500)RPMs using two sensors IIS3DWB (FS = 4, ODR = 26666 Hz), ISM330DHCX (FS=4, ODR = 1666 Hz ).
  	> HAR : Small data logs for four activities using HSDatalog. The activities include "Stationary", "Walking", "Jogging" and "Biking".
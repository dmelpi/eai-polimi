/**
 ******************************************************************************
 * @file    SMMessageParser.c
 * @author  SRA - MCD
 * @version 1.0.0
 * @date    6-Sep-2021
 *
 * @brief   Definition of the HID REport PArser API.
 *
 * Definition of the HID REport PArser API.
 *
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under ODE SOFTWARE LICENSE AGREEMENT
 * SLA0094, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0094
 *
 ******************************************************************************
 */

#include "SMMessageParser.h"

uint16_t SMMessageGetSize(uint8_t message_id)
{
  uint16_t nSize;
  switch(message_id)
  {
  case SM_MESSAGE_ID_DATA_READY:
    nSize = sizeof(struct sensorDataReadyMessage_t);
    break;

  case SM_MESSAGE_ID_FORCE_STEP:
    nSize = sizeof(struct internalMessageFE_t);
    break;

  case SM_MESSAGE_ID_SD_CMD:
    nSize = sizeof(struct sdMessage_t);
    break;

  case SM_MESSAGE_ID_SENSOR_CMD:
    nSize = sizeof(struct sensorMessage_t);
    break;

  case SM_MESSAGE_ID_SPI_BUS_READ:
  case SM_MESSAGE_ID_SPI_BUS_WRITE:
    nSize = sizeof(struct spiIOMessage_t);
    break;

  case SM_MESSAGE_ID_I2C_BUS_READ:
  case SM_MESSAGE_ID_I2C_BUS_WRITE:
    nSize = sizeof(struct i2cIOMessage_t);
    break;

  default:
    nSize = sizeof(struct internalMessageFE_t);
  }

  return nSize;
}

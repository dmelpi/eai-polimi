/**
  ******************************************************************************
  * @file    SMMessageParser.h
  * @author  SRA - MCD
  * @version 1.0.0
  * @date    6-Sep-2021
  *
  * @brief   Input Output and Feature report Declaration.
  *
  * This file declare the Input, Output, Feature and Internal report for
  * the application. The reports are generated by the sensors as well as
  * form the MCU (internal reports).
  *
  * Report ID list:
  * - 0x01: inputReport                (Sensor --> MCU). ISM330DHCX
  * - 0xFE: internalReport             (MCU --> MCU).    FORCE_STEP
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

#ifndef SM_MESSAGE_PARSER_H_
#define SM_MESSAGE_PARSER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "services/syserror.h"
#include "SPIBusIF.h"
#include "I2CBusIF.h"

#define SM_MESSAGE_ID_DATA_READY                0x01  ///< Sensor data ready report
#define SM_MESSAGE_ID_DATA_READY_MLC            0x02  ///< MLC data ready report
#define SM_MESSAGE_ID_SENSOR_CMD                0x04  ///< Sensor command encapsulated in a message.
#define SM_MESSAGE_ID_SD_CMD                    0x05  ///< SDCARD command encapsulated in a message.
#define SM_MESSAGE_ID_SPI_BUS_READ              0x06  ///< Command to read from the SPI bus
#define SM_MESSAGE_ID_SPI_BUS_WRITE             0x07  ///< Command to write in the SPI bus.
#define SM_MESSAGE_ID_I2C_BUS_READ              0x08  ///< Command to read from the I2C bus
#define SM_MESSAGE_ID_I2C_BUS_WRITE             0x09  ///< Command to write in the I2C bus.
#define SM_MESSAGE_ID_FORCE_STEP                0xFE  ///< Special ID used by the INIT task to force the execution of ManagedTaskEx step.


typedef union _SMMessage
{
  uint8_t messageID;

  //--------------------------------------------------------------------------------
  //  SensorReport 01 (Device --> MCU) - Sensor data ready report
  //--------------------------------------------------------------------------------

  struct sensorDataReadyMessage_t
  {
    uint8_t  messageId;                                // Report ID = 0x01 (1)
    uint8_t  half;                                     // used only by the microphone
    uint8_t  nReserved[2];                             // Reserved
    double   fTimestamp;                               // timestamp in s.
  } sensorDataReadyMessage;

  //--------------------------------------------------------------------------------
  //  SensorReport 04 (MCU --> MCU) - Generic Sensor command
  //--------------------------------------------------------------------------------

  struct sensorMessage_t
  {
    uint8_t   messageId;                                // Report ID = 0x04 (4)
    uint8_t   nSensorId;                               // Specify the sensor ID
    uint16_t  nCmdID;                                  // Specify the command ID
    uint32_t  nParam;                                  // Specify an optional parameter.
  } sensorMessage;

  //--------------------------------------------------------------------------------
  //  SDCARD Report 05 (MCU --> MCU) - SDCARD command
  //--------------------------------------------------------------------------------

  struct sdMessage_t
  {
    uint8_t   messageId;                                // Report ID = 0x05 (5)
    uint8_t   nPadding;                                // Padding byte
    uint16_t  nCmdID;                                  // Specify the command ID
    uint32_t  nParam;                                  // Specify an optional parameter.
  } sdMessage;

  //--------------------------------------------------------------------------------
  //  SPI Read / Write command 06 07 (MCU --> MCU) - SPI Bus command
  //--------------------------------------------------------------------------------

  struct spiIOMessage_t
  {
    uint8_t   messageId;                                // Report ID = 0x06 / 0x07 (6 / 7)
    uint8_t   nRegAddr;
    uint16_t  nDataSize;
    uint8_t  *pnData;
    SPIBusIF *pxSensor;
  } spiIOMessage;

  //--------------------------------------------------------------------------------
  //  I2C Read / Write command 08 09 (MCU --> MCU) - SPI Bus command
  //--------------------------------------------------------------------------------

  struct i2cIOMessage_t
  {
    uint8_t   messageId;                                // Report ID = 0x08 / 0x09 (8 / 9)
    uint8_t   nRegAddr;
    uint16_t  nDataSize;
    uint8_t  *pnData;
    I2CBusIF *pxSensor;
  } i2cIOMessage;

  //--------------------------------------------------------------------------------
  //  internalReport (MCU)
  //--------------------------------------------------------------------------------

  struct internalMessageFE_t
  {
    uint8_t  messageId;                                 // Report ID = 0xFE
    uint8_t  nData;                                    // reserved. It can be ignored
  } internalMessageFE;

} SMMessage;


/**
  * Get the size of the report with a specified ID
  *
  * @param nReportID [IN] specifies a report ID
  * @return the size of the report with the specified ID or SYS_INVALID_PARAMETER_ERROR_CODE
  */
uint16_t SMMessageGetSize(uint8_t nReportID);


#ifdef __cplusplus
}
#endif


#endif /* SM_MESSAGE_PARSER_H_ */

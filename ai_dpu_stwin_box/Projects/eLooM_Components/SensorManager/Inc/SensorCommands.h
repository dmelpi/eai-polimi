/**
 ******************************************************************************
 * @file    SensorCommands.h
 * @author  SRA - MCD
 * @version 1.0.0
 * @date    6-Sep-2021
 *
 * @brief
 *
 * <DESCRIPTIOM>
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
#ifndef SENSORCOMMANDS_H_
#define SENSORCOMMANDS_H_

#ifdef __cplusplus
extern "C" {
#endif

// Command ID. These are all commands supported by a sensor task.
#define SENSOR_CMD_ID_INIT				((uint16_t)0x0001)              ///< START command ID.
#define SENSOR_CMD_ID_SET_ODR			((uint16_t)0x0002)              ///< SET ODR command ID.
#define SENSOR_CMD_ID_SET_FS			((uint16_t)0x0003)              ///< SET FS command ID.
#define SENSOR_CMD_ID_ENABLE			((uint16_t)0x0004)              ///< ENABLE command ID.
#define SENSOR_CMD_ID_DISABLE			((uint16_t)0x0005)              ///< DISABLE command ID.

#ifdef __cplusplus
}
#endif

#endif /* SENSORCOMMANDS_H_ */

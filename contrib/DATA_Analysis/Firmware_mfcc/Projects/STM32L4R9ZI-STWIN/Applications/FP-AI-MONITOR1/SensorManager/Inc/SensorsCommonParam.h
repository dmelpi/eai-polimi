/**
 ******************************************************************************
 * @file    SensorsCommonParam.h
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
#ifndef SENSORSCOMMONPARAM_H_
#define SENSORSCOMMONPARAM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "services/systypes.h"

#ifndef SENSOR_CFG_MAX_SUBSENSOR_COUNT
#define SENSOR_CFG_MAX_SUBSENSOR_COUNT    4
#endif

/**
 * Create a type name for _SensorInitParam.
 */
typedef struct _SensorInitParam SensorInitParam;

/**
 * Sensor parameters. This set of parameters are common to all supported sensor.
 */
struct _SensorInitParam
{
  /**
   * Output Data Rate. All sub sensors will run with the same ODR
   */
  float fODR[SENSOR_CFG_MAX_SUBSENSOR_COUNT];

  /**
   * Full Scale. Note that one sensor device can have up to four sub sensors
   */
  float pfFS[SENSOR_CFG_MAX_SUBSENSOR_COUNT];

  /**
   * Status of the sub sensor.
   */
  boolean_t pbSubSensorActive[SENSOR_CFG_MAX_SUBSENSOR_COUNT];
};

#ifdef __cplusplus
}
#endif

#endif /* SENSORSCOMMONPARAM_H_ */

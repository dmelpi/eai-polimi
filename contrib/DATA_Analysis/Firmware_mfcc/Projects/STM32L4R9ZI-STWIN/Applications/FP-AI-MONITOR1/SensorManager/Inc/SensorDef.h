/**
 ******************************************************************************
 * @file    SensorDef.h
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
#ifndef HSDCORE_INC_SENSORDEF_H_
#define HSDCORE_INC_SENSORDEF_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "services/systp.h"
#include "services/syserror.h"

#define HSD_USE_DUMMY_DATA 0

#define COM_TYPE_ACC    1
#define COM_TYPE_MAG    2
#define COM_TYPE_GYRO   3
#define COM_TYPE_TEMP   4
#define COM_TYPE_PRESS  5
#define COM_TYPE_HUM    6
#define COM_TYPE_MIC    7
#define COM_TYPE_MLC    8

#define SM_MAX_SENSOR_COMBO          4
#define SM_MAX_SUPPORTED_ODR         16
#define SM_MAX_SUPPORTED_FS          16
#define SM_MAX_DIM_LABELS            16U
#define DIM_LABELS_LENGTH           3U

#define DATA_TYPE_UINT8     (uint8_t)(0x00)
#define DATA_TYPE_INT8      (uint8_t)(0x01)
#define DATA_TYPE_UINT16    (uint8_t)(0x02)
#define DATA_TYPE_INT16     (uint8_t)(0x03)
#define DATA_TYPE_UINT32    (uint8_t)(0x04)
#define DATA_TYPE_INT32     (uint8_t)(0x05)
#define DATA_TYPE_FLOAT     (uint8_t)(0x06)

#define COM_END_OF_LIST_INT -1
#define COM_END_OF_LIST_FLOAT -1.0f

#define COM_LIST_SEPARATOR_INT -2
#define COM_LIST_SEPARATOR_FLOAT -2.0f

  /**
   * Create  type name for _SubSensorDescriptor_t.
   */
  typedef struct _SubSensorDescriptor_t SubSensorDescriptor_t;

  /**
   *  SubSensorDescriptor_t internal structure.
   */
  struct _SubSensorDescriptor_t {

    /**
     * Specifies the subsenor unique ID.
     */
    uint8_t IdSub;

    /**
     * Specifies the sensor type (ACC, GYRO, TEMP, ...).
     */
    uint8_t SensorType;

    /**
     * Specifies the supported data rates.
     */
    float pODR[SM_MAX_SUPPORTED_ODR];

    /**
     * Specifies the supported full scales.
     */
    float pFS[SM_MAX_SUPPORTED_FS];

    /**
     * Specifies the data type (int16, float, ...).
     */
    uint8_t DataType;

    /**
     * Specifies the data width (1, 3 axes, ...).
     */
    uint16_t Dimensions;

    /**
     * Specifies a label for each axes.
     */
    char DimensionsLabel[SM_MAX_DIM_LABELS][DIM_LABELS_LENGTH];

    /**
     * Specifies the unit of measurement for each axes.
     */
    char unit[SM_MAX_DIM_LABELS];

    /**
     * Specifies the supported values for SamplesPerTimestamp variable.
     */
    uint16_t pSamplesPerTimestamp[2];
  };


  /**
   * Create  type name for _SensorDescriptor_t.
   */
  typedef struct _SensorDescriptor_t SensorDescriptor_t;

  /**
   *  SensorDescriptor_t internal structure.
   */
  struct _SensorDescriptor_t {

    /**
     * Specifies the sensor name.
     */
    char Name[SM_MAX_DIM_LABELS];

    /**
     * Specifies the number of subsensors available.
     */
    uint8_t NumberSubSensors;

    /**
     * Specifies the capabilities of each subsensor.
     */
    SubSensorDescriptor_t pSubSensorDescriptor[SM_MAX_SENSOR_COMBO];
  };


  /**
   * Create  type name for _SubSensorStatus_t.
   */
  typedef struct _SubSensorStatus_t SubSensorStatus_t;

  /**
   *  SubSensorStatus_t internal structure.
   */
  struct _SubSensorStatus_t {

    /**
     * Specifies the full scale.
     */
    float FS;

    /**
     * Specifies the sensitivity.
     */
    float Sensitivity;

    /**
     * Specifies if the subsensor is active or not.
     */
    boolean_t IsActive;

    /**
     * Specifies the nominal data rate.
     */
    float ODR;

    /**
     * Specifies the effective data rate.
     */
    float MeasuredODR;

    /**
     * Specifies the time offset before first data were available.
     */
    float InitialOffset;

    /**
     * Specifies how many samples there are between two consecutive timestamps.
     */
    uint16_t SamplesPerTimestamp;
  };


  /**
   * Create  type name for _SensorManager_t.
   */
  typedef struct _SensorStatus_t SensorStatus_t;

  /**
   *  SensorStatus_t internal structure.
   */
  struct _SensorStatus_t {

    /**
     * Specifies the actual configuration for each subsensor.
     */
    SubSensorStatus_t pSubSensorStatus[SM_MAX_SENSOR_COMBO];
  };



  /* Public API declaration */
  /**************************/


  /* Inline functions definition */
  /*******************************/


#ifdef __cplusplus
}
#endif

#endif /* HSDCORE_INC_SENSORDEF_H_ */

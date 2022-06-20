/**
 ******************************************************************************
 * @file    SensorDef.h
 * @author  SRA - MCD
 * @version 1.1.0
 * @date    10-Dec-2021
 *
 * @brief
 *
 * <DESCRIPTIOM>
 *
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 *
 ******************************************************************************
 */
#ifndef HSDCORE_INC_SENSORDEF_H_
#define HSDCORE_INC_SENSORDEF_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "SensorDefConfig.h"

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
#define COM_TYPE_VDI    9   ///< Virtual Sensor for Data Injection
#define COM_TYPE_CSTM   10  ///< Custom sensor type

#define SM_MAX_SENSOR_COMBO          4
#define SM_MAX_SUPPORTED_ODR         16
#define SM_MAX_SUPPORTED_FS          16
#define SM_MAX_DIM_LABELS            16U
#define SM_DIM_LABELS_LENGTH            4U

#define COM_END_OF_LIST_INT -1
#define COM_END_OF_LIST_FLOAT -1.0f

#define COM_LIST_SEPARATOR_INT -2
#define COM_LIST_SEPARATOR_FLOAT -2.0f

#ifndef SM_SPI_BUS_IF
#define SM_SPI_BUS_IF 1
#endif /* SM_SPI_BUS_IF */

#ifndef SM_I2C_BUS_IF
#define SM_I2C_BUS_IF 1
#endif /* SM_I2C_BUS_IF */

  // TEST Sensors structs redefinition: PROPOSAL

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
	 * Specifies a label for each axes.
	 */
	char DimensionsLabel[SM_MAX_DIM_LABELS][SM_DIM_LABELS_LENGTH];

	/**
	 * Specifies the unit of measurement for each axes.
	 */
	char unit[SM_MAX_DIM_LABELS];
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
   * Specifies the data type (int16, float, ...).
   */
  uint8_t DataType;

  /**
   * Specifies the data width (1, 3 axes, ...).
   */
  uint16_t Dimensions;
  };


  /* Public API declaration */
  /**************************/


  /* Inline functions definition */
  /*******************************/


#ifdef __cplusplus
}
#endif

#endif /* HSDCORE_INC_SENSORDEF_H_ */

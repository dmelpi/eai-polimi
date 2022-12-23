/**
 ******************************************************************************
 * @file    SensorDefConfig.h
 * @author  STMicroelectronics - AIS - MCD Team
 * @version $Version$
 * @date    $Date$
 * @brief   Global System configuration file
 *
 * This file include some configuration parameters grouped here for user
 * convenience. This file override the default configuration value, and it is
 * used in the "Preinclude file" section of the "compiler > prepocessor"
 * options.
 *
 *********************************************************************************
 * @attention
 *
 * <h2><center>COPYRIGHT &copy; 2021 STMicroelectronics</center></h2>
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file in
 * the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *********************************************************************************
 */

#ifndef SENSORDEFCONFIG_H_
#define SENSORDEFCONFIG_H_

#define IIS2DLPC_FIFO_ENABLED	1
#define IIS2ICLX_FIFO_ENABLED	1
#define IIS3DWB_FIFO_ENABLED	1
#define ISM330DHCX_FIFO_ENABLED	1

#define LPS22DF_FIFO_ENABLED	1
#define LIS2DU12_FIFO_ENABLED	0
#define LSM6DSV16X_FIFO_ENABLED	1

#endif /* SENSORDEFCONFIG_H_ */

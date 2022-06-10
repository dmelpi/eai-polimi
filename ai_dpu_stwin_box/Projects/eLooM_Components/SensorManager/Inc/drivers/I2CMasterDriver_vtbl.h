/**
 ******************************************************************************
 * @file    I2CMasterDriver_vtbl.h
 * @author  SRA - MCD
 * @version 1.0.0
 * @date    6-Sep-2021
 *
 * @brief   ::I2CMAsterDriver_t virtual funciton declaration
 *
 * This file declare all the virtual function overridden by the class.
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
#ifndef INCLUDE_DRIVERS_I2CMASTERDRIVER_VTBL_H_
#define INCLUDE_DRIVERS_I2CMASTERDRIVER_VTBL_H_

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @sa IDrvInit
 */
sys_error_code_t I2CMasterDriver_vtblInit(IDriver *_this, void *p_params);

/**
 * @sa IDrvStart
 */
sys_error_code_t I2CMasterDriver_vtblStart(IDriver *_this);

/**
 * @sa IDrvStop
 */
sys_error_code_t I2CMasterDriver_vtblStop(IDriver *_this);

/**
 *
 * @sa IDrvDoEnterPowerMode
 */
sys_error_code_t I2CMasterDriver_vtblDoEnterPowerMode(IDriver *_this, const EPowerMode active_power_mode, const EPowerMode new_power_mode);

/**
 * @sa IDrvReset
 */
sys_error_code_t I2CMasterDriver_vtblReset(IDriver *_this, void *p_params);

/**
 * @sa IIODrvWrite
 */
sys_error_code_t I2CMasterDriver_vtblWrite(IIODriver *_this, uint8_t *p_data_buffer, uint16_t data_size, uint16_t channel);

/**
 * @sa IIODrvRead
 */
sys_error_code_t I2CMasterDriver_vtblRead(IIODriver *_this, uint8_t *p_data_buffer, uint16_t data_size, uint16_t channel);

#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_DRIVERS_I2CMASTERDRIVER_VTBL_H_ */

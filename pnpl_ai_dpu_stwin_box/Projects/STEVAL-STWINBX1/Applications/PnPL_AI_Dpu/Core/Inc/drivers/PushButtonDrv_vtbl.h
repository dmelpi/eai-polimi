/*
 * PushButtonDrv_vtbl.h
 *
 *  Created on: 27 nov 2021
 *      Author: stefano
 */

#ifndef INC_DRIVERS_PUSHBUTTONDRV_VTBL_H_
#define INC_DRIVERS_PUSHBUTTONDRV_VTBL_H_

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @sa IDrvInit
 */
sys_error_code_t PushButtonDrv_vtblInit(IDriver *_this, void *p_params);

/**
 * @sa IDrvStart
 */
sys_error_code_t PushButtonDrv_vtblStart(IDriver *_this);

/**
 * @sa IDrvStop
 */
sys_error_code_t PushButtonDrv_vtblStop(IDriver *_this);

/**
 *
 * @sa IDrvDoEnterPowerMode
 */
sys_error_code_t PushButtonDrv_vtblDoEnterPowerMode(IDriver *_this, const EPowerMode active_power_mode, const EPowerMode new_power_mode);

/**
 * @sa IDrvReset
 */
sys_error_code_t PushButtonDrv_vtblReset(IDriver *_this, void *p_params);

#ifdef __cplusplus
}
#endif

#endif /* INC_DRIVERS_PUSHBUTTONDRV_VTBL_H_ */

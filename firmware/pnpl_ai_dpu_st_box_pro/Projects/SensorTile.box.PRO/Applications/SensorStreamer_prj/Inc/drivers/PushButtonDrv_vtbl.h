
/**
 ******************************************************************************
 * @file    PushButtonDrv_vtbl.h
 * @author  STMicroelectronics - AIS - MCD Team
 * @version 3.0.1
 * @date    21/01/2022 10:35:15
 *
 * @brief   Declare the virtual table for the PushButtonDrv
 *
 * This file declares all the virtual functions implemented by the
 * ::PushButtonDrv.
 *
 *********************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file in
 * the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *********************************************************************************
*/

#ifndef PUSHBUTTONDRV_VTBL_H_
#define PUSHBUTTONDRV_VTBL_H_

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

#endif /* PUSHBUTTONDRV_VTBL_H_ */

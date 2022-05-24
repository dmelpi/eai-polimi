/**
 ******************************************************************************
 * @file    apperror.h
 * @author  STMicroelectronics - AIS - MCD Team
 * @version $Version$
 * @date    $Date$
 *
 * @brief Application specific error code
 *
 * Application defines its own error code in this file starting form the
 * constant APP_BASE_ERROR_CODE.
 * It is recommended to group the error code in the following groups:
 * - Low Level API error code
 * - Service Level error code
 * - Task Level error code
 *
 *********************************************************************************
 * @attention
 *
 * Copyright (c) 2021 STMicroelectronics
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file in
 * the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *********************************************************************************
 */
#ifndef APPERROR_H_
#define APPERROR_H_

#ifdef __cplusplus
extern "C" {
#endif

// Low Level API error code
// ************************

#define SYS_BASE_LL_ERROR_CODE                                APP_BASE_ERROR_CODE
#define SYS_LL_UNDEFINED_ERROR_CODE                           SYS_BASE_LL_ERROR_CODE + 1

// IP error
//#define SYS_BASE_XX_ERROR_CODE                               APP_BASE_ERROR_CODE

// Service Level error code
// ************************

// Task Level error code
// *********************

#ifdef __cplusplus
}
#endif

#endif /* APPERROR_H_ */

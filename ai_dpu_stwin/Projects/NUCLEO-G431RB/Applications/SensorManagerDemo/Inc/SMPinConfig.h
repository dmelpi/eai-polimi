/**
 ******************************************************************************
 * @file    SMPinConfig.h
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
 * Copyright (c) 2021 STMicroelectronics
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file in
 * the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *********************************************************************************
 */

#ifndef SMPINCONFIG_H_
#define SMPINCONFIG_H_


/* Define HTS221_USER_PIN_CONFIG to override default HTS221 pins configuration */
#define HTS221_USER_PIN_CONFIG          1
#define HTS221_INT_Pin                  GPIO_PIN_10
#define HTS221_INT_GPIO_Port            GPIOA
#define HTS221_INT_EXTI_IRQn            EXTI15_10_IRQn
#define HTS221_INT_GPIO_ADDITIONAL()
#define HTS221_INT_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOA_CLK_ENABLE()


#endif /* SMPINCONFIG_H_ */

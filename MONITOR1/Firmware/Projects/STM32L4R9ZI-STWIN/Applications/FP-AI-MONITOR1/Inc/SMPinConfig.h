/**
 ******************************************************************************
 * @file    SMPinConfig.h
 * @author  STMicroelectronics - AIS - MCD Team
 * @version V1.0.0
 * @date    15-September-2021
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
#define HTS221_INT_Pin                  GPIO_PIN_6
#define HTS221_INT_GPIO_Port            GPIOG
#define HTS221_INT_EXTI_IRQn            EXTI9_5_IRQn
#define HTS221_INT_GPIO_ADDITIONAL()    HAL_PWREx_EnableVddIO2()
#define HTS221_INT_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOG_CLK_ENABLE()

/* Define IIS3DWB_CFG_USER_CONFIG to override default IIS3DWB pins configuration */
#define IIS3DWB_USER_PIN_CONFIG                   1
#define IIS3DWB_SPI_CS_Pin                        GPIO_PIN_5
#define IIS3DWB_SPI_CS_GPIO_Port                  GPIOF
#define IIS3DWB_SPI_CS_GPIO_CLK_ENABLE()          __HAL_RCC_GPIOF_CLK_ENABLE()
#define IIS3DWB_INT1_Pin                          GPIO_PIN_14
#define IIS3DWB_INT1_GPIO_Port                    GPIOE
#define IIS3DWB_INT1_GPIO_CLK_ENABLE()            __HAL_RCC_GPIOE_CLK_ENABLE()
#define IIS3DWB_INT1_EXTI_IRQn                    EXTI15_10_IRQn

/* Define ISM330DHCX_CFG_USER_CONFIG to override default ISM330DHCX pins configuration */
#define ISM330DHCX_USER_PIN_CONFIG                   1
#define ISM330DHCX_SPI_CS_Pin                        GPIO_PIN_13
#define ISM330DHCX_SPI_CS_GPIO_Port                  GPIOF
#define ISM330DHCX_SPI_CS_GPIO_CLK_ENABLE()          __HAL_RCC_GPIOF_CLK_ENABLE()
#define ISM330DHCX_INT1_Pin                          GPIO_PIN_8
#define ISM330DHCX_INT1_GPIO_Port                    GPIOE
#define ISM330DHCX_INT1_GPIO_CLK_ENABLE()            __HAL_RCC_GPIOE_CLK_ENABLE()
#define ISM330DHCX_INT1_EXTI_IRQn                    EXTI9_5_IRQn
#define ISM330DHCX_INT2_Pin                          GPIO_PIN_4
#define ISM330DHCX_INT2_GPIO_Port                    GPIOF
#define ISM330DHCX_INT2_GPIO_CLK_ENABLE()            __HAL_RCC_GPIOF_CLK_ENABLE()
#define ISM330DHCX_INT2_EXTI_IRQn                    EXTI4_IRQn



#endif /* SMPINCONFIG_H_ */

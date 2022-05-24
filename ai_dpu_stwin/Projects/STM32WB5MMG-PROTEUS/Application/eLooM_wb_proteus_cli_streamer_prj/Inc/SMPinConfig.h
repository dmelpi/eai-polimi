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
 * <h2><center>COPYRIGHT &copy; 2021 STMicroelectronics</center></h2>
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file in
 * the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *********************************************************************************
 */

#ifndef SMPINCONFIG_H_
#define SMPINCONFIG_H_


/* Define IIS3DWB_CFG_USER_CONFIG to override default IIS3DWB pins configuration */
#define IIS3DWB_USER_PIN_CONFIG                      1
#define IIS3DWB_SPI_CS_Pin                           GPIO_PIN_15
#define IIS3DWB_SPI_CS_GPIO_Port                     GPIOA
#define IIS3DWB_SPI_CS_GPIO_CLK_ENABLE()             __HAL_RCC_GPIOA_CLK_ENABLE()
#define IIS3DWB_INT1_Pin                             GPIO_PIN_3
#define IIS3DWB_INT1_GPIO_Port                       GPIOE
#define IIS3DWB_INT1_GPIO_CLK_ENABLE()               __HAL_RCC_GPIOE_CLK_ENABLE()
#define IIS3DWB_INT1_EXTI_IRQn                       EXTI3_IRQn

/* Define ISM330DHCX_CFG_USER_CONFIG to override default ISM330DHCX pins configuration */
#define ISM330DHCX_USER_PIN_CONFIG                   1
#define ISM330DHCX_SPI_CS_Pin                        GPIO_PIN_4
#define ISM330DHCX_SPI_CS_GPIO_Port                  GPIOA
#define ISM330DHCX_SPI_CS_GPIO_CLK_ENABLE()          __HAL_RCC_GPIOA_CLK_ENABLE()
#define ISM330DHCX_INT1_Pin                          GPIO_PIN_4
#define ISM330DHCX_INT1_GPIO_Port                    GPIOE
#define ISM330DHCX_INT1_GPIO_CLK_ENABLE()            __HAL_RCC_GPIOE_CLK_ENABLE()
#define ISM330DHCX_INT1_EXTI_IRQn                    EXTI4_IRQn
#define ISM330DHCX_INT2_Pin                          GPIO_PIN_7
#define ISM330DHCX_INT2_GPIO_Port                    GPIOC
#define ISM330DHCX_INT2_GPIO_CLK_ENABLE()            __HAL_RCC_GPIOC_CLK_ENABLE()
#define ISM330DHCX_INT2_EXTI_IRQn                    EXTI5_9IRQn

#endif /* SMPINCONFIG_H_ */

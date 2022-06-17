/**
 ******************************************************************************
 * @file    SMPinConfig.h
 * @author  STMicroelectronics - ST-Korea - MCD Team
 * @version 1.2.0
 * @date    Jan 5, 2017
 * @brief   Global System configuration file
 *
 * This file include some configuration parameters grouped here for user
 * convenience. This file override the default configuration value, and it is
 * used in the "Preinclude file" section of the "compiler > prepocessor"
 * options.
 *
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
 *
 * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *        http://www.st.com/software_license_agreement_liberty_v2
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************
 */

#ifndef SMPINCONFIG_H_
#define SMPINCONFIG_H_

#define HSD_USE_DUMMY_DATA              0

/* Define HTS221_USER_PIN_CONFIG to override default HTS221 pins configuration */
#define HTS221_USER_PIN_CONFIG          1
#define HTS221_INT_Pin                  GPIO_PIN_6
#define HTS221_INT_GPIO_Port            GPIOI
#define HTS221_INT_EXTI_IRQn            EXTI6_IRQn
#define HTS221_INT_GPIO_ADDITIONAL()    HAL_PWREx_EnableVddIO2()
#define HTS221_INT_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOI_CLK_ENABLE()

/* Define IIS3DWB_CFG_USER_CONFIG to override default IIS3DWB pins configuration */
#define IIS3DWB_USER_PIN_CONFIG                   1
#define IIS3DWB_SPI_CS_Pin                        GPIO_PIN_12
#define IIS3DWB_SPI_CS_GPIO_Port                  GPIOF
#define IIS3DWB_SPI_CS_GPIO_CLK_ENABLE()          __HAL_RCC_GPIOF_CLK_ENABLE()
#define IIS3DWB_INT1_Pin                          GPIO_PIN_15
#define IIS3DWB_INT1_GPIO_Port                    GPIOF
#define IIS3DWB_INT1_GPIO_CLK_ENABLE()            __HAL_RCC_GPIOF_CLK_ENABLE()
#define IIS3DWB_INT1_EXTI_IRQn                    EXTI15_IRQn

/* Define ISM330DHCX_CFG_USER_CONFIG to override default ISM330DHCX pins configuration */
#define ISM330DHCX_USER_PIN_CONFIG                   1
#define ISM330DHCX_SPI_CS_Pin                        GPIO_PIN_15
#define ISM330DHCX_SPI_CS_GPIO_Port                  GPIOH
#define ISM330DHCX_SPI_CS_GPIO_CLK_ENABLE()          __HAL_RCC_GPIOH_CLK_ENABLE()
#define ISM330DHCX_INT1_Pin                          GPIO_PIN_8
#define ISM330DHCX_INT1_GPIO_Port                    GPIOB
#define ISM330DHCX_INT1_GPIO_CLK_ENABLE()            __HAL_RCC_GPIOB_CLK_ENABLE()
#define ISM330DHCX_INT1_EXTI_IRQn                    EXTI8_IRQn
#define ISM330DHCX_INT2_Pin                          GPIO_PIN_4
#define ISM330DHCX_INT2_GPIO_Port                    GPIOF
#define ISM330DHCX_INT2_GPIO_CLK_ENABLE()            __HAL_RCC_GPIOF_CLK_ENABLE()
#define ISM330DHCX_INT2_EXTI_IRQn                    EXTI4_IRQn

/* Define IIS2DLPC_CFG_USER_CONFIG to override default IIS2DLPC pins configuration */
#define IIS2DLPC_USER_PIN_CONFIG                   1
#define IIS2DLPC_SPI_CS_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOH_CLK_ENABLE()
#define IIS2DLPC_SPI_CS_GPIO_Port                    GPIOH
#define IIS2DLPC_SPI_CS_Pin                     GPIO_PIN_6
#define IIS2DLPC_INT1_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOF_CLK_ENABLE()
#define IIS2DLPC_INT1_GPIO_Port                  GPIOF
#define IIS2DLPC_INT1_Pin                   GPIO_PIN_1
#define IIS2DLPC_INT1_EXTI_IRQn             EXTI1_IRQn
#define IIS2DLPC_INT2_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOF_CLK_ENABLE()
#define IIS2DLPC_INT2_GPIO_Port                  GPIOF
#define IIS2DLPC_INT2_Pin                   GPIO_PIN_2
#define IIS2DLPC_INT2_EXTI_IRQn             EXTI2_IRQn

/* Define IIS2MDC_CFG_USER_CONFIG to override default IIS2MDC pins configuration */
#define IIS2MDC_USER_PIN_CONFIG           1
#define IIS2MDC_INT1_Pin                          GPIO_PIN_9
#define IIS2MDC_INT1_GPIO_Port                    GPIOF
#define IIS2MDC_INT1_GPIO_CLK_ENABLE()            __HAL_RCC_GPIOF_CLK_ENABLE()
#define IIS2MDC_INT1_EXTI_IRQn                    EXTI9_IRQn
#define IIS2MDC_INT1_EXTI_LINE                    EXTI_LINE_9

/* Define IIS2ICLX_CFG_USER_CONFIG to override default IIS2ICLX pins configuration */
#define IIS2ICLX_USER_PIN_CONFIG                   1
#define IIS2ICLX_SPI_CS_Pin                        GPIO_PIN_7
#define IIS2ICLX_SPI_CS_GPIO_Port                  GPIOI
#define IIS2ICLX_SPI_CS_GPIO_CLK_ENABLE()          __HAL_RCC_GPIOI_CLK_ENABLE()
#define IIS2ICLX_INT1_Pin                          GPIO_PIN_3
#define IIS2ICLX_INT1_GPIO_Port                    GPIOF
#define IIS2ICLX_INT1_GPIO_CLK_ENABLE()            __HAL_RCC_GPIOF_CLK_ENABLE()
#define IIS2ICLX_INT1_EXTI_IRQn                    EXTI3_IRQn
#define IIS2ICLX_INT2_Pin                          GPIO_PIN_11
#define IIS2ICLX_INT2_GPIO_Port                    GPIOF
#define IIS2ICLX_INT2_GPIO_CLK_ENABLE()            __HAL_RCC_GPIOF_CLK_ENABLE()
#define IIS2ICLX_INT2_EXTI_IRQn                    EXTI11_IRQn

#endif /* SMPINCONFIG_H_ */

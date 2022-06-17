/**
 ******************************************************************************
 * @file    sensor_parameters.h
 * @author  STMicroelectronics
 * @version 1.0.0
 * @date    May 21, 2022
 *
 * @brief USB_CDC Class
 *
 * This managed task is responsible to manage the USB_CDC communication
 *
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT 2017 STMicroelectronics</center></h2>
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


#ifndef SENSOR_PARAMETERS_H_
#define SENSOR_PARAMETERS_H_

#ifdef __cplusplus
extern "C" {
#endif

#define ISM330DHCX_ODR 1666.0      						// [Hz] set ODR (Output Data Rate) value based on supported ODR available list of the sensor
#define ISM330DHCX_FS 16.0

#ifdef __cplusplus
}
#endif

#endif /* SENSOR_PARAMETERS_H_ */

/**
 ******************************************************************************
 * @file    params.h
 * @author  STMicroelectronics
 * @version 1.0.0
 * @date    May 21, 2022
 *
 * @brief Handling of parameters generated with Handlebars.
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
#ifndef PARAMS_H_
#define PARAMS_H_

#ifdef __cplusplus
extern "C" {
#endif


/* Sensors. */
#define ISM330DHCX_ACC_FS        (16.0)
#define ISM330DHCX_ACC_ODR       (1666.0)

/* Pre-Processing. */
#define INPUT_BUFFER_SIZE        (512)
#define SELECTED_AXIS            (X)
#define TRIANGULAR_FILTERS_SCALE (TRIANGULAR_FILTERS_SCALE_HZ)
#define SIGNAL_WINDOWING         (HANNING)

/* AI-Processing. */
#define NETWORK_NAME             ("network")

/* Post-Processing. */


#ifdef __cplusplus
}
#endif

#endif /* PARAMS_H_ */

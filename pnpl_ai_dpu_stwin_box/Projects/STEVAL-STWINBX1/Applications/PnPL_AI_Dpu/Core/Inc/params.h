/**
 ******************************************************************************
 * @file    params.h
 * @author  STMicroelectronics
 * @version 1.0.0
 * @date    June 29, 2022
 *
 * @brief File generated with Handlebars.
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
//ISM330DHCX_ACC
#define ISM330DHCX_ACC_FS (16.0)
#define ISM330DHCX_ACC_ODR (1666.0)

#define IMP34DT05_MIC_FS (122.5)
#define IMP34DT05_MIC_ODR (16000.0)


/* Pre-Processing. */
//for the ACC (base)
//#define INPUT_BUFFER_SIZE (512)
//for the MIC
//#define INPUT_BUFFER_SIZE (256)
// for the MIC, but for VAD
#define INPUT_BUFFER_SIZE	(1024)	// TODO: remove or merge with HOP_LEN
#define N_FFT				(2048)
#define HOP_LEN				(1024)
#define N_MELS				(20)
#define N_DCT				(12)
#define N_MFCC_HIST			(155)	// func. of dataset (10s audio fragments), sample rate and hop_size

#define NN_QUANT_SCALE		(0.5041020512580872f)
#define NN_QUANT_ZERO		(45.0f)

// Axis selection.
#define AXIS_SELECTION_AXIS (X)
// MFCC.
#define MFCC_TRIANGULAR_FILTERS_SCALE (TRIANGULAR_FILTERS_SCALE_HZ)
#define MFCC_SIGNAL_WINDOWING (HANNING)


/* AI-Processing. */
#define NETWORK_NAME ("network")


/* Post-Processing. */


#ifdef __cplusplus
}
#endif

#endif /* PARAMS_H_ */

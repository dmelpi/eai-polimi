/**
 ******************************************************************************
 * @file    pre_processing_core.h
 * @author  STMicroelectronics
 * @version 1.0.0
 * @date    June 29, 2022
 *
 * @brief File generated with Handlebars.
 *
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT 2022 STMicroelectronics</center></h2>
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

#ifndef PRE_PROCESSING_CORE_H_
#define PRE_PROCESSING_CORE_H_


/* Includes ------------------------------------------------------------------*/

#include "arm_math.h"


/* Definitions ---------------------------------------------------------------*/

#define MFCC_TRIANGULAR_FILTERS_BANK_SIZE     (128)
#define MFCC_TRIANGULAR_FILTERS_BANK_FRACTION (0.45)


/* Exported Types ------------------------------------------------------------*/

typedef struct
{
  float32_t x;           /*  x axis  */
  float32_t y;           /*  y axis  */
  float32_t z;           /*  z axis  */
} tridimensional_data_t;

typedef struct
{
  float32_t x;           /*  x axis  */
} monodimensional_data_t;

typedef enum {
	X = 0,
	Y,
	Z
} axis_t;

typedef enum {
	RECTANGULAR = 0,
	HANNING
} signal_windowing_t;

typedef enum {
	TRIANGULAR_FILTERS_SCALE_HZ = 0,
	TRIANGULAR_FILTERS_SCALE_MEL
} triangular_filters_scale_t;


/* Exported Functions --------------------------------------------------------*/

void axis_selection(tridimensional_data_t * data_in, uint32_t data_in_size, float32_t * data_out, uint32_t data_out_size, axis_t axis);
void signal_normalization(float32_t *data_in, uint32_t data_in_size, float32_t * data_out, uint32_t data_out_size, float32_t peak_to_peak, float32_t offset);
void mean_removal (float32_t * data_in, uint32_t data_in_size, float32_t * data_out, uint32_t data_out_size);
void triangular_filters_init(uint32_t number_of_samples, uint32_t triangular_filters_bank_size, float32_t triangular_filters_bank_fraction, float32_t odr, triangular_filters_scale_t triangular_filters_scale, uint32_t* bin);
void multipliers_init(float32_t* multipliers, uint32_t data_size, signal_windowing_t signal_windowing);
void hanning(float32_t* data_in, uint32_t data_in_size, float32_t* data_out, uint32_t data_out_size, float32_t* multipliers);
void fft(float32_t *data_in, uint32_t data_in_size, float32_t * data_out, uint32_t data_out_size, arm_rfft_fast_instance_f32 * fft_handler, signal_windowing_t signal_windowing, float32_t* multipliers);
void triangular_filters_convolution(float32_t * data_in, uint32_t data_in_size, float32_t * data_out,  uint32_t data_out_size, uint32_t * bin);
void mfcc(float32_t * data_in, uint32_t data_in_size, float32_t * data_out, uint32_t data_out_size, uint32_t * bin, arm_dct4_instance_f32 * dct4f32, arm_rfft_fast_instance_f32 * fft_handler, signal_windowing_t signal_windowing, float32_t* multipliers);

#endif /* PRE_PROCESSING_CORE_H_ */

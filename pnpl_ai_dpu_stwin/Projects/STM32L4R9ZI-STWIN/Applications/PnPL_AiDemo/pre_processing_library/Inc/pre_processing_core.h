/*
 * pre_processing_core.h
 *
 *  Created on: Feb 16, 2022
 *      Author: leonardoiacussi
 */

#ifndef FEATURE_EXTRACTION_LIBRARY_INC_FEATURE_EXTRACTION_LIBRARY_H_
#define FEATURE_EXTRACTION_LIBRARY_INC_FEATURE_EXTRACTION_LIBRARY_H_

#include "arm_math.h"

#define TRIANGULAR_FILTERS_BANK_SIZE     (128)
#define TRIANGULAR_FILTERS_BANK_FRACTION (0.45f)

typedef struct
{
  float32_t x;           /*  x axis  */
  float32_t y;           /*  y axis  */
  float32_t z;           /*  z axis  */
} tridimensional_data_t;

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

typedef struct{
	arm_dct4_instance_f32 dct4f32;
	arm_rfft_fast_instance_f32 fft_handler;
	arm_status status;
	arm_cfft_radix4_instance_f32 cfftradix4f32;
	arm_rfft_instance_f32 rfftf32;
	uint32_t bin[TRIANGULAR_FILTERS_BANK_SIZE+2];
	axis_t axis;
	triangular_filters_scale_t triangular_filters_scale;
	float32_t* multipliers;
	signal_windowing_t signal_windowing;
} pre_processing_data_t;


/* Exported Functions --------------------------------------------------------*/

void axis_selection( tridimensional_data_t * data_in, uint32_t data_in_size, float32_t * data_out, uint32_t data_out_size, axis_t axis);
void signal_normalization(float32_t *data_in, uint32_t data_in_size, float32_t * data_out, uint32_t data_out_size, float32_t peak_to_peak, float32_t offset);
void mean_removal (float32_t * data_in, uint32_t data_in_size, float32_t * data_out, uint32_t data_out_size);
void triangular_filters_init(uint32_t number_of_samples, float32_t odr, triangular_filters_scale_t triangular_filters_scale, uint32_t* bin);
void multipliers_init(float32_t* multipliers, uint32_t data_size, signal_windowing_t signal_windowing);
void hanning(float32_t* data_in, uint32_t data_in_size, float32_t* data_out, uint32_t data_out_size, float32_t* multipliers);
void fft(float32_t *data_in, uint32_t data_in_size, float32_t * data_out, uint32_t data_out_size, arm_rfft_fast_instance_f32 * fft_handler, signal_windowing_t signal_windowing, float32_t* multipliers);
void triangular_filters_convolution(float32_t * data_in, uint32_t data_in_size, float32_t * data_out,  uint32_t data_out_size, uint32_t * bin);
void mfcc(float32_t * data_in, uint32_t data_in_size, float32_t * data_out, uint32_t data_out_size, uint32_t * bin, arm_dct4_instance_f32 * dct4f32, arm_rfft_fast_instance_f32 * fft_handler, signal_windowing_t signal_windowing, float32_t* multipliers);

#endif /* FEATURE_EXTRACTION_LIBRARY_INC_FEATURE_EXTRACTION_LIBRARY_H_ */

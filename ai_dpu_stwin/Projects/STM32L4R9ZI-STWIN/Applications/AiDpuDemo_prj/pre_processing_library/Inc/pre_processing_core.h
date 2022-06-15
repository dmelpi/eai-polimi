/*
 * pre_processing_core.h
 *
 *  Created on: Feb 16, 2022
 *      Author: leonardoiacussi
 */

#ifndef FEATURE_EXTRACTION_LIBRARY_INC_FEATURE_EXTRACTION_LIBRARY_H_
#define FEATURE_EXTRACTION_LIBRARY_INC_FEATURE_EXTRACTION_LIBRARY_H_


#include "AiDPU.h"
#define FILTER_BANK_SIZE 128

typedef enum {
	X=0,
	Y,
	Z
} axis_t;


typedef struct{
	arm_dct4_instance_f32 dct4f32;
	arm_rfft_fast_instance_f32 fft_handler;
	arm_status status;
	arm_cfft_radix4_instance_f32 cfftradix4f32;
	arm_rfft_instance_f32 rfftf32;
	int bin[FILTER_BANK_SIZE+2];
	axis_t axis;
} pre_processing_data_t;



/* Exported Functions --------------------------------------------------------*/
void signal_normalization(float32_t *data_in, float32_t * data_out);
void hanning(float32_t *data_in, float32_t * data_out );
void fft(float32_t *data_in ,float32_t * data_out, arm_rfft_fast_instance_f32 * fft_handler);
void mel_filters_bank(int * bin );
void mel_spectrum(float32_t * data_in, float32_t * data_out, int * bin);
void axis_selection( tridimensional_data_t * data_in  , float32_t * data_out, axis_t axis);
void mean_removal (float32_t * data_in , float32_t * data_out );
void mfcc(float32_t * data_in, float32_t * data_out, int * bin, arm_dct4_instance_f32 * dct4f32);

#endif /* FEATURE_EXTRACTION_LIBRARY_INC_FEATURE_EXTRACTION_LIBRARY_H_ */

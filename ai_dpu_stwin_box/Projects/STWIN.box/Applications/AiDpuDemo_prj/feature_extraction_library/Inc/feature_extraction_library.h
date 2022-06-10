/*
 * feature_extraction_library.h
 *
 *  Created on: Feb 16, 2022
 *      Author: leonardoiacussi
 */

#ifndef FEATURE_EXTRACTION_LIBRARY_INC_FEATURE_EXTRACTION_LIBRARY_H_
#define FEATURE_EXTRACTION_LIBRARY_INC_FEATURE_EXTRACTION_LIBRARY_H_

#define bank_size 128
#include "AiDPU.h"

/* Exported Functions --------------------------------------------------------*/
void signal_normalization(float32_t *dataIn, float32_t * dataOut);
void DoHanning(float32_t *dataIn, float32_t * dataOut );
void DoFFT(arm_rfft_fast_instance_f32 * fft_handler, float32_t *fft_in_buf ,float32_t * spectrum);
void Mel_Filters_Bank(int * bin );
void mel_spectrum(int * bin , float32_t * power_spectrum, float32_t * mel_spectra );


#endif /* FEATURE_EXTRACTION_LIBRARY_INC_FEATURE_EXTRACTION_LIBRARY_H_ */

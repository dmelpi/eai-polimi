/*
 *  preProcessingApp.c
 *
 *  Created on: May 26, 2022
 *      Author: leonardoiacussi
 *
 *
 *      In this file the signal pre-processing is computed calling single the feature-extraction functions
 *
 *      Inside preProcessing_Init function all the initialization parameters needed by the process function will be initilized
 *
 *      In preProcessing_Process
 *
 *          dataIn -> represents the raw data coming from the sensor
 *          dataOut -> represents the processed data to be fed to a neural network algorithm
 *
 */


#include "preProcessingApp.h"
#include "feature_extraction_library.h"
#include "arm_math.h"
#include "AiDPU.h"


arm_dct4_instance_f32 dct4f32;


void preProcessing_Init(){

	arm_status status;
	arm_cfft_radix4_instance_f32 cfftradix4f32;
	arm_rfft_instance_f32 rfftf32;

	arm_rfft_fast_init_f32(&fft_handler , AIDPU_NB_SAMPLE);
	status=arm_dct4_init_f32(&dct4f32,&rfftf32,&cfftradix4f32,bank_size,bank_size/2,0.125);
	if (status != ARM_MATH_SUCCESS){
		// kind of error
	}
}


void preProcessing_Process(tridimensional_data_t * data_in , float * data_out){

	// Preprocessing variables
	float32_t input_vector_mean;
	float32_t preprocessing_input_array[AIDPU_NB_SAMPLE];

	//FFT Variables
	float32_t spectrum[AIDPU_NB_SAMPLE/2];
	arm_rfft_fast_instance_f32 fft_handler;

	// filter bank variables
	int bin[bank_size+2];
	float32_t mel_spectra[bank_size];
	float32_t dct_status[bank_size*2];


    for (int i=0; i<AIDPU_NB_SAMPLE ; i++){
    	preprocessing_input_array[i]=data_in[i].y;
    }

	arm_mean_f32(preprocessing_input_array, AIDPU_NB_SAMPLE, &input_vector_mean);

	for (int i=0 ; i < AIDPU_NB_SAMPLE ; i++){
		preprocessing_input_array[i] = preprocessing_input_array[i] - input_vector_mean;
	}

	// aggiungere commenti

	DoHanning(preprocessing_input_array, preprocessing_input_array);


	DoFFT(&fft_handler, preprocessing_input_array  ,spectrum);

	Mel_Filters_Bank(bin);

	mel_spectrum(bin , spectrum, data_out);


	for (int i = 0; i<bank_size; i++){
		data_out[i] = 20*log10(data_out[i]);
	}

	arm_dct4_f32(&dct4f32, dct_status,data_out);

}


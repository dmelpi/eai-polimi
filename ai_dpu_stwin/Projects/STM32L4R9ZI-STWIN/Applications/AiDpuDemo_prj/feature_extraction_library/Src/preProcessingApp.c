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


float32_t input_vector_mean;

//FFT Variables
float32_t spectrum[AIDPU_NB_SAMPLE/2];
arm_rfft_fast_instance_f32 fft_handler;

// filter bank variables
int bin[bank_size+2];
float32_t mel_spectra[bank_size];
float32_t dct_status[bank_size*2];

float ai_results[2];

arm_cfft_radix4_instance_f32 cfftradix4f32;
arm_rfft_instance_f32 rfftf32;
arm_dct4_instance_f32 dct4f32;
arm_status status;


void preProcessing_Init(){
	arm_rfft_fast_init_f32(&fft_handler , AIDPU_NB_SAMPLE);  // da inizializzare prima
	status=arm_dct4_init_f32(&dct4f32,&rfftf32,&cfftradix4f32,bank_size,bank_size/2,0.125);
	if (status != ARM_MATH_SUCCESS){
		// kind of error
	}
}


void preProcessing_Process(float * dataIn , float * mel_spectra){

	arm_mean_f32(dataIn, AIDPU_NB_SAMPLE, &input_vector_mean);

	for (int i=0 ; i < AIDPU_NB_SAMPLE ; i++){
		dataIn[i] = dataIn[i] - input_vector_mean;
	}

	DoHanning(dataIn, dataIn);
	DoFFT(&fft_handler, dataIn  ,spectrum);

	Mel_Filters_Bank(bin);

	mel_spectrum(bin , spectrum, mel_spectra);


	for (int i = 0; i<bank_size; i++){
		mel_spectra[i] = 20*log10(mel_spectra[i]);
	}

	arm_dct4_f32(&dct4f32, dct_status,mel_spectra);

}


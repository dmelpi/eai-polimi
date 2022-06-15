/*
 *  pre_processing_app.c
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


#include "pre_processing_app.h"


void pre_processing_init(pre_processing_data_t * pre_processing_data){
	arm_status status;

	arm_rfft_fast_init_f32(&((*pre_processing_data).fft_handler), AIDPU_NB_SAMPLE);
	status=arm_dct4_init_f32(&((*pre_processing_data).dct4f32), &((*pre_processing_data).rfftf32), &((*pre_processing_data).cfftradix4f32), FILTER_BANK_SIZE, FILTER_BANK_SIZE/2, 0.125);

	if (status != ARM_MATH_SUCCESS){
		// kind of error
	}

	pre_processing_data->axis = Y;

	// calculation of the mel filter bank
	mel_filters_bank((*pre_processing_data).bin);
}


void pre_processing_process(tridimensional_data_t * data_in , float32_t * data_out, pre_processing_data_t * pre_processing_data){

	/*
	 ################# SIGNAL PROCESSING #####################
	 */

	// select one single axis (y axis)
	tridimensional_data_t * data_in_1 = data_in;
	float32_t data_out_1[AIDPU_NB_SAMPLE];

	axis_selection(data_in_1  ,  data_out_1, pre_processing_data->axis);

	float32_t * data_in_2 = data_out_1;
	float32_t data_out_2[AIDPU_NB_SAMPLE];

	mean_removal(data_in_2 , data_out_2 );

	// apply hanning window to the signal before doing fft
	float32_t * data_in_3 = data_out_2;
	float32_t data_out_3[AIDPU_NB_SAMPLE];

	hanning(data_in_3, data_out_3);

	// do the fft transform of the signal
	float32_t * data_in_4 = data_out_3;
	float32_t data_out_4[AIDPU_NB_SAMPLE/2];

	fft(data_in_4, data_out_4 , &((*pre_processing_data).fft_handler));

	// multiplication of the filter bank to the spectrum data and calculate and bring it to the log scale
	float32_t * data_in_5 = data_out_4;
	float32_t data_out_5[FILTER_BANK_SIZE];

	mfcc(data_in_5, data_out_5, ((*pre_processing_data).bin), &((*pre_processing_data).dct4f32));
	data_out = data_out_5;
}


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

#include "services/sysdebug.h"
#include "pre_processing_app.h"
#define SYS_DEBUGF(level, message)      SYS_DEBUGF3(SYS_DBG_AI, level, message)

void pre_processing_init(pre_processing_data_t * pre_processing_data){
	arm_status status;

	arm_rfft_fast_init_f32(&((*pre_processing_data).fft_handler), INPUT_BUFFER_SIZE);
	status=arm_dct4_init_f32(&((*pre_processing_data).dct4f32), &((*pre_processing_data).rfftf32), &((*pre_processing_data).cfftradix4f32), FILTER_BANK_SIZE, FILTER_BANK_SIZE/2, 0.125);

	if (status != ARM_MATH_SUCCESS){
		// kind of error
	}

	pre_processing_data->axis = Y;
	pre_processing_data->signal_windowing = HANNING;

	// calculation of the mel filter bank
	mel_filters_bank((*pre_processing_data).bin);
}


void pre_processing_process(tridimensional_data_t * data_in , uint32_t data_in_size , float32_t * data_out, uint32_t data_out_size, pre_processing_data_t * pre_processing_data){

	/*
	 ################# SIGNAL PRE PROCESSING #####################
	 */

	// accelerometer axis selection among X, Y, Z
	float32_t data_1[data_in_size];
	axis_selection(data_in, data_in_size, data_1, data_in_size, pre_processing_data->axis);

	// remove mean value from the signal
	float32_t data_2[data_in_size];
	mean_removal(data_1, data_in_size, data_2, data_in_size);

	// Mel Frequency Cepstral Coefficient calculation
	mfcc(data_2, data_in_size, data_out, data_out_size, ((*pre_processing_data).bin), &((*pre_processing_data).dct4f32), &((*pre_processing_data).fft_handler), (*pre_processing_data).signal_windowing);

}


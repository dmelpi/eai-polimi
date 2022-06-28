/*
 *  pre_processing_app.c
 *
 *  Created on: May 26, 2022
 *      Author: leonardoiacussi
 *
 *
 *      In this file the signal pre-processing is computed calling single feature-extraction functions.
 *
 *      Inside the pre_processing_init() function all the initialization parameters needed by the process function are initilized.
 *
 *      Inside the pre_processing_process() function all the functions are called sequentially.
 *          data_in: represents the raw data coming from the sensor;
 *          data_out: represents the processed data to be fed to a neural network algorithm.
 *
 */

#include "services/syserror.h"
#include "arm_math.h"
#include "params.h"
#include "pre_processing_app.h"
#include <stdlib.h>

void pre_processing_init(pre_processing_data_t * pre_processing_data) {
	// axis_selection.
    pre_processing_data->axis = SELECTED_AXIS;

    // mfcc.
    arm_status status;
    arm_rfft_fast_init_f32(&((*pre_processing_data).fft_handler), INPUT_BUFFER_SIZE);
    status = arm_dct4_init_f32(&((*pre_processing_data).dct4f32), &((*pre_processing_data).rfftf32), &((*pre_processing_data).cfftradix4f32), TRIANGULAR_FILTERS_BANK_SIZE, TRIANGULAR_FILTERS_BANK_SIZE/2, 0.125);
    if (status != ARM_MATH_SUCCESS) {
        sys_error_handler();
    }
    pre_processing_data->triangular_filters_scale = TRIANGULAR_FILTERS_SCALE;
    pre_processing_data->signal_windowing = SIGNAL_WINDOWING;
    triangular_filters_init(INPUT_BUFFER_SIZE, ISM330DHCX_ACC_ODR, pre_processing_data->triangular_filters_scale, (*pre_processing_data).bin);
    (*pre_processing_data).multipliers = (float32_t*) calloc(INPUT_BUFFER_SIZE, sizeof(float32_t));
    multipliers_init((*pre_processing_data).multipliers, INPUT_BUFFER_SIZE, (*pre_processing_data).signal_windowing);


}

void pre_processing_process(tridimensional_data_t * data_in , uint32_t data_in_size , float32_t * data_out, uint32_t data_out_size, pre_processing_data_t * pre_processing_data) {
    // axis_selection.
    float32_t data_0[data_in_size];
    axis_selection(data_in, data_in_size, data_0, data_in_size, pre_processing_data->axis);

    // mean_removal.
    float32_t data_1[data_in_size];
    mean_removal(data_0, data_in_size, data_1, data_in_size);

    // mfcc.
    mfcc(data_1, data_in_size, data_out, data_out_size, ((*pre_processing_data).bin), &((*pre_processing_data).dct4f32), &((*pre_processing_data).fft_handler), (*pre_processing_data).signal_windowing, (*pre_processing_data).multipliers);


}

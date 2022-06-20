/*
 *  pre_processing_app.h
 *
 *  Created on: May 26, 2022
 *      Author: leonardoiacussi
 */

#ifndef PREPROCESSINGAPP_INC_PREPROCESSINGAPP_H_
#define PREPROCESSINGAPP_INC_PREPROCESSINGAPP_H_

#include "AiDPU.h"
#include "arm_math.h"
#include "pre_processing_core.h"


/* Exported Functions --------------------------------------------------------*/
void pre_processing_init(pre_processing_data_t * pre_processing_data);
void pre_processing_process(tridimensional_data_t * data_in , uint32_t data_in_size , float32_t * data_out, uint32_t data_out_size, pre_processing_data_t * pre_processing_data);


#endif /* PREPROCESSINGAPP_INC_PREPROCESSINGAPP_H_ */

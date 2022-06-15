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
void pre_processing_process(tridimensional_data_t * data_in , float32_t * data_out, pre_processing_data_t * pre_processing_data);


#endif /* PREPROCESSINGAPP_INC_PREPROCESSINGAPP_H_ */

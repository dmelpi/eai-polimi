/*
 *  preProcessingApp.h
 *
 *  Created on: May 26, 2022
 *      Author: leonardoiacussi
 */

#ifndef PREPROCESSINGAPP_INC_PREPROCESSINGAPP_H_
#define PREPROCESSINGAPP_INC_PREPROCESSINGAPP_H_

#include "AiDPU.h"

/* Exported Functions --------------------------------------------------------*/
void preProcessing_Init();
void preProcessing_Process(tridimensional_data_t *  , float * );


#endif /* PREPROCESSINGAPP_INC_PREPROCESSINGAPP_H_ */

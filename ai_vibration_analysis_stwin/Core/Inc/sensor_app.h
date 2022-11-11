/*
 * sensor_app.h
 *
 *  Created on: Oct 21, 2022
 *      Author: leonardoiacussi
 */

#ifndef INC_SENSOR_APP_H_
#define INC_SENSOR_APP_H_

#include "main.h"
#include "pre_processing_app.h"
#include "aiApp.h"
#include "params.h"

#include "STWIN.h"
#include "stm32l4xx_it.h"
#include "stm32l4xx_hal_conf.h"
#include "app_mems_int_pin_a_interface.h"
#include "STWIN_motion_sensors_ex.h"


void sensor_init(void);
void sensor_process(void);


#endif /* INC_SENSOR_APP_H_ */

/*
 * sensor_app.c
 *
 *  Created on: Oct 21, 2022
 *      Author: leonardoiacussi
 */

#include "sensor_app.h"
#include "STWIN.box_motion_sensors.h"
#include "usb_device.h"
#include <stdio.h>
#include "arm_math.h"
#include "usbd_cdc_interface.h"

// define number of samples in the ISM330DHCX FIFO BUFFER
#define NUMBER_OF_SAMPLES 256  // max 256 samples
#define MAX_STRING_BUF_SIZE 30

// printing variables
static char string_out[MAX_STRING_BUF_SIZE];

// error return variable
int32_t ret=0;
uint8_t ret_uint=0;

// Pre processing init variable
pre_processing_data_t pre_processing_data;
static int32_t PushButtonState = GPIO_PIN_RESET;
float ism330dhcx_acc_sensitivity;
HAL_StatusTypeDef ret_hal_status;
uint8_t ButtonEventDetected;
uint8_t MemsEventDetected;
volatile int BP_Interrupt = 0;
uint32_t num_samples =0;


// process variables
int16_t DataOut[3];
tridimensional_data_t ism330dhcx_acc[NUMBER_OF_SAMPLES];
float32_t preprocessing_output_array[NUMBER_OF_SAMPLES/2];
float ai_out[2];

// time measurement variables
int32_t time_elapsed=0;
int32_t post_FIFO_read;
int32_t pre_FIFO_read;

void sensor_init(void){

	// init GREEN LED and Turn it on
	BSP_LED_Init(LED1);

	// init user button
	ret = BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);
	PushButtonState = (BSP_PB_GetState(BUTTON_USER)) ?  0 : 1;


	// init mems exti 8
	set_mems_int_pin_a_exti();

	// init ism330dhcx sensor
	ret_uint = Sensor_IO_SPI_CS_Init_All();
	ret = BSP_MOTION_SENSOR_Init(ISM330DHCX_0,MOTION_ACCELERO);
	ret = BSP_MOTION_SENSOR_SetOutputDataRate(ISM330DHCX_0, MOTION_ACCELERO,ISM330DHCX_ODR);
	ret = BSP_MOTION_SENSOR_FIFO_Set_BDR( ISM330DHCX_0,  MOTION_ACCELERO, ISM330DHCX_ODR);
	ret = BSP_MOTION_SENSOR_SetFullScale( ISM330DHCX_0,  MOTION_ACCELERO, ISM330DHCX_FS);


	// get sensor sensitivity
	ret = BSP_MOTION_SENSOR_GetSensitivity(ISM330DHCX_0, MOTION_ACCELERO, &ism330dhcx_acc_sensitivity);

	// sensor fifo init
	ret = BSP_MOTION_SENSOR_FIFO_Set_INT1_FIFO_Full(ISM330DHCX_0, ENABLE);
	ret = BSP_MOTION_SENSOR_FIFO_Set_Watermark_Level( ISM330DHCX_0, INPUT_BUFFER_SIZE);
	ret = BSP_MOTION_SENSOR_FIFO_Set_Stop_On_Fth(ISM330DHCX_0,  ENABLE);
	ret = BSP_MOTION_SENSOR_FIFO_Set_Mode(ISM330DHCX_0,  ISM330DHCX_STREAM_MODE);

	// AI init
	aiInit();
	// PRE PROCESSING init
	pre_processing_init(&pre_processing_data);

}

void sensor_process(void){

	/*if(BP_Interrupt == 1){
		BP_Interrupt = 0;

		// Debouncing
		if(ButtonEventDetected==0){
			// Debouncing
			HAL_Delay(10);
			if(BSP_PB_GetState(BUTTON_USER) == 1){
				ButtonEventDetected = 1;
			}
			else{
				ButtonEventDetected = 0;
			}
		}
		else{
			// Debouncing
			HAL_Delay(10);
			if(BSP_PB_GetState(BUTTON_USER) == 1){
				ButtonEventDetected = 0;
			}
			else{
				ButtonEventDetected = 1;
			}
		}
	}*/

	/*if(ButtonEventDetected==1){
		ButtonEventDetected = 0;*/
	if (MemsEventDetected == 1){

		//pre_FIFO_read = BSP_GetTick();
		MemsEventDetected = 0;

		BSP_LED_On(LED1);

		for(int jj=0;jj<num_samples;jj++){
			BSP_MOTION_SENSOR_FIFO_Get_Data_Word(ISM330DHCX_0, MOTION_ACCELERO, &DataOut);
			ism330dhcx_acc[jj].x = ((float)DataOut[0])*ism330dhcx_acc_sensitivity*0.001; // from [mg] to [g]
			ism330dhcx_acc[jj].y = ((float)DataOut[1])*ism330dhcx_acc_sensitivity*0.001;
			ism330dhcx_acc[jj].z = ((float)DataOut[2])*ism330dhcx_acc_sensitivity*0.001;
		}

		// call pre_processing_functions
		pre_processing_process(ism330dhcx_acc, INPUT_BUFFER_SIZE, preprocessing_output_array, INPUT_BUFFER_SIZE/2, &pre_processing_data);

		// call ai process
		aiProcess(preprocessing_output_array, ai_out);

		// print to Virtual COM
		snprintf(string_out, sizeof(string_out), "label: %.0f , Accuracy: %.2f \r\n", ai_out[0], ai_out[1]);
		CDC_Transmit_FS((uint8_t*)string_out, strlen(string_out));

		// print to STLINK
		//printf("Class: %f, Accuracy: %f \r\n",ai_out[0],ai_out[1]);

		BSP_LED_Off(LED1);
		//post_FIFO_read = BSP_GetTick();

		//time_elapsed = post_FIFO_read - pre_FIFO_read;



	}

	//Get number of unread FIFO samples
	BSP_MOTION_SENSOR_FIFO_Get_Num_Samples(ISM330DHCX_0, &num_samples);
	//}


	/*
	  if (MemsEventDetected == 1){

		  //pre_FIFO_read = BSP_GetTick();
		  BSP_LED_On(LED1);

		  // Read data from FIFO (no DMA)
		  for (int jj=0; jj<NUMBER_OF_SAMPLES; jj++){
			  BSP_MOTION_SENSOR_FIFO_Get_Data_Word(ISM330DHCX_0, MOTION_ACCELERO, &DataOut);
			  ism330dhcx_acc[jj].x = ((float)DataOut[0])*ism330dhcx_acc_sensitivity*0.001; // from [mg] to [g]
			  ism330dhcx_acc[jj].y = ((float)DataOut[1])*ism330dhcx_acc_sensitivity*0.001;
			  ism330dhcx_acc[jj].z = ((float)DataOut[2])*ism330dhcx_acc_sensitivity*0.001;
		  }

		  // call pre_processing_functions
		  pre_processing_process(ism330dhcx_acc, INPUT_BUFFER_SIZE, preprocessing_output_array, INPUT_BUFFER_SIZE/2, &pre_processing_data);

		  // call ai process
		  //aiProcess(preprocessing_output_array, ai_out);

		  // print to Virtual COM
		  //snprintf(string_out, sizeof(string_out), "label: %.0f , Accuracy: %.2f \r\n", ai_out[0], ai_out[1]);
		  //CDC_Transmit_FS((uint8_t*)string_out, strlen(string_out));
		  printf("AccX: %f, AccY: %f, AccZ: %f \r\n");


		  BSP_LED_Off(LED1);
		  MemsEventDetected = 0;

	  }
	}*/
}


void BSP_PB_Callback(Button_TypeDef Button)
{
	BP_Interrupt=1;
}



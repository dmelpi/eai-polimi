/**
 ******************************************************************************
 * @file    pre_processing_core.c
 * @author  STMicroelectronics
 * @version 1.0.0
 * @date    June 29, 2022
 *
 * @brief File generated with Handlebars.
 *
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT 2022 STMicroelectronics</center></h2>
 *
 * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *        http://www.st.com/software_license_agreement_liberty_v2
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************
 */

#include "pre_processing_core.h"
#include "arm_math.h"
#include "AiDPU.h"
#include "services/sysmem.h"

/*----------------------------------------------------------------------------*/
/* Support functions          			      						          */
/*----------------------------------------------------------------------------*/

// calculation of the complex absolute value
float complex_abs(float real, float compl) {
	return sqrtf(real*real+compl*compl);
}

// Dot product calculation using CMSIS DSP library
float32_t dot_product(float32_t* in_1, uint32_t data_in1_size, float32_t* in_2, uint32_t data_in2_size) {

	float32_t multOutput[data_in1_size];
	float32_t dot_out=0;
	arm_mult_f32(in_1, in_2, multOutput, data_in1_size);
	for (int i=0;  i<data_in1_size;   i++){
		arm_add_f32(&dot_out, &multOutput[i], &dot_out, 1);
	}
	return dot_out;
}

// conversion between Hz and Mel
float hz_to_mel(float f_Hz_in) {
	return 2595*log10(1+f_Hz_in/700);
}

float mel_to_hz(float f_Mel_in) {
	return (700*(pow(10,f_Mel_in/2505)-1));
}


/*----------------------------------------------------------------------------*/
/* Signal pre-processing :  Axis selection        						      */
/*----------------------------------------------------------------------------*/

void axis_selection(tridimensional_data_t * data_in, uint32_t data_in_size, float32_t * data_out, uint32_t data_out_size, axis_t axis) {
	if (axis == X)
	{
		for (int i=0; i<data_out_size ; i++){
	    	data_out[i]=data_in[i].x;
	    }
	}
	else if (axis == Y)
	{
		for (int i=0; i<data_out_size ; i++){
	    	data_out[i]=data_in[i].y;
	    }
	}
	else if (axis == Z)
	{
		for (int i=0; i<data_out_size ; i++){
	    	data_out[i]=data_in[i].z;
	    }
	}
}


/*----------------------------------------------------------------------------*/
/* Signal pre-processing :  Signal normalization between -1 and 1             */
/*----------------------------------------------------------------------------*/

void signal_normalization(float32_t *data_in, uint32_t data_in_size, float32_t * data_out, uint32_t data_out_size, float32_t peak_to_peak, float32_t offset) {

	float32_t max, min;
	uint32_t maxpos, minpos;

	arm_max_f32(data_in,data_in_size,&max,&maxpos);
	arm_min_f32(data_in,data_in_size,&min,&minpos);

	for(int i=0; i<data_out_size ; i++){
		data_out[i] = ((data_in[i] - min)/(max - min))*peak_to_peak - ((peak_to_peak / 2) + offset);
	}
}


/*----------------------------------------------------------------------------*/
/* Signal pre-processing :  Remove mean from signal     			          */
/*----------------------------------------------------------------------------*/

void mean_removal (float32_t * data_in, uint32_t data_in_size, float32_t * data_out, uint32_t data_out_size) {

	static float32_t mean;

	// calculation of the signal mean value
	arm_mean_f32(data_in, data_in_size, &mean);

	// subtract the mean value from the signal in order to remove gravity effect on the accelerometric data
	for (int i=0 ; i < data_out_size ; i++){
		data_out[i] = data_in[i] - mean;
	}
}


/*----------------------------------------------------------------------------*/
/* Signal pre-processing :  Calculation of the triangular filters bank        */
/*----------------------------------------------------------------------------*/

void triangular_filters_init(uint32_t number_of_samples, uint32_t triangular_filters_bank_size, float32_t triangular_filters_bank_fraction, float32_t odr, triangular_filters_scale_t triangular_filters_scale, uint32_t* bin) {

	float32_t low_freq;
	float32_t high_freq;
	float32_t* Hz_points = (float32_t*) SysAlloc((triangular_filters_bank_size + 2) * sizeof(float32_t));
	float32_t d_hz_points;
	float32_t bin_sep;
	float32_t f_max;
	float32_t f_min = 0.0;

	f_max = odr * triangular_filters_bank_fraction;

	if (triangular_filters_scale == TRIANGULAR_FILTERS_SCALE_MEL) {
	    low_freq = hz_to_mel(f_min);
	    high_freq = hz_to_mel(f_max);
	} else {
        low_freq = f_min;
	    high_freq = f_max;
	}

	d_hz_points = (high_freq - low_freq) / (float32_t) (triangular_filters_bank_size + 2);

	bin_sep = odr / (float32_t) number_of_samples;

	for (int i = 0; i < triangular_filters_bank_size + 2; i++){
		Hz_points[i] = (float32_t) (low_freq + i * d_hz_points);
		bin[i] = round((Hz_points[i] / bin_sep));
	}

	SysFree(Hz_points);
}


/*----------------------------------------------------------------------------*/
/* Signal pre-processing :  Initializing the multipliers array                */
/*----------------------------------------------------------------------------*/

void multipliers_init(float32_t* multipliers, uint32_t data_size, signal_windowing_t signal_windowing) {
	switch (signal_windowing) {
		case HANNING:
		{
			for (int i = 0; i < data_size; i++) {
				multipliers[i] = 0.5 * (1 - arm_cos_f32((float32_t) 2*PI*i/(data_size-1)));
			}
			break;
		}
		default: {
			// no window applied (rectangular case)
			break;
		}
	}
}


/*----------------------------------------------------------------------------*/
/* Signal pre-processing :  Multiply the Hanning window to the input signal   */
/*----------------------------------------------------------------------------*/

void hanning(float32_t* data_in, uint32_t data_in_size, float32_t* data_out, uint32_t data_out_size, float32_t* multipliers) {
	for (int i = 0; i < data_in_size; i++) {
		data_out[i] = multipliers[i] * data_in[i];
	}
}


/*----------------------------------------------------------------------------*/
/* Signal pre-processing :  FFT Calculation using CMSIS DSP Library           */
/*----------------------------------------------------------------------------*/

void fft(float32_t *data_in, uint32_t data_in_size, float32_t * data_out, uint32_t data_out_size, arm_rfft_fast_instance_f32 * fft_handler, signal_windowing_t signal_windowing, float32_t* multipliers) {

	float32_t fft_out_buf[data_in_size];

	switch (signal_windowing){
		case HANNING:
		{
			hanning(data_in, data_in_size, data_in, data_in_size, multipliers);
			break;
		}
		default: {
			// no window applied (rectangular case)
			break;
		}
	}

	arm_rfft_fast_f32(fft_handler, (float32_t*)  data_in, (float32_t*) fft_out_buf, 0);

	int freqpoint = 0;

	for (int i=0;   i<data_in_size   ; i=i+2) {
	  data_out[freqpoint] =(complex_abs(fft_out_buf[i], fft_out_buf[i+1])); //(sqrt(data_in_size));
	  //data_out[freqpoint] = 2 * data_out[freqpoint]*data_out[freqpoint] /(float32_t)ISM330DHCX_ODR;

	  if(data_out[freqpoint] < 1e-3){
		  data_out[freqpoint] = 1e-3;
	  }
	  freqpoint++;
	}
}


/*----------------------------------------------------------------------------*/
/* Signal pre-processing :  Convolution of the triangular filters bank        */
/*----------------------------------------------------------------------------*/

void triangular_filters_convolution(float32_t * data_in, uint32_t data_in_size, float32_t * data_out,  uint32_t data_out_size, uint32_t * bin) {

	int f_m_minus=0, f_m=0, f_m_plus=0;
	float32_t in_vector[data_in_size];

	for (int m=1; m < data_out_size+1; m++){
		f_m_minus = bin[m-1];
		f_m = bin[m];
		f_m_plus = bin[m+1];

		for (int i=0; i<data_in_size   ;i++){
			in_vector[i] = (float32_t) 0.0;
		}
		for (int k=f_m_minus;k < f_m;k++){
			in_vector[k] = (((float32_t)k-f_m_minus)/(f_m-f_m_minus));
		}
		for (int j=f_m; j<f_m_plus;j++){
			in_vector[j] = (float32_t)((f_m_plus-(float32_t)j)/(f_m_plus-f_m));
		}
		data_out[m-1] = dot_product(in_vector, data_in_size,data_in, data_in_size);
	}
}


/*----------------------------------------------------------------------------*/
/* Signal pre-processing :  MFCC		        					          */
/*----------------------------------------------------------------------------*/

void mfcc(float32_t * data_in, uint32_t data_in_size, float32_t * data_out, uint32_t data_out_size, uint32_t * bin, arm_dct4_instance_f32 * dct4f32, arm_rfft_fast_instance_f32 * fft_handler,  signal_windowing_t signal_windowing, float32_t* multipliers) {

	float32_t dct_status[data_out_size*2];
	float32_t fft_out[data_in_size/2];

	fft(data_in, data_in_size, fft_out, data_in_size/2, fft_handler, signal_windowing, multipliers);

	triangular_filters_convolution(fft_out, data_in_size/2, data_out, data_out_size, bin);
	for (int i = 0; i<data_out_size; i++) {
		data_out[i] = 20*log10(data_out[i]);
	}

	// apply the direct cosine transform in order to have the MFCC
	arm_dct4_f32(dct4f32, dct_status, data_out);
}

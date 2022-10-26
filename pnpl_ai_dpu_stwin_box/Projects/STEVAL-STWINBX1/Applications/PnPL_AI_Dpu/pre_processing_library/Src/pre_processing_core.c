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
	return (700*(pow(10,f_Mel_in/2595)-1));
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
	//float32_t real_multiplier[512]={0.0, 0.0, 0.0002, 0.0003, 0.0006, 0.0009, 0.0014, 0.0019, 0.0024, 0.0031, 0.0038, 0.0046, 0.0054, 0.0064, 0.0074, 0.0085, 0.0096, 0.0109, 0.0122, 0.0136, 0.015, 0.0166, 0.0182, 0.0199, 0.0216, 0.0234, 0.0253, 0.0273, 0.0293, 0.0315, 0.0336, 0.0359, 0.0382, 0.0406, 0.0431, 0.0456, 0.0482, 0.0509, 0.0536, 0.0564, 0.0593, 0.0622, 0.0652, 0.0683, 0.0714, 0.0746, 0.0779, 0.0812, 0.0846, 0.088, 0.0916, 0.0951, 0.0988, 0.1025, 0.1062, 0.11, 0.1139, 0.1179, 0.1219, 0.1259, 0.13, 0.1342, 0.1384, 0.1427, 0.147, 0.1514, 0.1558, 0.1603, 0.1648, 0.1694, 0.1741, 0.1787, 0.1835, 0.1883, 0.1931, 0.198, 0.2029, 0.2079, 0.2129, 0.2179, 0.223, 0.2282, 0.2333, 0.2386, 0.2438, 0.2491, 0.2544, 0.2598, 0.2652, 0.2707, 0.2762, 0.2817, 0.2872, 0.2928, 0.2984, 0.3041, 0.3097, 0.3154, 0.3212, 0.3269, 0.3327, 0.3385, 0.3443, 0.3502, 0.3561, 0.362, 0.3679, 0.3738, 0.3798, 0.3857, 0.3917, 0.3977, 0.4038, 0.4098, 0.4159, 0.4219, 0.428, 0.4341, 0.4402, 0.4463, 0.4524, 0.4585, 0.4647, 0.4708, 0.477, 0.4831, 0.4892, 0.4954, 0.5015, 0.5077, 0.5138, 0.52, 0.5261, 0.5323, 0.5384, 0.5445, 0.5506, 0.5567, 0.5628, 0.5689, 0.575, 0.5811, 0.5872, 0.5932, 0.5992, 0.6053, 0.6113, 0.6172, 0.6232, 0.6292, 0.6351, 0.641, 0.6469, 0.6528, 0.6586, 0.6644, 0.6702, 0.676, 0.6817, 0.6874, 0.6931, 0.6988, 0.7044, 0.71, 0.7156, 0.7211, 0.7266, 0.732, 0.7375, 0.7429, 0.7482, 0.7535, 0.7588, 0.7641, 0.7693, 0.7744, 0.7795, 0.7846, 0.7896, 0.7946, 0.7996, 0.8045, 0.8093, 0.8141, 0.8189, 0.8236, 0.8283, 0.8329, 0.8374, 0.842, 0.8464, 0.8508, 0.8552, 0.8595, 0.8637, 0.8679, 0.8721, 0.8761, 0.8802, 0.8841, 0.888, 0.8919, 0.8957, 0.8994, 0.9031, 0.9067, 0.9102, 0.9137, 0.9171, 0.9205, 0.9238, 0.927, 0.9302, 0.9333, 0.9363, 0.9393, 0.9422, 0.945, 0.9478, 0.9505, 0.9531, 0.9557, 0.9582, 0.9606, 0.963, 0.9652, 0.9675, 0.9696, 0.9717, 0.9737, 0.9756, 0.9775, 0.9793, 0.981, 0.9826, 0.9842, 0.9857, 0.9871, 0.9885, 0.9897, 0.9909, 0.9921, 0.9931, 0.9941, 0.995, 0.9958, 0.9966, 0.9973, 0.9979, 0.9984, 0.9989, 0.9992, 0.9995, 0.9998, 0.9999, 1.0, 1.0, 0.9999, 0.9998, 0.9995, 0.9992, 0.9989, 0.9984, 0.9979, 0.9973, 0.9966, 0.9958, 0.995, 0.9941, 0.9931, 0.9921, 0.9909, 0.9897, 0.9885, 0.9871, 0.9857, 0.9842, 0.9826, 0.981, 0.9793, 0.9775, 0.9756, 0.9737, 0.9717, 0.9696, 0.9675, 0.9652, 0.963, 0.9606, 0.9582, 0.9557, 0.9531, 0.9505, 0.9478, 0.945, 0.9422, 0.9393, 0.9363, 0.9333, 0.9302, 0.927, 0.9238, 0.9205, 0.9171, 0.9137, 0.9102, 0.9067, 0.9031, 0.8994, 0.8957, 0.8919, 0.888, 0.8841, 0.8802, 0.8761, 0.8721, 0.8679, 0.8637, 0.8595, 0.8552, 0.8508, 0.8464, 0.842, 0.8374, 0.8329, 0.8283, 0.8236, 0.8189, 0.8141, 0.8093, 0.8045, 0.7996, 0.7946, 0.7896, 0.7846, 0.7795, 0.7744, 0.7693, 0.7641, 0.7588, 0.7535, 0.7482, 0.7429, 0.7375, 0.732, 0.7266, 0.7211, 0.7156, 0.71, 0.7044, 0.6988, 0.6931, 0.6874, 0.6817, 0.676, 0.6702, 0.6644, 0.6586, 0.6528, 0.6469, 0.641, 0.6351, 0.6292, 0.6232, 0.6172, 0.6113, 0.6053, 0.5992, 0.5932, 0.5872, 0.5811, 0.575, 0.5689, 0.5628, 0.5567, 0.5506, 0.5445, 0.5384, 0.5323, 0.5261, 0.52, 0.5138, 0.5077, 0.5015, 0.4954, 0.4892, 0.4831, 0.477, 0.4708, 0.4647, 0.4585, 0.4524, 0.4463, 0.4402, 0.4341, 0.428, 0.4219, 0.4159, 0.4098, 0.4038, 0.3977, 0.3917, 0.3857, 0.3798, 0.3738, 0.3679, 0.362, 0.3561, 0.3502, 0.3443, 0.3385, 0.3327, 0.3269, 0.3212, 0.3154, 0.3097, 0.3041, 0.2984, 0.2928, 0.2872, 0.2817, 0.2762, 0.2707, 0.2652, 0.2598, 0.2544, 0.2491, 0.2438, 0.2386, 0.2333, 0.2282, 0.223, 0.2179, 0.2129, 0.2079, 0.2029, 0.198, 0.1931, 0.1883, 0.1835, 0.1787, 0.1741, 0.1694, 0.1648, 0.1603, 0.1558, 0.1514, 0.147, 0.1427, 0.1384, 0.1342, 0.13, 0.1259, 0.1219, 0.1179, 0.1139, 0.11, 0.1062, 0.1025, 0.0988, 0.0951, 0.0916, 0.088, 0.0846, 0.0812, 0.0779, 0.0746, 0.0714, 0.0683, 0.0652, 0.0622, 0.0593, 0.0564, 0.0536, 0.0509, 0.0482, 0.0456, 0.0431, 0.0406, 0.0382, 0.0359, 0.0336, 0.0315, 0.0293, 0.0273, 0.0253, 0.0234, 0.0216, 0.0199, 0.0182, 0.0166, 0.015, 0.0136, 0.0122, 0.0109, 0.0096, 0.0085, 0.0074, 0.0064, 0.0054, 0.0046, 0.0038, 0.0031, 0.0024, 0.0019, 0.0014, 0.0009, 0.0006, 0.0003, 0.0002, 0.0, 0.0 };
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

	  if(data_out[freqpoint] < 1e-6){
		  data_out[freqpoint] = 1e-6;
	  }
	  data_out[freqpoint] = 20*log10(data_out[freqpoint]);
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
		if (data_out[i] < 10e-10){
			data_out[i] = 10e-10;
		}
		data_out[i] = 20*log10(data_out[i]);
	}

	// apply the direct cosine transform in order to have the MFCC
	arm_dct4_f32(dct4f32, dct_status, data_out);
}

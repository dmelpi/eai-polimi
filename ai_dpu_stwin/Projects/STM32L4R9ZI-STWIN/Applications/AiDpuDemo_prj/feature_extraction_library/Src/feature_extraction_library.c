/*
 * feature_extraction_library.c
 *
 *  Created on: Feb 16, 2022
 *      Author: leonardoiacussi
 */


#include "feature_extraction_library.h"
#include "arm_math.h"
#include "AiDPU.h"





/*----------------------------------------------------------------------------*/
/* Support functions          			      						          */
/*----------------------------------------------------------------------------*/

// calculation of the complex absolute value
float complexABS(float real, float compl) {
	return sqrtf(real*real+compl*compl);
}



// Dot product calculation using CMSIS DSP library
float32_t dot_product(float32_t* in_1, float32_t* in_2){

	float32_t multOutput[AIDPU_NB_SAMPLE/2];
	float32_t dot_out=0;
	arm_mult_f32(in_1, in_2, multOutput, AIDPU_NB_SAMPLE/2);
	for (int i=0;  i<AIDPU_NB_SAMPLE/2;   i++){
		arm_add_f32(&dot_out, &multOutput[i], &dot_out, 1);
	}
	return dot_out;
}

// conversion between Hz and Mel
float Hz_to_Mel(float f_Hz_in) {
	return 2595*log10(1+f_Hz_in/700);
}

float Mel_to_Hz(float f_Mel_in) {
	return (700*(pow(10,f_Mel_in/2505)-1));
}


/*----------------------------------------------------------------------------*/
/* Signal pre-processing :  Signal normalization between -1 and 1             */
/*----------------------------------------------------------------------------*/

// generalizzare
void signal_normalization(float32_t *dataIn, float32_t * dataOut){

	float32_t max, min;
	uint32_t maxpos, minpos;

	arm_max_f32(dataIn,AIDPU_NB_SAMPLE,&max,&maxpos);
	arm_min_f32(dataIn,AIDPU_NB_SAMPLE,&min,&minpos);

	for(int i=0; i<AIDPU_NB_SAMPLE ; i++){
		dataOut[i] = ((dataIn[i] - min)/(max - min))*2 - 1;
	}
}


/*----------------------------------------------------------------------------*/
/* Signal pre-processing :  Multiply the Hanning window to the input signal   */
/*----------------------------------------------------------------------------*/

void DoHanning(float32_t *dataIn, float32_t * dataOut ){
	for (int i = 0; i < AIDPU_NB_SAMPLE; i++) {
		float32_t multiplier = 0.5 * (1 - arm_cos_f32((float32_t) 2*PI*i/(AIDPU_NB_SAMPLE-1)));
	    dataOut[i] = multiplier * dataIn[i];
	}
}


/*----------------------------------------------------------------------------*/
/* Signal pre-processing :  FFT Calculation using CMSIS DSP Library           */
/*----------------------------------------------------------------------------*/

void DoFFT(arm_rfft_fast_instance_f32 * fft_handler, float32_t *fft_in_buf ,float32_t * spectrum){

	static float32_t fft_out_buf[AIDPU_NB_SAMPLE];
	arm_rfft_fast_f32(fft_handler, (float32_t*)  fft_in_buf, (float32_t*) fft_out_buf, 0);

	int freqpoint = 0;

	for (int i=0;   i<AIDPU_NB_SAMPLE   ; i=i+2) {
	  spectrum[freqpoint] =(complexABS(fft_out_buf[i], fft_out_buf[i+1]))/(sqrt(AIDPU_NB_SAMPLE));
	  spectrum[freqpoint] = 2 * spectrum[freqpoint]*spectrum[freqpoint] /(float32_t)SET_ODR;
	  //spectrum[freqpoint] = 20 * log10(spectrum[freqpoint]);
	  freqpoint++;
	}
}


/*----------------------------------------------------------------------------*/
/* Signal pre-processing :  Calculation of the MEL Filters bank               */
/*----------------------------------------------------------------------------*/

void Mel_Filters_Bank(int * bin ){

	static float low_freq_mel;
	static float high_freq_mel;
	static float Hz_points[bank_size+2];
	static float d_hz_points;
	static float bin_sep;
	static float f_max;
	static float f_min = 0.0;


	f_max = (float)SET_ODR * 0.45;

	low_freq_mel = Hz_to_Mel(f_min);
	high_freq_mel = Hz_to_Mel(f_max);
	d_hz_points = (high_freq_mel-low_freq_mel)/(float32_t)( bank_size+2);

	bin_sep=SET_ODR/(float32_t)AIDPU_NB_SAMPLE;

	for (int i=0; i < bank_size+2; i++){
		Hz_points[i] = Mel_to_Hz((float)(low_freq_mel + i * d_hz_points));
		bin[i] = round((Hz_points[i]/bin_sep));
	}
}


/*----------------------------------------------------------------------------*/
/* Signal pre-processing :  Mel Spectrum calculation 		                  */
/*----------------------------------------------------------------------------*/

void mel_spectrum(int * bin , float32_t * power_spectrum, float32_t * mel_spectra ){

	static int f_m_minus=0, f_m=0, f_m_plus=0;
	static float32_t in_vector[AIDPU_NB_SAMPLE/2];

	for (int m=1; m < bank_size+1; m++){
		f_m_minus = bin[m-1];
		f_m = bin[m];
		f_m_plus = bin[m+1];

		for (int i=0; i<AIDPU_NB_SAMPLE/2   ;i++){
			in_vector[i] = (float32_t) 0.0;
		}

		for (int k=f_m_minus;k < f_m;k++){
			in_vector[k] = (((float32_t)k-f_m_minus)/(f_m-f_m_minus));
		}

		for (int j=f_m; j<f_m_plus;j++){
			in_vector[j] = (float32_t)((f_m_plus-(float32_t)j)/(f_m_plus-f_m));
		}
		mel_spectra[m-1] = dot_product(in_vector, power_spectrum);
	}
}





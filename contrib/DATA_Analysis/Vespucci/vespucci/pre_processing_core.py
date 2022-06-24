from distutils.log import error
import numpy as np
import scipy.signal as sig
import matplotlib.pyplot as plt
import cmsisdsp as dsp
import cmsisdsp.fixedpoint as f
import scipy.signal as sig
from cmsisdsp.datatype import F32


TRIANGULAR_FILTERS_BANK_SIZE = 128

'''
        BASIC MATH FUNCTIONS
'''
def complex_abs( real,  compl):
    return np.sqrt(real*real + compl*compl)

def dot_product(in_1, in_2):
    mult = dsp.arm_mult_f32(in_1,in_2)
    sum = 0
    for i in range(len(mult)):
        sum = sum + mult[i]
    return sum

def hz_to_mel(f_hz):
    return(2595*np.log10(1+f_hz/700))

def mel_to_hz(f_mel):
    return(700*(10**(f_mel/2505)-1))


'''
        SIGNAL PROCESSING FUNCTIONS
'''
def axis_selection(data_in_df , accelerometer_axis):
    signal = np.array( data_in_df['A_'+accelerometer_axis+' [g]'] )
    return signal

def mean_removal(data_in):
    mean = np.mean(data_in)
    data_out = data_in-mean
    return data_out

def signal_normalization(data_in, peak_to_peak, offset):
    max = max(data_in)
    min = min(data_in)
    data_out = ((data_in-min)/(max-min))*peak_to_peak - ((peak_to_peak/2)+offset)
    return data_out

def hanning(data_in):
    multiplier = sig.hanning(len(data_in))
    data_out = data_in * multiplier
    return data_out

def fft_(data_in, signal_windowing):
    number_of_samples = len(data_in)
    rfftf32=dsp.arm_rfft_fast_instance_f32()
    fft_status=dsp.arm_rfft_fast_init_f32(rfftf32,number_of_samples)
    if (signal_windowing == 'HANNING'):
        data_in = hanning(data_in)

    if (fft_status == 0):
        fft_out_buffer = dsp.arm_rfft_fast_f32(rfftf32,data_in,0)
        fft = np.zeros(int(number_of_samples/2))
        freq_point = 0
        for ii in range(0, number_of_samples ,2):
            fft[freq_point] = complex_abs( fft_out_buffer[ii],  fft_out_buffer[ii+1])
            if fft[freq_point] < 1e-3:
                fft[freq_point] = 1e-3
            freq_point = freq_point+1
        
        return fft
    else:
        print('ERROR: INPUT DATA DIMENSIONS NOT SUPPORTED!!!!')
        return error

def triangular_filters_init(number_of_samples, sampling_frequency, triangular_filter_scale):
    TRIANGULAR_FILTERS_BANK_FRACTION = 0.45
    f_min = 0
    f_max = sampling_frequency * TRIANGULAR_FILTERS_BANK_FRACTION

    if (triangular_filter_scale == 'TRIANGULAR_FILTERS_SCALE_MEL'):
        low_freq = hz_to_mel(f_min)
        high_freq = hz_to_mel(f_max)
    else:
        low_freq = f_min
        high_freq = f_max

    d_hz_points = (high_freq-low_freq)/(TRIANGULAR_FILTERS_BANK_SIZE+2)
    hz_points = np.zeros(TRIANGULAR_FILTERS_BANK_SIZE+2)
    bin_sep=sampling_frequency/number_of_samples
    bin = np.zeros(TRIANGULAR_FILTERS_BANK_SIZE+2)

    for ii in range(TRIANGULAR_FILTERS_BANK_SIZE+2):
        if (triangular_filter_scale == 'TRIANGULAR_FILTERS_SCALE_MEL'):
            hz_points[ii] = mel_to_hz((low_freq + ii * d_hz_points))
        else:
            hz_points[ii] = low_freq + ii * d_hz_points
        
        bin[ii] =  np.round(hz_points[ii]/bin_sep)
    
    return bin
        
def triangular_filters_convolution(data_in, bin ):
    # fft or spectrum as input
    number_of_samples = len(data_in)
    fbank=np.zeros((TRIANGULAR_FILTERS_BANK_SIZE,int(np.floor(number_of_samples))))
    for m in range(1,TRIANGULAR_FILTERS_BANK_SIZE+1):
        f_m_minus=int(bin[m-1])
        f_m=int(bin[m])
        f_m_plus=int(bin[m+1])
        for k in range(f_m_minus, f_m):
            fbank[m-1,k]=(k-f_m_minus)/(f_m-f_m_minus)
        for k in range(f_m,f_m_plus):
            fbank[m-1,k]=(f_m_plus-k)/(f_m_plus-f_m)
    
    mel_spectrum = np.zeros(128)
    for ii in range(fbank.shape[0]):
        mel_spectrum[ii] = dot_product(fbank[ii,:], data_in)  
    
    return mel_spectrum
  
def mfcc_(data_in, bin, signal_windowing):
    dct_support_vector= np.zeros(int(TRIANGULAR_FILTERS_BANK_SIZE * 2))
    spectrum  = fft_(data_in, signal_windowing)
    dct4f32=dsp.arm_dct4_instance_f32()
    cfftradix4f32=dsp.arm_cfft_radix4_instance_f32()
    rfftf32=dsp.arm_rfft_fast_instance_f32()
    dct_status=dsp.arm_dct4_init_f32(dct4f32,rfftf32,cfftradix4f32,TRIANGULAR_FILTERS_BANK_SIZE,int(TRIANGULAR_FILTERS_BANK_SIZE/2),0.125)
   
    if dct_status == 0:
        mel_spectrum = triangular_filters_convolution(spectrum , bin )
        mel_spectrum = 20* np.log10(mel_spectrum)
        mel_frequency_cepstral_coef = dsp.arm_dct4_f32(dct4f32,dct_support_vector,mel_spectrum)
        return mel_frequency_cepstral_coef
    else:
        return error


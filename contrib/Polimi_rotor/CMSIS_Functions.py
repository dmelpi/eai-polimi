import numpy as np
import scipy.signal as sig
import matplotlib.pyplot as plt
import cmsisdsp as dsp
import cmsisdsp.fixedpoint as f
import cmsisdsp.mfcc as mfcc
import scipy.signal as sig
from cmsisdsp.datatype import F32


#######################################################################################################
#####################################      SUPPORT FUNCTIONS      ##################################### 
#######################################################################################################

# calculation of the complex absolute value when the FFT real function is used
def complexABS( real,  compl):
  return np.sqrt(real*real + compl*compl)

def dot_product_ARM(in_1, in_2):
  mult = dsp.arm_mult_f32(in_1,in_2)
  sum = 0
  for i in range(len(mult)):
    sum = sum + mult[i]
  return sum

# Hz/Mel axis conversion
def Hz_to_Mel(f_hz):
  return(2595*np.log10(1+f_hz/700))

def Mel_to_Hz(f_mel):
  return(700*(10**(f_mel/2505)-1))


# Mel Filter Bank generator, it generates a defined number (nfilt) triangular filters equispaced in mel frequency domain between f_min and f_max 
def Mel_filters(nfilt, f_min, f_max, sample_rate, n_samples ):
  low_freq_mel=Hz_to_Mel(f_min)
  high_freq_mel=Hz_to_Mel(f_max)

  low_freq_mel=f_min
  high_freq_mel=f_max

  d_hz_points = (high_freq_mel-low_freq_mel)/(nfilt+2)

  hz_points = np.zeros(nfilt+2)
  bin_sep=sample_rate/n_samples
  bin = np.zeros(nfilt+2)
  for ii in range(nfilt+2):
    #hz_points[ii] = Mel_to_Hz((low_freq_mel + ii * d_hz_points))
    hz_points[ii] = low_freq_mel + ii * d_hz_points
    bin[ii] =  np.round(hz_points[ii]/bin_sep)
  
  #initialize filterbank
  fbank=np.zeros((nfilt,int(np.floor(n_samples/2))))
  for m in range(1,nfilt+1):
    f_m_minus=int(bin[m-1])
    f_m=int(bin[m])
    f_m_plus=int(bin[m+1])

    for k in range(f_m_minus, f_m):
      fbank[m-1,k]=(k-f_m_minus)/(f_m-f_m_minus)
    for k in range(f_m,f_m_plus):
      fbank[m-1,k]=(f_m_plus-k)/(f_m_plus-f_m)
  return fbank






#######################################################################################################
#####################################        POWER SPECTRUM        ####################################
#######################################################################################################

# this function calculates the power spectrum using the CMSIS DSP Library realFFT function

def CMSIS_PowerSpectrum (signal, sampling_frequency):
  # insert a signal with a lenght of 512, 1024, 2048, etc... 
  n_samples = len(signal)
  rfftf32=dsp.arm_rfft_fast_instance_f32()
  status=dsp.arm_rfft_fast_init_f32(rfftf32,n_samples)

  resultI = dsp.arm_rfft_fast_f32(rfftf32,signal,0)

  power_spectrum = np.zeros(int(n_samples/2))
  freq_point = 0

  for ii in range(0, int(n_samples) ,2):
    power_spectrum[freq_point] = complexABS( resultI[ii],  resultI[ii+1])
    #power_spectrum[freq_point] = power_spectrum[freq_point]/np.sqrt(n_samples)
    #power_spectrum[freq_point] = 2 *power_spectrum[freq_point] **2 / sampling_frequency
    if power_spectrum[freq_point] < 1e-3:
      power_spectrum[freq_point] = 1e-3
    freq_point = freq_point+1

  return power_spectrum


#######################################################################################################
##################################        POWER SPECTROGRAM        ####################################
#######################################################################################################

# This function generates a matrix containing the spectrogram of the input signal
# The shift_samples parameters can be used to overlap the windows in the spectrogram 

def CMSIS_PowerSpectrogram(signal, sampling_frequency, n_samples = 2048, shift_samples=256):
    n_samples_tot =int(signal.shape[0])
    N=0
    while (n_samples_tot-(N*shift_samples+n_samples)) > 0:
        N=N+1

    spectrogram_matrix = np.zeros((int(n_samples/2), N))
    window = sig.hanning(n_samples)
    for ii in range(N):
        sub_signal = signal[ int(ii*shift_samples) : int(ii*shift_samples + n_samples) ]
        sub_signal = window*sub_signal
        spectrogram_matrix[: , ii] = CMSIS_PowerSpectrum(sub_signal, sampling_frequency)
    
    return spectrogram_matrix


#######################################################################################################
####################################         Mel Spectrum         #####################################
#######################################################################################################

# this function calculates the dot product between the filter bank and the power spectrum 

def mel_spectrum_ARM(fbank,  power_spectrum):
  mel_spectrum = np.zeros(128)
  for ii in range(fbank.shape[0]):
    mel_spectrum[ii] = dot_product_ARM(fbank[ii,:], power_spectrum)  
  return mel_spectrum


#######################################################################################################
###################################        MFCC_Spectrogram        ####################################
#######################################################################################################

def CMSIS_MFCC_Spectrogram_V4(signal,sampling_frequency, freq_min = 0, numOfDctOutputs=13, numOfMelFilters = 128 , n_samples = 2048, shift_samples=256):
    n_samples_tot =int(signal.shape[0])
    N=0
    while (n_samples_tot-(N*shift_samples+n_samples)) > 0:
        N=N+1

    mcc_spectrogram_matrix = np.zeros((numOfDctOutputs, N))
    
    freq_high = sampling_frequency * 0.45
    fbank = Mel_filters(128, 0, freq_high, sampling_frequency, n_samples)


    cfftradix4f32=dsp.arm_cfft_radix4_instance_f32()
    rfftf32=dsp.arm_rfft_instance_f32()
    dct4f32=dsp.arm_dct4_instance_f32()
    status=dsp.arm_dct4_init_f32(dct4f32,rfftf32,cfftradix4f32,numOfMelFilters,int(numOfMelFilters/2),0.125)

    window = sig.hanning(n_samples)

    for ii in range(N):
        sub_signal = np.array(signal[ii*shift_samples : ii*shift_samples + n_samples])
        sub_signal = sub_signal - np.mean(sub_signal)
        sub_signal = sub_signal*window
        power_spectra = CMSIS_PowerSpectrum (sub_signal, sampling_frequency)

        mel_spectra = mel_spectrum_ARM(fbank,  power_spectra)
        mel_spectra = 20*np.log10(mel_spectra)

        state=np.zeros(int(2*numOfMelFilters))
        resultI = dsp.arm_dct4_f32(dct4f32,state,mel_spectra)
        mcc_spectrogram_matrix[:,ii]=resultI[0:numOfDctOutputs]
    
    return mcc_spectrogram_matrix


#######################################################################################################
###############################        Spectrogram Matrix Plot        #################################
#######################################################################################################

def show_specgram(X,ratio,limits,cmin=None,cmax=None, title='Insert a title', y_axis = "Frequency [Hz]"):
  ''' this function represent matrix X within a figure having the desired
   aspect ratio. 
   INPUT:
   X: matrix to represent
   ratio: aspect ratio of axes
   limits: array with [xmin,xmax,ymax,ymin]
   cmin: minimum value correspondig to the bottom of the colormap scale.
   '''
  w, h=plt.figaspect(ratio)
  fig = plt.figure(figsize=(w*2,h*2))
  ax = fig.add_axes([0.1,0.1,0.8,0.8])
  im = ax.imshow(X, extent = limits, cmap ='viridis',interpolation='none')
  xleft, xright = ax.get_xlim()
  ybottom, ytop = ax.get_ylim()
  ax.set_aspect(abs((xright-xleft)/(ybottom-ytop))*ratio)
  ax.invert_yaxis()
  ax.set_xlabel('Time [s]')
  ax.set_ylabel(y_axis)
  ax.set_title(title)
  cbar = plt.colorbar(im);

  if cmin == None:
    cmin = np.min(X)

  if cmax == None:
    cmax = np.max(X)  
    
  cbar.mappable.set_clim(cmin,cmax);
    
  return ax
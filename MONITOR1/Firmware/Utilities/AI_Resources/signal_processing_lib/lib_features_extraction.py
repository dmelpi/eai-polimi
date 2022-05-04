#from numpy import arange, array, cos, cumsum, exp, fft, floor, hamming, mean, pi, sqrt, sum as npsum, zeros
from numpy import *
import math

EPSILON_6 = 1e-6

# feature nth moment
# input : data vector or matrix (N x n_ax), where n_ax >=1, n
# out : moment/s (calculated on first axis)
def featNthMoment( data, n = 1, ax = 0 ):
	if data.shape[0] == 1:
		data = data.reshape( -1, 1 )
	moments = mean( data, axis = ax, keepdims = True )
	# print( moments )
	if n <= 0:
		print( 'Order should be greater than or equal to 1 ' )
	elif n > 1:	
		moments = sum( ( data - moments )**n, axis = ax, keepdims = True ) / ( data.shape[ax] - 1 )
	return moments
# feature magnitude: compute magnitude from input matrix and returns magnitude vector
# input: list of data vectors
# output: nd.array of magnitude values
def featMagnitude(data):
    data = array(data)
#    if len(data.shape) == 1:
#        return sqrt(transpose(data)**2)
    tmp = array(data[0])**2
#    if len(data.shape) > 1:
    for j in data[1:]:
        tmp = tmp + array(j)**2
    return sqrt(tmp)


# feature RMS: compute RMS from input signal, manages both cases whether input is a vector or a matrix
# input: data, either a 1-dim vector or a matrix of vectors,
# input: ax, axis along which the RMS is to be found
# output: returns energy
def featRMS(data, ax = 0 ):
    if len(data) == 0:
        print("Error when computing energy, data list is empty")
        return 0.
    return sqrt((data**2).mean(axis = ax ))


# feature skewness computation on temporal data:
# input: data
# output: skewness (info on data asymmetry)
def featSkewness(data, unbiased = 0):
    SpCentered = data - mean(data)
    N = size(data)
    if N < 3:
        N = 3
    s2 = sum(SpCentered**2)/(N)    # this is the biased variance estimator
    m3 = mean(SpCentered**3)
    if s2 > EPSILON_6:
        skewness = m3 / (s2**(1.5) + EPSILON_6)
    else:
        skewness = 0
    if (unbiased != 0):
        skewness = skewness * (sqrt(N*(N-1))/(N-2))
    return skewness


# feature flatness computation on temporal:
# input: data
# output: flatness (1 is flat, 0 is non flat...)
def featFlatness(data):
    MeanLog = mean(log(data + EPSILON_6))
    Mean = mean(data) + EPSILON_6
    fft_flatness = exp(MeanLog) / Mean
    return fft_flatness


# feature AR order 3 computation:
# input: list of data vectors
# output: AR(1), AR(2) and AR(3)
def featAR3(data):
    x = data - mean(data) + EPSILON_6    # to avoid completely NULL vectors
    N = size(x)
    # Correlation values R
    R_1 = sum(x[1:N]*x[1:N])
    R_2 = sum(x[1:N-1]*x[2:N])
    R_3 = sum(x[1:N-2]*x[3:N])

    # Ar coefficients calculation
    AR_1 = 1.0
    # iteration 1
    if R_1 < EPSILON_6:
        R_1 = EPSILON_6
    Lambda = -R_2 / R_1
    AR_2 = Lambda
    Error = R_1 * (1.0 - Lambda*Lambda)

    # iteration 2
    if Error < EPSILON_6:
        Error = EPSILON_6
    Lambda = -((R_3*AR_1) + (R_2*AR_2)) / Error
    AR_2 = AR_2 * (1.0 + Lambda)
    AR_3 = Lambda
    return AR_2, AR_3


# feature ZCR computation:
# input: list of data vectors
# output: nd.array of zcr values
def featEnvZCR(data, mode):
    if len(data.shape) == 1:
        if mode == 0:
            meanxyz = 0
        else:
            meanxyz = mean(data)
        tmp = sign(round_(data - meanxyz, 8))
        tmp = abs(diff(tmp))
        zcr = sum(sign(tmp), dtype=float32)
    else:
        if mode == 0:
            z = zeros(data.shape)
            meanxyz = z
        else:
            meanxyz = repeat(expand_dims(mean(data, axis=1), axis=1), data.shape[1], axis=1)
        tmp = sign(round_(data - meanxyz, 8))
        tmp = abs(diff(tmp, axis=1))
        zcr = sum(sign(tmp), axis=1, dtype=float32)
    return zcr


# feature Entropy computation:
# input: list of data vectors
# output: Entropy value
def featEntropy(data):
    logVec = log(data + EPSILON_6)
    Nbin = 32
    # Histogram distribution
    [pdf_bar, pdf_bin] = histogram(logVec, Nbin)
    pdf_bar = pdf_bar + EPSILON_6
    pdf_bar_norm = pdf_bar / sum(pdf_bar)
    Entropy = -sum(pdf_bar_norm * log2(pdf_bar_norm))
    return Entropy


# feature Goertzel computation :
# input: list of data vectors
# output: nd.array of Goertzel values (power, freq)
def featGoertzel(data, Fe, Freq_min, Freq_max, Freq_step):
    data = data - mean(data)
    # Settings
    N = size(data)
    BLOCK_SIZE = N
    nb_freq = round((Freq_max - Freq_min) / Freq_step) + 1

    # Goertzel feedback
    freq_search = linspace(Freq_min, Freq_max, nb_freq)
    V = zeros((nb_freq,3))

    for n in range(BLOCK_SIZE):
        freq_idx = 0
        for FreqToDetect in freq_search:
            k = floor(0.5 + (BLOCK_SIZE*FreqToDetect/Fe))
            w = k * (2 * pi / BLOCK_SIZE)
            coeff = 2 * cos(w)
            V[freq_idx, 0] = coeff *V[freq_idx, 1] - V[freq_idx, 2] + data[n]
            V[freq_idx, 2] = V[freq_idx, 1]
            V[freq_idx, 1] = V[freq_idx, 0]
            freq_idx += 1

    # Goertzel feedforward
    goertzel_power = 0
    goertzel_freq = 0
    freq_idx = 0
    puissance_tmp = zeros(nb_freq)
    for FreqToDetect in freq_search:
        k = floor(0.5 + (BLOCK_SIZE * FreqToDetect / Fe))
        w = k * (2 * pi / BLOCK_SIZE)
        coeff = 2 * cos(w)
        puissance_tmp[freq_idx] = V[freq_idx, 1]**2 + V[freq_idx, 2]**2 - V[freq_idx, 1] * V[freq_idx, 2] * coeff
        if puissance_tmp[freq_idx] > goertzel_power:
            goertzel_power = puissance_tmp[freq_idx]
            goertzel_freq = FreqToDetect
        freq_idx += 1
    if goertzel_power <= 1:
        goertzel_power = 0.0
    else:
        goertzel_power = 10*log10(goertzel_power)
#    return array([goertzel_power, goertzel_freq])
    return goertzel_power, goertzel_freq


# feature flatness computation on FFT spectrum:
# input: data spectrum
# input: b1 (first bin to compute the flatness)
# input: b2 (last bin to compute the flatness)
# output: fft flatness (1 is flat, 0 is non flat...)
def featFFT_Flatness(Spectrum, b1, b2):
    Nfreq = b2 - b1 + 1
    Spectrum_part = Spectrum[b1:b2+1]
    MeanLog = mean(log(Spectrum_part + EPSILON_6))
    Mean = mean(Spectrum_part) + EPSILON_6
    fft_flatness = exp(MeanLog) / Mean
    return fft_flatness

# feature Spectral Centroid, spread, skewness computation on spectrum:
# input: data spectrum
# input: Sampling freq (-1 for results in Bins)
# input: b1 (first bin to compute the centroid)
# input: b2 (last bin to compute the centroid)
# output: fft centroid (centroid is 0 if spectrum null)
# output: fft spread value around centroid
# output: fft skewness (info on spectral asymmetry)
# output: fft Kurtosis (info on flatness and non gaussianity)
def featFFT_Centroid(Spectrum, Fe, b1, b2):
    Nfreq = (size(Spectrum, axis=0))
#    Nfreq = b2 - b1 + 1
    if Fe == -1:
        # Results in frequency Bins
        FreqAxis = range(b1, b2+1)
    else:
        # Results in Hz
        FreqAxis = linspace(b1*Fe/(2*Nfreq), (b2+1)*Fe/(2*Nfreq), b2 - b1 + 1, endpoint=False)
        # print(FreqAxis)
    Spectrum_part = Spectrum[b1:b2+1]
    fft_centroid = sum(FreqAxis*Spectrum_part)/(sum(Spectrum_part) + EPSILON_6)
    DispCentroid = (FreqAxis - fft_centroid)**2
#    fft_spread = log(sum(DispCentroid * Spectrum) / (sum(Spectrum) + EPSILON_6) + EPSILON_6)
    fft_spread = sqrt(sum(DispCentroid * Spectrum_part) / (sum(Spectrum_part) + EPSILON_6) + EPSILON_6)
    if fft_spread <= EPSILON_6:
        fft_skewness = 0.0
        fft_kurtosis = 0.0
    else:
        fft_skewness = (sum((((FreqAxis - fft_centroid) / fft_spread) ** 3) * Spectrum_part) / (sum(Spectrum_part) + EPSILON_6))
        fft_kurtosis = (sum((((FreqAxis - fft_centroid) / fft_spread) ** 4) * Spectrum_part) / (sum(Spectrum_part) + EPSILON_6))
    return fft_centroid, fft_spread, fft_skewness, fft_kurtosis


# feature crest factor computation on FFT spectrum:
# input: data spectrum
# input: b1 (first bin to compute the crest)
# input: b2 (last bin to compute the crest)
# output: fft crest
def featFFT_Crest(Spectrum, b1, b2):
    Nfreq = b2 - b1 + 1
    Spectrum_part = Spectrum[b1:b2+1]
    Max = max(Spectrum_part)
    Mean = mean(Spectrum_part) + EPSILON_6
    fft_crest = Max / Mean
    return fft_crest


# feature FFT Entropy computation on env sens:
# input: spectrum
# input: b1 (first bin to compute the entropy)
# input: b2 (last bin to compute the entropy)
# output: Entropy value
def featFFT_Entropy(Spectrum, b1, b2):
    Nfreq = b2 - b1 + 1
    Spectrum_part = Spectrum[b1:b2+1]
    logVec = log(Spectrum_part + EPSILON_6)
    fft_entropy = -sum(logVec * Spectrum_part) / (log(Nfreq) + EPSILON_6)
    return fft_entropy


# feature peak power and freq and SNR harmonic computation on FFT spectrum:
# input: data spectrum
# input: b1 (first bin to compute the entropy)
# input: b2 (last bin to compute the entropy)
# input: Sampling freq (-1 for results in Bins)
# output: peak power
# output: fft peak freq
# output: fft SNR harmonic
def featFFT_PowerFreq(Spectrum, Fe, b1, b2):
#    Nfreq = (float)(size(Spectrum, axis=0))
    dataSize = size(Spectrum, axis=0)
    Nfreq = b2 - b1 + 1
    MaxSNR = 100
    Spectrum_part = Spectrum[b1:b2+1]
    fft_PeakPower = max(Spectrum_part)
    PowerPeakIndex = argmax(Spectrum_part) + b1
    if Fe == -1:
        # Results in frequency Bins
        fft_PeakFreq = PowerPeakIndex
    else:
        # Results in Hz
        fft_PeakFreq = (float)((PowerPeakIndex * (float)(Fe)) / (2.0*dataSize))
    if PowerPeakIndex == 0:
        NoisePower = sum(Spectrum_part) - Spectrum[PowerPeakIndex] - Spectrum[PowerPeakIndex + 1]
        NoisePower = NoisePower / (Nfreq - 2)
    elif PowerPeakIndex == (dataSize - 1):
        NoisePower = sum(Spectrum_part) - Spectrum[PowerPeakIndex - 1] - Spectrum[PowerPeakIndex]
        NoisePower = NoisePower / (Nfreq - 2)
    else:
        NoisePower = sum(Spectrum_part) - Spectrum[PowerPeakIndex - 1] - Spectrum[PowerPeakIndex] - Spectrum[PowerPeakIndex + 1]
        NoisePower = NoisePower / (Nfreq - 3)

    if NoisePower > 0:
        fft_SNRHarmonic = 10 * log10(fft_PeakPower / NoisePower)
    else:
        fft_SNRHarmonic = (fft_PeakPower > 0) * MaxSNR

    if fft_PeakPower < EPSILON_6:
        fft_PeakPower = -6  # min in dB
    else :
        fft_PeakPower = log10(fft_PeakPower)
    return fft_PeakPower, fft_PeakFreq, fft_SNRHarmonic


# Statistical features
# input: signal, either a 1-dim vector
# output: returns min, max, mean, std and unbiased variance
def featStat(data):
    if len(data) == 0:
        print("Error when computing energy, data list is empty")
        return 0.
    Min = min(data)
    Max = max(data)
    Mean = mean(data)
    Std = sqrt(mean((data - Mean)**2))
    Var = sum((data - Mean)**2) / (len(data) - 1)
    return Min, Max, Mean, Std, Var




#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
from numpy import absolute, mean, loadtxt, savetxt, transpose

from features import featMagnitude, featRMS, featEnvZCR, featGoertzel, featAR3, featEntropy, featSkewness, featFlatness
from features import featFFT_Flatness, featFFT_Centroid, featFFT_Crest, featFFT_PowerFreq, featFFT_Entropy
from features import featStat

def returnNANvalues(size):
    from numpy import empty,NAN
    a = empty((size,1))
    a[:] = NAN
    return a

def fftSampledSource(values, fftsize):
    from numpy.fft import fft
    from numpy import hamming
    h = hamming(fftsize)
    values = values[:fftsize] * h
    return fft(values[:fftsize] - mean(values), n=fftsize)

class Flags(object):
    """Store configuration flags."""
    def __init__(self, confdict):
        for key, value in confdict.items():
            setattr(self, key, value)


def main():
    """Script entry point."""
    import argparse
    import warnings
    import json

    warnings.simplefilter("ignore")  # remove warnings messages (= '-W ignore')

    parser = argparse.ArgumentParser()
    parser.add_argument('--config_file', type=str, default='',
                        help='The path to the config file. If any config file is specified,'
                             'its parameters overwrite any other cmd line parameters.')
    args = parser.parse_args()
    if args.config_file:
        with open(args.config_file, 'r') as conf:
            flags_dict = json.loads(conf.read())
        if 'verbosity' not in flags_dict:
            flags_dict['verbosity'] = 1
        params = Flags(flags_dict)
    else:
        params = args

#    data = loadtxt(params.C_input_data_path + 'ref_goertzel_in.txt')
#    mag = array(data)
#    mag_tmp = []
#    for i in range(len(mag)):
#        tmp = mag[i]
#        mag_tmp.append(tmp)
#    savetxt(params.C_input_data_path + 'goertzel_data_in.txt', mag_tmp, fmt="%f", delimiter=',')

    #  Statistical domain processing starts here
    if params.Feature == "Statistical" or params.Feature == "ALL":
        print('\n')
        print('Statistical domain processing starts here')
        print('--------------------------------------')
        data = loadtxt(params.C_input_data_path + 'data_in.txt', delimiter=',')
        [Min, Max, Mean, Std, Var] = featStat(data)
        savetxt(params.C_ref_data_path + 'stat_data_out.txt', [Min, Max, Mean, Std, Var], fmt="%f", delimiter=',')
        with open(params.C_ref_data_path + "Full_data_out.txt", "wt") as f:
            f.write("Stat Min : {:.5f}\n".format(Min))
            f.write("Stat Max : {:.5f}\n".format(Max))
            f.write("Stat Mean : {:.5f}\n".format(Mean))
            f.write("Stat Std : {:.5f}\n".format(Std))
            f.write("Stat Var : {:.5f}\n".format(Var))
        print('Statistical descriptors Completed \n')


    #  Temporal domain processing starts here
    print('Temporal domain processing starts here')
    print('--------------------------------------')
    if params.Feature == "Temporal" or params.Feature == "ALL":
        # Magnitude Computation
        data = loadtxt(params.C_input_data_path + 'mag_data_in.txt', delimiter=',')
        data = transpose(data)
        mag = featMagnitude(data)
        savetxt(params.C_ref_data_path + 'mag_data_out.txt', [mag], fmt="%f", delimiter=',')
        with open(params.C_ref_data_path + "Full_data_out.txt", "at") as f:
            f.write("Temporal Magnitude : {:.5f}\n".format(mag))
        print('Magnitude completed')

        # RMS Computation
        data = loadtxt(params.C_input_data_path + 'data_in.txt', delimiter=',')
        rms = featRMS(data)
        savetxt(params.C_ref_data_path + 'rms_data_out.txt', [rms], fmt="%f", delimiter=',')
        with open(params.C_ref_data_path + "Full_data_out.txt", "at") as f:
            f.write("Temporal RMS : {:.5f}\n".format(rms))
        print('RMS completed')

        #  Skewness computation
        data = loadtxt(params.C_input_data_path + 'data_in.txt', delimiter=',')
        Skewness = featSkewness(data, unbiased=1)
        savetxt(params.C_ref_data_path + 'skewness_data_out.txt', [Skewness], fmt="%f", delimiter=',')
        with open(params.C_ref_data_path + "Full_data_out.txt", "at") as f:
            f.write("Temporal Skewness : {:.5f}\n".format(Skewness))
        print('Skewness completed')

        #  Flatness computation
        data = loadtxt(params.C_input_data_path + 'data_in.txt', delimiter=',')
        Flatness = featFlatness(data)
        savetxt(params.C_ref_data_path + 'flatness_data_out.txt', [Flatness], fmt="%f", delimiter=',')
        with open(params.C_ref_data_path + "Full_data_out.txt", "at") as f:
            f.write("Temporal Flatness : {:.5f}\n".format(Flatness))
        print('Flatness completed')

        #  AR coefficients computation
        data = loadtxt(params.C_input_data_path + 'data_in.txt', delimiter=',')
        [AR_2, AR_3] = featAR3(data)
        savetxt(params.C_ref_data_path + 'ar3_data_out.txt', (AR_2, AR_3), fmt="%f", delimiter=',')
        with open(params.C_ref_data_path + "Full_data_out.txt", "at") as f:
            f.write("Temporal AR2 : {:.5f}\n".format(AR_2))
            f.write("Temporal AR3 : {:.5f}\n".format(AR_3))
        print('AR3 completed')

        # ZCR Computation
        data = loadtxt(params.C_input_data_path + 'zcr_data_in.txt', delimiter=',')
        zcr_xyz = featEnvZCR(data, 0)   # 0 means Zero crossing rate, else the mean of the data will be used instead of 0
        savetxt(params.C_ref_data_path + 'zcr_data_out.txt', [zcr_xyz], fmt="%f", delimiter=',')
        with open(params.C_ref_data_path + "Full_data_out.txt", "at") as f:
            f.write("Temporal ZCR : {:.5f}\n".format(zcr_xyz))
        print('ZCR completed')

        #  Entropy computation
        data = loadtxt(params.C_input_data_path + 'data_in.txt', delimiter=',')
        entropy = featEntropy(data)
        savetxt(params.C_ref_data_path + 'entropy_data_out.txt', [entropy], fmt="%f", delimiter=',')
        with open(params.C_ref_data_path + "Full_data_out.txt", "at") as f:
            f.write("Temporal Entropy : {:.5f}\n".format(entropy))
        print('Entropy completed')

        #  Goertzel power and frequency detection
        mag = loadtxt(params.C_input_data_path + 'goertzel_data_in.txt', delimiter=',') #'ref_goertzel_in.txt')
        Fe = params.Fe
        Freq_min = 0.0
        Freq_max = 6
        Freq_step = 0.4
        [goertzel_power, goertzel_freq] = featGoertzel(mag, Fe, Freq_min, Freq_max, Freq_step)
        savetxt(params.C_ref_data_path + 'goertzel_data_out.txt', (goertzel_power, goertzel_freq), fmt="%f", delimiter=',')
#        with open(params.C_ref_data_path + "Full_data_out.txt", "at") as f:
#            f.write("Temporal Goertzel Power : {:.5f}\n".format(goertzel_power))
#            f.write("Temporal Goertzel Freq : {:.5f}\n".format(goertzel_freq))
        print('Goertzel completed')

    #  Spectral domain processing starts here
    if params.Feature == "Spectral" or params.Feature == "ALL":
        print('\n')
        print('Spectral domain processing starts here')
        print('--------------------------------------')
        data = loadtxt(params.C_input_data_path + 'fft_python_data_in.txt', delimiter=',')
        data = transpose(data)
        mag = featMagnitude(data)
        m = mean(mag)
        mag = mag - m
        savetxt(params.C_input_data_path + 'fft_data_in.txt', [mag], fmt="%f", delimiter=',')
        fft_size = params.fft_size
        if len(mag) <= fft_size:
            #  PSD calculation
            fft_mag = (absolute(fftSampledSource(mag, fft_size))) ** 2
        else:
            #  Welch Periodogram calculation
            fft_mag = (absolute(fftSampledSource(mag[:fft_size], fft_size))) ** 2
            fft_i = 0
            fft_e = fft_size
            count = 1
            while len(mag) - fft_e > fft_size:
                fft_i += fft_size / 2
                fft_e += fft_size / 2
                fft_mag += (absolute(fftSampledSource(mag[fft_i:fft_e], fft_size))) ** 2
                count += 1
            if fft_e != len(mag) - 1:
                fft_e = len(mag)
                fft_i = len(mag) - fft_size
                fft_mag += (absolute(fftSampledSource(mag[fft_i:fft_e], fft_size))) ** 2
                count += 1
            fft_mag /= count
        spectrum = fft_mag[:int(fft_size / 2)]
        savetxt(params.C_ref_data_path + 'fft_data_out.txt', spectrum, fmt="%f", delimiter=',')
        savetxt(params.C_input_data_path + 'fft_proc_data_in.txt', [spectrum], fmt="%f", delimiter=',')
        print('FFT - PSD completed')

        b1 = params.b1
        b2 = params.b2
        sampl_fr = params.Fe   # put -1 for result in frequency Bins

        #  Spectral flatness
        FFT_Flatness = featFFT_Flatness(spectrum, b1, b2)
        savetxt(params.C_ref_data_path + 'fft_flatness_out.txt', [FFT_Flatness], fmt="%f", delimiter=',')
        with open(params.C_ref_data_path + "Full_data_out.txt", "at") as f:
            f.write("Spectral Flatness : {:.5f}\n".format(FFT_Flatness))
        print('Spectral Flatness Completed')

        #  Spectral centroid, spread, skewness and kurtosis
        [FFT_Centroid, FFT_Spread, FFT_Skewness, FFT_Kurtosis] = featFFT_Centroid(spectrum, sampl_fr, b1, b2)
        savetxt(params.C_ref_data_path + 'fft_centroid_spread_out.txt', [FFT_Centroid, FFT_Spread, FFT_Skewness, FFT_Kurtosis], fmt="%f", delimiter=',')
        with open(params.C_ref_data_path + "Full_data_out.txt", "at") as f:
            f.write("Spectral Centroid : {:.5f}\n".format(FFT_Centroid))
            f.write("Spectral Spread : {:.5f}\n".format(FFT_Spread))
            f.write("Spectral Skewness : {:.5f}\n".format(FFT_Skewness))
            f.write("Spectral Kurtosis : {:.5f}\n".format(FFT_Kurtosis))
        print('Spectral Centroid, Spread, Skewness and Kurtosis Completed')

        #  Spectral crest factor
        FFT_Crest = featFFT_Crest(spectrum, b1, b2)
        savetxt(params.C_ref_data_path + 'fft_crest_out.txt', [FFT_Crest], fmt="%f", delimiter=',')
        with open(params.C_ref_data_path + "Full_data_out.txt", "at") as f:
            f.write("Spectral Crest Factor : {:.5f}\n".format(FFT_Crest))
        print('Spectral Crest factor Completed')

        #  Spectral entropy
        FFT_Entropy = featFFT_Entropy(spectrum, b1, b2)
        savetxt(params.C_ref_data_path + 'fft_entropy_out.txt', [FFT_Entropy], fmt="%f", delimiter=',')
        with open(params.C_ref_data_path + "Full_data_out.txt", "at") as f:
            f.write("Spectral Entropy : {:.5f}\n".format(FFT_Entropy))
        print('Spectral entropy Completed')

        #  Spectral peak, power and SNR
        [FFT_PowerPeak, FFT_FreqPeak, FFT_SNRHarmonic] = featFFT_PowerFreq(spectrum, sampl_fr, b1, b2)
        savetxt(params.C_ref_data_path + 'fft_peak_snrHarmonic_out.txt', [FFT_PowerPeak, FFT_FreqPeak, FFT_SNRHarmonic], fmt="%f", delimiter=',')
        with open(params.C_ref_data_path + "Full_data_out.txt", "at") as f:
            f.write("Spectral Peak Power : {:.5f}\n".format(FFT_PowerPeak))
            f.write("Spectral Peak Freq : {:.5f}\n".format(FFT_FreqPeak))
            f.write("Spectral Peak SNR : {:.5f}\n".format(FFT_SNRHarmonic))
        print('Spectral frequency peak, power and SNR Completed')


if __name__ == "__main__":
    main()



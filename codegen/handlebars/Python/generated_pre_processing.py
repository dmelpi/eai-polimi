from pre_processing_core import *

class preProcessingData(object):
    
    # Data slicing.
    input_buffer_size = None
    shift_samples = None
    dataset_cols_size = None

    # Sensors.
    ism330dhcx_acc_fs = None
    ism330dhcx_acc_odr = None

    # Axis selection.
    axis_selection_axis = None

    # Signal normalization.
    signal_normalization_peak_to_peak = None
    signal_normalization_offset = None

    # MFCC.
    mfcc_triangular_filters_scale = None
    mfcc_signal_windowing = None
    mfcc_bin = None


def pre_processing_init():
    
    # Data initialization.
    pre_processing_data = preProcessingData()

    # Sensors.
    pre_processing_data.input_buffer_size = 512
    pre_processing_data.ism330dhcx_acc_fs = 16.0
    pre_processing_data.ism330dhcx_acc_odr = 1666.0
    
    # Axis selection.
    pre_processing_data.axis_selection_axis = "x"

    # Signal normalization.
    pre_processing_data.signal_normalization_peak_to_peak = 2.0
    pre_processing_data.signal_normalization_offset = 0.0

    # MFCC.
    pre_processing_data.mfcc_triangular_filters_scale = "TRIANGULAR_FILTERS_SCALE_MEL"
    pre_processing_data.mfcc_signal_windowing = "HANNING"
    pre_processing_data.mfcc_bin = triangular_filters_init(pre_processing_data.input_buffer_size, pre_processing_data.ism330dhcx_acc_odr, pre_processing_data.mfcc_triangular_filters_scale)

    # Data slicing.
    pre_processing_data.shift_samples = 10
    pre_processing_data.dataset_cols_size = 128

    return pre_processing_data 


def pre_processing_process(data_in_df, pre_processing_data):

    # Data slicing.
    pre_processing_data.dataset_rows_size = int(np.floor((len(data_in_df) - pre_processing_data.input_buffer_size) / pre_processing_data.shift_samples))

    # Data initialization.
    X = np.zeros((pre_processing_data.dataset_rows_size, pre_processing_data.dataset_cols_size))

    # Data processing.
    for row in range(pre_processing_data.dataset_rows_size):
        # Input data.
        sub_df = data_in_df.iloc[row * pre_processing_data.shift_samples : row * pre_processing_data.shift_samples + pre_processing_data.input_buffer_size, :]

        # Axis selection.
        data_0 = axis_selection(sub_df, pre_processing_data.axis_selection_axis)

        # Signal normalization.
        data_1 = signal_normalization(data_0, pre_processing_data.signal_normalization_peak_to_peak, pre_processing_data.signal_normalization_offset)

        # Mean removal.
        data_2 = mean_removal(data_1)

        # MFCC.
        data_out = mfcc_(data_2, pre_processing_data.mfcc_bin, pre_processing_data.mfcc_signal_windowing)

        # Output data.
        X[row,:] = data_out

    return X
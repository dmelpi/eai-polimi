from pre_processing_core import *

class preProcessingData(object):
    shift_samples = None
    input_buffer_size = None
    ism330dhcx_acc_fs = None
    ism330dhcx_acc_odr = None
    axis = None
    triangular_filters_scale = None
    signal_windowing = None
    bin = None


def pre_processing_init():
    
    # Data initialization.
    pre_processing_data = preProcessingData()

    # Sensors.
    pre_processing_data.input_buffer_size = 512
    pre_processing_data.ism330dhcx_acc_fs = 16.0
    pre_processing_data.ism330dhcx_acc_odr = 1666.0
    
    # Axis selection.
    pre_processing_data.axis = "x"

    # MFCC.
    pre_processing_data.triangular_filters_scale = "TRIANGULAR_FILTERS_SCALE_MEL"
    pre_processing_data.signal_windowing = "HANNING"
    pre_processing_data.bin = triangular_filters_init(pre_processing_data.input_buffer_size, pre_processing_data.ism330dhcx_odr, pre_processing_data.triangular_filters_scale)

    # Data slicing.
    pre_processing_data.shift_samples = 10
    pre_processing_data.data_out_size = 128

    

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

        # axis selection
        data_0 = axis_selection(sub_df, pre_processing_data.axis)

        # mean removal
        data_1 = mean_removal(data_0)

        # mfcc
        data_out = mfcc_(data_1, pre_processing_data.bin, pre_processing_data.signal_windowing)

        # Output data.
        X[row,:] = data_out

    return X
    
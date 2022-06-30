Handlebars.registerHelper("pre_processing_data_each", function(pre_processing, options) {
    let ret = "";
    let code = "";
    const functions = pre_processing.functions;
    const elements = functions.length;

    for (var element = 0; element < elements; element++) {
        const item = functions[element];
        switch (item.name) {
        case "axis_selection":
            code = "// Axis selection.\n    axis_t axis_selection_axis;";
            break;
        case "mean_removal":
            code = "";
            break;
        case "signal_normalization":
            code = "// Signal normalization.\n    float32_t signal_normalization_peak_to_peak;\n    float32_t signal_normalization_offset;";
            break;
        case "hanning":
            code = "// Hanning.\n    float32_t* hanning_multipliers;";
            break;
        case "fft":
            code = "// FFT.\n    arm_rfft_fast_instance_f32 fft_handler;\n    signal_windowing_t fft_signal_windowing;\n    float32_t* fft_multipliers;";
            break;
        case "mfcc":
            code = "// MFCC.\n    arm_status mfcc_status;\n    arm_rfft_fast_instance_f32 mfcc_handler;\n    arm_dct4_instance_f32 mfcc_dct4f32;\n    arm_rfft_instance_f32 mfcc_rfftf32;\n    arm_cfft_radix4_instance_f32 mfcc_cfftradix4f32;\n    triangular_filters_scale_t mfcc_triangular_filters_scale;\n    signal_windowing_t mfcc_signal_windowing;\n    uint32_t mfcc_bin[MFCC_TRIANGULAR_FILTERS_BANK_SIZE + 2];\n    float32_t* mfcc_multipliers;";
            break;
        }
        
        ret += code;
        ret += (element === elements - 1 || code === "" ? "" : "\n\n    ");
    }

    return ret;
});
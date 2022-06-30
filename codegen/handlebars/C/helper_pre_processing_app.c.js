Handlebars.registerHelper("pre_processing_init_each", function(context, options) {
  let ret = "";
  let code = "";
  const elements = context.length;

  for (var element = 0; element < elements; element++) {
    const item = context[element];
    switch (item.name) {
      case "axis_selection":
        code = "// Axis selection.\n    pre_processing_data->axis_selection_axis = AXIS_SELECTION_AXIS;";
        break;
      case "mean_removal":
        code = "";
        break;
      case "signal_normalization":
        code = "// Signal normalization.\n    pre_processing_data->signal_normalization_peak_to_peak = SIGNAL_NORMALIZATION_PEAK_TO_PEAK;\n    pre_processing_data->signal_normalization_offset = SIGNAL_NORMALIZATION_OFFSET;";
        break;
      case "hanning":
        code = "// Hanning.\n    (*pre_processing_data).hanning_multipliers = (float32_t*) calloc(INPUT_BUFFER_SIZE, sizeof(float32_t));\n    multipliers_init((*pre_processing_data).hanning_multipliers, INPUT_BUFFER_SIZE, (*pre_processing_data).hanning_signal_windowing);";
        break;
      case "fft":
        code = "// FFT.\n    arm_rfft_fast_init_f32(&((*pre_processing_data).fft_handler), {{DATA_IN_SIZE}});\n\n    pre_processing_data->fft_signal_windowing = FFT_SIGNAL_WINDOWING;\n    (*pre_processing_data).fft_multipliers = (float32_t*) calloc(INPUT_BUFFER_SIZE, sizeof(float32_t));\n    multipliers_init((*pre_processing_data).fft_multipliers, INPUT_BUFFER_SIZE, (*pre_processing_data).fft_signal_windowing);";
        break;
      case "mfcc":
        code = "// MFCC.\n    arm_status mfcc_status;\n    arm_rfft_fast_init_f32(&((*pre_processing_data).mfcc_handler), INPUT_BUFFER_SIZE);\n    mfcc_status = arm_dct4_init_f32(&((*pre_processing_data).mfcc_dct4f32), &((*pre_processing_data).mfcc_rfftf32), &((*pre_processing_data).mfcc_cfftradix4f32), MFCC_TRIANGULAR_FILTERS_BANK_SIZE, MFCC_TRIANGULAR_FILTERS_BANK_SIZE / 2.0, 0.125);\n    if (mfcc_status != ARM_MATH_SUCCESS) {\n        sys_error_handler();\n    }\n    pre_processing_data->mfcc_triangular_filters_scale = MFCC_TRIANGULAR_FILTERS_SCALE;\n    pre_processing_data->mfcc_signal_windowing = MFCC_SIGNAL_WINDOWING;\n    triangular_filters_init(INPUT_BUFFER_SIZE, MFCC_TRIANGULAR_FILTERS_BANK_SIZE, MFCC_TRIANGULAR_FILTERS_BANK_FRACTION, ISM330DHCX_ACC_ODR, pre_processing_data->mfcc_triangular_filters_scale, (*pre_processing_data).mfcc_bin);\n    (*pre_processing_data).mfcc_multipliers = (float32_t*) calloc(INPUT_BUFFER_SIZE, sizeof(float32_t));\n    multipliers_init((*pre_processing_data).mfcc_multipliers, INPUT_BUFFER_SIZE, (*pre_processing_data).mfcc_signal_windowing);";
        break;
    }
    ret += code;
    ret += (element === elements - 1 || code === "" ? "" : "\n\n    ");
  }

  return ret;
});

Handlebars.registerHelper("pre_processing_process_each", function(context, options) {
  let ret = "";
  let header = "";
  let variables = "";
  let functions = "";
  let code = "";
  let data_out_size_mode = undefined;
  let data_out_size = undefined;
  let previous_item_data_out_size = undefined;
  const elements = context.length;
  
  for (var element = 0; element < elements; element++) {
    const item = context[element];
    //let previous_item = element > 0 ? context[element - 1] : undefined;
    switch (item.name) {
      case "axis_selection":
        header = "// Axis selection.\n    ";
        variables = "float32_t {{DATA_OUT}}[{{DATA_OUT_SIZE}}];\n    ";
        functions = "axis_selection({{DATA_IN}}, {{DATA_IN_SIZE}}, {{DATA_OUT}}, {{DATA_OUT_SIZE}}, pre_processing_data->axis_selection_axis);";
        data_out_size = "1";
        data_out_size_mode = "relative";
        break;
      case "mean_removal":
        header = "// Mean removal.\n    ";
        variables = "float32_t {{DATA_OUT}}[{{DATA_OUT_SIZE}}];\n    ";
        functions = "mean_removal({{DATA_IN}}, {{DATA_IN_SIZE}}, {{DATA_OUT}}, {{DATA_OUT_SIZE}});";
        data_out_size = "1";
        data_out_size_mode = "relative";
        break;
      case "signal_normalization":
        header = "// Signal normalization.\n    ";
        variables = "float32_t {{DATA_OUT}}[{{DATA_OUT_SIZE}}];\n    ";
        functions = "signal_normalization({{DATA_IN}}, {{DATA_IN_SIZE}}, {{DATA_OUT}}, {{DATA_OUT_SIZE}}, pre_processing_data->signal_normalization_peak_to_peak, pre_processing_data->signal_normalization_offset);";
        data_out_size = "1";
        data_out_size_mode = "relative";
        break;
      case "hanning":
        header = "// Hanning.\n    ";
        variables = "";
        functions = "hanning({{DATA_IN}}, {{DATA_IN_SIZE}}, (*pre_processing_data).hanning_multipliers, {{DATA_OUT_SIZE}});";
        data_out_size = "1";
        data_out_size_mode = "relative";
        break;
      case "fft":
        header = "// FFT.\n    ";
        variables = "float32_t {{DATA_OUT}}[{{DATA_OUT_SIZE}}];\n    ";
        functions = "fft({{DATA_IN}}, {{DATA_IN_SIZE}}, {{DATA_OUT}}, {{DATA_OUT_SIZE}}, &((*pre_processing_data).fft_handler), (*pre_processing_data).fft_signal_windowing, (*pre_processing_data).fft_multipliers);";
        data_out_size = "0.5";
        data_out_size_mode = "relative";
        break;
      case "mfcc":
        header = "// MFCC.\n    ";
        variables = "float32_t {{DATA_OUT}}[{{DATA_OUT_SIZE}}];\n    ";
        functions = "mfcc({{DATA_IN}}, {{DATA_IN_SIZE}}, {{DATA_OUT}}, {{DATA_OUT_SIZE}}, ((*pre_processing_data).mfcc_bin), &((*pre_processing_data).mfcc_dct4f32), &((*pre_processing_data).mfcc_handler), (*pre_processing_data).mfcc_signal_windowing, (*pre_processing_data).mfcc_multipliers);";
        data_out_size = "128";
        data_out_size_mode = "absolute";
        break;
    }
    
    if (element === elements - 1) {
      code = header + functions;
    } else {
      code = header + variables + functions;
    }
    
    switch (element) {
      case 0:
        code = code.replaceAll("{{DATA_IN}}", "data_in");
        code = code.replaceAll("{{DATA_OUT}}", "data_0");
        code = code.replaceAll("{{DATA_IN_SIZE}}", "data_in_size");
        previous_item_data_out_size = (data_out_size_mode === "relative" ? ("data_in_size" + (data_out_size === "1" ? "" : (" * " + data_out_size))) : (data_out_size));
        code = code.replaceAll("{{DATA_OUT_SIZE}}", previous_item_data_out_size);
        break;
      case (elements - 1):
        code = code.replaceAll("{{DATA_IN}}", "data_" + (element - 1));
        code = code.replaceAll("{{DATA_OUT}}", "data_out");
        code = code.replaceAll("{{DATA_IN_SIZE}}", previous_item_data_out_size);
        code = code.replaceAll("{{DATA_OUT_SIZE}}", "data_out_size");
        break;
      default:
        code = code.replaceAll("{{DATA_IN}}", "data_" + (element - 1));
        code = code.replaceAll("{{DATA_OUT}}", "data_" + element);
        code = code.replaceAll("{{DATA_IN_SIZE}}", previous_item_data_out_size);
        previous_item_data_out_size = (data_out_size_mode === "relative" ? (previous_item_data_out_size + (data_out_size === "1" ? "" : (" * " + data_out_size))) : (data_out_size));
        code = code.replaceAll("{{DATA_OUT_SIZE}}", previous_item_data_out_size);
        break;
    }
    ret += code;
    ret += (element === elements - 1 || code === "" ? "" : "\n\n    ");
  }

  return ret;
});

Handlebars.registerHelper("pre_processing_free_each", function(context, options) {
  let ret = "";
  let code = "";
  const elements = context.length;

  for (var element = 0; element < elements; element++) {
    const item = context[element];
    switch (item.name) {
      case "axis_selection":
        code = "";
        break;
      case "mean_removal":
        code = "";
        break;
      case "signal_normalization":
        code = "";
        break;
      case "hanning":
        code = "// Hanning.\n    free((*pre_processing_data).hanning_multipliers);";
        break;
      case "fft":
        code = "// FFT.\n    free((*pre_processing_data).fft_multipliers);";
        break;
      case "mfcc":
        code = "// MFCC.\n    free((*pre_processing_data).mfcc_multipliers);";
        break;
    }
    ret += code;
    ret += (element === elements - 1 || code === "" ? "" : "\n\n    ");
  }

  return ret;
});

//ret = ret + options.fn(item);
Handlebars.registerHelper("pre_processing_data_each", function(sensors, pre_processing, options) {
  let ret = "";
  let code = "";
  const functions = pre_processing.functions;
  const elements = functions.length;

  ret += "# Data slicing.\n    input_buffer_size = None\n    shift_samples = None\n    dataset_cols_size = None";
  
  ret += "\n\n    # Sensors.\n";
  Object.keys(sensors).forEach((component_name) => {
      Object.keys(sensors[component_name]).forEach((parameter_name) => {
          ret += "    " + (component_name + "_" + parameter_name).toLowerCase() + " = None\n";
      });
  });
  ret += "\n"
  
  for (var element = 0; element < elements; element++) {
    const item = functions[element];
    switch (item.name) {
      case "axis_selection":
        code = "    # Axis selection.\n    axis_selection_axis = None";
        break;
      case "mean_removal":
        code = "";
        break;
      case "signal_normalization":
        code = "    # Signal normalization.\n    signal_normalization_peak_to_peak = None\n    signal_normalization_offset = None";
        break;
      case "hanning":
        code = "";
        break;
      case "fft":
        code = "    # FFT.\n    fft_signal_windowing = None";
        break;
      case "mfcc":
        code = "    # MFCC.\n    mfcc_triangular_filters_scale = None\n    mfcc_signal_windowing = None\n    mfcc_bin = None";
        break;
    }
    
    ret += code;
    ret += (element === elements - 1 || code === "" ? "" : "\n\n");
  }

  return ret;
});

Handlebars.registerHelper("sensors_each", function(context, options) {
  let res = ""
  Object.keys(context).forEach((component_name) => {
      Object.keys(context[component_name]).forEach((parameter_name) => {
          res += "pre_processing_data." + (component_name + "_" + parameter_name).toLowerCase() + " = " + context[component_name][parameter_name] + "\n    ";
      });    
  });
  return res;
})

Handlebars.registerHelper("pre_processing_init_each", function(context, options) {
  let ret = "";
  let code = "";
  let data_out_size_mode = undefined;
  let data_out_size = undefined;
  const functions = context.functions;
  const elements = functions.length;
  let final_data_out_size = context.input_buffer_size;

  for (var element = 0; element < elements; element++) {
    const item = functions[element];
    switch (item.name) {
      case "axis_selection":
        code = "# Axis selection.\n    pre_processing_data.axis_selection_axis = \"" + item.parameters.axes[0].toLowerCase() + "\"";
        data_out_size = "1";
        data_out_size_mode = "relative";
        break;
      case "mean_removal":
        code = "";
        data_out_size = "1";
        data_out_size_mode = "relative";
        break;
      case "signal_normalization":
        code = "# Signal normalization.\n    pre_processing_data.signal_normalization_peak_to_peak = " + item.parameters.peak_to_peak + "\n    pre_processing_data.signal_normalization_offset = " + item.parameters.offset + "";
        data_out_size = "1";
        data_out_size_mode = "relative";
        break;
      case "hanning":
        code = "";
        data_out_size = "1";
        data_out_size_mode = "relative";
        break;
      case "fft":
        code = "# FFT.\n    pre_processing_data.fft_signal_windowing = \"" + item.parameters.signal_windowing + "\"";
        data_out_size = "0.5";
        data_out_size_mode = "relative";
        break;
      case "mfcc":
        code = "# MFCC.\n    pre_processing_data.mfcc_triangular_filters_scale = \"" + item.parameters.triangular_filters_scale + "\"\n    " +
               "pre_processing_data.mfcc_signal_windowing = \"" + item.parameters.signal_windowing + "\"\n    " +
    	       "pre_processing_data.mfcc_bin = triangular_filters_init(pre_processing_data.input_buffer_size, pre_processing_data.ism330dhcx_acc_odr, pre_processing_data.mfcc_triangular_filters_scale)";
        data_out_size = "128";
        data_out_size_mode = "absolute";
        break;
    }
    
    final_data_out_size = (data_out_size_mode === "relative" ? (final_data_out_size + (data_out_size === "1" ? "" : (" * " + data_out_size))) : (data_out_size));

    ret += code;
    ret += (element === elements - 1 || code === "" ? "" : "\n\n    ");
  }
  
  ret += "\n\n    # Data slicing.\n    pre_processing_data.shift_samples = " + context.shift_samples + "\n    pre_processing_data.dataset_cols_size = " + final_data_out_size;

  return ret;
});

Handlebars.registerHelper("pre_processing_process_each", function(context, options) {
  let ret = "";
  let code = "";
  const elements = context.length;

  for (var element = 0; element < elements; element++) {
    const item = context[element];
    //let previous_item = element > 0 ? context[element - 1] : undefined;
    switch (item.name) {
      case "axis_selection":
        code = "# Axis selection.\n        {{DATA_OUT}} = axis_selection({{DATA_IN}}, pre_processing_data.axis_selection_axis)";
        break;
      case "mean_removal":
        code = "# Mean removal.\n        {{DATA_OUT}} = mean_removal({{DATA_IN}})";
        break;
      case "signal_normalization":
        code = "# Signal normalization.\n        {{DATA_OUT}} = signal_normalization({{DATA_IN}}, pre_processing_data.signal_normalization_peak_to_peak, pre_processing_data.signal_normalization_offset)";
        break;
      case "hanning":
        code = "# Hanning.\n        {{DATA_OUT}} = hanning({{DATA_IN}})";
        break;
      case "fft":
        code = "# FFT.\n        {{DATA_OUT}} = fft_({{DATA_IN}}, pre_processing_data.fft_signal_windowing)";
        break;
      case "mfcc":
        code = "# MFCC.\n        {{DATA_OUT}} = mfcc_({{DATA_IN}}, pre_processing_data.mfcc_bin, pre_processing_data.mfcc_signal_windowing)";
        break;
    }
  
    switch (element) {
      case 0:
        code = code.replaceAll("{{DATA_IN}}", "sub_df");
        code = code.replaceAll("{{DATA_OUT}}", "data_0");
        break;
      case (elements - 1):
        code = code.replaceAll("{{DATA_IN}}", "data_" + (element - 1));
        code = code.replaceAll("{{DATA_OUT}}", "data_out");
        break;
      default:
        code = code.replaceAll("{{DATA_IN}}", "data_" + (element - 1));
        code = code.replaceAll("{{DATA_OUT}}", "data_" + element);
        break;
    }
  
    //ret = ret + options.fn(item);
    ret += code;
    ret += (element === elements - 1 || code === "" ? "" : "\n\n        ");
  }

  return ret;
});
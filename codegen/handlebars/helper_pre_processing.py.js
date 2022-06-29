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
    //let previous_item = element > 0 ? context[element - 1] : undefined;
    switch (item.name) {
      case "axis_selection":
        code = "# Axis selection.\n    pre_processing_data.axis = \"" + item.parameters.selected_axes[0].toLowerCase() + "\"\n\n    ";
        data_out_size = "1";
        data_out_size_mode = "relative";
        break;
      case "mean_removal":
        code = "";
        data_out_size = "1";
        data_out_size_mode = "relative";
        break;
      case "signal_normalization":
        code = "";
        data_out_size = "1";
        data_out_size_mode = "relative";
        break;
      case "hanning":
        code = "";
        data_out_size = "1";
        data_out_size_mode = "relative";
        break;
      case "fft":
        code = "# FFT.\n    pre_processing_data.signal_windowing = \"" + item.parameters.signal_windowing + "\"\n\n    ";
        data_out_size = "0.5";
        data_out_size_mode = "relative";
        break;
      case "mfcc":
        code = "# MFCC.\n    pre_processing_data.triangular_filters_scale = \"" + item.parameters.triangular_filters_scale + "\"\n    " +
               "pre_processing_data.signal_windowing = \"" + item.parameters.signal_windowing + "\"\n    " +
    	       "pre_processing_data.bin = triangular_filters_init(pre_processing_data.input_buffer_size, pre_processing_data.ism330dhcx_odr, pre_processing_data.triangular_filters_scale)\n\n    ";
        data_out_size = "128";
        data_out_size_mode = "absolute";
        break;
    }
    
    final_data_out_size = (data_out_size_mode === "relative" ? (final_data_out_size + (data_out_size === "1" ? "" : (" * " + data_out_size))) : (data_out_size));

    ret += code;
  }
  
  ret += "# Data slicing.\n    pre_processing_data.shift_samples = " + context.shift_samples + "\n    pre_processing_data.data_out_size = " + final_data_out_size + "\n\n    ";

  return ret;
});

//ret = ret + options.fn(item);
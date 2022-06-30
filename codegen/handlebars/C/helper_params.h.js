Handlebars.registerHelper("sensors_each", function(context, options) {
  let res = ""
  Object.keys(context).forEach((component_name) => {
      Object.keys(context[component_name]).forEach((parameter_name) => {
          //let f = ""
          //if (context[component_name][parameter_name].toString().includes("."))
          //    f = "f"
          res += "#define " + (component_name + "_" + parameter_name).toUpperCase() + " (" + context[component_name][parameter_name] + ")\n";
      });    
  });
  return res;
})

Handlebars.registerHelper("pre_processing_each", function(context, options) {
let ret = "";
let code = "";
const functions = context.functions;
const elements = functions.length;

ret += "#define INPUT_BUFFER_SIZE (" + context.input_buffer_size + ")\n";

for (var element = 0; element < elements; element++) {
  const item = functions[element];
  switch (item.name) {
    case "axis_selection":
      code = "// Axis selection.\n#define AXIS_SELECTION_AXIS (" + item.parameters.axes[0] + ")\n";
      break;
    case "mean_removal":
      code = "";
      break;
    case "signal_normalization":
      code = "// Signal normalization.\n#define SIGNAL_NORMALIZATION_PEAK_TO_PEAK (" + item.parameters.peak_to_peak + ")\n#define SIGNAL_NORMALIZATION_OFFSET (" + item.parameters.offset + ")\n";
      break;
    case "hanning":
      code = "";
      break;
    case "fft":
      code = "// FFT.\n#define FTT_SIGNAL_WINDOWING (" + item.parameters.signal_windowing + ")\n";
      break;
    case "mfcc":
      code = "// MFCC.\n#define MFCC_TRIANGULAR_FILTERS_SCALE (" + item.parameters.triangular_filters_scale + ")\n#define MFCC_SIGNAL_WINDOWING (" + item.parameters.signal_windowing + ")\n";
      break;
  }
  
  ret += code;
}

return ret;
});

Handlebars.registerHelper("ai_processing_each", function(context, options) {
let ret = "";

ret += "#define NETWORK_NAME (\"" + context.network_name + "\")\n";

return ret;
});
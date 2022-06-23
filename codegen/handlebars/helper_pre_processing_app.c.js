Handlebars.registerHelper("pipeline_input_output_each", function(context, options) {
    let ret = "";
	const length = context.length;
    let previousItemDataOutSize = undefined;
    
    for (var i = 0; i < length; i++) {
      const item =  context[i];
      let previousItem = i>0 ? context[i-1] : undefined;
      switch(i){
        case 0:
          item.code_process = item.code_process.replaceAll("{{DATA_IN}}", "data_in");
          item.code_process = item.code_process.replaceAll("{{DATA_OUT}}", "data_0");
          item.code_process = item.code_process.replaceAll("{{DATA_IN_SIZE}}", "data_in_size");
          item.code_process = item.code_process.replaceAll("{{DATA_OUT_SIZE}}", "data_in_size" + (item.data_in_out_size_factor === "1" ? "" : (" * " +  item.data_in_out_size_factor)));
          previousItemDataOutSize = "data_in_size" + (item.data_in_out_size_factor === "1" ? "" : (" * " +  item.data_in_out_size_factor));
          break;
        case (length-1):
          item.code_process = item.code_process.replaceAll("{{DATA_IN}}", "data_" + (i-1));
          item.code_process = item.code_process.replaceAll("{{DATA_OUT}}", "data_out");
          item.code_process = item.code_process.replaceAll("{{DATA_IN_SIZE}}", previousItemDataOutSize);
          item.code_process = item.code_process.replaceAll("{{DATA_OUT_SIZE}}", "data_out_size");
          break;
        default:
          item.code_process = item.code_process.replaceAll("{{DATA_IN}}", "data_" + (i-1));
          item.code_process = item.code_process.replaceAll("{{DATA_OUT}}", "data_" + i);
          item.code_process = item.code_process.replaceAll("{{DATA_IN_SIZE}}", previousItemDataOutSize);
          item.code_process = item.code_process.replaceAll("{{DATA_OUT_SIZE}}", previousItemDataOutSize + (item.data_in_out_size_factor === "1" ? "" : (" * " +  item.data_in_out_size_factor)));
          previousItemDataOutSize = previousItemDataOutSize + (item.data_in_out_size_factor === "1" ? "" : (" * " +  item.data_in_out_size_factor));
          break;
      }
      ret = ret + options.fn(item);
    }

    return ret;
});
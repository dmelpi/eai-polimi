Handlebars.registerHelper("sensors_each", function(context, options) {
    let res = ""
    Object.keys(context).forEach((component_name) => {
        Object.keys(context[component_name]).forEach((parameter_name) => {
            let f = ""
            if (context[component_name][parameter_name].toString().includes("."))
                f = "f"
            res += "#define " + (component_name + "_" + parameter_name).toUpperCase() + " (" + context[component_name][parameter_name] + f + ")\n";
        });    
    });
    return res;
})

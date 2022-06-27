Handlebars.registerHelper("component_parameter_each", function(context, options) {
    let res = ""
    Object.keys(context).forEach( componentName => {
        Object.keys(context[componentName]).forEach((paramName) => {
            let f = ""
            if (context[componentName][paramName].toString().includes("."))
                f = "f"
            res += "#define " + componentName.substring(0, componentName.length - 3).toUpperCase() + paramName.toUpperCase() + " (" + context[componentName][paramName] + f + ")\n";
        });    
    });
    return res;
})
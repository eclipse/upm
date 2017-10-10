
const fs = require('fs');

const template_js_module =
`    <js-module src="www/_CLASS_NAME_.js" name="_CLASS_NAME_">
        <clobbers target="window._CLASS_NAME_" />
    </js-module>
`;

const template_config_feature = 
`            <feature name="_CLASS_NAME_Plugin">
                <param name="android-package" value="upm_plugin._CLASS_NAME_Plugin"/>
            </feature>
`;

const template_source_file =
`        <source-file src="src/android/_CLASS_NAME_Plugin.java" target-dir="src/upm_plugin" />
`;

var generatePluginXml = function(javaPackage) {
    var template = fs.readFileSync([__dirname, 'template', 'plugin.xml'].join('/')).toString();
    template = template.replace(/_PACKAGE_NAME_/g, javaPackage.name);
    template = template.replace(/_VERSION_/g, javaPackage.version);

    var jsModuleStr = '';
    var configFeatureStr = '';
    var sourceFileStr = '';

    javaPackage.classes.forEach(function(javaClass) {
        jsModuleStr += template_js_module;
        jsModuleStr = jsModuleStr.replace(/_CLASS_NAME_/g, javaClass.name);

        configFeatureStr += template_config_feature;
        configFeatureStr = configFeatureStr.replace(/_CLASS_NAME_/g, javaClass.name);

        sourceFileStr += template_source_file;
        sourceFileStr = sourceFileStr.replace(/_CLASS_NAME_/g, javaClass.name);
    });

    template = template.replace('_JS_MODULES', jsModuleStr);
    template = template.replace('_CONFIG_FEATURE_', configFeatureStr);
    template = template.replace('_SOURCE_FILE_', sourceFileStr);

    return template;
};

var generatePackagejson = function(javaPackage) {
    var template = fs.readFileSync([__dirname, 'template', 'package.json'].join('/')).toString();
    template = template.replace(/_PACKAGE_NAME_/g, javaPackage.name);
    template = template.replace(/_VERSION_/g, javaPackage.version);
    return template;
};

var generateGradle = function(javaPackage) {
    var template = fs.readFileSync([__dirname, 'template', 'androidThings.gradle'].join('/')).toString();
    template = template.replace(/_PACKAGE_NAME_/g, javaPackage.name);
    template = template.replace(/_VERSION_/g, javaPackage.version);
    return template;
};

const template_method_interface_no_return =
`    _METHOD_NAME_(_ARGS_): void;

`;

const template_method_interface_return =
`    _METHOD_NAME_(_PREFIX_ARGS_onResult: (result: _RETURN_TYPE_) => void): void;

`;

const template_class_interface =
`interface _CLASS_NAME_ {
    _id: number;

_METHOD_INTERFACE_
};
`;

const template_declare_interface =
`
declare var _CLASS_NAME_: {
    new (_ARGS_): _CLASS_NAME_;
};
`;

var generateInterface = function(javaPackage) {
    var ret = '';

    javaPackage.classes.forEach(function(javaClass) {
        var methodInterfaceStr = '';
        javaClass.methods.forEach(function(method) {
            if (method.returnType == 'void') {
                var template = template_method_interface_no_return;
                template = template.replace(/_METHOD_NAME_/g, method.name);
                template = template.replace(/_ARGS_/g, method.getJsArgs());
                methodInterfaceStr += template;
            } else {
                var template = template_method_interface_return;
                template = template.replace(/_METHOD_NAME_/g, method.name);
                template = template.replace(/_RETURN_TYPE_/g, method.getJsReturnType);
                template = template.replace(/_PREFIX_ARGS_/g, method.getPrefixJsArgs());
                methodInterfaceStr += template;
            }
        });

        var template = template_class_interface;
        template = template.replace(/_CLASS_NAME_/g, javaClass.name);
        template = template.replace(/_METHOD_INTERFACE_/g, methodInterfaceStr);
        ret += template;

        if (javaClass.constructor != null) {
            var template = template_declare_interface;
            template = template.replace(/_CLASS_NAME_/g, javaClass.name);
            template = template.replace(/_ARGS_/g, javaClass.constructor.getJsArgs());
            ret += template;
        }
    });
    return ret;
};

const template_no_return =
`_CLASS_NAME_.prototype._METHOD_NAME_ = function(_ARG_NAMES_) {
    exec(null, null, "_CLASS_NAME_Plugin", "_METHOD_NAME_", [this.id_POSTFIX_ARG_NAMES_]);
};

`;

const template_return =
`_CLASS_NAME_.prototype._METHOD_NAME_ = function(_PREFIX_ARG_NAMES_onResult) {
    var callback = function(result) { 
        onResult(result[0]); 
    }
    exec(callback, null, "_CLASS_NAME_Plugin", "_METHOD_NAME_", [this.id_POSTFIX_ARG_NAMES_]);
};

`;

const template_constructor =
`
    exec(null, null, "_CLASS_NAME_Plugin", "create", [this.id_POSTFIX_ARG_NAMES_]);
`;

var generatePluginJs = function(javaClass) {
    var template = fs.readFileSync([__dirname, 'template', 'plugin.js'].join('/')).toString();

    var constructorStr = '';
    if (javaClass.constructor != null) {
        var template1 = template_constructor;
        template1 = template1.replace(/_POSTFIX_ARG_NAMES_/g, javaClass.constructor.getPostfixArgNames());
        template1 = template1.replace(/_PREFIX_ARG_NAMES_/g, javaClass.constructor.getPrefixArgNames());
        template1 = template1.replace(/_ARG_NAMES_/g, javaClass.constructor.getArgNames());
        constructorStr += template1;
    } else {
    }
    template = template.replace('_CONSTRUCTOR_', constructorStr);

    var methodStr = '';
    javaClass.methods.forEach(function(method) {
        if (method.returnType == 'void') {
            methodStr += template_no_return;
            methodStr = methodStr.replace(/_METHOD_NAME_/g, method.name);
            methodStr = methodStr.replace(/_POSTFIX_ARG_NAMES_/g, method.getPostfixArgNames());
            methodStr = methodStr.replace(/_PREFIX_ARG_NAMES_/g, method.getPrefixArgNames());
            methodStr = methodStr.replace(/_ARG_NAMES_/g, method.getArgNames());
        } else {
            methodStr += template_return;
            methodStr = methodStr.replace(/_METHOD_NAME_/g, method.name);
            methodStr = methodStr.replace(/_POSTFIX_ARG_NAMES_/g, method.getPostfixArgNames());
            methodStr = methodStr.replace(/_PREFIX_ARG_NAMES_/g, method.getPrefixArgNames());
            methodStr = methodStr.replace(/_ARG_NAMES_/g, method.getArgNames());
        }
    });

    template = template.replace('_METHODS_', methodStr);
    template = template.replace(/_CLASS_NAME_/g, javaClass.name);
    return template;
};

const template_invoke_no_return =
`        if (action.equals("_METHOD_NAME_")) {
           object._METHOD_NAME_();
           return true;
        } 

`;
        
const template_invoke_return =
`        if (action.equals("_METHOD_NAME_")) {
            _RETURN_TYPE_ result = object._METHOD_NAME_(args.getInt(1));
            callbackContext.success(new JSONArray().put(result));
            return true;
        }

`;

var generatePluginJava = function(javaClass) {
    var invocationStr = '';
    javaClass.methods.forEach(function(method) {
        if (method.returnType == 'void') {
            var template = template_invoke_no_return;
            template = template.replace(/_METHOD_NAME_/g, method.name);
            invocationStr += template;
        } else {
            var template = template_invoke_return;
            template = template.replace(/_METHOD_NAME_/g, method.name);
            template = template.replace(/_RETURN_TYPE_/g, method.returnType);
            invocationStr += template;
        }
    });

    var template = fs.readFileSync([__dirname, 'template', 'plugin.java'].join('/')).toString();
    template = template.replace(/_PACKAGE_NAME_/g, javaClass.packageName);
    template = template.replace(/_CLASS_NAME_/g, javaClass.className);
    template = template.replace(/_INVOKATIONS_/g, invocationStr);
    return template;
};

module.exports = {
    generatePluginXml,
    generatePackagejson,
    generateGradle,
    generateInterface,
    generatePluginJs,
    generatePluginJava
};


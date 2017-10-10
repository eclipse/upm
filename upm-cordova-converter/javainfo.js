
const Java2JSType = {
    "int": "number",
    "String": "string"
};

var JavaPackage = function(name) {
    this.name = name;
    this.classes = new Array();
    this.version = '1.0';
}

JavaPackage.prototype.getPluginName = function() {
    return "cordova-plugin-upm-" + this.name;
};

JavaPackage.prototype.getUpmPackageName = function() {
    return "upm_" + this.name;
};

var JavaClass = function(name) {
    this.name = name;
    this.constructor = null;
    this.methods = new Array();
    this.packageName = null;
}

var JavaMethod = function(name) {
    this.name = name;
    this.returnType = null;
    this.argTypes = new Array();
    this.argNames = new Array();
}

JavaMethod.prototype.getArgNames = function() {
    return this.argNames.join(', ');
}

JavaMethod.prototype.getPrefixArgNames = function() {
    var ret = this.getArgNames();
    if (ret.length == 0) return '';
    return ret + ', ';
}

JavaMethod.prototype.getPostfixArgNames = function() {
    var ret = this.getArgNames();
    if (ret.length == 0) return '';
    return ', ' + ret;
}

JavaMethod.prototype.getJsArgs = function() {
    var ret = '';
    for (var i = 0; i < this.argTypes.length; ++i) {
        if (ret.length > 0) ret += ', ';
        var type = Java2JSType[this.argTypes[i]];
        if (type == undefined) type = "object";
        ret += this.argNames[i] + ': ' + type;
    }
    return ret;
}

JavaMethod.prototype.getPrefixJsArgs = function() {
    var ret = this.getJsArgs();
    if (ret.length == 0) return '';
    return ret + ', ';
}

JavaMethod.prototype.getJsReturnType = function() {
    var type = Java2JSType[this.returnType];
    if (type == undefined) type = "object";
    return type
}

JavaMethod.prototype.parseArgs = function(args) {
    if (args.length == 0) return;

    var self = this;
    args.split(',').forEach(function(arg) {
        self.argTypes.push(arg.split(' ')[0]);
        self.argNames.push(arg.split(' ')[1]);
    });
};

module.exports = {
    JavaPackage,
    JavaClass,
    JavaMethod
};


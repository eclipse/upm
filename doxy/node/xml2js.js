/*
 * Author: Heidi Pan <heidi.pan@intel.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

// dependencies
var peg     = require('pegjs')
  , fs      = require('fs')
  , path    = require('path')
  , Promise = require('bluebird')
  , _       = require('lodash')
  , util    = require('util');


// use promise-style programming rather than spaghetti callbacks
Promise.promisifyAll(fs);


var xml2js = {

  // js-format specs
  // MODULES: <module name>
  // ENUMS: {
  //   <enum name>: {
  //     type: <enum type>,
  //     description: <enum description>
  //   }, ...
  // }
  // ENUMS_BY_GROUP: {
  //   <enum type>: {
  //     description: <enum group description>
  //     members: [ <enum name>, ... ]
  //   }, ...
  // }
  // METHODS: {
  //   <method name>: {
  //     description: <method description>,
  //     params: {
  //       <param name>: {
  //         type: <param type>,
  //         description: <param description >
  //       }, ...
  //     },
  //     return: {
  //       type: <return type>,
  //       description: <return description>
  //     }
  //   }, ...
  // }
  // CLASSES: {
  //   <class name>: {
  //     description: <class description>,
  //     methods: { ... },
  //     variables: {
  //       <variable name>: {
  //         type: <variable type>,
  //         description: <variable description>
  //       }
  //     },
  //     enums: { ... },
  //     enums_by_group: { ... }
  //   }, ...
  // }
  MODULE: '',
  ENUMS: {},
  ENUMS_BY_GROUP: {},
  METHODS: {},
  CLASSES: {},


  // c -> js type mapping
  TYPEMAPS: {
    '^(const)?\\s*(unsigned|signed)?\\s*(int|short|long|float|double|size_t|u?int\\d{1,2}_t)?$': 'Number',
    '^bool$': 'Boolean',
    '^(const)?\\s*(unsigned|signed)?\\s*(char|char\\s*\\*|std::string)$': 'String'  // TODO: verify that String mappings work
  },


  // add command line options for this module
  addOptions: function(opts) {
    xml2js.opts = opts;
    return opts
      .option('-i, --inputdir [directory]', 'directory for xml files', __dirname + '/xml/mraa')
      .option('-c, --custom [file]', 'json for customizations', __dirname + '/custom.json')
      .option('-s, --strict', 'leave out methods/variables if unknown type')
  },


  // parse doxygen xml -> js-format specs
  // TODO: figure out whether we need to document any protected methods/variables
  parse: function() {
    var XML_GRAMMAR_SPEC = 'grammars/xml.peg';
    var NAMESPACE_SPEC = xml2js.opts.inputdir + '/namespace' + xml2js.opts.module + '.xml';
    var CLASS_SPEC = function(c) { return xml2js.opts.inputdir + '/' + c + '.xml'; }
    var TYPES_SPEC = xml2js.opts.inputdir + '/types_8h.xml';
    xml2js.MODULE = xml2js.opts.module;
    return Promise.join(createXmlParser(XML_GRAMMAR_SPEC),
                        fs.readFileAsync(NAMESPACE_SPEC, 'utf8'),
                        fs.existsSync(TYPES_SPEC) ? fs.readFileAsync(TYPES_SPEC, 'utf8') : Promise.resolve(null),
                        function(xmlparser, xml, xml_types) {
      if (xml_types != null) {
        _.extend(xml2js.ENUMS, getEnums(xmlparser.parse(xml_types)[0], false));
        _.extend(xml2js.ENUMS_BY_GROUP, getEnums(xmlparser.parse(xml_types)[0], true));
      }
      var spec_c = xmlparser.parse(xml)[0];
      _.extend(xml2js.ENUMS, getEnums(spec_c, false));
      _.extend(xml2js.ENUMS_BY_GROUP, getEnums(spec_c, true));
      _.extend(xml2js.METHODS, getMethods(spec_c));
      return Promise.all(_.map(getClasses(spec_c), function(c) {
        return fs.readFileAsync(CLASS_SPEC(c), 'utf8').then(function(xml) {
          try {
            var spec_c = xmlparser.parse(xml)[0];
            var className = getClassName(spec_c);
            xml2js.CLASSES[className] = {
              description: getClassDescription(spec_c),
              enums: getEnums(spec_c, false, className),
              enums_by_group: getEnums(spec_c, true, className),
              variables: getVariables(spec_c, className),
              methods: getMethods(spec_c, className),
            };
          } catch(e) {
            console.log(e.toString() + ': ' + c + ' was not parsed correctly.');
          }
        });
      })).then(function() {
        if (fs.existsSync(xml2js.opts.custom)) {
          return fs.readFileAsync(xml2js.opts.custom, 'utf8').then(function(custom) {
            try {
              customizeMethods(JSON.parse(custom));
            } catch(e) {
              console.log('invalid custom.json, ignored. ' + e.toString());
            }
          });
        } else {
          console.log((xml2js.opts.custom == __dirname + '/custom.json') ? 'No customizations given.' : 'Error: No such customization file exists: ' + xml2js.opts.custom);
        }
      }).then(function() {
        validateMethods();
        validateVars();
        return _.pick(xml2js, 'MODULE', 'ENUMS', 'ENUMS_BY_GROUP', 'METHODS', 'CLASSES');
      });
    });
  }

};


// create an xml parser
function createXmlParser(XML_GRAMMAR_SPEC) {
  return fs.readFileAsync(XML_GRAMMAR_SPEC, 'utf8').then(function(xmlgrammar) {
    return peg.buildParser(xmlgrammar);
  });
}


// override autogenerated methods with custom configuration
function customizeMethods(custom) {
  _.each(custom, function(classMethods, className) {
    _.extend(xml2js.CLASSES[className].methods, _.pick(classMethods, function(methodSpec, methodName) {
      return isValidMethodSpec(methodSpec, className + '.' + methodName);
    }));
  });
}

// make sure methods have valid types, otherwise warn (& don't include if strict)
function validateMethods() {
  xml2js.METHODS = _.pick(xml2js.METHODS, function(methodSpec, methodName) {
    hasValidTypes(methodSpec, methodName);
  });
  _.each(xml2js.CLASSES, function(classSpec, className) {
    var valid = _.pick(classSpec.methods, function(methodSpec, methodName) {
      return hasValidTypes(methodSpec, className + '.' + methodName, className);
    });
    if (xml2js.opts.strict) {
      xml2js.CLASSES[className].methods = valid;
    }
  });
}


// make sure variables have valid types, otherwise warn (& don't include if strict)
function validateVars() {
  _.each(xml2js.CLASSES, function(classSpec, className) {
    var valid = _.pick(classSpec.variables, function(varSpec, varName) {
      return ofValidType(varSpec, className + '.' + varName, className);
    });
    if (xml2js.opts.strict) {
      xml2js.CLASSES[className].variables = valid;
    }
  });
}


// verify that the json spec is well formatted
function isValidMethodSpec(methodSpec, methodName) {
  var valid = true;
  var printIgnoredMethodOnce = _.once(function() { console.log(methodName + ' from ' + path.basename(xml2js.opts.custom) + ' is omitted from JS documentation.'); });
  function checkRule(rule, errMsg) {
    if (!rule) {
      printIgnoredMethodOnce();
      console.log('  ' + errMsg);
      valid = false;
    }
  }
  checkRule(_.has(methodSpec, 'description'), 'no description given');
  checkRule(_.has(methodSpec, 'params'), 'no params given (specify "params": {} for no params)');
  _.each(methodSpec.params, function(paramSpec, paramName) {
    checkRule(_.has(paramSpec, 'type'), 'no type given for param ' + paramName);
    checkRule(_.has(paramSpec, 'description'), 'no description given for param ' + paramName);
  });
  checkRule(_.has(methodSpec, 'return'), 'no return given (specify "return": {} for no return value)');
  checkRule(_.has(methodSpec.return, 'type'), 'no type given for return value');
  checkRule(_.has(methodSpec.return, 'description'), 'no description given for return value');
  return valid;
}


// get enum specifications
function getEnums(spec_c, bygroup, parent) {
  var spec_js = {};
  var enumGroups = _.find(getChildren(spec_c, 'sectiondef'), function(section) {
    var kind = getAttr(section, 'kind');
    return ((kind == 'enum') || (kind == 'public-type'));
  });
  if (enumGroups) {
    _.each(enumGroups.children, function(enumGroup) {
      var enumGroupName = getText(getChild(enumGroup, 'name'), 'name');
      var enumGroupDescription = getText(getChild(enumGroup, 'detaileddescription'), 'description');
      var enumGroupVals = getChildren(enumGroup, 'enumvalue');
      if (bygroup) {
        spec_js[enumGroupName] = {
          description: enumGroupDescription,
          members: []
        };
      }
      _.each(enumGroupVals, function(e) {
        // TODO: get prefix as option
        var enumName = getText(getChild(e, 'name'), 'name').replace(/^MRAA_/, '');
        var enumDescription = getText(getChild(e, 'detaileddescription'), 'description');
        if (!bygroup) {
          spec_js[enumName] = {
            type: enumGroupName,
            description: enumDescription
          };
        } else {
          spec_js[enumGroupName].members.push(enumName);
        }
      });
    });
  }
  return spec_js;
}


// get the classes (xml file names) for the given module
function getClasses(spec_c) {
  return _.map(getChildren(spec_c, 'innerclass'), function(innerclass) {
    return getAttr(innerclass, 'refid');
  });
}


// get the description of the class
function getClassDescription(spec_c) {
  return getText(getChild(spec_c, 'detaileddescription'), 'description');
}


function hasParams(paramsSpec) {
  return !(_.isEmpty(paramsSpec) ||
           ((_.size(paramsSpec) == 1) && getText(getChild(paramsSpec[0], 'type')) == 'void'));
}


// get method specifications for top-level module or a given class
// TODO: overloaded functions
// TODO: functions w/ invalid parameter(s)/return
function getMethods(spec_c, parent) {
  var spec_js = {};
  var methods = _.find(getChildren(spec_c, 'sectiondef'), function(section) {
    var kind = getAttr(section, 'kind');
    return ((kind == 'public-func') || (kind == 'func'));
  });
  if (methods) {
    _.each(methods.children, function(method) {
      var methodName = getText(getChild(method, 'name'), 'name');
      if (methodName[0] != '~') { // filter out destructors
        try {
          var description = getChild(method, 'detaileddescription');
          var methodDescription = getText(description, 'description');
          var paramsSpec = getChildren(method, 'param');
          var params = {};
          if (hasParams(paramsSpec)) {
              params = getParams(paramsSpec, getParamsDetails(description), (parent ? (parent + '.') : '') + methodName);
          }
          var returnSpec = getChild(method, 'type');
          var retval = {};
          if (!_.isEmpty(returnSpec)) {
            retval = getReturn(returnSpec, getReturnDetails(description));
          }
          spec_js[methodName] = {
            description: methodDescription,
            params: params,
            return: retval
          };
        } catch(e) {
          console.log((parent ? (parent + '.') : '') + methodName + ' is omitted from JS documentation.');
          console.log('  ' + e.toString());
        }
      }
    });
  }
  return spec_js;
}


// get variable specifications for a class
function getVariables(spec_c, parent) {
  var spec_js = {};
  var vars = _.find(getChildren(spec_c, 'sectiondef'), function(section) {
    var kind = getAttr(section, 'kind');
    return (kind == 'public-attrib');
  });
  if (vars) {
    _.each(_.filter(vars.children, function(variable) {
      return (getAttr(variable, 'kind') == 'variable');
    }), function(variable) {
      var varName = getText(getChild(variable, 'name'), 'name');
      var varType = getType(getText(getChild(variable, 'type')));
      var varDescription = getText(getChild(variable, 'detaileddescription'));
      spec_js[varName] = {
        type: varType,
        description: varDescription
      }
    });
  }
  return spec_js;
}


// get return value specs of a method
function getReturn(spec_c, details) {
  var retType = getType(getText(spec_c, 'type'));
  var retDescription = (details ? getText(details, 'description') : '');
  return ((retType == 'void') ? {} : {
    type: retType,
    description: retDescription
  });
}


// get paramater specs of a method
function getParams(spec_c, details, method) {
  var spec_js = {};
  _.each(spec_c, function(param) {
    try {
      var paramType = getType(getText(getChild(param, 'type'), 'type'));
      var paramName = getText(getChild(param, 'declname'), 'name');
      spec_js[paramName] = { type: paramType };
    } catch(e) {
      if (paramType == '...') {
        spec_js['arguments'] = { type: paramType };
      } else {
        throw e;
      }
    }
  });
  _.each(details, function(param) {
    var getParamName = function(p) { return getText(getChild(getChild(p, 'parameternamelist'), 'parametername'), 'name'); }
    var paramName = getParamName(param);
    var paramDescription = getText(getChild(param, 'parameterdescription'), 'description');
    if (_.has(spec_js, paramName)) {
      spec_js[paramName].description = paramDescription;
    } else {
      var msg = ' has documentation for an unknown parameter: ' + paramName + '. ';
      var suggestions = _.difference(_.keys(spec_js), _.map(details, getParamName));
      var msgAddendum = (!_.isEmpty(suggestions) ? ('Did you mean ' + suggestions.join(', or ') + '?') : '');
      console.log('Warning: ' + method + msg + msgAddendum);
    }
  });
  return spec_js;
}


// get the equivalent javascript type from the given c type
function getType(type_c) {
  var type_js = type_c;
  _.find(xml2js.TYPEMAPS, function(to, from) {
    var pattern = new RegExp(from, 'i');
    if (type_c.search(pattern) == 0) {
      type_js = to;
    }
  });
  // TODO: temporary solution
  // remove extra whitespace from pointers
  // permanent solution would be to get rid of pointers all together
  if (type_js.search(/\S+\s*\*$/) != -1) {
    type_js = type_js.replace(/\s*\*$/, '*');
  }
  return type_js;
}


// verify that all types associated with the method are valid
function hasValidTypes(methodSpec, methodName, parent) {
  var valid = true;
  var msg = (xml2js.opts.strict ? ' is omitted from JS documentation.' : ' has invalid type(s).');
  var printIgnoredMethodOnce = _.once(function() { console.log(methodName + msg); });
  _.each(methodSpec.params, function(paramSpec, paramName) {
    if (!isValidType(paramSpec.type, parent)) {
      valid = false;
      printIgnoredMethodOnce();
      console.log('  Error: parameter ' + paramName + ' has invalid type ' + paramSpec.type);
    }
  });
  if (!_.isEmpty(methodSpec.return) && !isValidType(methodSpec.return.type, parent)) {
    valid = false;
    printIgnoredMethodOnce();
    console.log('  Error: returns invalid type ' + methodSpec.return.type);
  }
  return valid;
}


// verify that type of variable is valid
function ofValidType(varSpec, varName, parent) {
  if (isValidType(varSpec.type, parent)) {
    return true;
  } else {
    var msgAddendum = (xml2js.opts.strict ? ' Omitted from JS documentation.' : '');
    console.log('Error: ' + varName + ' is of invalid type ' + varSpec.type + '.' + msgAddendum);
    return false;
  }
}


// verify whether the given type is valid JS
// TODO: check class-specific types
function isValidType(type, parent) {
  return (_.contains(_.values(xml2js.TYPEMAPS), type) ||
          _.has(xml2js.CLASSES, type) ||
          _.has(xml2js.ENUMS_BY_GROUP, type) ||
          _.contains(['Buffer', 'Function', 'mraa_result_t'], type) ||
          _.has((parent ? xml2js.CLASSES[parent].enums_by_group : []), type));
}


// determines whether a type looks like a c pointer
function isPointer(type) {
  return (type.search(/\w+\s*\*/) != -1);
}


// get the detailed description of a method's parameters
function getParamsDetails(spec_c) {
  var paras = getChildren(spec_c, 'para');
  var details = _.find(_.map(paras, function(para) {
    return getChild(para, 'parameterlist');
  }), function(obj) { return (obj != undefined); });
  return (details ? details.children : undefined);
}


// get the detailed description of a method's return value
function  getReturnDetails(spec_c) {
  var paras = getChildren(spec_c, 'para');
  return _.find(_.map(paras, function(para) {
    return getChild(para, 'simplesect');
  }), function(obj) { return ((obj != undefined) && (getAttr(obj, 'kind') == 'return')); });
}


// get (and flatten) the text of the given object
function getText(obj, why) {
  // TODO: links ignored for now, patched for types for
  var GENERATE_LINK = function(x) { return x + ' '; }
  return _.reduce(obj.children, function(text, elem) {
    if (_.isString(elem)) {
      return text += elem.trim() + ' ';
    } else if (_.isPlainObject(elem)) {
      switch(elem.name) {
        case 'para':
          return text += getText(elem, why) + '  \n';
        case 'ref':
          return text += GENERATE_LINK(getText(elem, why));
        case 'parameterlist':
        case 'simplesect':
          return text; // to be handled elsewhere
        case 'programlisting':
        case 'htmlonly':
          return text; // ignored
        // TODO: html doesn't seem to work, using markdown for now
        case 'itemizedlist':
          return text += '\n' + getText(elem, why) + '\n';
        case 'listitem':
          return text += '+ ' + getText(elem, why) + '\n';
        case 'bold':
          return text += '__' + getText(elem, why).trim() + '__ ';
        case 'ulink':
          return text += '[' + getText(elem, why).trim() + '](' + getAttr(elem, 'url').trim() + ') ';
        case 'image':
          // TODO: copy images over; hard coded for now
          var fn = getAttr(elem, 'name');
          return text += '  \n  \n![' + fn + '](' + '../../../../docs/images/' + fn + ') ';
        case 'linebreak':
          return text += '  \n';
        case 'ndash':
          return text += '&ndash; ';
        default:
          // TODO: incomplete list of doxygen xsd implemented
          throw new Error('NYI Unknown Object Type: ' + elem.name);
      }
    } else {
      throw new Error('NYI Unknown Type: ' + (typeof elem));
    }
  }, '').trim();
}


// get the value of attribute with the given name of the given object
function getAttr(obj, name) {
  return _.find(obj.attr, function(item) {
    return item.name == name;
  }).value;
}


// get the child object with the given name of the given object
function getChild(obj, name) {
  return _.find(obj.children, function(child) {
    return child.name == name;
  });
}


// get all children objects with the given name of the given object
function getChildren(obj, name) {
  return _.filter(obj.children, function(child) {
    return child.name == name;
  });
}


// get the class name from its xml spec
function getClassName(spec_c) {
  return getText(getChild(spec_c, 'compoundname'), 'name').replace(xml2js.opts.module + '::', '');
}


// debug helper: print untruncated object
function printObj(obj) {
  console.log(util.inspect(obj, false, null));
}


module.exports = xml2js;

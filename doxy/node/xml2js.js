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
  // MODULE: <module name>
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
  //     parent: <parent class name>,
  //     group: <group name>,
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
  // CLASSGROUPS: {
  //   <group name>: {
  //     description: <group description>,
  //     classes: [ <class name>, ... ],
  //     enums: { ... },
  //     enums_by_group: { ... }
  //   }, ...
  // }
  MODULE: '',
  ENUMS: {},
  ENUMS_BY_GROUP: {},
  METHODS: {},
  CLASSES: {},
  CLASSGROUPS: {},


  // baseline c -> js type mapping
  TYPEMAPS: {
    '^(const)?\\s*(unsigned|signed)?\\s*(int|short|long|float|double|size_t|u?int\\d{1,2}_t)?$': 'Number',
    '^bool$': 'Boolean',
    '^(const)?\\s*(unsigned|signed)?\\s*(char|char\\s*\\*|std::string)$': 'String',  // TODO: verify that swig does this mapping
    '^void\\s*\\(\\s*\\*\\s*\\)\\s*\\(\\s*void\\s*\\*\\)\\s*$': 'Function'
  },


  // custom c -> js type mapping for pointers
  // ARRAY_TYPEMAPS: {
  //    <pointer data type>: {
  //      arrayType: <swig generated array type that will replace pointers of data type>,
  //      classes: [ <class that contains arrayType>, ... ]
  //    }, ...
  // }
  // POINTER_TYPEMAPS: {
  //    <class that contains pointerType>: {
  //      <c pointer data type>: <js swig generated pointer type that will replace pointers of data type>, ...
  //    }, ...
  // }
  ARRAY_TYPEMAPS: {},
  POINTER_TYPEMAPS: {},


  // add command line options for this module
  addOptions: function(opts) {
    xml2js.opts = opts;
    return opts
      .option('-i, --inputdir [directory]', 'directory for xml files', __dirname + '/xml/mraa')
      .option('-c, --custom [file]', 'json for customizations')
      .option('-t, --typemaps [directory]', 'directory for custom pointer type maps')
      .option('-g, --imagedir [directory]', 'directory to link to where the images will be kept', '')
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
                        xml2js.opts.typemaps ? initCustomPointerTypemaps(xml2js.opts.typemaps) : Promise.resolve(),
                        fs.readFileAsync(NAMESPACE_SPEC, 'utf8'),
                        fs.existsSync(TYPES_SPEC) ? fs.readFileAsync(TYPES_SPEC, 'utf8') : Promise.resolve(null),
                        function(xmlparser, ignore, xml, xml_types) {
      if (xml_types != null) {
        _.extend(xml2js.ENUMS, getEnums(xmlparser.parse(xml_types)[0], false));
        _.extend(xml2js.ENUMS_BY_GROUP, getEnums(xmlparser.parse(xml_types)[0], true));
      }
      var spec_c = xmlparser.parse(xml)[0];
      _.extend(xml2js.ENUMS, getEnums(spec_c, false));
      _.extend(xml2js.ENUMS_BY_GROUP, getEnums(spec_c, true));
      _.extend(xml2js.METHODS, getMethods(spec_c));
      _.each(getSubclassNames(spec_c), function(className) { xml2js.CLASSES[className] = {} });
      var parseClasses = _.map(getSubclasses(spec_c), function(c) {
        return fs.readFileAsync(CLASS_SPEC(c), 'utf8').then(function(xml) {
          try {
            var spec_c = xmlparser.parse(xml)[0];
            var className = getName(spec_c);
            _.extend(xml2js.CLASSES[className], {
              description: getDescription(spec_c),
              parent: getParent(spec_c, className),
              enums: getEnums(spec_c, false, className),
              enums_by_group: getEnums(spec_c, true, className),
              variables: getVariables(spec_c, className),
              methods: getMethods(spec_c, className)
            });
          } catch(e) {
            console.log(e.toString() + ': class ' + className + ' was not parsed correctly.');
          }
        });
      });
      var parseGroups = fs.readdirAsync(xml2js.opts.inputdir).then(function(files) {
        var groupxmlfiles = _.filter(files, function(fn) {
          return ((path.extname(fn) == '.xml') && (path.basename(fn).search(/^group/) != -1));
        });
        return Promise.all(_.map(groupxmlfiles, function(fn) {
          return fs.readFileAsync(xml2js.opts.inputdir + '/' + fn, 'utf8').then(function(xml) {
            var spec_c = xmlparser.parse(xml)[0];
            if (_.isEmpty(getSubmodules(spec_c))) {
              var group = getName(spec_c);
              var classes = getSubclassNames(spec_c);
              xml2js.CLASSGROUPS[group] = {
                description: getDescription(spec_c),
                classes: classes
              };
              _.each(classes, function(c) {
                if (_.has(xml2js.CLASSES, c)) {
                  xml2js.CLASSES[c].group = group;
                } else {
                  console.log('Warning: Group ' + group + ' has unknown class ' + c);
                }
              });
            }
          });
        }));
      });
      return Promise.all(parseClasses.concat(parseGroups));
    }).then(function() {
      if (!_.isEmpty(xml2js.CLASSGROUPS)) {
        // try to categorize ungrouped classes, if any
        var grouped = _.flatten(_.pluck(_.values(xml2js.CLASSGROUPS), 'classes'));
        var ungrouped = _.difference(_.keys(xml2js.CLASSES), grouped);
        _.each(ungrouped, function(c) {
          _.each(findUsage(c), function(group) {
            xml2js.CLASSGROUPS[group].classes.push(c);
          });
        });
        grouped = _.flatten(_.pluck(_.values(xml2js.CLASSGROUPS), 'classes'));
        ungrouped = _.difference(_.keys(xml2js.CLASSES), grouped);
        // try to categorize ungrouped enums, if any
        _.each(xml2js.ENUMS_BY_GROUP, function(enumGroupSpec, enumGroupName) {
          _.each(findUsage(enumGroupName, true), function(c) {
            xml2js.CLASSES[c].enums_by_group[enumGroupName] = enumGroupSpec;            
            _.each(enumGroupSpec.members, function(enumName) {
              xml2js.CLASSES[c].enums[enumName] = xml2js.ENUMS[enumName];
              delete xml2js.ENUMS[enumName];
            });
            delete xml2js.ENUMS_BY_GROUP[enumGroupName];
          });
        });
      }
    }).then(function() {
      if (xml2js.opts.custom && fs.existsSync(xml2js.opts.custom)) {
        return fs.readFileAsync(xml2js.opts.custom, 'utf8').then(function(custom) {
          try {
            customizeMethods(JSON.parse(custom));
          } catch(e) {
            console.log('invalid custom.json, ignored. ' + e.toString());
          }
        });
      } else {
        console.log(xml2js.opts.custom ? ('Error: No such customization file exists: ' + xml2js.opts.custom) : 'No customizations given.');
      }
    }).then(function() {
      generateCustomPointerClasses();       
      validateMethods();
      validateVars();
      return _.pick(xml2js, 'MODULE', 'ENUMS', 'ENUMS_BY_GROUP', 'METHODS', 'CLASSES', 'CLASSGROUPS');
    });
  }
};


// create an xml parser
function createXmlParser(XML_GRAMMAR_SPEC) {
  return fs.readFileAsync(XML_GRAMMAR_SPEC, 'utf8').then(function(xmlgrammar) {
    return peg.buildParser(xmlgrammar);
  });
}


// configure c->js typemaps from custom swig directives
// TODO: many built in assumptions based on current upm file structures & .i customizations
function initCustomPointerTypemaps(typemapsdir) {
  return fs.readdirAsync(typemapsdir).then(function(dirs) {
    return Promise.all(_.map(dirs, function(dir) {
      // get all js*.i directives from class-specific subdirectories, to be parsed below for %typemaps directives
      return fs.readdirAsync(typemapsdir + '/' + dir).then(function(files) {
        var directive = _.find(files, function(fn) {
          return ((path.extname(fn) == '.i') && (path.basename(fn).search(/^js/) != -1));
        });
        var data = {};
        if (directive) {
          data[dir] = typemapsdir + '/' + dir + '/' + directive;
        }
        return data;
      }).catch(function(e) {
        // get all .i directives from top level directory, and parse for %array_class directives
        if (e.code == 'ENOTDIR') {
          var fn = dir;
          if (path.extname(fn) == '.i') {
            return fs.readFileAsync(typemapsdir + '/' + fn, 'utf8').then(function(directives) {
              var arraytypes = _.filter(directives.split(/\n/), function(line) {
                return (line.search(/^%array_class/) != -1);
              });
              _.each(arraytypes, function(arraytype) {
                var parsed = arraytype.match(/%array_class\(([A-Za-z0-9_]+)[\s]*,[\s]*([A-Za-z0-9_]+)\)/);
                if (parsed) {
                  var from = parsed[1];
                  var to = parsed[2];
                  xml2js.ARRAY_TYPEMAPS[from] = { arrayType: to, classes: [] };
                } else {
                  console.log('Incorrectly parsed array_class from ' + fn + ': ' + arraytype);
                }
              });
            });
          }
        } else {
          throw e;
        }
      });
    }));
  }).then(function(__directivesFiles) {
    // parse for %typemaps & %pointer_functions directives
    var _directivesFiles = _.filter(__directivesFiles, function(data) { return !_.isEmpty(data); });
    var directivesFiles = _.object(_.map(_directivesFiles, _.keys), _.flatten(_.map(_directivesFiles, _.values)));
    return Promise.all(_.map(directivesFiles, function(directivesFn, className) {
      return fs.readFileAsync(directivesFn, 'utf8').then(function(directives) {
        var typemaps = _.filter(directives.split(/\n/), function(line) {
          return (line.search(/^%typemap/) != -1);
        });
        _.each(typemaps, function(typemap) {
          var parsed = typemap.match(/%typemap\((in|out)\)[\s]+([A-Za-z0-9_]+[\s]*[\*])/);
          if (parsed) {
            var dir = parsed[1]; // TODO: ignored for now
            var type = normalizePointer(parsed[2]);
            var datatype = getPointerDataType(type);
            if (_.has(xml2js.ARRAY_TYPEMAPS, datatype)) {
              xml2js.ARRAY_TYPEMAPS[datatype].classes.push(className);
            } else {
              console.log('Ignored typemap from ' + directivesFn + ': ' + typemap.replace('{', '') + ' (no %array_class directive found for ' + datatype + ')');
            }
          } else {
            console.log('Ignored typemap from ' + directivesFn + ': ' + typemap.replace('{', '') + ' (only considering in/out typemaps of pointer types)');
          }
        });
        var ptrfns = _.filter(directives.split(/\n/), function(line) {
          return (line.search(/^%pointer_functions/) != -1);
        });
        _.each(ptrfns, function(ptrfn) {
          var parsed = ptrfn.match(/%pointer_functions\(([A-Za-z0-9_]+)[\s]*,[\s]*([A-Za-z0-9_]+)\)/);
          if (parsed) {
            var from = parsed[1];
            var to = parsed[2];
            if (!_.has(xml2js.POINTER_TYPEMAPS, className)) {
              xml2js.POINTER_TYPEMAPS[className] = {};
            }
            xml2js.POINTER_TYPEMAPS[className][from] = to;
          }
        });
      });
    }));
  });
}


// generate class specs for custom pointer types
function generateCustomPointerClasses() {
  var arrayTypes = _.pluck(_.values(xml2js.ARRAY_TYPEMAPS), 'arrayType');
  var pointerTypes = _.uniq(_.flatten(_.map(_.values(xml2js.POINTER_TYPEMAPS), _.values)));
  _.each(arrayTypes, function(arrayType) {
    var dataType = _.findKey(xml2js.ARRAY_TYPEMAPS, function(to) { return to.arrayType == arrayType; });
    xml2js.CLASSES[arrayType] = {
      description: 'Array of type ' + dataType + '.',
      enums: {},
      enums_by_group: {},
      variables: {},
      methods: {}
    };
    xml2js.CLASSES[arrayType].methods[arrayType] = {
      description: 'Instantiates the array.',
      params: {
        nelements: {
          type: 'Number',
          description: 'number of elements in the array'
        }
      },
      return: {}
    };
    xml2js.CLASSES[arrayType].methods.getitem = {
      description: 'Access a particular element in the array.',
      params: {
        index: {
          type: 'Number',
          description: 'index of array to read from'
        },
      },
      return: {
        type: getType(dataType),
        description: 'the value of the element found at the given index of the array'
      }
    };
    xml2js.CLASSES[arrayType].methods.setitem = {
      description: 'Modify a particular element in the array.',
      params: {
        index: {
          type: 'Number',
          description: 'index of array to write to'
        },
        value: {
          type: getType(dataType),
          description: 'the value to set the element found at the given index of the array'
        }
      },
      return: {}
    };
  });
  var pointerDataTypeMap = _.reduce(_.map(_.values(xml2js.POINTER_TYPEMAPS), _.invert), function(memo, typemap) {
    return _.extend(memo, typemap);
  }, {});
  _.each(pointerTypes, function(pointerType) {
    var dataType = pointerDataTypeMap[pointerType];
    xml2js.CLASSES[pointerType] = {
      description: 'Proxy object to data of type ' + dataType + '.',
      enums: {},
      enums_by_group: {},
      variables: {},
      methods: {}
    };
    xml2js.CLASSES[pointerType].methods[pointerType] = {
      description: 'Instantiates the proxy object.',
      params: {},
      return: {}
    };
    xml2js.CLASSES[pointerType].methods.value = {
      description: 'Get the value of the object.',
      params: {},
      return: {
        type: getType(dataType),
        description: 'the value of the object'
      }
    };
    xml2js.CLASSES[pointerType].methods.assign = {
      description: 'Set the value of the object.',
      params: {
        value: {
          type: getType(dataType),
          description: 'the value to set the object to'
        }
      },
      return: {}
    };
  });
}


// search for usage of a type
function findUsage(type, classOnly) {
  var filterClasses = function(fn) { return _.without(_.map(xml2js.CLASSES, fn), undefined); };
  var usesType = function(classSpec, className) {
    var methodsOfType = (_.find(classSpec.methods, function(methodSpec, methodName) {
      return ((!_.isEmpty(methodSpec.return) && methodSpec.return.type == type) || 
              (_.contains(_.pluck(methodSpec.params, 'type'), type)));
    }) != undefined);
    var variablesOfType = _.contains(_.pluck(classSpec.variable, 'type'), type);
    return ((methodsOfType || variablesOfType) ? className : undefined);
  };
  var extendsType = function(classSpec, className) {
    return ((classSpec.parent == type) ? className : undefined);
  };
  var classes = _.union(filterClasses(usesType), filterClasses(extendsType));
  if (classOnly) {
    return classes;
  } else {
    return _.without(_.uniq(_.pluck(_.pick(xml2js.CLASSES, classes), 'group')), undefined);
  }
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
    return hasValidTypes(methodSpec, methodName);
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


// get the name for the module/group/class
function getName(spec_c) {
  return getText(getChild(spec_c, 'compoundname'), 'name').replace(xml2js.opts.module + '::', '');
}


// get the description for the module/group/class
function getDescription(spec_c) {
  return getText(getChild(spec_c, 'detaileddescription'), 'description');
}


// get the classes (xml file names) for the given module
function getSubclasses(spec_c) {
  return _.map(getChildren(spec_c, 'innerclass'), function(innerclass) {
    return getAttr(innerclass, 'refid');
  });
}


// get the classes (class names) for the given module
function getSubclassNames(spec_c) {
  return _.map(getChildren(spec_c, 'innerclass'), function(innerclass) {
    return getText(innerclass).replace(xml2js.opts.module + '::', '');
  });
}


// get the submodules (xml file names) for the given module
function getSubmodules(spec_c) {
  return _.map(getChildren(spec_c, 'innergroup'), function(innergroup) {
    return getAttr(innergroup, 'refid');
  });
}


// get parent class, if any
function getParent(spec_c, className) {
  var parent = getChild(spec_c, 'basecompoundref');
  if (parent) {
    parent = getText(parent);
    if (!_.has(xml2js.CLASSES, parent)) {
      console.log('WARNING: Class ' + className + ' has unknown parent class ' + parent);
    }
  }
  return parent;
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
              params = getParams(paramsSpec, getParamsDetails(description), methodName, parent);
          }
          var returnSpec = getChild(method, 'type');
          var retval = {};
          if (!_.isEmpty(returnSpec)) {
            retval = getReturn(returnSpec, getReturnDetails(description), methodName, parent);
          }
          methodName = getUniqueMethodName(methodName, spec_js, parent);
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


// get a unique string to represent the name of an overloaded method
function getUniqueMethodName(methodName, module, parent) {
  if (methodName in module) {
    do {
      methodName += '!';
    } while (methodName in module);
  }
  return methodName;
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
      var varType = getType(getText(getChild(variable, 'type')), parent);
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
function getReturn(spec_c, details, method, parent) {
  var retType = getType(getText(spec_c, 'type'), parent);
  var retDescription = (details ? getText(details, 'description') : '');
  return ((retType == 'void') ? {} : {
    type: retType,
    description: retDescription
  });
}


// get paramater specs of a method
function getParams(spec_c, details, method, parent) {
  var spec_js = {};
  _.each(spec_c, function(param) {
    try {
      var paramType = getType(getText(getChild(param, 'type'), 'type'), parent);
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
      console.log('Warning: ' + (parent ? (parent + '.') : '') + method + msg + msgAddendum);
    }
  });
  return spec_js;
}


// get the equivalent javascript type from the given c type
function getType(type_c, parent) {
  var type_js = type_c;
  _.find(xml2js.TYPEMAPS, function(to, from) {
    var pattern = new RegExp(from, 'i');
    if (type_c.search(pattern) == 0) {
      type_js = to;
      return true;
    }
  });
  if (isPointer(type_js)) {
    var dataType = getPointerDataType(type_js);
    var className = parent.toLowerCase();
    if (_.has(xml2js.ARRAY_TYPEMAPS, dataType) && _.contains(xml2js.ARRAY_TYPEMAPS[dataType].classes, className)) {
      type_js = xml2js.ARRAY_TYPEMAPS[dataType].arrayType;
    } else if (_.has(xml2js.POINTER_TYPEMAPS, className) && _.has(xml2js.POINTER_TYPEMAPS[className], dataType)) {
      type_js = xml2js.POINTER_TYPEMAPS[className][dataType];
    } else if (_.has(xml2js.CLASSES, dataType)) { // TODO: verify that swig does this mapping
      type_js = dataType;
    } else {
      type_js = dataType + ' &#42;'
    }
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
      console.log('  Error: parameter ' + paramName + ' has invalid type ' + typeToString(paramSpec.type));
    }
  });
  if (!_.isEmpty(methodSpec.return) && !isValidType(methodSpec.return.type, parent)) {
    valid = false;
    printIgnoredMethodOnce();
    console.log('  Error: returns invalid type ' + typeToString(methodSpec.return.type));
  }
  return valid;
}


// verify that type of variable is valid
function ofValidType(varSpec, varName, parent) {
  if (isValidType(varSpec.type, parent)) {
    return true;
  } else {
    var msgAddendum = (xml2js.opts.strict ? ' Omitted from JS documentation.' : '');
    console.log('Error: ' + varName + ' is of invalid type ' + typeToString(varSpec.type) + '.' + msgAddendum);
    return false;
  }
}


// verify whether the given type is valid JS
function isValidType(type, parent) {
  return (_.contains(_.values(xml2js.TYPEMAPS), type) ||
          _.has(xml2js.CLASSES, type) ||
          _.has(xml2js.ENUMS_BY_GROUP, type) ||
          _.contains(['Buffer', 'Function', 'mraa_result_t'], type) ||
          _.has((parent ? xml2js.CLASSES[parent].enums_by_group : []), type) ||
          isValidPointerType(type, parent));
}


function isValidPointerType(type, parent) {
  var className = parent.toLowerCase();
  var arrayTypemap = _.find(xml2js.ARRAY_TYPEMAPS, function(to) { return to.arrayType == type; });
  var valid = ((arrayTypemap && _.contains(arrayTypemap.classes, className)) ||
          (_.has(xml2js.POINTER_TYPEMAPS, className) && (_.contains(_.values(xml2js.POINTER_TYPEMAPS[className]), type))));
  return valid;
}


// determines whether a type looks like a c pointer
function isPointer(type) {
  return (type.search(/\w+\s*(\*|&amp;)$/) != -1);
}


// remove extraneous whitespace from pointer types as canonical representation
function normalizePointer(ptr) {
  return ptr.replace(/\s*$/, '');
}


// get the data type of a pointer (e.g. int is the data type of int*)
function getPointerDataType(ptr) {
  return ptr.replace(/\s*(\*|&amp;)$/, '');
}


// print more human friendly type for error messages
function typeToString(type) {
  return type.replace('&#42;', '*');
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
        // TODO: html doesn't seem to work for yuidoc, using markdown for now
        case 'itemizedlist':
          return text += '\n' + getText(elem, why) + '  \n  \n';
        case 'listitem':
          return text += '+ ' + getText(elem, why) + '\n';
        case 'bold':
          return text += '__' + getText(elem, why).trim() + '__ ';
        case 'ulink':
          return text += '[' + getText(elem, why).trim() + '](' + getAttr(elem, 'url').trim() + ') ';
        case 'image':
          // TODO: copy images over; hard coded for now
          var fn = getAttr(elem, 'name');
          return text += '  \n  \n![' + fn + '](' + xml2js.opts.imagedir + '/' + fn + ') ';
        case 'linebreak':
          return text += '  \n';
        case 'ndash':
          return text += '&ndash; ';
        default:
          // TODO: incomplete list of doxygen xsd implemented
          console.warn('NYI Unknown Object Type: ' + elem.name);
          return text;
          //throw new Error('NYI Unknown Object Type: ' + elem.name);
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


// debug helper: print untruncated object
function printObj(obj) {
  console.log(util.inspect(obj, false, null));
}


module.exports = xml2js;
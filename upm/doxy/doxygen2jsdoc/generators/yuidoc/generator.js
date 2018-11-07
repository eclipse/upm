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
var _ = require('lodash');


// generate YuiDocs-style documentation
function generateDocs(specjs) {
  var docs = GENERATE_MODULE(specjs.MODULE, '');
  GENERATE_TYPE = (function(enums) { 
    return function(type) {
      return (_.contains(enums, type) ? ('Enum ' + type) : type);  
    }
  })(_.keys(specjs.ENUMS_BY_GROUP));
  docs = _.reduce(specjs.METHODS, function(memo, methodSpec, methodName) {
    return memo += GENERATE_METHOD(methodName, methodSpec);
  }, docs);
  docs = _.reduce(specjs.ENUMS, function(memo, enumSpec, enumName) {
    return memo += GENERATE_ENUM(enumName, enumSpec);
  }, docs);
  if (_.isEmpty(specjs.CLASSGROUPS)) {
      docs += GENERATE_CLASSES(specjs.CLASSES);
  } else {
    docs += GENERATE_MODULE('common', '');
    var grouped = _.flatten(_.pluck(_.values(specjs.CLASSGROUPS), 'classes'));
    var ungrouped = _.difference(_.keys(specjs.CLASSES), grouped);
    docs += GENERATE_CLASSES(_.pick(specjs.CLASSES, ungrouped), 'common'); 
    _.each(specjs.CLASSGROUPS, function(groupSpec, groupName) {
      docs += GENERATE_CLASSES(_.pick(specjs.CLASSES, groupSpec.classes), groupName);
    });
    // TODO: figure out why yuidoc won't associate the class with the right module if module definitions are interspersed
    _.each(specjs.CLASSGROUPS, function(groupSpec, groupName) {
        docs += GENERATE_MODULE(groupName, groupSpec.description);
    });
  }
  return docs;
}


// comment wrapper around entire spec
function GENERATE_DOC(text) {
  return '/**\n' + text + ' */\n';
}


// generate module spec
function GENERATE_MODULE(name, description) {
  return GENERATE_DOC(description + '\n' 
    + '@module ' + name + '\n');
}


// generate spec for the given list of classes
function GENERATE_CLASSES(classes, parent) {
  return _.reduce(classes, function(memo, classSpec, className) {
    return memo
      + GENERATE_CLASS(className, classSpec.description, parent, classSpec.parent)
      + _.reduce(classSpec.methods, function(memo, methodSpec, methodName) {
        return memo += GENERATE_METHOD(methodName, methodSpec, className);
      }, '') 
      + _.reduce(classSpec.variables, function(memo, variableSpec, variableName) {
        return memo += GENERATE_VAR(variableName, variableSpec, className);
      }, '')
      + _.reduce(classSpec.enums, function(memo, enumSpec, enumName) {
        return memo += GENERATE_ENUM(enumName, enumSpec, className);
      }, '');
  }, '');
}


// generate class spec
function GENERATE_CLASS(name, description, namespace, parent) {
  return GENERATE_DOC(description + '\n' 
    + '@class ' + name + '\n'
    + (namespace ? ('@module ' + namespace + '\n') : '')
    /*
    TODO: leave out until figure out what swig does with inheritance
    + (parent ? ('@extends ' + parent + '\n') : '')
    */
  );
}


// generate method spec with parent module/class
function GENERATE_METHOD(name, spec, parent) {
  name = name.replace(/!+$/, '');
  return GENERATE_DOC(spec.description + '\n'
    + '@method ' + name + '\n'
    + (parent ? ('@for ' + parent + '\n') : '@for common\n')
    + _.reduce(spec.params, function(memo, paramSpec, paramName) {
        return memo + '@param {' + GENERATE_TYPE(paramSpec.type) + '} ' + paramName + ' ' + paramSpec.description + '\n'; 
      }, '')
    + ( !_.isEmpty(spec.return) ? ('@return {' + GENERATE_TYPE(spec.return.type) + '} ' + spec.return.description + '\n') : ''));
}


// generate enum spec
function GENERATE_ENUM(name, spec, parent) {
  return GENERATE_DOC(spec.description + '\n'
    + '@property ' + name + '\n'
    + '@type Enum ' + spec.type + '\n'
    + '@for ' + (parent ? parent : 'common') + '\n');
}


// generate variable specs
function GENERATE_VAR(name, spec, parent) {
  return GENERATE_DOC(spec.description + '\n'
    + '@property ' + name + '\n'
    + '@type ' + spec.type + '\n'
    + '@for ' + parent + '\n');
}
  

// TODO
// generate link spec 
function GENERATE_LINK(text) {
  return '{{#crossLink "' + text + '"}}{{/crossLink}}';
}


module.exports = generateDocs;
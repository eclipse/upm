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


// generate json for ternjs input
function generateDocs(specjs) {
  var docs = GENERATE_MODULE(specjs.MODULE);
  GENERATE_TYPE = (function(enums) {
    return function(type) {
      return (_.contains(enums, type) ? ('Enum ' + type) : type);
    }
  })(_.keys(specjs.ENUMS_BY_GROUP));
  _.each(specjs.ENUMS, function(enumSpec, enumName) {
    _.extend(docs[specjs.MODULE], GENERATE_ENUM(enumName, enumSpec));
  });
  _.each(specjs.METHODS, function(methodSpec, methodName) {
    _.extend(docs[specjs.MODULE], GENERATE_METHOD(methodName, methodSpec));
  });
  _.each(specjs.CLASSES, function(classSpec, parentClass) {
    var constructor =  classSpec.methods[parentClass];
    _.extend(docs[specjs.MODULE], GENERATE_METHOD(parentClass, constructor ? constructor : { params: {}, return: {}, description: '' } ));
    _.each(classSpec.enums, function(enumSpec, enumName) {
      _.extend(docs[specjs.MODULE][parentClass], GENERATE_ENUM(enumName, enumSpec));
    });
    docs[specjs.MODULE][parentClass].prototype = {};
    _.each(_.omit(classSpec.methods, parentClass), function(methodSpec, methodName) {
      _.extend(docs[specjs.MODULE][parentClass].prototype, GENERATE_METHOD(methodName, methodSpec));
    });
    _.each(classSpec.variables, function(variableSpec, variableName) {
      _.extend(docs[specjs.MODULE][parentClass].prototype, GENERATE_VARIABLE(variableName, variableSpec));
    });
  });
  return JSON.stringify(docs, null, 2);
}


// generate module spec
function GENERATE_MODULE(module) {
  var docs = { '!name': module + 'library' };
  docs[module] = {};
  return docs;
}


// generate method spec
function GENERATE_METHOD(name, spec) {
  var doc = {};
  doc[name] = {
    '!type': 'fn(' + GENERATE_PARAMS(spec.params) + ')' + GENERATE_RETURN(spec.return),
    '!doc': spec.description
  }
  return doc;
}


// generate parameter signatures for method
function GENERATE_PARAMS(spec) {
  return _.map(spec, function(paramSpec, paramName) {
    return paramName + ': ' + paramSpec.type;
  }).join(', ');
}


// generate return signature for method
function GENERATE_RETURN(spec) {
  return (_.isEmpty(spec) ? '' : (' -> ' + spec.type));
}


// generate enum spec
function GENERATE_ENUM(name, spec) {
  var doc = {};
  doc[name] = 'Enum ' + spec.type ;
  return doc;
}


// generate variable spec
function GENERATE_VARIABLE(name, spec) {
  var doc = {};
  doc[name]= spec.type ;
  return doc;
}


module.exports = generateDocs;

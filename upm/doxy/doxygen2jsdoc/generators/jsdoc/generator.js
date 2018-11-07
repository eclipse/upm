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


// generate JSDoc-style documentation
function generateDocs(specjs) {
  var docs = GENERATE_MODULE(specjs.MODULE);
  docs = _.reduce(specjs.METHODS, function(memo, methodSpec, methodName) {
    return memo += GENERATE_METHOD(methodName, methodSpec);
  }, docs);
  docs = _.reduce(specjs.ENUMS, function(memo, enumSpec, enumName) {
    return memo += GENERATE_ENUM(enumName, enumSpec);
  }, docs);
  docs = _.reduce(specjs.CLASSES, function(memo, classSpec, parentClass) {
    return _.reduce(classSpec.methods, function(memo, methodSpec, methodName) {
      return memo += GENERATE_METHOD(methodName, methodSpec, parentClass);
    }, memo);
  }, docs);
  return docs;
}


// comment wrapper around entire spec
function GENERATE_DOC(text) {
  return '/**\n' + text + ' */\n';
}


// generate module spec
function GENERATE_MODULE(module) {
  return GENERATE_DOC('@module ' + module + '\n');
}


// generate method spec with parent module/class
function GENERATE_METHOD(name, spec, parent) {
  name = name.replace(/!+$/, '');
  return GENERATE_DOC(spec.description + '\n'
    + '@method ' + name + '\n'
    + '@instance\n'
    + (parent ? ('@memberof ' + parent + '\n') : '')
    + _.reduce(spec.params, function(memo, paramSpec, paramName) {
        return '@param {' + paramSpec.type + '} ' + paramName + ' ' + paramSpec.description + '\n';
      }, '')
    + ( !_.isEmpty(spec.return) ? ('@return {' + spec.return.type + '} ' + spec.return.description + '\n') : ''));
}


// generate enum spec
function GENERATE_ENUM(name, spec) {
  return GENERATE_DOC(spec.description + '\n\n'
    + '@var ' + name + '\n'
    + '@type Enum(' + spec.type + ')\n'
    + '@instance\n');
}


// TODO
// generate link spec
function GENERATE_LINK(text) {
    return '{@link ' + text + '}';
}


module.exports = generateDocs;

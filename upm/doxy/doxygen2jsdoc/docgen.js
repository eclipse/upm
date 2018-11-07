#!/usr/bin/env node

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
var xml2js  = require('./xml2js')
  , fs      = require('fs')
  , Promise = require('bluebird')
  , opts    = require('commander')
  , _       = require('lodash')
  , mkdirp  = require('mkdirp');


// parse command line arguments
_.extend(opts, { addOptions: function(module) { return module.addOptions(opts); } });
opts
  .option('-m, --module [module]', 'module name for which to build documentation', 'mraa')
  .option('-f, --formats [formats]', 'format for js comments', 'yuidoc,ternjs,jsdoc')
  .option('-o, --outdir [directory]', 'top directory to build documentation', __dirname + '/jsdoc')
  .addOptions(xml2js)
  .parse(process.argv);


// use promise-style programming rather than spaghetti callbacks
Promise.promisifyAll(fs);
Promise.promisifyAll(mkdirp);

// Global Error Handlers
process.on('uncaughtException', function (error) {
  console.error('Uncaught Exception:', error);
  process.exit(1); // eslint-disable-line no-process-exit
});

process.on('unhandledRejection', function (reason, p) {
  console.error('Unhandled Rejection at:', p, 'reason:', reason);
  process.exit(1); // eslint-disable-line no-process-exit
});

// main
var FORMATS = opts.formats.split(',');

xml2js.parse()
  .then(function(specjs) {
    return Promise.all(_.map(FORMATS, function(format) {
      var generateDocs = require(__dirname + '/generators/' + format + '/generator');
      var dir = opts.outdir + '/' + format + '/' + specjs.MODULE;
      return mkdirp.mkdirpAsync(dir).then(function() {
        return fs.writeFileAsync(dir + '/doc.js', generateDocs(specjs));
      });
    }));
  })
  .catch(function(error) {
    throw error;
  });

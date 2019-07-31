/********************************************************************************
 * Copyright (c) 2017-2019 Intel Corporation
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 ********************************************************************************/

var shell = require('shelljs');
var path = require('path');

var rootPath = path.resolve(__dirname, '../../');
var srcPath = path.resolve(rootPath, 'src');

var jsonlintCmd = path.resolve(__dirname, 'node_modules/.bin/jsonlint');
var jsonlintOpts = ' --quiet ';

var failures = [];

function getRelativePath(filePath) {
  return path.relative(rootPath, filePath);
}

function printSummaryAndExit() {
  var exitCode = 0;

  if (failures.length > 0) {
    console.error();
    console.error('Failures:');
    failures.forEach(function (file) {
      console.error('    ', getRelativePath(file));
    });
    exitCode = 1;
  }
  else {
    console.log();
    console.log('Success');
  }

  process.exit(exitCode);
}

var pending = 0;

shell.find(srcPath)
  .filter(function (file) {
    return file.match(/\.json$/);
  })
  .forEach(function (jsonFile) {
    pending++;
    var relativePath = getRelativePath(jsonFile);
    shell.exec(jsonlintCmd + jsonlintOpts + jsonFile, {silent: true}, function (code, stdout, stderr) {
      if (code) {
        console.error('Failed', relativePath);
        console.error(stderr);
        failures.push(jsonFile);
      }
      else {
        console.log('Success', relativePath);
      }

      pending--;

      if (pending == 0) {
        printSummaryAndExit();
      }
    });
  });


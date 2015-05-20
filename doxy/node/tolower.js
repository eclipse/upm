/*
 * Author: Dina M Suehiro <dina.m.suehiro@intel.com>
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
var opts    = require('commander'),     // for command line args
    fs      = require('fs'),                // for file system access
    path      = require('path');              // for file path parsing

// parse command line arguments
opts
  .option('-i, --inputdir [directory]', 'product documents directory', __dirname + '/docs/yuidoc/upm')
  .parse(process.argv);

// Set to true for console output
var debug = true;

// Global arrays tracking the files that have been renamed
var originalFiles = [];
var renamedFiles = [];

// Filter to get html files from different directories
var rootFiles = getHtmlFilenames(opts.inputdir);
var classesFiles = getHtmlFilenames(opts.inputdir + "/classes");
var modulesFiles = getHtmlFilenames(opts.inputdir + "/modules");

// Rename files in the classes directory to have lower-cased file names.
renameFiles(classesFiles);

classesFiles = getHtmlFilenames(opts.inputdir + "/classes");

// Go through the html files and update links to reflect the file names that we changed.
renameLinks(rootFiles);
renameLinks(classesFiles);
renameLinks(modulesFiles);

// Helper function that returns paths to the html files in the specified directory
function getHtmlFilenames (directory)
{
  return fs.readdirSync(directory).map(function (file) {
    return path.join(directory, file);
  }).filter(function (file) {
    return fs.statSync(file).isFile();
  }).filter(function (file) {
    return path.extname(file).toLowerCase() == ".html";
  });
}

// Goes through the files and renames them to be lower-cased and tracks them the
// renamed files in the originalFiles[] and renamedFiles[] arrays.
function renameFiles(files)
{
  files.forEach(function (file)
  {
    var originalName = path.basename(file);
    var newFileName = originalName.toLowerCase();
    var directory = path.dirname(file);
    if (originalName != newFileName)
    {
      fs.renameSync(file, directory + "/" + newFileName); //, function(err)

      if (debug)
        console.log('Renamed: %s --> %s', originalName, newFileName);

      originalFiles.push(originalName);
      renamedFiles.push(newFileName);
    }
  });
}

// Helper function goes through the specified files and does a file/replace of the
// originalFiles to the renamedFiles so that the .html links match what has been renamed.
function renameLinks (files)
{
  if (originalFiles.length <= 0)
  {
    if (debug)
      console.log("No links to rename.");
    return;
  }

  files.forEach(function (file)
  {
    // Read the file
    data = fs.readFileSync(file, 'ascii');

    // Find/replace the file names that were renamed
    for (var i = 0; i < originalFiles.length; i++)
    {
      var findString = '/' + originalFiles[i] + '\"';
      var replaceString = '/' + renamedFiles[i] + '\"';

      data = data.replace(findString, replaceString);
    }

    // Write back
    fs.writeFile(file, data, 'ascii', function (err) {
      if (err)
        throw err;
    });

    if (debug)
      console.log('Renamed links in: %s', file);
  });
}

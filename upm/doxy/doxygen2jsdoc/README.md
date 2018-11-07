# doxygen2jsdoc

[![Build Status](https://travis-ci.org/intel-iot-devkit/doxygen2jsdoc.svg?branch=master)](https://travis-ci.org/intel-iot-devkit/doxygen2jsdoc)

C to JavaScript auto documentation generation tool to support C codes that are
auto-translated to JavaScript using [SWIG](http://swig.org/).

Parses the XML format of the original Doxygen C comments, along with some
optional supporting metadata, to an intermediate JavaScript JSON format
(documented at the top of xml2js.js), that is then used to generate various
JavaScript metadata formats to feed into the corresponding JavaScript
documentation generation tools. Currently generated JavaScript formats are
dummy header files for [YuiDoc](http://yui.github.io/yuidoc/), and JSON for
[TernJS](http://ternjs.net/) (and dummy header files for
[JSDoc](http://usejsdoc.org/), but this is not well supported).

To use: `node docgen <options>`

To get a list of available options: `node docgen -h`

Currently used by [MRAA](https://github.com/intel-iot-devkit/mraa) and
[UPM](https://github.com/intel-iot-devkit/upm).

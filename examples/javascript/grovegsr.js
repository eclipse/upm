/*jslint node:true, vars:true, bitwise:true, unparam:true */
/*jshint unused:true */
/*
* Author: Zion Orent <zorent@ics.com>
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

var grovegsr_lib = require("jsupm_grovegsr");

// The was tested with the GroveGSR Galvanic Skin Response Sensor module.

// Instantiate a GroveGSR on analog pin A0
var grovegsr_obj = new grovegsr_lib.GroveGSR(0);
console.log("Calibrating....");
grovegsr_obj.calibrate();

var myInterval = setInterval(function()
{
	console.log(grovegsr_obj.value());
}, 500);


// When exiting: clear interval, and print message
process.on('SIGINT', function()
{
	clearInterval(myInterval);
	grovegsr_obj = null;
	grovegsr_lib.cleanUp();
	grovegsr_lib = null;
	console.log("Exiting...");
	process.exit(0);
});

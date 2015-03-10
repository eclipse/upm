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

// Load Grove Luminance sensor module
var luminanceSensor_lib = require('jsupm_apds9002');

// Instantiate a Grove Luminance sensor on analog pin A0
var myLuminanceSensor_obj = new luminanceSensor_lib.APDS9002(0);

var myInterval = setInterval(function()
{
	console.log("Luminance value is " + myLuminanceSensor_obj.value());
}, 1000);

// On exit: clear interval and memory, print message
process.on('SIGINT', function()
{
	clearInterval(myInterval);
	myLuminanceSensor_obj = null;
	luminanceSensor_lib.cleanUp();
	luminanceSensor_lib = null;
	console.log("Exiting");
	process.exit(0);
});

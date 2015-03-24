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
var flexSensor_lib = require('jsupm_flex');

// The was tested with a Spectra Symbol flex sensor.
// We attached a 22K resistor to a breadboard,
// with 1 end attached to GND and the other connected to
// both the flex sensor and A0.
// The flex sensor was connected on 1 pin to the 22K resistor and A0
// and on the other pin to 5V.

// Instantiate a UV sensor on analog pin A0
var myFlexSensor_obj = new flexSensor_lib.Flex(0);

var myInterval = setInterval(function()
{
	var outputStr = "Flex value: " + myFlexSensor_obj.value();
	console.log(outputStr);
}, 1000);

// Print message when exiting
process.on('SIGINT', function()
{
	clearInterval(myInterval);
	myFlexSensor_obj = null;
	flexSensor_lib.cleanUp();
	flexSensor_lib = null;
	console.log("Exiting");
	process.exit(0);
});

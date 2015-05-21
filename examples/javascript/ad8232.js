/*jslint node:true, vars:true, bitwise:true, unparam:true */
/*jshint unused:true */
/*
* Author: Jon Trulson <jtrulson@ics.com>
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
var ad8232Sensor_lib = require('jsupm_ad8232');

// Instantiate a AD8232 sensor on digital pins 10 (LO+), 11 (LO-)
// and an analog pin, 0 (OUTPUT)
var myAd8232Sensor_obj = new ad8232Sensor_lib.AD8232(10, 11, 0);

// Output the raw numbers from the ADC, for plotting elsewhere.
// A return of 0 indicates a Lead Off (LO) condition.
// In theory, this data could be fed to software like Processing 
// (https://www.processing.org/) to plot the data just like an 
// EKG you would see in a hospital.
var myInterval = setInterval(function()
{
	console.log(myAd8232Sensor_obj.value());
}, 1);

// Print message when exiting
process.on('SIGINT', function()
{
	clearInterval(myInterval);
	myAd8232Sensor_obj = null;
	ad8232Sensor_lib.cleanUp();
	ad8232Sensor_lib = null;
	console.log("Exiting");
	process.exit(0);
});

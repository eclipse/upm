/*jslint node:true, vars:true, bitwise:true, unparam:true */
/*jshint unused:true */
/*global */
/*
* Author: Zion Orent <zorent@ics.com>
* Copyright (c) 2014 Intel Corporation.
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

//Load Grove Moisture module
var grove_moisture = require('jsupm_grovemoisture');

// Instantiate a Grove Moisture sensor on analog pin A0
var myMoistureObj = new grove_moisture.GroveMoisture(0);

// Values (approximate):
// 0-300,   sensor in air or dry soil
// 300-600, sensor in humid soil
// 600+,    sensor in wet soil or submerged in water
// Read the value every second and print the corresponding moisture level
setInterval(function()
{
	var result;
	var moisture_val = parseInt(myMoistureObj.value());
	if (moisture_val >= 0 && moisture_val < 300)
		result = "Dry";
	else if (moisture_val >= 300 && moisture_val < 600)
		result = "Moist";
	else
		result = "Wet";
	console.log("Moisture value: " + moisture_val + ", " + result);
}, 1000);

// Print message when exiting
process.on('SIGINT', function()
{
	console.log("Exiting...");
	process.exit(0);
});

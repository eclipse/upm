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

//Load Barometer module
var bmpx8x = require('jsupm_bmpx8x');
// load this on i2c
var myBarometerObj = new bmpx8x.BMPX8X(0, bmpx8x.ADDR);
var pressure, temperature, altitude, sealevel;

// Print the pressure, altitude, sea level, and
// temperature values every 0.1 seconds
setInterval(function()
{
	var pressure = myBarometerObj.getPressure();
	var temperature = myBarometerObj.getTemperature();
	var altitude = myBarometerObj.getAltitude();
	var sealevel = myBarometerObj.getSealevelPressure();

	var BMPX8Xresults = "pressure value = " + pressure;
	BMPX8Xresults += ", altitude value = " + altitude;
	BMPX8Xresults += ", sealevel value = " + sealevel;
	BMPX8Xresults += ", temperature = " + temperature;
	console.log(BMPX8Xresults);
}, 100);

// Print message when exiting
process.on('SIGINT', function()
{
	console.log("Exiting...");
	process.exit(0);
});

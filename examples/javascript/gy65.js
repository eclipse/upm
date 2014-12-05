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
var gy65 = require('jsupm_gy65');
// load this on i2c
var myBarometerObj = new gy65.GY65(0, gy65.ADDR);
var pressure, temperature, altitude, sealevel;

// Print the pressure, altitude, sea level, and
// temperature values every 0.1 seconds
setInterval(function()
{
	var pressure = myBarometerObj.getPressure();
	var temperature = myBarometerObj.getTemperature();
	var altitude = myBarometerObj.getAltitude();
	var sealevel = myBarometerObj.getSealevelPressure();

	var GY65results = "pressure value = " + pressure;
	GY65results += ", altitude value = " + altitude;
	GY65results += ", sealevel value = " + sealevel;
	GY65results += ", temperature = " + temperature;
	console.log(GY65results);
}, 100);

// Print message when exiting
process.on('SIGINT', function()
{
	console.log("Exiting...");
	process.exit(0);
});

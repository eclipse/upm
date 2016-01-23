/*jslint node:true, vars:true, bitwise:true, unparam:true */
/*jshint unused:true */

/*
* Author: Henry Bruce <henry.bruce@intel.com>
* Copyright (c) 2016 Intel Corporation.
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

var bme280pkg = require('jsupm_bme280');

// Instantiate a BME280 Sensor on I2C sub-platform bus 0
var bme280 = new bme280pkg.BME280(512);

// update every second and print the values
var myInterval = setInterval(function()
{
	// print detected value
	console.log("Temperature: " + bme280.getTemperatureCelcius() + "C");
	console.log("Humidity:    " + bme280.getHumidityRelative() + "%");
	console.log("Pressure:    " + bme280.getPressurePa() + "Pa");
}, 1000);


// When exiting: clear interval and print message
process.on('SIGINT', function()
{
	clearInterval(myInterval);
	bme280 = null
	bme280pkg = null;
	console.log("Exiting");
	process.exit(0);
});

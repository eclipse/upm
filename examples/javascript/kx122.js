/*
* Author: Noel Eck <noel.eck@intel.com>
* Copyright (c) 2017 Intel Corporation.
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

var kx122 = require("jsupm_kx122");

// Instantiate a KX122
var kx122_sensor = new kx122.KX122(0, -1, 24);

// Reset the device
kx122_sensor.softwareReset();

// Reset the device
kx122_sensor.deviceInit(kx122.KX122_ODR_50,
		kx122.HIGH_RES,
		kx122.KX122_RANGE_2G);

var myInterval = setInterval(function()
{
	data = kx122_sensor.getAccelerationDataVector();
	console.log("Acceleration x: " + data.get(0)
		+ " y: " + data.get(1)
		+ " z: " + data.get(2));
}, 1000);

// When exiting: clear interval and print message
process.on('SIGINT', function()
{
	clearInterval(myInterval);
	console.log("Exiting...");
	process.exit(0);
});

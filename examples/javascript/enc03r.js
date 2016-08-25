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

// Load analog gyro module
var analogGyro = require('jsupm_enc03r');
// Instantiate a ENC03R on analog pin A0
var myGyro = new analogGyro.ENC03R(0);

var CALIBRATION_SAMPLES = 1000;

console.log("Please place the sensor in a stable location,\n" +
			"and do not move it while calibration takes place.\n" +
			"This may take a couple of minutes.");

myGyro.calibrate(CALIBRATION_SAMPLES);
console.log("Calibration complete.  Reference value: " +
            myGyro.calibrationValue());

// Read the input and print both the raw value and the angular velocity,
// waiting 0.1 seconds between readings
setInterval(function()
{
	var gyroVal = myGyro.value();
	var outputStr = "Raw value: " + gyroVal + ", "  +
					"angular velocity: " +
					roundNum(myGyro.angularVelocity(gyroVal), 5) + " deg/s";
	console.log(outputStr);
}, 100);

function roundNum(num, decimalPlaces)
{
	var extraNum = (1 / (Math.pow(10, decimalPlaces) * 1000));
	return (Math.round((num + extraNum) * (Math.pow(10, decimalPlaces))) / 
            Math.pow(10, decimalPlaces));
}

// Print message when exiting
process.on('SIGINT', function()
{
	console.log("Exiting...");
	process.exit(0);
});

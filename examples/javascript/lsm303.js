/*jslint node:true, vars:true, bitwise:true, unparam:true */
/*jshint unused:true */
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

var accelrCompassSensor = require('jsupm_lsm303');

// Instantiate LSM303 compass on I2C
var myAccelrCompass = new accelrCompassSensor.LSM303(0);

var successFail, coords, outputStr, accel;
var myInterval = setInterval(function()
{
	// Load coordinates into LSM303 object
	successFail = myAccelrCompass.getCoordinates();
	// in XYZ order. The sensor returns XZY,
	// but the driver compensates and makes it XYZ
	coords = myAccelrCompass.getRawCoorData();

    // Print out the X, Y, and Z coordinate data using two different methods
	outputStr = "coor: rX " + coords.getitem(0)
					+ " - rY " + coords.getitem(1)
					+ " - rZ " + coords.getitem(2);
	console.log(outputStr);
	outputStr = "coor: gX " + myAccelrCompass.getCoorX()
				+ " - gY " + myAccelrCompass.getCoorY()
				+ " - gZ " + myAccelrCompass.getCoorZ();
	console.log(outputStr);

    // Get and print out the heading
	console.log("heading: " + myAccelrCompass.getHeading());

    // Get the acceleration
	myAccelrCompass.getAcceleration();
	accel = myAccelrCompass.getRawAccelData();
    // Print out the X, Y, and Z acceleration data using two different methods
	outputStr = "acc: rX " + accel.getitem(0)
				+ " - rY " + accel.getitem(1)
				+ " - Z " + accel.getitem(2);
	console.log(outputStr);
	outputStr = "acc: gX " + myAccelrCompass.getAccelX()
				+ " - gY " + myAccelrCompass.getAccelY()
				+ " - gZ " + myAccelrCompass.getAccelZ();
	console.log(outputStr);
	console.log(" ");
}, 1000);

// Print message when exiting
process.on('SIGINT', function()
{
	clearInterval(myInterval);
	myAccelrCompass = null;
	accelrCompassSensor.cleanUp();
	accelrCompassSensor = null;
	console.log("Exiting");
	process.exit(0);
});

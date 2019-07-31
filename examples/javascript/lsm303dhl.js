/*
* Author: Zion Orent <zorent@ics.com>
* Copyright (c) 2014 Intel Corporation.
*
* This program and the accompanying materials are made available under the
* terms of the The MIT License which is available at
* https://opensource.org/licenses/MIT.
*
* SPDX-License-Identifier: MIT
*/

var accelrCompassSensor = require('jsupm_lsm303dlh');

// Instantiate LSM303DLH compass on I2C
var myAccelrCompass = new accelrCompassSensor.LSM303DLH(0);

var successFail, coords, outputStr, accel;
var myInterval = setInterval(function()
{
	// Load coordinates into LSM303DLH object
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

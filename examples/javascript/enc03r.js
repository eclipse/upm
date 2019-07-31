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
    myGyro.update();
    var outputStr = "Angular velocity: " +
	roundNum(myGyro.angularVelocity(), 5) + " deg/s";
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

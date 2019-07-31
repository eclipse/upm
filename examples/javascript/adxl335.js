/*
* Author: Zion Orent <zorent@ics.com>
* Copyright (c) 2015 Intel Corporation.
*
* This program and the accompanying materials are made available under the
* terms of the The MIT License which is available at
* https://opensource.org/licenses/MIT.
*
* SPDX-License-Identifier: MIT
*/

var analogGyro3Axis = require("jsupm_adxl335");

var g_addnumBool = true;
var g_cycleNum = 0.0;
var g_cycleCount = 0;

// Instantiate an ADXL335 accelerometer on analog pins A0, A1, and A2
var myAnalogGyro3Axis = new analogGyro3Axis.ADXL335(0, 1, 2);

console.log("Please make sure the sensor is completely still.");
console.log("Sleeping for 2 seconds");

var g_myInterval;

setTimeout(function()
{
	console.log("Calibrating...");
	myAnalogGyro3Axis.calibrate();
	// Get values from accelerometer every 0.2 seconds
	g_myInterval = setInterval(runAccelerometer, 200);
}, 2000);


var x = new analogGyro3Axis.new_intPointer();
var y = new analogGyro3Axis.new_intPointer();
var z = new analogGyro3Axis.new_intPointer();

var aX = new analogGyro3Axis.new_floatPointer();
var aY = new analogGyro3Axis.new_floatPointer();
var aZ = new analogGyro3Axis.new_floatPointer();

var outputStr;

function runAccelerometer()
{
	myAnalogGyro3Axis.values(x, y, z);
	outputStr = "Raw Values: X: " +
		analogGyro3Axis.intPointer_value(x) +
		" Y: " + analogGyro3Axis.intPointer_value(y) + 
		" Z: " + analogGyro3Axis.intPointer_value(z);
	console.log(outputStr);

	myAnalogGyro3Axis.acceleration(aX, aY, aZ);
	outputStr = "Acceleration: X: " +
	analogGyro3Axis.floatPointer_value(aX) + "g\n" +
	"Acceleration: Y: " +
	analogGyro3Axis.floatPointer_value(aY) + "g\n" +
	"Acceleration: Z: " +
	analogGyro3Axis.floatPointer_value(aZ) + "g";
	console.log(outputStr);

	console.log(" ");
}


// When exiting: clear interval and print exit message
process.on('SIGINT', function()
{
	clearInterval(g_myInterval);
	console.log("Exiting...");
	process.exit(0);
});

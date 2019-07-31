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

// Load heart rate sensor module
var heartRateSensor = require('jsupm_ehr');
// Instantiate a  Ear-clip Heart Rate sensor on digital pin D2
var myHeartRateSensor = new heartRateSensor.EHR(2);

// set the beat counter to 0, init the clock and start counting beats
myHeartRateSensor.clearBeatCounter();
myHeartRateSensor.initClock();
myHeartRateSensor.startBeatCounter();

var millis, beats, hr;
var myInterval = setInterval(function()
{
	// we grab these just for display purposes in this example
	millis = myHeartRateSensor.getMillis();
	beats = myHeartRateSensor.beatCounter();

	// heartRate() requires that at least 5 seconds pass before
	// returning anything other than 0
	hr = myHeartRateSensor.heartRate();

	// output milliseconds passed, beat count, and computed heart rate
	console.log("Millis: " + millis + " Beats: " + beats +
	            " Heart Rate: " + hr);
}, 1000);

// Print message when exiting
process.on('SIGINT', function()
{
	clearInterval(myInterval);
	myHeartRateSensor.stopBeatCounter();
	myHeartRateSensor = null
	heartRateSensor.cleanUp();
	heartRateSensor = null;

	console.log("Exiting");
	process.exit(0);
});

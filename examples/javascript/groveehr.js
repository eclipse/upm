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

// Load heart rate sensor module
var heartRateSensor = require('jsupm_groveehr');
// Instantiate a Grove Ear-clip Heart Rate sensor on digital pin D2
var myHeartRateSensor = new heartRateSensor.GroveEHR(2);

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

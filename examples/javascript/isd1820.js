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

var recorder_lib = require("jsupm_isd1820");

// Instantiate a ISD1820 on digital pins 2 (play) and 3 (record)
// This example was tested on the Grove Recorder.
var myRecorder = new recorder_lib.ISD1820(2, 3);


var doRecord = false;

if (process.argv.length > 2)
	doRecord = true;

// if an argument was specified (any argument), go into record mode,
// else playback a previously recorded sample
console.log("Supply any argument to the command line to record.");
console.log("Running this example without arguments will play back ");
console.log("any previously recorded sound.");
console.log("There is approximately 10 seconds of recording time.\n");

// depending on what was selected, do it, and sleep for 15 seconds
if (doRecord)
	myRecorder.record(true);
else
	myRecorder.play(true);

// There are about 10 seconds of recording/playback time, so we will
// sleep for a little extra time.
console.log("Sleeping for 15 seconds...");

setTimeout(function()
{
	// turn off whatever we were doing.
	if (doRecord)
		myRecorder.record(false);
	else
		myRecorder.play(false);
	console.log("Exiting");
	myRecorder = null;
	recorder_lib.cleanUp();
	recorder_lib = null;
	process.exit(0);
}, 15 * 1000);

/*jslint node:true, vars:true, bitwise:true, unparam:true */
/*jshint unused:true */
/*
* Author: Zion Orent <zorent@ics.com>
* Copyright (c) 2015 Intel Corporation.
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

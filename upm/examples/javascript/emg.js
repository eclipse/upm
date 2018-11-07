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
var emg_lib = require("jsupm_emg");

// The was tested with the EMG Muscle Signal Reader Sensor Module
// Instantiate a EMG on analog pin A0
var emg_obj = new emg_lib.EMG(0);
console.log("Calibrating....");
emg_obj.calibrate();

var myInterval = setInterval(function()
{
	console.log(emg_obj.value());
}, 100);

// When exiting: clear interval, and print message
process.on('SIGINT', function()
{
	clearInterval(myInterval);
	emg_obj = null;
	emg_lib.cleanUp();
	emg_lib = null;
	console.log("Exiting...");
	process.exit(0);
});

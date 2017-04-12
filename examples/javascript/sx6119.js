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

var FM_receiver_lib = require('jsupm_sx6119');

// Instantiate a SX6119 on digital pins 2 (power) and 3 (seek)
// This example was tested on the Grove FM Receiver.
var myFM_receiver_obj = new FM_receiver_lib.SX6119(2, 3);

// if an argument was specified (any argument), seek to the next
// station, else just toggle the power.


console.log("Supply any argument to the command line to seek to the");
console.log("next station.");
console.log("Running the example without an argument will toggle the");
console.log("power on or off.\n");

var doSeek = false;

if (process.argv.length > 2)
	doSeek = true;

// depending on what was selected, do it
if (doSeek)
	myFM_receiver_obj.seek();
else
	myFM_receiver_obj.togglePower();

console.log("Exiting");

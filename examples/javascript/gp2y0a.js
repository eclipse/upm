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
// JavaScript file for GP2Y0A family of IR Proximity Sensors

// analog voltage, usually 3.3 or 5.0
var GP2Y0A_AREF = 5.0;
var SAMPLES_PER_QUERY = 20;

var IRProximity = require('jsupm_gp2y0a');

// Note, for the Grove 80cm version of this sensor, due to the way it is wired,
// you need to plug this into the A0 port, where it will use the available
// A1 pin for data.
// Instantiate a GP2Y0A on analog pin A1
var myVolts = new IRProximity.GP2Y0A(1);

// The higher the voltage (closer to AREF) the closer the object is.
// NOTE: The measured voltage will probably not exceed 3.3 volts.
// Every second, print the averaged voltage value
// (averaged over 20 samples).
var myInterval = setInterval(function()
{
	console.log("AREF: " + GP2Y0A_AREF + 
                ", Voltage value (higher means closer): " + 
                myVolts.value(GP2Y0A_AREF, SAMPLES_PER_QUERY));
}, 1000);

// Print message when exiting
process.on('SIGINT', function()
{
	clearInterval(myInterval);
	myVolts = null;
	IRProximity.cleanUp();
	IRProximity = null;
	console.log("Exiting...");
	process.exit(0);
});

/*jslint node:true, vars:true, bitwise:true, unparam:true */
/*jshint unused:true */
/*global */
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

// Load alcohol sensor module
var mq303a = require('jsupm_mq303a');

// Instantiate an mq303a sensor on analog pin A0
// This device uses a heater powered from an analog I/O pin. 
// If using A0 as the data pin, then you need to use A1, as the heater
// pin (if using a grove mq303a).  For A1, we can use the D15 gpio, 
// setup as an output, and drive it low to power the heater.
var myAlcoholObj = new mq303a.MQ303A(0, 15);

console.log("Enabling heater and waiting 2 minutes for warmup.");

// give time updates every 30 seconds until 2 minutes have passed
// for the alcohol sensor to warm up
statusMessage(1);
statusMessage(2);
statusMessage(3);

function statusMessage(amt)
{
	setTimeout(function()
	{
		console.log((amt * 30) + " seconds have passed");
	}, 30000 * amt);
}

// run the alcohol sensor in 2 minutes
setTimeout(runAlcoholSensor, 120000);

function runAlcoholSensor()
{
	var notice = "This sensor may need to warm " +
				"until the value drops below about 450."
	console.log(notice);

    // Print the detected alcohol value every second
	setInterval(function()
	{
		var val = myAlcoholObj.value();
		var msg = "Alcohol detected ";
		msg += "(higher means stronger alcohol): ";
		console.log(msg + val);
	}, 1000);
}

// Print message when exiting
process.on('SIGINT', function()
{
	console.log("Exiting...");
	process.exit(0);
});

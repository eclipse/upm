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

var CO2_lib = require('jsupm_mhz16');

// Instantiate a MHZ16 serial CO2 sensor on uart 0.
// This example was tested on the Grove CO2 sensor module.
var myCO2_obj = new CO2_lib.MHZ16(0);

// make sure port is initialized properly.  9600 baud is the default.
if (!myCO2_obj.setupTty(CO2_lib.int_B9600))
{
	console.log("Failed to setup tty port parameters");
	process.exit(0);
}

outputStr = "Make sure that the sensor has had " +
			"at least 3 minutes to warm up";
console.log(outputStr);
outputStr = "or you will not get valid results.";
console.log(outputStr);
outputStr = "The temperature reported is not the ambient temperature,";
console.log(outputStr);
outputStr = "but rather the temperature of the sensor elements.";
console.log(outputStr);

var gas = CO2_lib.new_intp();
var temp = CO2_lib.new_intp();

function writeCO2data()
{
	myCO2_obj.getData(gas, temp);
	outputStr = "CO2 concentration: " + CO2_lib.intp_value(gas) +
	" PPM, " +
	"Temperature (in C): " + CO2_lib.intp_value(temp);
	console.log(outputStr);	
}
var myInterval;
setTimeout(function()
{
	myInterval = setInterval(writeCO2data, 2000);
}, 1000);


// Print message, clear memory when exiting
process.on('SIGINT', function()
{
	clearInterval(myInterval);
	myCO2_obj = null;
	CO2_lib.cleanUp();
	CO2_lib = null;
	console.log("Exiting");
	process.exit(0);
});

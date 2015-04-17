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

var UV_lib = require('jsupm_si114x');

// Instantiate a SI114x UV Sensor on I2C bus 0
var myUVSensor = new UV_lib.SI114X(0);

for (var x in myUVSensor)
	console.log(x);
// First initialize it
myUVSensor.initialize();

console.log("UV Index Scale:");
console.log("---------------");
console.log("11+        Extreme");
console.log("8-10       Very High");
console.log("6-7        High");
console.log("3-5        Moderate");
console.log("0-2        Low\n");

// update every second and print the currently measured UV Index
var myInterval = setInterval(function()
{
	// update current value(s)
	myUVSensor.update();

	// print detected value
	console.log("UV Index: " + myUVSensor.getUVIndex());
}, 1000);


// When exiting: clear interval and print message
process.on('SIGINT', function()
{
	clearInterval(myInterval);
	myUVSensor = null
	UV_lib.cleanUp();
	UV_lib = null;
	console.log("Exiting");
	process.exit(0);
});

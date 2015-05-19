/*jslint node:true, vars:true, bitwise:true, unparam:true */
/*jshint unused:true */

/*
* Author: Jon Trulson <jtrulson@ics.com>
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

var digitalAccelerometer = require('jsupm_h3lis331dl');

// Instantiate an H3LIS331DL on I2C bus 0
var myDigitalAccelerometer = new digitalAccelerometer.H3LIS331DL(
    digitalAccelerometer.H3LIS331DL_I2C_BUS, 
    digitalAccelerometer.H3LIS331DL_DEFAULT_I2C_ADDR);

// Initialize the device with default values
myDigitalAccelerometer.init();

var x, y, z;
x = digitalAccelerometer.new_intp();
y = digitalAccelerometer.new_intp();
z = digitalAccelerometer.new_intp();

var ax, ay, az;
ax = digitalAccelerometer.new_floatp();
ay = digitalAccelerometer.new_floatp();
az = digitalAccelerometer.new_floatp();

var outputStr;

var myInterval = setInterval(function()
{
	myDigitalAccelerometer.update();
	myDigitalAccelerometer.getRawXYZ(x, y, z);
	outputStr = "Raw: X = " + digitalAccelerometer.intp_value(x) +
	" Y = " + digitalAccelerometer.intp_value(y) +
	" Z = " + digitalAccelerometer.intp_value(z);
	console.log(outputStr);

	myDigitalAccelerometer.getAcceleration(ax, ay, az);
	outputStr = "Acceleration: AX = " 
		+ roundNum(digitalAccelerometer.floatp_value(ax), 6)
		+ " AY = " + roundNum(digitalAccelerometer.floatp_value(ay), 6) 
		+ " AZ = " + roundNum(digitalAccelerometer.floatp_value(az), 6);
	console.log(outputStr);
}, 500);

// round off output to match C example, which has 6 decimal places
function roundNum(num, decimalPlaces)
{
	var extraNum = (1 / (Math.pow(10, decimalPlaces) * 1000));
	return (Math.round((num + extraNum) 
		* (Math.pow(10, decimalPlaces))) / Math.pow(10, decimalPlaces));
}

// When exiting: clear interval and print message
process.on('SIGINT', function()
{
	clearInterval(myInterval);

	// clean up memory
	digitalAccelerometer.delete_intp(x);
	digitalAccelerometer.delete_intp(y);
	digitalAccelerometer.delete_intp(z);

	digitalAccelerometer.delete_floatp(ax);
	digitalAccelerometer.delete_floatp(ay);
	digitalAccelerometer.delete_floatp(az);

	console.log("Exiting...");
	process.exit(0);
});

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

var digitalAccelerometer = require('jsupm_mma7660');

// Instantiate an MMA7660 on I2C bus 0
var myDigitalAccelerometer = new digitalAccelerometer.MMA7660(
					digitalAccelerometer.MMA7660_I2C_BUS, 
					digitalAccelerometer.MMA7660_DEFAULT_I2C_ADDR);

// place device in standby mode so we can write registers
myDigitalAccelerometer.setModeStandby();

// enable 64 samples per second
myDigitalAccelerometer.setSampleRate(digitalAccelerometer.MMA7660.AUTOSLEEP_64);

// place device into active mode
myDigitalAccelerometer.setModeActive();

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
	myDigitalAccelerometer.getRawValues(x, y, z);
	outputStr = "Raw values: x = " + digitalAccelerometer.intp_value(x) +
	" y = " + digitalAccelerometer.intp_value(y) +
	" z = " + digitalAccelerometer.intp_value(z);
	console.log(outputStr);

	myDigitalAccelerometer.getAcceleration(ax, ay, az);
	outputStr = "Acceleration: x = " 
		+ roundNum(digitalAccelerometer.floatp_value(ax), 6)
		+ "g y = " + roundNum(digitalAccelerometer.floatp_value(ay), 6) 
		+ "g z = " + roundNum(digitalAccelerometer.floatp_value(az), 6) + "g";
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

	myDigitalAccelerometer.setModeStandby();

	console.log("Exiting...");
	process.exit(0);
});

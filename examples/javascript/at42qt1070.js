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

/* functions */
function printButtons(touchObj)
{
	var buttonPressed = false;
	var buttons = touchObj.getButtons();

	process.stdout.write("Buttons Pressed: ");
	for (var i=0; i<7; i++)
	{
		if (buttons & (1 << i))
		{
			process.stdout.write(i + " ");
			buttonPressed = true;
		}
	}

	if (!buttonPressed)
		process.stdout.write("None");

	console.log(" ");

	if (touchObj.isCalibrating())
		console.log("Calibration is occurring.");

	if (touchObj.isOverflowed())
		console.log("Overflow was detected.");
}


/* Global code that runs on startup */

var touchSensor_lib = require('jsupm_at42qt1070');

var I2C_BUS = touchSensor_lib.AT42QT1070_I2C_BUS;
var DEFAULT_I2C_ADDR = touchSensor_lib.AT42QT1070_DEFAULT_I2C_ADDR;
// Instantiate an AT42QT1070 on I2C bus 0
var mytouchSensor_obj = new touchSensor_lib.AT42QT1070(I2C_BUS,
                                               DEFAULT_I2C_ADDR);

var myInterval = setInterval(function()
{
	mytouchSensor_obj.updateState();
	printButtons(mytouchSensor_obj);
}, 100);

// Print message when exiting and clear interval/memory
process.on('SIGINT', function()
{
	clearInterval(myInterval);
	mytouchSensor_obj = null;
	touchSensor_lib.cleanUp();
	touchSensor_lib = null;
	console.log("Exiting");
	process.exit(0);
});

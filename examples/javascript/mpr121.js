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

var touchSensor = require('jsupm_mpr121');

var myTouchSensor = new touchSensor.MPR121(touchSensor.MPR121_I2C_BUS, touchSensor.MPR121_DEFAULT_I2C_ADDR);

myTouchSensor.configAN3944();

setInterval(function()
{
	myTouchSensor.readButtons();
	printButtons(myTouchSensor);
}, 1000);

function printButtons(touchSensor)
{
	var buttonPressed = false;

	var outputStr = "Buttons Pressed: ";
	for (var i=0; i<12; i++)
	{
		if (touchSensor.m_buttonStates & (1 << i))
		{
			outputStr += (i + " ");
			buttonPressed = true;
		}
	}

	if (!buttonPressed)
		outputStr += "None";

	console.log(outputStr);

	if (touchSensor.m_overCurrentFault)
		console.log("Over Current Fault detected!");
}

// Print message when exiting
process.on('SIGINT', function()
{
	console.log("Exiting...");
	process.exit(0);
});


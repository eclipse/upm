/*
* Author: Zion Orent <zorent@ics.com>
* Copyright (c) 2015 Intel Corporation.
*
* This program and the accompanying materials are made available under the
* terms of the The MIT License which is available at
* https://opensource.org/licenses/MIT.
*
* SPDX-License-Identifier: MIT
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

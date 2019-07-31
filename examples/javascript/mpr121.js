/*
* Author: Zion Orent <zorent@ics.com>
* Copyright (c) 2014 Intel Corporation.
*
* This program and the accompanying materials are made available under the
* terms of the The MIT License which is available at
* https://opensource.org/licenses/MIT.
*
* SPDX-License-Identifier: MIT
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


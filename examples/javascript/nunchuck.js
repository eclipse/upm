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

var nunchuck_lib = require('jsupm_nunchuck');

// Instantiate a nunchuck controller bus 3
var nunchuck_obj = new nunchuck_lib.NUNCHUCK(3);

setInterval(function()
{
	nunchuck_obj.update();

	var outputStr = "stickX: " + nunchuck_obj.stickX +
	", stickY: " + nunchuck_obj.stickY;
	console.log(outputStr);
	outputStr = "accelX: " + nunchuck_obj.accelX +
	", accelY: " + nunchuck_obj.accelY +
	", accelZ: " + nunchuck_obj.accelZ;
	console.log(outputStr);

	outputStr = "button C: " +
	((nunchuck_obj.buttonC) ? "pressed" : "not pressed");
	console.log(outputStr);
	outputStr = "button Z: " +
	((nunchuck_obj.buttonZ) ? "pressed" : "not pressed");
	console.log(outputStr);
}, 100);

// Print message when exiting
process.on('SIGINT', function()
{
	console.log("Exiting...");
	process.exit(0);
});

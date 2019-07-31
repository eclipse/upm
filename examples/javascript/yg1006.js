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

var flameSensor = require('jsupm_yg1006');

// Instantiate a flame sensor on digital pin D2
var myFlameSensor = new flameSensor.YG1006(2);

// Check every second for the presence of a flame
setInterval(function()
{
	if (myFlameSensor.flameDetected())
		console.log("Flame detected.");
	else
		console.log("No flame detected.");
}, 1000);

// Print message when exiting
process.on('SIGINT', function()
{
	console.log("Exiting...");
	process.exit(0);
});

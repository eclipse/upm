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

var hallEffectSensor = require('jsupm_a110x');

// Instantiate a Hall Effect magnet sensor on digital pin D2
var myHallEffectSensor = new hallEffectSensor.A110X(2);

// Check every second for the presence of a magnetic field (south polarity)
setInterval(function()
{
	if (myHallEffectSensor.magnetDetected())
		console.log("Magnet (south polarity) detected.");
	else
		console.log("No magnet detected.");
}, 1000);

// Print message when exiting
process.on('SIGINT', function()
{
	console.log("Exiting...");
	process.exit(0);
});

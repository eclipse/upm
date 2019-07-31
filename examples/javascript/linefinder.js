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

var lineFinderSensor = require('jsupm_linefinder');

// Instantiate a  line finder sensor on digital pin D2
var myLineFinderSensor = new lineFinderSensor.LineFinder(2);

// Check every second for the presence of white detection
setInterval(function()
{
	if (myLineFinderSensor.whiteDetected())
		console.log("White detected.");
	else
		console.log("Black detected.");
}, 1000);

// Turn relay off when exiting
process.on('SIGINT', function()
{
	console.log("Exiting...");
	process.exit(0);
});

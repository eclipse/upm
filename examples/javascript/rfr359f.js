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

var distInterrupter = require("jsupm_rfr359f");

// Instantiate an RFR359F digital pin D2
// This was tested on the Grove IR Distance Interrupter
var myDistInterrupter = new distInterrupter.RFR359F(2);

// The range appears to be about 4 inches, depending on adjustment
var myInterval = setInterval(function()
{
	if (myDistInterrupter.objectDetected())
		console.log("Object detected");
	else
		console.log("Area is clear");
}, 100);

// When exiting: turn off LED, clear interval, and print message
process.on('SIGINT', function()
{
	clearInterval(myInterval);
	console.log("Exiting...");
	process.exit(0);
});

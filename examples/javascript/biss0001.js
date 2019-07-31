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

//Load Grove Motion module
var grove_motion = require('jsupm_biss0001');
// Instantiate a Grove Motion sensor on GPIO pin D2
var myMotionObj = new grove_motion.BISS0001(2);
setInterval(function()
{
	if (myMotionObj.value())
		console.log("Detecting moving object");
	else
		console.log("No moving objects detected");
}, 1000);

// Print message when exiting
process.on('SIGINT', function()
{
	console.log("Exiting...");
	process.exit(0);
});

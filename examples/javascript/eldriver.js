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

var eldriver_lib = require("jsupm_eldriver");

// The was tested with the  El Driver Module
// Instantiate a  El Driver on digital pin D2
var eldriver_obj = new eldriver_lib.ElDriver(2);

var lightState = true;

var myInterval = setInterval(function()
{
	if (lightState)
		eldriver_obj.on();
	else
		eldriver_obj.off();
	lightState = !lightState;
}, 1000);

// When exiting: turn off EL wire, clear interval, and print message
process.on('SIGINT', function()
{
	clearInterval(myInterval);
	eldriver_obj.off();
	eldriver_obj = null;
	eldriver_lib.cleanUp();
	eldriver_lib = null;
	console.log("Exiting...");
	process.exit(0);
});

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

var waterlevel_lib = require("jsupm_waterlevel");

// The was tested with the Water Level Sensor
// Instantiate a Water Level Sensor on digital pin D2
var waterlevel_obj = new waterlevel_lib.WaterLevel(2);

var myInterval = setInterval(function()
{
	if (waterlevel_obj.isSubmerged())
		console.log("Sensor is submerged in liquid");
	else
		console.log("Liquid is below water level sensor");
}, 1000);

// When exiting: clear interval, clean up memory, and print message
process.on('SIGINT', function()
{
	clearInterval(myInterval);
	waterlevel_obj = null;
	waterlevel_lib.cleanUp();
	waterlevel_lib = null;
	console.log("Exiting");
	process.exit(0);
});

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

var reflectiveSensor = require("jsupm_rpr220");

// This example uses a simple method to determine current status

// Instantiate an RPR220 digital pin D2
// This was tested on the Grove IR Reflective Sensor
var myReflectiveSensor = new reflectiveSensor.RPR220(2);

var myInterval = setInterval(function()
{
	if (myReflectiveSensor.blackDetected())
		console.log("Black detected");
	else
		console.log("Black NOT detected");
}, 100);

// When exiting: clear interval and print message
process.on('SIGINT', function()
{
	clearInterval(myInterval);
	console.log("Exiting...");
	process.exit(0);
});

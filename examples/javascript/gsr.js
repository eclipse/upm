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

var gsr_lib = require("jsupm_gsr");

// The was tested with the GSR Galvanic Skin Response Sensor module.

// Instantiate a GSR on analog pin A0
var gsr_obj = new gsr_lib.GSR(0);
console.log("Calibrating....");
gsr_obj.calibrate();

var myInterval = setInterval(function()
{
	console.log(gsr_obj.value());
}, 500);


// When exiting: clear interval, and print message
process.on('SIGINT', function()
{
	clearInterval(myInterval);
	gsr_obj = null;
	gsr_lib.cleanUp();
	gsr_lib = null;
	console.log("Exiting...");
	process.exit(0);
});

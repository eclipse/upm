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

var digitalLightSensor = require('jsupm_tsl2561');

// Instantiate a digital light sensor TSL2561 on I2C
var myDigitalLightSensor = new digitalLightSensor.TSL2561();


setInterval(function()
{
	console.log("Light value is " + myDigitalLightSensor.getLux());
}, 1000);

// Print message when exiting
process.on('SIGINT', function()
{
	console.log("Exiting...");
	process.exit(0);
});

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

// Load dust sensor module
var dustSensor = require('jsupm_ppd42ns');
// Instantiate a dust sensor on digital pin D8
var myDustSensor = new dustSensor.PPD42NS(8);

var data;

// Continue until user ends program
var notice = "This program will give readings ";
notice += "every 30 seconds until you stop it"
console.log(notice);
while(1)
{
	data = myDustSensor.getData();
	console.log("Low pulse occupancy: " + data.lowPulseOccupancy);
	console.log("Ratio: " + data.ratio);
	console.log("Concentration: " + data.concentration);
}

// Print message when exiting
process.on('SIGINT', function()
{
	console.log("Exiting...");
	process.exit(0);
});

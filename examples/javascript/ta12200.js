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

var electricitySensor = require('jsupm_ta12200');
// Instantiate a TA12-200 sensor on analog pin A0
var myElectricitySensor = new electricitySensor.TA12200(0);

var maxVal, current;
var myInterval = setInterval(function()
{
	maxVal = myElectricitySensor.highestValue();
	current = myElectricitySensor.milliAmps(maxVal);

	outputStr = "Max ADC Value: " + maxVal +
	", current: " + current + "mA";
	console.log(outputStr);
}, 100);

// Print message when exiting
process.on('SIGINT', function()
{
	clearInterval(myInterval);
	myElectricitySensor = null;
	electricitySensor.cleanUp();
	electricitySensor = null;
	console.log("Exiting...");
	process.exit(0);
});

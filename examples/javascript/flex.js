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
var flexSensor_lib = require('jsupm_flex');

// The was tested with a Spectra Symbol flex sensor.
// We attached a 22K resistor to a breadboard,
// with 1 end attached to GND and the other connected to
// both the flex sensor and A0.
// The flex sensor was connected on 1 pin to the 22K resistor and A0
// and on the other pin to 5V.

// Instantiate a UV sensor on analog pin A0
var myFlexSensor_obj = new flexSensor_lib.Flex(0);

var myInterval = setInterval(function()
{
	var outputStr = "Flex value: " + myFlexSensor_obj.value();
	console.log(outputStr);
}, 1000);

// Print message when exiting
process.on('SIGINT', function()
{
	clearInterval(myInterval);
	myFlexSensor_obj = null;
	flexSensor_lib.cleanUp();
	flexSensor_lib = null;
	console.log("Exiting");
	process.exit(0);
});

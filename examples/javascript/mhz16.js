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

var CO2_lib = require('jsupm_mhz16');

// Instantiate a MHZ16 serial CO2 sensor on uart 0.
// This example was tested on the Grove CO2 sensor module.
var myCO2_obj = new CO2_lib.MHZ16(0);

// make sure port is initialized properly.  9600 baud is the default.
if (!myCO2_obj.setupTty(CO2_lib.int_B9600))
{
	console.log("Failed to setup tty port parameters");
	process.exit(0);
}

outputStr = "Make sure that the sensor has had " +
			"at least 3 minutes to warm up";
console.log(outputStr);
outputStr = "or you will not get valid results.";
console.log(outputStr);
outputStr = "The temperature reported is not the ambient temperature,";
console.log(outputStr);
outputStr = "but rather the temperature of the sensor elements.";
console.log(outputStr);

function writeCO2data()
{
	if (!myCO2_obj.getData())
		console.log("Failed to retrieve data");
	else
	{
		outputStr = "CO2 concentration: " + myCO2_obj.getGas() +
		" PPM, " +
		"Temperature (in C): " + myCO2_obj.getTemperature();
		console.log(outputStr);
	}
}
var myInterval;
setTimeout(function()
{
	myInterval = setInterval(writeCO2data, 2000);
}, 1000);


// Print message, clear memory when exiting
process.on('SIGINT', function()
{
	clearInterval(myInterval);
	myCO2_obj = null;
	CO2_lib.cleanUp();
	CO2_lib = null;
	console.log("Exiting");
	process.exit(0);
});

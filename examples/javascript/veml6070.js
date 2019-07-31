/*
* Author: Abhishek Malik <abhishek.malik@intel.com>
* Copyright (c) 2017 Intel Corporation.
*
* This program and the accompanying materials are made available under the
* terms of the The MIT License which is available at
* https://opensource.org/licenses/MIT.
*
* SPDX-License-Identifier: MIT
*/

var veml6070 = require("jsupm_veml6070");

// Instantiate a Vishay UV sensor at bus 0
var veml6070_sensor = new veml6070.VEML6070(0);

var myInterval = setInterval(function()
{
	console.log("UV value: " + veml6070_sensor.getUVIntensity());
}, 100);

// When exiting: clear interval and print message
process.on('SIGINT', function()
{
	clearInterval(myInterval);
	console.log("Exiting...");
	process.exit(0);
});

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

var abp = require("jsupm_abp");

// Instantiate a Honeywell ABP Pressure Sensor at bus 0
var abp_sensor = new abp.ABP(0, 0x28);

var myInterval = setInterval(function()
{
	abp_sensor.update();
	console.log("Pressure: " + abp_sensor.getPressure());
	console.log("Temperature: " + abp_sensor.getTemperature());
}, 100);

// When exiting: clear interval and print message
process.on('SIGINT', function()
{
	clearInterval(myInterval);
	console.log("Exiting...");
	process.exit(0);
});

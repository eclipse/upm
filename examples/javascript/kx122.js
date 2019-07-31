/*
* Author: Noel Eck <noel.eck@intel.com>
* Copyright (c) 2017 Intel Corporation.
*
* This program and the accompanying materials are made available under the
* terms of the The MIT License which is available at
* https://opensource.org/licenses/MIT.
*
* SPDX-License-Identifier: MIT
*/

var kx122 = require("jsupm_kx122");

// Instantiate a KX122
var kx122_sensor = new kx122.KX122(0, -1, 24);

// Reset the device
kx122_sensor.softwareReset();

// Reset the device
kx122_sensor.deviceInit(kx122.KX122_ODR_50,
		kx122.HIGH_RES,
		kx122.KX122_RANGE_2G);

var myInterval = setInterval(function()
{
	data = kx122_sensor.getAccelerationDataVector();
	console.log("Acceleration x: " + data.get(0)
		+ " y: " + data.get(1)
		+ " z: " + data.get(2));
}, 1000);

// When exiting: clear interval and print message
process.on('SIGINT', function()
{
	clearInterval(myInterval);
	console.log("Exiting...");
	process.exit(0);
});

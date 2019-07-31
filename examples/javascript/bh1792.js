/*
* The MIT License (MIT)
*
* Author: Assam Boudjelthia
* Copyright (c) 2018 Rohm Semiconductor.
*
* This program and the accompanying materials are made available under the
* terms of the The MIT License which is available at
* https://opensource.org/licenses/MIT.
*
* SPDX-License-Identifier: MIT
*/

var bh1792 = require("/usr/include/nodejs/lib/node_modules/jsupm_bh1792");

var bh1792_sensor = new bh1792.BH1792();

bh1792_sensor.SoftReset();
bh1792_sensor.EnableSyncMode(256, 32);
bh1792_sensor.StartMeasurement();

var waitTime = bh1792_sensor.GetMeasurementTimeMS();
var counter = 10;
console.log("Heart beat sensor readings: ");
var interval = setInterval(function() {
	data = bh1792_sensor.GetGreenData();
	console.log("off: " + data.get(0) + ", on: " + data.get(1) +
				", timestamp: " + Date.now() + " msec");
	counter--;
	if (counter == 0) {
		clearInterval(interval);
	}
}, waitTime);

process.on("SIGINT", function() {
	clearInterval(interval);
	console.log("Exiting...");
	process.exit(0);
});

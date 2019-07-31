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

var kxtj3 = require("jsupm_kxtj3");

var kxtj3_sensor = new kxtj3.KXTJ3(0x0, 0x0f);

kxtj3_sensor.SensorInit(
	kxtj3.KXTJ3_ODR_25,
	kxtj3.HIGH_RES,
	kxtj3.KXTJ3_RANGE_16G_14);

var waitTime = kxtj3_sensor.GetAccelerationSamplePeriod();
var counter = 10;
console.log("Setting settings:\nODR: 25 Hz\nResolution: \
			High\nAcceleration range: 16g with 14bits");
console.log("Acceleration: ");
var interval = setInterval(function() {
	data = kxtj3_sensor.GetAccelerationVector();
	console.log(
		"x: " + data.get(0) + " y: " + data.get(1) + " z: " + data.get(2)
	);
	counter--;
	if (counter == 0) {
		clearInterval(interval);
	}
}, waitTime * 1000);

process.on("SIGINT", function() {
	clearInterval(interval);
	console.log("Exiting...");
	process.exit(0);
});

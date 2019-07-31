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

var bh1749 = require("jsupm_bh1749");

var bh1749_sensor = new bh1749.BH1749();

bh1749_sensor.SensorInit(
	bh1749.INT_JUDGE_1,
	bh1749.MEAS_240MS,
	bh1749.RGB_GAIN_1X,
	bh1749.IR_GAIN_1X,
	bh1749.RED);
bh1749_sensor.SetThresholdHigh(511);
bh1749_sensor.Enable();

var waitTime = bh1749_sensor.GetMeasurementTime();
var counter = 10;
console.log("Color readings: ");
var interval = setInterval(function() {
	data = bh1749_sensor.GetMeasurements();
	console.log(
		"R: " + data.get(0) + ", G: " + data.get(1) + ", B: " + data.get(2) +
		", IR: " + data.get(3) + ", G2: " + data.get(4)
	);
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

/*
* The MIT License (MIT)
*
* Author: Assam Boudjelthia
* Copyright (c) 2018 Rohm Semiconductor.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of
* this software and associated documentation files (the "Software"), to deal in
* the Software without restriction, including without limitation the rights to
* use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
* the Software, and to permit persons to whom the Software is furnished to do so,
* subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
* FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
* COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
* IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
* CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
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

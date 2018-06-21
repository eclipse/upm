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

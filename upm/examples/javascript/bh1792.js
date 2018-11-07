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

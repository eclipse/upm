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

var UV_lib = require('jsupm_si114x');

// Instantiate a SI114x UV Sensor on I2C bus 0
var myUVSensor = new UV_lib.SI114X(0);

for (var x in myUVSensor)
	console.log(x);
// First initialize it
myUVSensor.initialize();

console.log("UV Index Scale:");
console.log("---------------");
console.log("11+        Extreme");
console.log("8-10       Very High");
console.log("6-7        High");
console.log("3-5        Moderate");
console.log("0-2        Low\n");

// update every second and print the currently measured UV Index
var myInterval = setInterval(function()
{
	// update current value(s)
	myUVSensor.update();

	// print detected value
	console.log("UV Index: " + myUVSensor.getUVIndex());
}, 1000);


// When exiting: clear interval and print message
process.on('SIGINT', function()
{
	clearInterval(myInterval);
	myUVSensor = null
	UV_lib.cleanUp();
	UV_lib = null;
	console.log("Exiting");
	process.exit(0);
});

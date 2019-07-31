/*
* Author: Jon Trulson <jtrulson@ics.com>
* Copyright (c) 2015 Intel Corporation.
*
* This program and the accompanying materials are made available under the
* terms of the The MIT License which is available at
* https://opensource.org/licenses/MIT.
*
* SPDX-License-Identifier: MIT
*/
var ad8232Sensor_lib = require('jsupm_ad8232');

// Instantiate a AD8232 sensor on digital pins 10 (LO+), 11 (LO-)
// and an analog pin, 0 (OUTPUT)
var myAd8232Sensor_obj = new ad8232Sensor_lib.AD8232(10, 11, 0);

// Output the raw numbers from the ADC, for plotting elsewhere.
// A return of 0 indicates a Lead Off (LO) condition.
// In theory, this data could be fed to software like Processing 
// (https://www.processing.org/) to plot the data just like an 
// EKG you would see in a hospital.
var myInterval = setInterval(function()
{
	console.log(myAd8232Sensor_obj.value());
}, 1);

// Print message when exiting
process.on('SIGINT', function()
{
	clearInterval(myInterval);
	myAd8232Sensor_obj = null;
	ad8232Sensor_lib.cleanUp();
	ad8232Sensor_lib = null;
	console.log("Exiting");
	process.exit(0);
});

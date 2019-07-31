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


var sensorObj = require('jsupm_dfrph');

// Instantiate a DFRPH sensor on analog pin A0, with an analog
// reference voltage of 5.0
var sensor = new sensorObj.DFRPH(0, 5.0);


// After calibration, set the offset (based on calibration with a pH
// 7.0 buffer solution).  See the UPM sensor documentation for
// calibrations instructions.
sensor.setOffset(0.065);

// Every second, sample the pH and output it's corresponding
// analog voltage.

setInterval(function()
{
    console.log("Detected volts: " + sensor.volts());
    console.log("pH value: " + sensor.pH());
}, 1000);

// exit on ^C
process.on('SIGINT', function()
{
    sensor = null;
    sensorObj.cleanUp();
    sensorObj = null;
    console.log("Exiting.");
    process.exit(0);
});


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


var sensorObj = require('jsupm_urm37');

// Instantiate a URM37 sensor on analog pin A0, reset pin on D2,
// trigger pin on D3 with an analog reference voltage of 5.0
var sensor = new sensorObj.URM37(0, 2, 3, 5.0);

// Every half a second, sample the URM37 and output the measured
// distance in cm.

setInterval(function()
{
    console.log("Detected distance (cm): " + sensor.getDistance());
}, 500);

// exit on ^C
process.on('SIGINT', function()
{
    sensor = null;
    sensorObj.cleanUp();
    sensorObj = null;
    console.log("Exiting.");
    process.exit(0);
});


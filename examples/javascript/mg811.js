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


var sensorObj = require('jsupm_mg811');

// Instantiate an MG811 on analog pin A0, and digital pin D2 with an
// analog reference voltage of MG811_AREF (5.0)

var sensor = new sensorObj.MG811(0, 2, 5.0);

// Every tenth of a second, sample the sensor and output it's
// detected CO2 concentration in parts per million (ppm)

setInterval(function()
{
    console.log("CO2 concentration in PPM: " + sensor.ppm());
}, 100);

// exit on ^C
process.on('SIGINT', function()
{
    sensor = null;
    sensorObj.cleanUp();
    sensorObj = null;
    console.log("Exiting.");
    process.exit(0);
});


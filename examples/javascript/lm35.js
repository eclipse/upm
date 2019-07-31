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


var sensorObj = require('jsupm_lm35');

// Instantiate a LM35 on analog pin A0, with a default analog
// reference voltage of 5.0
var sensor = new sensorObj.LM35(0);

// Every half second, sample the sensor and output the temperature

setInterval(function()
{
    console.log("Temperature: " + sensor.getTemperature() + " C");
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


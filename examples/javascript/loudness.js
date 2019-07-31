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


var sensorObj = require('jsupm_loudness');

// Instantiate a Loudness sensor on analog pin A0, with an analog
// reference voltage of 5.0
var sensor = new sensorObj.Loudness(0, 5.0);

// Every tenth of a second, sample the loudness and output it's
// corresponding analog voltage. 

setInterval(function()
{
    console.log("Detected loudness (volts): " + sensor.loudness());
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


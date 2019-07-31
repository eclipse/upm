/*jslint node:true, vars:true, bitwise:true, unparam:true */
/*jshint unused:true */

/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

var sensorObj = require('jsupm_bh1750');

// Instantiate a BH1750 sensor using defaults (I2C bus (0), using
// the default I2C address (0x23), and setting the mode to highest
// resolution, lowest power mode).
var sensor = new sensorObj.BH1750();

// Every second, sample the BH1750 and output the measured lux value

setInterval(function()
{
    console.log("Detected Light Level (lux): " + sensor.getLux());
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


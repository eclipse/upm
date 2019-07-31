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


var sensorObj = require('jsupm_adxrs610');

// Instantiate a ADXRS610 sensor on analog pin A0 (dataout), and
// analog A1 (temp out) with an analog reference voltage of
// 5.0
var sensor = new sensorObj.ADXRS610(0, 1, 5.0);

// set a deadband region around the zero point to report 0.0 (optional)
sensor.setDeadband(0.015);

// Every tenth of a second, sample the ADXRS610 and output it's
// corresponding temperature and angular velocity 

setInterval(function()
{
    console.log("Vel (deg/s): " + sensor.getAngularVelocity());
    console.log("Temp (C): " + sensor.getTemperature());
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


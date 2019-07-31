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

var sensorObj = require('jsupm_ecezo');

// Instantiate a ECEZO sensor on uart 0 at 9600 baud.
var sensor = new sensorObj.ECEZO(0, 9600, false);

// For I2C, assuming the device is configured for address 0x64 on
// I2C bus 0, you could use something like:
//
// var sensor = new sensorObj.ECEZO(0, 0x64, true);

setInterval(function()
{
    // update our values from the sensor
    sensor.update();

    console.log("EC "
                + sensor.getEC()
                + " uS/cm, TDS "
                + sensor.getTDS()
                + " mg/L, Salinity "
                + sensor.getSalinity()
                + " PSS-78, SG "
                + sensor.getSG());

}, 5000);

// exit on ^C
process.on('SIGINT', function()
{
    sensor = null;
    sensorObj.cleanUp();
    sensorObj = null;
    console.log("Exiting.");
    process.exit(0);
});

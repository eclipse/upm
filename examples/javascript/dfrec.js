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

var sensorObj = require('jsupm_dfrec');

// Instantiate a DFRobot EC sensor on analog pin A0, with a ds18b20
// temperature sensor connected to UART 0, and a device index (for
// the ds1820b uart bus) of 0, and an analog reference voltage of
// 5.0.
var sensor = new sensorObj.DFREC(0, 0, 0, 5.0);

// Every 2 seconds, update and print values
setInterval(function()
{
    sensor.update();

    console.log("EC = "
                + sensor.getEC()
                + " ms/cm");
    console.log("Volts = "
                + sensor.getVolts()
                + ", Temperature = "
                + sensor.getTemperature()
                + " C");

    console.log();

}, 2000);

// exit on ^C
process.on('SIGINT', function()
{
    sensor = null;
    sensorObj.cleanUp();
    sensorObj = null;
    console.log("Exiting.");
    process.exit(0);
});

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

var sensorObj = require('jsupm_sht1x');

// Instantiate a SHT1X sensor using D2 as the clock, and D3 as the
// data pin.
var sensor = new sensorObj.SHT1X(2, 3);

// Every 2 seconds, update and print values
setInterval(function()
{
    sensor.update();

    console.log("Temperature: "
                + sensor.getTemperature()
                + " C");
    console.log("Humidity:    "
                + sensor.getHumidity()
                + " RH");

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

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

var sensorObj = require('jsupm_ms5803');

// Instantiate a MS5803 instance using bus 0 and default i2c address
var sensor = new sensorObj.MS5803(0);

// For SPI, bus 0, you would pass -1 as the address, and a valid pin for CS:
// MS5803(0, -1, 10);

setInterval(function()
{
    // update our values from the sensor
    sensor.update();

    console.log("Temperature: "
                + sensor.getTemperature()
                + " C, Pressure: "
                + sensor.getPressure()
                + " mbar");
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

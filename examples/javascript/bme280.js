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

var sensorObj = require('jsupm_bmp280');

// Instantiate a BME280 instance using default i2c bus and address
var sensor = new sensorObj.BME280();

// For SPI, bus 0, you would pass -1 as the address, and a valid pin for CS:
// BME280(0, -1, 10);

setInterval(function()
{
    // update our values from the sensor
    sensor.update();

    console.log("Compensation Temperature: "
                + sensor.getTemperature()
                + " C / "
                + sensor.getTemperature(true)
                + " F");

    console.log("Pressure: "
                + sensor.getPressure()
                + " Pa");

    console.log("Computed Altitude: "
                + sensor.getAltitude()
                + " m");

    console.log("Humidity: "
                + sensor.getHumidity()
                + " %RH");

    console.log();

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

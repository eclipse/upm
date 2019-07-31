/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * The MIT License
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

var sensorObj = require('jsupm_bmm150');

// Instantiate a BMM150 instance using default i2c bus and address
var sensor = new sensorObj.BMM150();

// For SPI, bus 0, you would pass -1 as the address, and a valid pin for CS:
// BMM150(0, -1, 10);

// now output data every 250 milliseconds
setInterval(function()
{
    // update our values from the sensor
    sensor.update();

    var data = sensor.getMagnetometer();
    console.log("Magnetometer x: "
                + data.get(0)
                + " y: " + data.get(1)
                + " z: " + data.get(2)
                + " uT");

    console.log();

}, 250);

// exit on ^C
process.on('SIGINT', function()
{
    sensor = null;
    sensorObj.cleanUp();
    sensorObj = null;
    console.log("Exiting.");
    process.exit(0);
});

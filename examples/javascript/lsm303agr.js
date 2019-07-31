/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2017 Intel Corporation.
 *
 * The MIT License
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

var sensorObj = require('jsupm_lsm303agr');

// Instantiate a LSM303AGR instance using default i2c bus and address
var sensor = new sensorObj.LSM303AGR();

// now output data every 250 milliseconds
setInterval(function()
{
    // update our values from the sensor
    sensor.update();

    var data = sensor.getAccelerometer();
    console.log("Accelerometer x: "
                + data.get(0)
                + " y: " + data.get(1)
                + " z: " + data.get(2)
                + " g");

    data = sensor.getMagnetometer();
    console.log("Magnetometer x: "
                + data.get(0)
                + " y: " + data.get(1)
                + " z: " + data.get(2)
                + " uT");

    console.log("Temperature: "
                + sensor.getTemperature());

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

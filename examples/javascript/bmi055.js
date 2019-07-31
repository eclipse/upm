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

var sensorObj = require('jsupm_bmx055');

// Instantiate a BMI055 instance using default i2c bus and address
var sensor = new sensorObj.BMI055();

var x = new sensorObj.new_floatp();
var y = new sensorObj.new_floatp();
var z = new sensorObj.new_floatp();

// now output data every 250 milliseconds
setInterval(function()
{
    // update our values from the sensor
    sensor.update();

    sensor.getAccelerometer(x, y, z);
    console.log("Accelerometer x: "
                + sensorObj.floatp_value(x)
                + " y: " + sensorObj.floatp_value(y)
                + " z: " + sensorObj.floatp_value(z)
                + " g");

    sensor.getGyroscope(x, y, z);
    console.log("Gyroscope x: "
                + sensorObj.floatp_value(x)
                + " y: " + sensorObj.floatp_value(y)
                + " z: " + sensorObj.floatp_value(z)
                + " degrees/s");

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

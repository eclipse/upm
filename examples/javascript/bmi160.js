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


var sensorObj = require('jsupm_bmi160');

// Instantiate a BMI160 instance using default i2c bus and address
var sensor = new sensorObj.BMI160();

var x = new sensorObj.new_floatp();
var y = new sensorObj.new_floatp();
var z = new sensorObj.new_floatp();

// Output data every half second until interrupted
setInterval(function()
{
    // update our values from the sensor
    sensor.update();

    sensor.getAccelerometer(x, y, z);
    console.log("Accelerometer: AX: " + sensorObj.floatp_value(x) +
                " AY: " + sensorObj.floatp_value(y) +
                " AZ: " + sensorObj.floatp_value(z));

    sensor.getGyroscope(x, y, z);
    console.log("Gyroscope:     GX: " + sensorObj.floatp_value(x) +
                " AY: " + sensorObj.floatp_value(y) +
                " AZ: " + sensorObj.floatp_value(z));

    sensor.getMagnetometer(x, y, z);
    console.log("Magnetometer:  MX: " + sensorObj.floatp_value(x) +
                " MY: " + sensorObj.floatp_value(y) +
                " MZ: " + sensorObj.floatp_value(z));

    console.log();

}, 500);

// exit on ^C
process.on('SIGINT', function()
{
    sensor = null;
    sensorObj.cleanUp();
    sensorObj = null;
    console.log("Exiting.");
    process.exit(0);
});

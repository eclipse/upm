/*jslint node:true, vars:true, bitwise:true, unparam:true */
/*jshint unused:true */

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

var sensorObj = require('jsupm_mma7361');

// Instantiate a MMA7361 sensor on analog pins A0 (X), A1 (Y) A2
// (Z), selftest pin on D2, sleep pin on D3 nd an analog reference
// value of 5.0.  The freefall pin and the range pin are unused
// (-1).
var sensor = new sensorObj.MMA7361(0, 1, 2, 2, 3, -1, -1, 5.0);

var x = new sensorObj.new_floatp();
var y = new sensorObj.new_floatp();
var z = new sensorObj.new_floatp();

// 1.5g (true = 6g)
sensor.setRange(false);

// Every 10th of a second, update and print values
setInterval(function()
{
    // update our values from the sensor
    sensor.update();

    sensor.getAcceleration(x, y, z);
    console.log("Accelerometer x: "
                + sensorObj.floatp_value(x)
                + " y: " + sensorObj.floatp_value(y)
                + " z: " + sensorObj.floatp_value(z));

    sensor.getVolts(x, y, z);
    console.log("Volts x: "
                + sensorObj.floatp_value(x)
                + " y: " + sensorObj.floatp_value(y)
                + " z: " + sensorObj.floatp_value(z));

    console.log();

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

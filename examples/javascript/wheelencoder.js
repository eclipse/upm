/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */


var sensorObj = require('jsupm_wheelencoder');

// Instantiate a DFRobot Wheel Encoder on digital pin D2
var sensor = new sensorObj.WheelEncoder(2);

// set the counter to 0 and start counting
sensor.clearCounter();
sensor.startCounter();

setInterval(function()
{
    // output milliseconds passed and current sensor count
    console.log("Millis: " + sensor.getMillis() + " Count: " + 
                sensor.counter());
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


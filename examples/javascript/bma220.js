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


var sensorObj = require('jsupm_bma220');

// Instantiate an BMA220 using default parameters (bus 0, addr 0x0a)
var sensor = new sensorObj.BMA220();

var x = new sensorObj.new_floatp();
var y = new sensorObj.new_floatp();
var z = new sensorObj.new_floatp();

// Output data every half second until interrupted
setInterval(function()
{
    sensor.update();
    
    sensor.getAccelerometer(x, y, z);
    console.log("Accelerometer: AX: " + sensorObj.floatp_value(x) + 
                " AY: " + sensorObj.floatp_value(y) + 
                " AZ: " + sensorObj.floatp_value(z));

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


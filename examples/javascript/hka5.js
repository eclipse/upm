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

var sensorObj = require('jsupm_hka5');

// Instantiate a HKA5 sensor on uart 0.  We don't use the set or
// reset pins, so we pass -1 for them.
var sensor = new sensorObj.HKA5(0, -1, -1);

// update once every 2 seconds and output data
setInterval(function()
{
    sensor.update()

    console.log("PM 1  : "
                + sensor.getPM1()
                + " ug/m3");

    console.log("PM 2.5: "
                + sensor.getPM2_5()
                + " ug/m3");

    console.log("PM 10 : "
                + sensor.getPM10()
                + " ug/m3");

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

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


var sensorObj = require('jsupm_hdxxvxta');


/************** Main code **************/

console.log("Initializing...");

// Instantiate an HDXXVXTA instance, using A1 for humidity and A0
// for temperature
var sensor = new sensorObj.HDXXVXTA(1, 0);

// update and print available values every second
setInterval(function()
{
    // update our values from the sensor
    sensor.update();

    // we show both C and F for temperature
    console.log("Temperature:", sensor.getTemperature(),
                "C /", sensor.getTemperature(true), "F");

    console.log("Humidity:", sensor.getHumidity(), "%");

    console.log("");

}, 1000);


process.on('SIGINT', function()
{
    sensor = null;
    sensorObj.cleanUp();
    sensorObj = null;
    console.log("Exiting...");
    process.exit(0);
});

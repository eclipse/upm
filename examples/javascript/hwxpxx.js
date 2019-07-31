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


var sensorObj = require('jsupm_hwxpxx');


/************** Main code **************/

var defaultDev = "/dev/ttyUSB0";

// if an argument was specified, use it as the device instead
if (process.argv.length > 2)
{
    defaultDev = process.argv[2];
}

console.log("Using device " + defaultDev);
console.log("Initializing...");

// Instantiate an HWXPXX instance, using MODBUS slave address 3, and
// default comm parameters (19200, 8, N, 2)
var sensor = new sensorObj.HWXPXX(defaultDev, 3);

// output the Slave ID (manufacturer, model, serno)
console.log("Slave ID:", sensor.getSlaveID());

// stored temperature and humidity offsets
console.log("Temperature Offset:", sensor.getTemperatureOffset());
console.log("Humidity Offset:", sensor.getHumidityOffset());

console.log("");

// update and print available values every second
setInterval(function()
{
    // update our values from the sensor
    sensor.update();

    // we show both C and F for temperature
    console.log("Temperature:", sensor.getTemperature(),
                "C /", sensor.getTemperature(true), "F");

    console.log("Humidity:", sensor.getHumidity(), "%");

    console.log("Slider:", sensor.getSlider(), "%");

    console.log("Override Switch Status:", sensor.getOverrideSwitchStatus());

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

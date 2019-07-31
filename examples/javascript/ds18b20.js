/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016-2017 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */


var sensorObj = require('jsupm_ds18b20');


/************** Main code **************/

console.log("Initializing...");

// Instantiate an DS18B20 instance using the default values (uart 0)
var sensor = new sensorObj.DS18B20(0);

console.log("Found", sensor.devicesFound(), "device(s)");
console.log("");

if (!sensor.devicesFound())
{
    process.exit(1);
}

// update and print available values every 2 seconds
setInterval(function()
{
    // update our values for all of the detected sensors
    sensor.update(-1);

    // we show both C and F for temperature for the sensors
    for (var i=0; i<sensor.devicesFound(); i++)
    {
        console.log("Device:", i, "Temperature:", sensor.getTemperature(i),
                    "C /", sensor.getTemperature(i, true), "F");
    }

    console.log();
}, 2000);


process.on('SIGINT', function()
{
    sensor = null;
    sensorObj.cleanUp();
    sensorObj = null;
    console.log("Exiting...");
    process.exit(0);
});

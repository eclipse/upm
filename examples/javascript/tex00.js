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


var sensorObj = require('jsupm_tex00');


/************** Main code **************/

console.log("Initializing...");

// Instantiate an TEX00 instance, using A0 for the analog input.  In
// this example, we are using a 10K Ohm balance resistor and a TED
// (10k type 2) thermistor.
var sensor = new sensorObj.TEX00(0, 10000, 
                                 sensorObj.TEX00.STYPE_THERMISTOR_TED);

console.log("Minimum temperature:", sensor.getTemperatureRangeMin(),
            "C");
console.log("Maximum temperature:", sensor.getTemperatureRangeMax(),
            "C");
console.log("");

// update and print available values every second
setInterval(function()
{
    // update our values from the sensor
    sensor.update();

    if (sensor.isOutOfRange())
    {
        console.log("Temperature out of range");
    }
    else
    {
        // we show both C and F for temperature
        console.log("Temperature:", sensor.getTemperature(),
                    "C /", sensor.getTemperature(true), "F");
    }

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

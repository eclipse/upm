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


var sensorObj = require('jsupm_teams');


/************** Main code **************/

console.log("Initializing...");

// Instantiate an TEAMS instance, using A0 for temperature, and
// 165.0 ohms for the rResistor value (for the libelium 4-20ma
// interface)
var sensor = new sensorObj.TEAMS(0, 165.0);

// update and print available values every second
setInterval(function()
{
    // update our values from the sensor
    sensor.update();

    // is the sensor connected? (current >= 4ma)
    console.log("Is Connected:", sensor.isConnected());
    
    // print computed current on the loop.  This includes the offset,
    // if one was set by setOffsetMilliamps().
    console.log("Milliamps:", sensor.getRawMilliamps());

    // we show both C and F for temperature
    console.log("Temperature:", sensor.getTemperature(),
                "C /", sensor.getTemperature(true), "F");

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

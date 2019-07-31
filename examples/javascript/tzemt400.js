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


var sensorObj = require('jsupm_ozw');

/************** Main code **************/
var defaultDev = "/dev/ttyACM0";

// if an argument was specified, use it as the device instead
if (process.argv.length > 2)
{
    defaultDev = process.argv[2];
}
console.log("Using device " + defaultDev);

// Instantiate a TZEMT400 instance, on device node 13.  You will
// almost certainly need to change this to reflect your own network.
// Use the ozwdump example to see what nodes are available.
var sensor = new sensorObj.TZEMT400(13);

// The first thing to do is create options, then lock them when done.
sensor.optionsCreate();
sensor.optionsLock();

// Next, initialize it.
console.log("Initializing, this may take awhile depending on your ZWave network");

sensor.init(defaultDev);
console.log("Initialization complete");

console.log("Querying data...");

setInterval(function()
{
    sensor.update();

    console.log("Temperature: "
                + sensor.getTemperature()
                + " C / "
                + sensor.getTemperature(true)
                + " F");

    console.log("Mode: "
                + sensor.getMode());

    console.log("Operating State: "
                + sensor.getOperatingState());

    console.log("Heating Point: "
                + sensor.getHeatingPointTemperature()
                + " C / "
                + sensor.getHeatingPointTemperature(true)
                + " F");

    console.log("Cooling Point: "
                + sensor.getCoolingPointTemperature()
                + " C / "
                + sensor.getCoolingPointTemperature(true)
                + " F");

    console.log("Fan Mode: "
                + sensor.getFanMode());

    console.log("Fan State: "
                + sensor.getFanState());

    console.log();
}, 5000);


// exit on ^C
process.on('SIGINT', function()
{
    sensor = null;
    sensorObj.cleanUp();
    sensorObj = null;
    console.log("Exiting.");
    process.exit(0);
});

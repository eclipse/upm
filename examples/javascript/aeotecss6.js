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

function sleepFor(millis)
{
    var now = new Date().getTime();
    while(new Date().getTime() < now + millis)
    {
        /* do nothing */
    }
}

/************** Main code **************/
var defaultDev = "/dev/ttyACM0";

// if an argument was specified, use it as the device instead
if (process.argv.length > 2)
{
    defaultDev = process.argv[2];
}
console.log("Using device " + defaultDev);

// Instantiate an Aeotec Smart Switch 6 instance, on device node 11.
// You will almost certainly need to change this to reflect your own
// network.  Use the ozwdump example to see what nodes are available.
var sensor = new sensorObj.AeotecSS6(11);

// The first thing to do is create options, then lock them when done.
sensor.optionsCreate();
sensor.optionsLock();

// Next, initialize it.
console.log("Initializing, this may take awhile depending on your ZWave network");

sensor.init(defaultDev);
console.log("Initialization complete");

// turn light on
console.log("Turning switch on, then sleeping for 5 secs");
sensor.on();
sleepFor(5000);

console.log("Querying data...");

setInterval(function()
{
    sensor.update();

    console.log("Switch status: "
                + sensor.isOn());

    console.log("Volts: "
                + sensor.getVolts()
                + " volts");

    console.log("Energy Consumption: "
                + sensor.getEnergy()
                + " kWh");

    console.log("Watts: "
                + sensor.getWatts());

    console.log("Current: "
                + sensor.getCurrent()
                + " amps");

    console.log();
}, 3000);


// exit on ^C
process.on('SIGINT', function()
{
    console.log("Turning switch off and sleeping for 5 seconds...");
    sensor.off();
    sleepFor(5000);

    sensor = null;
    sensorObj.cleanUp();
    sensorObj = null;
    console.log("Exiting.");
    process.exit(0);
});

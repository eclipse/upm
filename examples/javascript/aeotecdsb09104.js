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

// Instantiate an Aeotec DSB09104 instance, on device node 12.  You
// will almost certainly need to change this to reflect your own
// network.  Use the ozwdump example to see what nodes are
// available.
var sensor = new sensorObj.AeotecDSB09104(12);

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

    console.log("Watts, Channel 1: "
                + sensor.getWattsC1()
                + " W");

    console.log("Watts, Channel 2: "
                + sensor.getWattsC2()
                + " W");

    console.log("Watts, Channel 3: "
                + sensor.getWattsC3()
                + " W");

    console.log("Energy, Channel 1: "
                + sensor.getEnergyC1()
                + " kWh");

    console.log("Energy, Channel 2: "
                + sensor.getEnergyC2()
                + " kWh");

    console.log("Energy, Channel 3: "
                + sensor.getEnergyC3()
                + " kWh");

    console.log("Battery Level: "
                + sensor.getBatteryLevel()
                + "%");

    console.log();
}, 3000);


// exit on ^C
process.on('SIGINT', function()
{
    sensor = null;
    sensorObj.cleanUp();
    sensorObj = null;
    console.log("Exiting.");
    process.exit(0);
});

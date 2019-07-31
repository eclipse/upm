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


var sensorObj = require('jsupm_e50hx');


/************** Main code **************/

// You will need to edit this example to conform to your site and your
// devices, specifically the Device Object Instance ID passed to the
// constructor, and the arguments to initMaster() that are
// appropriate for your BACnet network.

var defaultDev = "/dev/ttyUSB0";

// if an argument was specified, use it as the device instead
if (process.argv.length > 2)
{
    defaultDev = process.argv[2];
}

console.log("Using device " + defaultDev);
console.log("Initializing...");

// Instantiate an E50HX object for an E50HX device that has 1075425
// as it's unique Device Object Instance ID.  NOTE: You will
// certainly want to change this to the correct value for your
// device(s).
var sensor = new sensorObj.E50HX(1075425);

// Initialize our BACnet master, if it has not already been
// initialized, with the device and baudrate, choosing 1000001 as
// our unique Device Object Instance ID, 2 as our MAC address and
// using default values for maxMaster and maxInfoFrames
sensor.initMaster(defaultDev, 38400, 1000001, 2);

// Uncomment to enable debugging output
// sensor.setDebug(true);

console.log("");
console.log("Device Description:", sensor.getDeviceDescription());
console.log("Device Location:", sensor.getDeviceLocation());
console.log("");

// update and print a few values every 5 seconds
setInterval(function()
{
    console.log("System Voltage:",
                sensor.getAnalogValue(sensorObj.E50HX.AV_System_Voltage),
                " ",
                sensor.getAnalogValueUnits(sensorObj.E50HX.AV_System_Voltage));

    console.log("System Type:",
                sensor.getAnalogValue(sensorObj.E50HX.AV_System_Type));

    console.log("Energy Consumption:",
                sensor.getAnalogInput(sensorObj.E50HX.AI_Energy),
                " ",
                sensor.getAnalogInputUnits(sensorObj.E50HX.AI_Energy));

    console.log("Power Up Counter:",
                sensor.getAnalogInput(sensorObj.E50HX.AI_Power_Up_Count));

    console.log("");

}, 5000);


process.on('SIGINT', function()
{
    sensor = null;
    sensorObj.cleanUp();
    sensorObj = null;
    console.log("Exiting...");
    process.exit(0);
});

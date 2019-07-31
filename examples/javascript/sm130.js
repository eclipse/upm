/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */


var sensorObj = require('jsupm_sm130');

// Instantiate a UART based SM130 RFID Module using defaults
var sensor = new sensorObj.SM130();

// Set the baud rate, 19200 baud is the default.
if (sensor.setBaudRate(19200))
{
    console.log("Failed to set baud rate");
    process.exit(0);
}

console.log("Resetting...");
sensor.reset();

console.log("Firmware revision: " + sensor.getFirmwareVersion());

console.log("Waiting up to 5 seconds for a tag...");

if (sensor.waitForTag(5000))
{
    console.log("Found tag, UID: " +
                sensor.string2HexString(sensor.getUID()));
    console.log("Tag Type: " +
                sensor.tag2String(sensor.getTagType()));
}
else
{
    // error
    console.log("waitForTag failed: " +
                sensor.getLastErrorString());
}

/************** Exit code **************/
process.on('SIGINT', function()
{
    sensor = null;
    sensorObj.cleanUp();
    sensorObj = null;
    console.log("Exiting...");
    process.exit(0);
});

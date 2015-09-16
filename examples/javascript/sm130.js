/*jslint node:true, vars:true, bitwise:true, unparam:true */
/*jshint unused:true */

/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
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

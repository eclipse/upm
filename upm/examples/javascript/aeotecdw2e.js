/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
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


var sensorObj = require('jsupm_ozw');

/************** Main code **************/
var defaultDev = "/dev/ttyACM0";

// if an argument was specified, use it as the device instead
if (process.argv.length > 2)
{
    defaultDev = process.argv[2];
}
console.log("Using device " + defaultDev);

// Instantiate an Aeotec Door/Window 2nd Edition sensor instance, on
// device node 10.  You will almost certainly need to change this to
// reflect your own network.  Use the ozwdump example to see what
// nodes are available.
var sensor = new sensorObj.AeotecDW2E(10);

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
    if (sensor.isDeviceAvailable())
    {
        console.log("Alarm status: "
                    + sensor.isAlarmTripped());

        console.log("Tamper Switch status: "
                    + sensor.isTamperTripped());

        console.log("Battery Level: "
                    + sensor.getBatteryLevel()
                    + "%");

        console.log();
    }
    else
    {
        console.log("Device has not yet responded to probe.");
        console.log("Try waking it, or wait until it wakes itself if "
                    + "configured to do so.");

        console.log();
    }

}, 1000);


// exit on ^C
process.on('SIGINT', function()
{
    sensor = null;
    sensorObj.cleanUp();
    sensorObj = null;
    console.log("Exiting.");
    process.exit(0);
});

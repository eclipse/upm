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

// Instantiate an Aeotec Smart Dimmer Gen2 instance, on device node
// 9.  You will almost certainly need to change this to reflect your
// own network.  Use the ozwdump example to see what nodes are
// available.
var sensor = new sensorObj.AeotecSDG2(9);

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
var dim = false;
setInterval(function()
{
    // put on a light show...
    if (dim)
        sensor.setLevel(25);
    else
        sensor.on();

    dim = !dim;

    sensor.update();

    console.log("Current Level: "
                + sensor.getLevel());

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
}, 5000);


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

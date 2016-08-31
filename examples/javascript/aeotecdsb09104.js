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

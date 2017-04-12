/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016-2017 Intel Corporation.
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


var sensorObj = require('jsupm_ds18b20');


/************** Main code **************/

console.log("Initializing...");

// Instantiate an DS18B20 instance using the default values (uart 0)
var sensor = new sensorObj.DS18B20(0);

console.log("Found", sensor.devicesFound(), "device(s)");
console.log("");

if (!sensor.devicesFound())
{
    process.exit(1);
}

// update and print available values every 2 seconds
setInterval(function()
{
    // update our values for all of the detected sensors
    sensor.update(-1);

    // we show both C and F for temperature for the sensors
    for (var i=0; i<sensor.devicesFound(); i++)
    {
        console.log("Device:", i, "Temperature:", sensor.getTemperature(i),
                    "C /", sensor.getTemperature(i, true), "F");
    }

    console.log();
}, 2000);


process.on('SIGINT', function()
{
    sensor = null;
    sensorObj.cleanUp();
    sensorObj = null;
    console.log("Exiting...");
    process.exit(0);
});

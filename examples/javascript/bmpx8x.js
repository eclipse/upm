/*
* Author: Zion Orent <zorent@ics.com>
* Author: Jon Trulson <jtrulson@ics.com>
* Copyright (c) 2014-2017 Intel Corporation.
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

var sensorObj = require('jsupm_bmpx8x');

// Instantiate a BMPX8X sensor on I2C using defaults.
var sensor = new sensorObj.BMPX8X();

// Print the pressure, altitude, sea level, and
// temperature values every 0.5 seconds
setInterval(function()
{
    sensor.update();

    console.log("Pressure: "
                + sensor.getPressure()
                + " Pa, Temperature: "
                + sensor.getTemperature()
                + " C, Altitude: "
                + sensor.getAltitude()
                + " m, Sea level: "
                + sensor.getSealevelPressure()
                + " Pa");
}, 500);

// exit on ^C
process.on('SIGINT', function()
{
    sensor = null;
    sensorObj.cleanUp();
    sensorObj = null;
    console.log("Exiting.");
    process.exit(0);
});

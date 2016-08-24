/*jslint node:true, vars:true, bitwise:true, unparam:true */
/*jshint unused:true */

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

var sensorObj = require('jsupm_mma7361');

// Instantiate a MMA7361 sensor on analog pins A0 (X), A1 (Y) A2
// (Z), selftest pin on D2, sleep pin on D3 nd an analog reference
// value of 5.0.  The freefall pin and the range pin are unused
// (-1).
var sensor = new sensorObj.MMA7361(0, 1, 2, 2, 3, -1, -1, 5.0);

var x = new sensorObj.new_floatp();
var y = new sensorObj.new_floatp();
var z = new sensorObj.new_floatp();

// 1.5g (true = 6g)
sensor.setRange(false);

// Every 10th of a second, update and print values
setInterval(function()
{
    // update our values from the sensor
    sensor.update();

    sensor.getAcceleration(x, y, z);
    console.log("Accelerometer x: "
                + sensorObj.floatp_value(x)
                + " y: " + sensorObj.floatp_value(y)
                + " z: " + sensorObj.floatp_value(z));

    sensor.getVolts(x, y, z);
    console.log("Volts x: "
                + sensorObj.floatp_value(x)
                + " y: " + sensorObj.floatp_value(y)
                + " z: " + sensorObj.floatp_value(z));

    console.log();

}, 100);

// exit on ^C
process.on('SIGINT', function()
{
    sensor = null;
    sensorObj.cleanUp();
    sensorObj = null;
    console.log("Exiting.");
    process.exit(0);
});

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

var sensorObj = require('jsupm_bmx055');

// Instantiate a BMX055 instance using default i2c bus and address
var sensor = new sensorObj.BMX055();

var x = new sensorObj.new_floatp();
var y = new sensorObj.new_floatp();
var z = new sensorObj.new_floatp();

// now output data every 250 milliseconds
setInterval(function()
{
    // update our values from the sensor
    sensor.update();

    sensor.getAccelerometer(x, y, z);
    console.log("Accelerometer x: "
                + sensorObj.floatp_value(x)
                + " y: " + sensorObj.floatp_value(y)
                + " z: " + sensorObj.floatp_value(z)
                + " g");

    sensor.getGyroscope(x, y, z);
    console.log("Gyroscope x: "
                + sensorObj.floatp_value(x)
                + " y: " + sensorObj.floatp_value(y)
                + " z: " + sensorObj.floatp_value(z)
                + " degrees/s");

    sensor.getMagnetometer(x, y, z);
    console.log("Magnetometer x: "
                + sensorObj.floatp_value(x)
                + " y: " + sensorObj.floatp_value(y)
                + " z: " + sensorObj.floatp_value(z)
                + " uT");

    console.log();

}, 250);

// exit on ^C
process.on('SIGINT', function()
{
    sensor = null;
    sensorObj.cleanUp();
    sensorObj = null;
    console.log("Exiting.");
    process.exit(0);
});

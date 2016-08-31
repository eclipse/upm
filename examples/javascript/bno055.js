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


var sensorObj = require('jsupm_bno055');

// Instantiate an BNO055 using default parameters (bus 0, addr
// 0x28).  The default running mode is NDOF absolute orientation
// mode.
var sensor = new sensorObj.BNO055();

var mag = new sensorObj.new_intp();
var acc = new sensorObj.new_intp();
var gyr = new sensorObj.new_intp();
var syst = new sensorObj.new_intp();

var w = new sensorObj.new_floatp();
var x = new sensorObj.new_floatp();
var y = new sensorObj.new_floatp();
var z = new sensorObj.new_floatp();

console.log("First we need to calibrate.  4 numbers will be output every");
console.log("second for each sensor.  0 means uncalibrated, and 3 means");
console.log("fully calibrated.");
console.log("See the UPM documentation on this sensor for instructions on");
console.log("what actions are required to calibrate.");
console.log("");

// do the calibration...
var calInterval = setInterval(function()
{
    if (sensor.isFullyCalibrated())
    {
        clearInterval(calInterval);
        console.log("");
        console.log("Calibration complete.");
        console.log("");

        setInterval(outputData, 250)
    }
    else
    {
        sensor.getCalibrationStatus(mag, acc, gyr, syst);
        console.log("Magnetometer: " + sensorObj.intp_value(mag) +
                    " Accelerometer: " + sensorObj.intp_value(acc) +
                    " Gyroscope: " + sensorObj.intp_value(gyr) +
                    " System: " + sensorObj.intp_value(syst));
    }

}, 1000);


// now output various fusion data every 250 milliseconds
function outputData()
{
    sensor.update();

    sensor.getEulerAngles(x, y, z);
    console.log("Euler: Heading: " + sensorObj.floatp_value(x) +
                " Roll: " + sensorObj.floatp_value(y) +
                " Pitch: " + sensorObj.floatp_value(z) +
                " degrees");

    sensor.getQuaternions(w, x, y, z);
    console.log("Quaternion: W: " + sensorObj.floatp_value(w) +
                " X:" + sensorObj.floatp_value(x) +
                " Y: " + sensorObj.floatp_value(y) +
                " Z: " + sensorObj.floatp_value(z));

    sensor.getLinearAcceleration(x, y, z);
    console.log("Linear Acceleration: X: " + sensorObj.floatp_value(x) +
                " Y: " + sensorObj.floatp_value(y) +
                " Z: " + sensorObj.floatp_value(z) +
                " m/s^2");

    sensor.getGravityVectors(x, y, z);
    console.log("Gravity Vector: X: " + sensorObj.floatp_value(x) +
                " Y: " + sensorObj.floatp_value(y) +
                " Z: " + sensorObj.floatp_value(z) +
                " m/s^2");

    console.log("");
};

// exit on ^C
process.on('SIGINT', function()
{
    sensor = null;
    sensorObj.cleanUp();
    sensorObj = null;
    console.log("Exiting.");
    process.exit(0);
});

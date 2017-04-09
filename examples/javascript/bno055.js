/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016-2017 Intel Corporation.
 *
 * The MIT License
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
        var intData = sensor.getCalibrationStatus();
        console.log("Magnetometer: " + intData.get(0) +
                    " Accelerometer: " + intData.get(1) +
                    " Gyroscope: " + intData.get(2) +
                    " System: " + intData.get(3));
    }

}, 1000);


// now output various fusion data every 250 milliseconds
function outputData()
{
    sensor.update();

    var floatData = sensor.getEulerAngles();
    console.log("Euler: Heading: " + floatData.get(0)
                + " Roll: " + floatData.get(1)
                + " Pitch: " + floatData.get(2)
                + " degrees");

    floatData = sensor.getQuaternions();
    console.log("Quaternion: W: " + floatData.get(0)
                + " X:" + floatData.get(1)
                + " Y: " + floatData.get(2)
                + " Z: " + floatData.get(3));

    floatData = sensor.getLinearAcceleration();
    console.log("Linear Acceleration: X: " + floatData.get(0)
                + " Y: " + floatData.get(1)
                + " Z: " + floatData.get(2)
                + " m/s^2");

    floatData = sensor.getGravityVectors();
    console.log("Gravity Vector: X: " + floatData.get(0)
                + " Y: " + floatData.get(1)
                + " Z: " + floatData.get(2)
                + " m/s^2");

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

/*jslint node:true, vars:true, bitwise:true, unparam:true */
/*jshint unused:true */
/*global */
/*
* Author: Sarah Knepper <sarah.knepper@intel.com>
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

// Load gyroscope module
var itg3200 = require('jsupm_itg3200');
// load this on i2c
var gyro = new itg3200.Itg3200(0);

var rot;
var ang;

// Note: Sensor not supported on Intel Edison with Arduino breakout
setInterval(function()
{
    gyro.update(); // Update the data
    rot = gyro.getRawValues(); // Read raw sensor data
    ang = gyro.getRotation(); // Read rotational speed (deg/sec)
    var rots = (rot.getitem(0) + " ");
    rots += (rot.getitem(1) + " ");
    rots += rot.getitem(2);
    var temp = round2Digits(gyro.getTemperature());
    var raw = round2Digits(gyro.getRawTemp());
    console.log("Raw: " + rots);
    console.log("AngX: " + round2Digits(ang.getitem(0)));
    console.log("AngY: " + round2Digits(ang.getitem(1)));
    console.log("AngZ: " + round2Digits(ang.getitem(2)));
    console.log("Temp: " + temp + " Raw: " + raw);
}, 1000);

function round2Digits(num)
{
    return Math.round((num + 0.00001) * 100) / 100;
}

// Print message when exiting
process.on('SIGINT', function()
{
    console.log("Exiting...");
    process.exit(0);
});

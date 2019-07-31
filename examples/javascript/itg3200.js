/*
* Author: Sarah Knepper <sarah.knepper@intel.com>
* Copyright (c) 2015 Intel Corporation.
*
* This program and the accompanying materials are made available under the
* terms of the The MIT License which is available at
* https://opensource.org/licenses/MIT.
*
* SPDX-License-Identifier: MIT
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

/*jslint node:true, vars:true, bitwise:true, unparam:true */
/*jshint unused:true */

/*
 * Author: Jon Trulson <jtrulson@ics.com>
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


var sensorObj = require('jsupm_mpu9150');

// Instantiate an MPU9105 on default I2C bus and address
var sensor = new sensorObj.MPU9150();

// Initialize the device with default values
sensor.init();

var x = new sensorObj.new_floatp();
var y = new sensorObj.new_floatp();
var z = new sensorObj.new_floatp();

// Output data every half second until interrupted
setInterval(function()
{
    sensor.update();
    
    sensor.getAccelerometer(x, y, z);
    console.log("Accelerometer: AX: " + sensorObj.floatp_value(x) + 
                " AY: " + sensorObj.floatp_value(y) + 
                " AZ: " + sensorObj.floatp_value(z));

    sensor.getGyroscope(x, y, z);
    console.log("Gyroscope:     GX: " + sensorObj.floatp_value(x) + 
                " AY: " + sensorObj.floatp_value(y) + 
                " AZ: " + sensorObj.floatp_value(z));
 
    sensor.getMagnetometer(x, y, z);
    console.log("Magnetometer:  MX: " + sensorObj.floatp_value(x) + 
                " MY: " + sensorObj.floatp_value(y) + 
                " MZ: " + sensorObj.floatp_value(z));

    console.log("Temperature:   " + sensor.getTemperature());
 
    console.log();

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


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

var sensorObj = require('jsupm_dfrorp');

// Instantiate a DFRobot ORP sensor on analog pin A0 with an analog
// reference voltage of 5.0.
var sensor = new sensorObj.DFRORP(0, 5.0);

// To calibrate:
//
// Disconnect the sensor probe (but leave the sensor interface board
// connected).  Then run one of the examples while holding down the
// 'calibrate' button on the device.  Read the ORP value reported
// (it should be fairly small).
//
// This value is what you should supply to setCalibrationOffset().
// Then reconnect the probe to the interface board and you should be
// ready to go.
//
// DO NOT press the calibrate button on the interface board while
// the probe is attached or you can permanently damage the probe.
sensor.setCalibrationOffset(0.97);

// Every second, update and print values
setInterval(function()
{
    sensor.update();

    console.log("ORP: "
                + sensor.getORP()
                + " mV");

    console.log();

}, 1000);

// exit on ^C
process.on('SIGINT', function()
{
    sensor = null;
    sensorObj.cleanUp();
    sensorObj = null;
    console.log("Exiting.");
    process.exit(0);
});

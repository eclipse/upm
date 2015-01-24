/*jslint node:true, vars:true, bitwise:true, unparam:true */
/*jshint unused:true */
/*global */
/*
 * Author: Zion Orent <zorent@ics.com>
 * Copyright (c) 2014 Intel Corporation.
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

//Load servo module.
var servoModule = require("jsupm_servo");

//Instantiate ES08A Servo module on GPIO 5
var servo = new servoModule.ES08A(5);

// function to initialize servo
    // timeOffset: how long after hitting "run"
    //		should we start this servo instance
    // timeInterval: how frequently should this instance run after timeOffset
    // angle: the angle for this instance
function startServo(timeOffset, timeInterval, angle)
{
    // Start running this instance after timeOffset milliseconds
    setTimeout(function()
    {
        // run this instance every timeInterval milliseconds
        setInterval(function()
        {
            servo.setAngle(angle);
            console.log("Set angle to " + angle);
        }, timeInterval);
    }, timeOffset);
    // timeOffset tells setTimeout when
    //		to execute the function specified in the first param
    // angle is passed as a param to the specified function
}
// start immediately, run every 3 seconds, go 0 degrees
startServo(0, 3000, 0);

// start in 1 second, run every 3 seconds, go 90 degrees
startServo(1000, 3000, 90);

// start in 2 seconds, run every 3 seconds, go 180 degrees
startServo(2000, 3000, 180);

// Print message when exiting
process.on('SIGINT', function()
{
	console.log("Exiting...");
	process.exit(0);
});

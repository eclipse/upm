/*
 * Author: Zion Orent <zorent@ics.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
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

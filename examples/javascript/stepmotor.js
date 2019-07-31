/*
* Author:  Mihai Tudor Panu <mihai.tudor.panu@intel.com>
* Copyright (c) 2015 Intel Corporation.
*
* This program and the accompanying materials are made available under the
* terms of the The MIT License which is available at
* https://opensource.org/licenses/MIT.
*
* SPDX-License-Identifier: MIT
*/

var lib = require('jsupm_stepmotor');

// Instantiate a stepper motor driver on pins 2 (direction) and 3 (step)
var stepper = new lib.StepMotor(2, 3);

function delay(milliseconds) {
    var startTime = Date.now();
    while (Date.now() - startTime < milliseconds);
}

stepper.quit = function()
{
    stepper = null;
    lib.cleanUp();
    lib = null;
    console.log("Exiting");
    process.exit(0);
};

// The driver is synchronous and blocking
console.log("Rotating 1 revolution forward and back at 60 rpm.");
stepper.setSpeed(60);
stepper.stepForward(200);
delay(1000);
stepper.stepBackward(200);
delay(1000);

console.log("Rotating 1 revolution forward and back at 150 rpm.");
stepper.setSpeed(150);
stepper.stepForward(200);
delay(1000);
stepper.stepBackward(200);
delay(1000);

console.log("Rotating 1 revolution forward and back at 300 rpm.");
stepper.setSpeed(300);
stepper.stepForward(200);
delay(1000);
stepper.stepBackward(200);
delay(1000);

stepper.quit();

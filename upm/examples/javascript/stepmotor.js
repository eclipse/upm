/*
* Author:  Mihai Tudor Panu <mihai.tudor.panu@intel.com>
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

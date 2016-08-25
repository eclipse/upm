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

var grovemdObj = require('jsupm_grovemd');

// This example demonstrates using the GroveMD to drive a stepper motor

function start()
{
    if (motor)
    {
        // configure it, for this example, we'll assume 200 steps per rev
        motor.configStepper(200);
        motor.setStepperSteps(100);
        // start it going at 10 RPM
        motor.enableStepper(grovemdObj.GroveMD.STEP_DIR_CW, 10);
    }
}

function reverse()
{
    if (motor)
    {
	// now reverse
        motor.setStepperSteps(100);
        // start it going at 10 RPM
        motor.enableStepper(grovemdObj.GroveMD.STEP_DIR_CCW, 10);
	}
}

function end()
{
    if (motor)
    {
	motor.disableStepper();
    }
    exit();
}

// When exiting: clear memory and print exit message
function exit()
{
    if (motor)
    {
	motor = null;
	grovemdObj.cleanUp();
    }
    grovemdObj = null;
    console.log("Exiting");
    process.exit(0);	
}


// Instantiate an I2C Grove Motor Driver on I2C bus 0
var motor = new grovemdObj.GroveMD(
    grovemdObj.GROVEMD_I2C_BUS, 
    grovemdObj.GROVEMD_DEFAULT_I2C_ADDR);

start();

setTimeout(function()
{
    reverse();
    setTimeout(end, 3000);
}, 3000);


process.on('SIGINT', function()
{
    exit();
});

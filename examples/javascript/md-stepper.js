/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

var mdObj = require('jsupm_md');

// This example demonstrates using the MD to drive a stepper motor

function start()
{
    if (motor)
    {
        // configure it, for this example, we'll assume 200 steps per rev
        motor.configStepper(200);
        motor.setStepperSteps(100);
        // start it going at 10 RPM
        motor.enableStepper(mdObj.MD.STEP_DIR_CW, 10);
    }
}

function reverse()
{
    if (motor)
    {
	// now reverse
        motor.setStepperSteps(100);
        // start it going at 10 RPM
        motor.enableStepper(mdObj.MD.STEP_DIR_CCW, 10);
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
	mdObj.cleanUp();
    }
    mdObj = null;
    console.log("Exiting");
    process.exit(0);	
}


// Instantiate an I2C Motor Driver on I2C bus 0
var motor = new mdObj.MD(
    mdObj.MD_I2C_BUS, 
    mdObj.MD_DEFAULT_I2C_ADDR);

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

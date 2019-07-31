/*
* Author: Zion Orent <zorent@ics.com>
* Copyright (c) 2015 Intel Corporation.
*
* This program and the accompanying materials are made available under the
* terms of the The MIT License which is available at
* https://opensource.org/licenses/MIT.
*
* SPDX-License-Identifier: MIT
*/

function exit()
{
	console.log("Exiting");

	myMotorShield_obj = null;
	if (MotorShield_lib)
	{
		MotorShield_lib.cleanUp();
		MotorShield_lib = null;
	}
	process.exit(0);
}

var MotorShield_lib = require('jsupm_adafruitms1438');

/* Import header values */
var I2CBus = MotorShield_lib.ADAFRUITMS1438_I2C_BUS;
var I2CAddr = MotorShield_lib.ADAFRUITMS1438_DEFAULT_I2C_ADDR;

var M12motor = MotorShield_lib.AdafruitMS1438.STEPMOTOR_M12;
var MotorDirCW = MotorShield_lib.AdafruitMS1438.DIR_CW;
var MotorDirCCW = MotorShield_lib.AdafruitMS1438.DIR_CCW;


// Instantiate an Adafruit MS 1438 on I2C bus 0
var myMotorShield_obj = new MotorShield_lib.AdafruitMS1438(I2CBus, I2CAddr);


// Setup for use with a stepper motor connected to the M1 & M2 ports

// disable first, to be safe
myMotorShield_obj.disableStepper(M12motor);

// configure for a NEMA-17, 200 steps per revolution
myMotorShield_obj.stepConfig(M12motor, 200);

// set speed at 10 RPM's
myMotorShield_obj.setStepperSpeed(M12motor, 10);
myMotorShield_obj.setStepperDirection(M12motor, MotorDirCW);

console.log("Enabling...");
myMotorShield_obj.enableStepper(M12motor);

console.log("Rotating 1 full revolution at 10 RPM speed.");
myMotorShield_obj.stepperSteps(M12motor, 200);

console.log("Sleeping for 2 seconds...");


setTimeout(function()
{
	console.log("Rotating 1/2 revolution in opposite direction at 10 RPM speed.");

	myMotorShield_obj.setStepperDirection(M12motor, MotorDirCCW);
	myMotorShield_obj.stepperSteps(M12motor, 100);

	console.log("Disabling...");
	myMotorShield_obj.disableStepper(M12motor);
	exit();
}, 2000);


process.on('SIGINT', function()
{
	exit();
});

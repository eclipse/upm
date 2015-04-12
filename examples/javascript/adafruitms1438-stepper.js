/*jslint node:true, vars:true, bitwise:true, unparam:true */
/*jshint unused:true */
/*
* Author: Zion Orent <zorent@ics.com>
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

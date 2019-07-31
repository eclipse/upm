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

var M3motor = MotorShield_lib.AdafruitMS1438.MOTOR_M3;
var MotorDirCW = MotorShield_lib.AdafruitMS1438.DIR_CW;
var MotorDirCCW = MotorShield_lib.AdafruitMS1438.DIR_CCW;


// Instantiate an Adafruit MS 1438 on I2C bus 0
var myMotorShield_obj = new MotorShield_lib.AdafruitMS1438(I2CBus, I2CAddr);


// Setup for use with a DC motor connected to the M3 port

// set a PWM period of 50Hz
myMotorShield_obj.setPWMPeriod(50);

// disable first, to be safe
myMotorShield_obj.disableMotor(M3motor);

// set speed at 50%
myMotorShield_obj.setMotorSpeed(M3motor, 50);
myMotorShield_obj.setMotorDirection(M3motor, MotorDirCW);

process.stdout.write("Spin M3 at half speed for 3 seconds, ");
console.log("then reverse for 3 seconds.");
myMotorShield_obj.enableMotor(M3motor);

setTimeout(function()
{
	console.log("Reversing M3");
	myMotorShield_obj.setMotorDirection(M3motor, MotorDirCCW);
}, 3000);


setTimeout(function()
{
	console.log("Stopping M3");
	myMotorShield_obj.disableMotor(M3motor);
	exit();
}, 6000);

process.on('SIGINT', function()
{
	exit();
});

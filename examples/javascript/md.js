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

var MotorDriver_lib = require('jsupm_md');

function start()
{
	if (my_MotorDriver_obj)
	{
		// set direction to CW and set speed to 50%
		console.log("Spin M1 and M2 at half speed for 3 seconds");
		my_MotorDriver_obj.setMotorDirections(MotorDriver_lib.MD.DIR_CW,
                                                     MotorDriver_lib.MD.DIR_CW);
		my_MotorDriver_obj.setMotorSpeeds(127, 127);
	}
}

function reverse()
{
	if (my_MotorDriver_obj)
	{
		// counter clockwise
		console.log("Reversing M1 and M2 for 3 seconds");
		my_MotorDriver_obj.setMotorDirections(MotorDriver_lib.MD.DIR_CCW,
                                                     MotorDriver_lib.MD.DIR_CCW);	
	}
}

function end()
{
	if (my_MotorDriver_obj)
	{
		console.log("Stopping motors");
		my_MotorDriver_obj.setMotorSpeeds(0, 0);
	}
	exit();
}

// When exiting: clear memory and print exit message
function exit()
{
	if (my_MotorDriver_obj)
	{
		my_MotorDriver_obj = null;
		MotorDriver_lib.cleanUp();
	}
	MotorDriver_lib = null;
	console.log("Exiting");
	process.exit(0);	
}


// Instantiate an I2C Motor Driver on I2C bus 0
var my_MotorDriver_obj = new MotorDriver_lib.MD(
	MotorDriver_lib.MD_I2C_BUS, 
	MotorDriver_lib.MD_DEFAULT_I2C_ADDR);

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

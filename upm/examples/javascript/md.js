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

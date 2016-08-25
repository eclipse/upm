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

	if (myLEDController_obj)
	{
		// clear the bits we set earlier
		myLEDController_obj.ledFullOff(3, false);
		myLEDController_obj.ledFullOn(4, false);
	}
	myLEDController_obj = null;
	if (LEDController_lib)
	{
		LEDController_lib.cleanUp();
		LEDController_lib = null;
	}
	process.exit(0);
}

// The pca9685 is an led controller.
// It's being used in this case to drive motors.
var LEDController_lib = require('jsupm_pca9685');

var I2CBus = LEDController_lib.PCA9685_I2C_BUS;
var I2CAddr = LEDController_lib.PCA9685_DEFAULT_I2C_ADDR;
// Instantiate an PCA9685 on I2C bus 0
var myLEDController_obj = new LEDController_lib.PCA9685(I2CBus, I2CAddr);

// put device to sleep
myLEDController_obj.setModeSleep(true);

// setup a period of 50Hz
myLEDController_obj.setPrescaleFromHz(50);
  
// wake device up
myLEDController_obj.setModeSleep(false);

// Setup a 50% duty cycle -- on time at 0, off time at 2048 (4096 / 2)
// Set for all channels
var LEDNum = LEDController_lib.PCA9685_ALL_LED;
myLEDController_obj.ledOnTime(LEDNum, 0);
myLEDController_obj.ledOffTime(LEDNum, 2048);

// but, turn channel 3 full off and channel 4 full on
console.log("Turning channel 3 off, and channel 4 on.");
console.log("All other channels will be PWM'd at a 50% duty cycle.");

myLEDController_obj.ledFullOff(3, true);
myLEDController_obj.ledFullOn(4, true);

  // now, just sleep for 5 seconds, reset channels 3 and 4, and exit.
console.log("Sleeping for 5 seconds...");

setTimeout(exit, 5000);

process.on('SIGINT', function()
{
	exit();
});

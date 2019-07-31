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

var digitalAccelerometer = require('jsupm_mma7660');

// Instantiate an MMA7660 on I2C bus 0
var myDigitalAccelerometer = new digitalAccelerometer.MMA7660(
					digitalAccelerometer.MMA7660_DEFAULT_I2C_BUS,
					digitalAccelerometer.MMA7660_DEFAULT_I2C_ADDR);

// place device in standby mode so we can write registers
myDigitalAccelerometer.setModeStandby();

// enable 64 samples per second
myDigitalAccelerometer.setSampleRate(digitalAccelerometer.MMA7660_AUTOSLEEP_64);

// place device into active mode
myDigitalAccelerometer.setModeActive();

var ax, ay, az;
ax = digitalAccelerometer.new_floatp();
ay = digitalAccelerometer.new_floatp();
az = digitalAccelerometer.new_floatp();

var outputStr;

var myInterval = setInterval(function()
{
	myDigitalAccelerometer.getAcceleration(ax, ay, az);
	outputStr = "Acceleration: x = " 
		+ roundNum(digitalAccelerometer.floatp_value(ax), 6)
		+ "g y = " + roundNum(digitalAccelerometer.floatp_value(ay), 6) 
		+ "g z = " + roundNum(digitalAccelerometer.floatp_value(az), 6) + "g";
	console.log(outputStr);
	console.log();
}, 500);

// round off output to match C example, which has 6 decimal places
function roundNum(num, decimalPlaces)
{
	var extraNum = (1 / (Math.pow(10, decimalPlaces) * 1000));
	return (Math.round((num + extraNum) 
		* (Math.pow(10, decimalPlaces))) / Math.pow(10, decimalPlaces));
}

// When exiting: clear interval and print message
process.on('SIGINT', function()
{
	clearInterval(myInterval);

	// clean up memory
	digitalAccelerometer.delete_intp(x);
	digitalAccelerometer.delete_intp(y);
	digitalAccelerometer.delete_intp(z);

	digitalAccelerometer.delete_floatp(ax);
	digitalAccelerometer.delete_floatp(ay);
	digitalAccelerometer.delete_floatp(az);

	myDigitalAccelerometer.setModeStandby();

	console.log("Exiting...");
	process.exit(0);
});

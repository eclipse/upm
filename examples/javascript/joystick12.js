/*
* Author: Zion Orent <zorent@ics.com>
* Copyright (c) 2014 Intel Corporation.
*
* This program and the accompanying materials are made available under the
* terms of the The MIT License which is available at
* https://opensource.org/licenses/MIT.
*
* SPDX-License-Identifier: MIT
*/

var joystick = require('jsupm_joystick12');

// Instantiate a joystick on analog pins A0 and A1
var myJoystick = new joystick.Joystick12(0, 1);

// Print the X and Y input values every second
setInterval(function()
{
	var XString = "Driving X:" + roundNum(myJoystick.getXInput(), 6);
	var YString = ": and Y:" + roundNum(myJoystick.getYInput(), 6);
	console.log(XString + YString);
}, 1000);

function roundNum(num, decimalPlaces)
{
	var extraNum = (1 / (Math.pow(10, decimalPlaces) * 1000));
	var numerator = Math.round((num + extraNum) * (Math.pow(10, decimalPlaces)));
	var denominator = Math.pow(10, decimalPlaces);
	return (numerator / denominator);
}

// Print message when exiting
process.on('SIGINT', function()
{
	console.log("Exiting...");
	process.exit(0);
});

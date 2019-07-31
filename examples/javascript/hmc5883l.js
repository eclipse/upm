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

// Load Compass module
var hmc5883l = require('jsupm_hmc5883l');
// load this on i2c
var myCompass = new hmc5883l.Hmc5883l(0);
// Set your declination from true north in radians
myCompass.set_declination(0.2749);

var coordsObj;
// Print out the coordinates, heading, and direction every second
setInterval(function()
{
	myCompass.update(); // Update the coordinates
	coordsObj = myCompass.coordinates();
	var coords = (coordsObj.getitem(0) + " ");
	coords += (coordsObj.getitem(1) + " ");
	coords += coordsObj.getitem(2);
	var heading = round2Digits(myCompass.heading());
	var direction = round2Digits(myCompass.direction());
	var compassResults = "coords: " + coords + 
		"    heading: " + heading + " direction " + direction;
	console.log(compassResults);
}, 1000);

function round2Digits(num)
{
	return Math.round((num  + 0.00001) * 100) / 100;
}

// Print message when exiting
process.on('SIGINT', function()
{
	console.log("Exiting...");
	process.exit(0);
});

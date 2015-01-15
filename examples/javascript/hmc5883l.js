/*jslint node:true, vars:true, bitwise:true, unparam:true */
/*jshint unused:true */
/*global */
/*
* Author: Zion Orent <zorent@ics.com>
* Copyright (c) 2014 Intel Corporation.
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

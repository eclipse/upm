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

var nunchuck_lib = require('jsupm_nunchuck');

// Instantiate a nunchuck controller bus 0
var nunchuck_obj = new nunchuck_lib.NUNCHUCK(0);

// always do this first
console.log("Initializing... ");
if (!nunchuck_obj.init())
{
	console.log("nunchuck->init() failed.");
	process.exit(0);
}

setInterval(function()
{
	nunchuck_obj.update();

	var outputStr = "stickX: " + nunchuck_obj.stickX +
	", stickY: " + nunchuck_obj.stickY;
	console.log(outputStr);
	outputStr = "accelX: " + nunchuck_obj.accelX +
	", accelY: " + nunchuck_obj.accelY +
	", accelZ: " + nunchuck_obj.accelZ;
	console.log(outputStr);

	outputStr = "button C: " +
	((nunchuck_obj.buttonC) ? "pressed" : "not pressed");
	console.log(outputStr);
	outputStr = "button Z: " +
	((nunchuck_obj.buttonZ) ? "pressed" : "not pressed");
	console.log(outputStr);
}, 100);

// Print message when exiting
process.on('SIGINT', function()
{
	console.log("Exiting...");
	process.exit(0);
});

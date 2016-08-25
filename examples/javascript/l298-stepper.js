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

var HBridge_lib = require('jsupm_l298');

// Instantiate a Stepper motor on a L298 Dual H-Bridge.
// This was tested with the NEMA-17 12V, 350mA, with 200 steps per rev.
var myHBridge_obj = new HBridge_lib.L298(200, 3, 4, 7, 8, 9);

/**************************************
 * Instantiate H-bridge stepper object
***************************************/
myHBridge_obj.goForward = function()
{
	myHBridge_obj.setSpeed(10); // 10 RPMs
	myHBridge_obj.setDirection(HBridge_lib.L298.DIR_CW);
	myHBridge_obj.enable(true);
	console.log("Rotating 1 full revolution at 10 RPM speed.");
	// move 200 steps, a full rev
	myHBridge_obj.stepperSteps(200);
};

myHBridge_obj.reverseDirection = function()
{
	console.log("Rotating 1/2 revolution in opposite direction at 10 RPM speed.");
	myHBridge_obj.setDirection(HBridge_lib.L298.DIR_CCW);
	myHBridge_obj.stepperSteps(100);
};

myHBridge_obj.stop = function()
{
	myHBridge_obj.enable(false);
};

myHBridge_obj.quit = function()
{
	myHBridge_obj = null;
	HBridge_lib.cleanUp();
	HBridge_lib = null;
	console.log("Exiting");
	process.exit(0);
};


/************************
 * Run H-bridge stepper!
*************************/
myHBridge_obj.goForward();
setTimeout(myHBridge_obj.reverseDirection, 2000);
setTimeout(function()
{
	myHBridge_obj.stop();
	myHBridge_obj.quit();
}, 4000);

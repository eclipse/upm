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

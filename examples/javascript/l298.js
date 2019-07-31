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

// Instantiate one of the 2 possible DC motors on a L298 Dual
// H-Bridge.  For controlling a stepper motor, see the l298-stepper
// example.
var myHBridge_obj = new HBridge_lib.L298(3, 4, 7);

/*****************************
 * Instantiate H-bridge object
******************************/
myHBridge_obj.goForward = function()
{
	console.log("Starting motor at 50% for 3 seconds...");
	myHBridge_obj.setSpeed(50);
	myHBridge_obj.setDirection(HBridge_lib.L298.DIR_CW);
	myHBridge_obj.enable(true);
};

myHBridge_obj.reverseDirection = function()
{
	console.log("Reversing direction...");
	myHBridge_obj.setDirection(HBridge_lib.L298.DIR_NONE); // fast stop
	myHBridge_obj.setDirection(HBridge_lib.L298.DIR_CCW);
};

myHBridge_obj.stop = function()
{
	myHBridge_obj.setSpeed(0);
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
 * Run H-bridge!
*************************/
myHBridge_obj.goForward();
setTimeout(myHBridge_obj.reverseDirection, 3000);
setTimeout(function()
{
	myHBridge_obj.stop();
	myHBridge_obj.quit();
}, 6000);

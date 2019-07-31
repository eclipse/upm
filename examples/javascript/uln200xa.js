/*
* Author:  Jon Trulson <jtrulson@ics.com>
* Copyright (c) 2015-2016 Intel Corporation.
*
* This program and the accompanying materials are made available under the
* terms of the The MIT License which is available at
* https://opensource.org/licenses/MIT.
*
* SPDX-License-Identifier: MIT
*/

var Uln200xa_lib = require('jsupm_uln200xa');

// Instantiate a Stepper motor on a ULN200XA Darlington Motor Driver
// This was tested with the Grove Geared Step Motor with Driver

// Instantiate a ULN2003XA stepper object
var myUln200xa_obj = new Uln200xa_lib.ULN200XA(4096, 8, 9, 10, 11);

myUln200xa_obj.goForward = function()
{
    myUln200xa_obj.setSpeed(5); // 5 RPMs
    myUln200xa_obj.setDirection(Uln200xa_lib.ULN200XA_DIR_CW);
    console.log("Rotating 1 revolution clockwise.");
    myUln200xa_obj.stepperSteps(4096);
};

myUln200xa_obj.reverseDirection = function()
{
	console.log("Rotating 1/2 revolution counter clockwise.");
	myUln200xa_obj.setDirection(Uln200xa_lib.ULN200XA_DIR_CCW);
	myUln200xa_obj.stepperSteps(2048);
};

myUln200xa_obj.stop = function()
{
	myUln200xa_obj.release();
};

myUln200xa_obj.quit = function()
{
	myUln200xa_obj = null;
	Uln200xa_lib.cleanUp();
	Uln200xa_lib = null;
	console.log("Exiting");
	process.exit(0);
};

// Run ULN200xa driven stepper
myUln200xa_obj.goForward();
setTimeout(myUln200xa_obj.reverseDirection, 2000);
setTimeout(function()
{
	myUln200xa_obj.stop();
	myUln200xa_obj.quit();
}, 2000);

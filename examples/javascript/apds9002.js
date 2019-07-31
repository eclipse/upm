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

// Load Grove Luminance sensor module
var luminanceSensor_lib = require('jsupm_apds9002');

// Instantiate a Grove Luminance sensor on analog pin A0
var myLuminanceSensor_obj = new luminanceSensor_lib.APDS9002(0);

var myInterval = setInterval(function()
{
	console.log("Luminance value is " + myLuminanceSensor_obj.value());
}, 1000);

// On exit: clear interval and memory, print message
process.on('SIGINT', function()
{
	clearInterval(myInterval);
	myLuminanceSensor_obj = null;
	luminanceSensor_lib.cleanUp();
	luminanceSensor_lib = null;
	console.log("Exiting");
	process.exit(0);
});

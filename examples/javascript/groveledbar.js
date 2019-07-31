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

var LEDBar = require("jsupm_my9221");

// Instantiate a MY9221, we use D8 for the data, and D9 for the
// data clock.  This was tested with a Grove LED bar.
var myLEDBar = new LEDBar.GroveLEDBar(8, 9);

var directionBool = true;

// A note on timing:
//	In the C++ example, the system sleeps 10 times for 50 milliseconds
//	between each LED lighting. After the LED has reached the last light
//	of the cycle, the system sleeps again for 1 second.
//	The sleeps are cumulative, so the system has slept for 1.5 seconds
//	sum total for each cycle.
//	setInterval and setTimeout make asynchronous function calls;
//	they aren't cumulative.
//	In order to approximate the behavior of the C++ example, we need
//	to call each iteration 1.5 seconds apart instead of 1 second apart.
var myInterval = setInterval(function()
{
	// start showing LED strip with just the first one lit
	show_LED(1, directionBool);
}, (1000 + (10*50)) );

function show_LED(level, direction)
{
	// If it's less than 10
	//	light up the LED now
	//	call show_LED again in 50 ms
	if (level <= 10)
	{
		myLEDBar.setBarLevel(level, directionBool);
		setTimeout(show_LED, 50, ++level, directionBool);	
	}
	// Switch LED lighting directions between lighting cycles
	else
		directionBool = !directionBool;
}

// When exiting: clear LED strip lights, clear interval, print exit message
process.on('SIGINT', function()
{
	myLEDBar.setBarLevel(0, true);
	clearInterval(myInterval);
	console.log("Exiting...");
	process.exit(0);
});

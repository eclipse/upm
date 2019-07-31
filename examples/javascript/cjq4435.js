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

var MOSFETsensor = require("jsupm_cjq4435");

var g_addnumBool = true;
var g_cycleNum = 0.0;
var g_cycleCount = 0;

// Instantiate a CJQ4435 MOSFET on a PWM capable digital pin D3
var myMOSFETsensor = new MOSFETsensor.CJQ4435(3);

myMOSFETsensor.setPeriodMS(10);
myMOSFETsensor.enable(true);

// A note on timing:
//	In the C++ example, the system sleeps 11 times for 100 milliseconds
//	between each duty cycle. After reaching the last point of the cycle,
//	the system sleeps again for 1 second.
//	The sleeps are cumulative, so the system has slept for 2.1 seconds
//	sum total for each cycle.
//	setInterval and setTimeout make asynchronous function calls;
//	they aren't cumulative.
//	In order to approximate the behavior of the C++ example, we need
//	to call each iteration 2.1 seconds apart instead of 1 second apart.

var myInterval = setInterval(function()
{
	setDutyCycle();
}, (1000 + (11*100)) );


// We start with a duty cycle of 0.0 (off) and increment to 1.0 (on)
// Then we take it back down,
// starting with a duty cycle of 1.0 (on) and decrement to 0.0 (off)
function setDutyCycle()
{
	myMOSFETsensor.setDutyCycle(g_cycleNum);
	if (g_addnumBool)
		g_cycleNum += 0.1;
	else
		g_cycleNum -= 0.1;
	g_cycleCount++;
	if (g_cycleCount > 10)
	{
		g_addnumBool = !g_addnumBool;
		g_cycleCount = 0;
	}
	else
		setTimeout(setDutyCycle, 100);
}


// When exiting: clear interval and print exit message
process.on('SIGINT', function()
{
	clearInterval(myInterval);
	myMOSFETsensor = null;
	MOSFETsensor.cleanUp();
	MOSFETsensor = null;
	console.log("Exiting...");
	process.exit(0);
});

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

var electromagnet_lib = require("jsupm_electromagnet");
// The was tested with the  Electromagnetic Module
// Instantiate a  Electromagnet on digital pin D2
var electromagnet_obj = new electromagnet_lib.Electromagnet(2);
console.log("Starting up magnet....");
electromagnet_obj.off();

var magnetState = false;

// Turn magnet on and off every 5 seconds
var myInterval = setInterval(function()
{
	magnetState = !magnetState;
	if (magnetState)
		electromagnet_obj.on();
	else
		electromagnet_obj.off();
	console.log("Turning magnet " + ((magnetState) ? "on" : "off"));
}, 5000);

// When exiting: clear interval, turn off magnet, run memory cleanup, and print message
process.on('SIGINT', function()
{
	clearInterval(myInterval);
	electromagnet_obj.off();
	electromagnet_obj = null;
	electromagnet_lib.cleanUp();
	electromagnet_lib = null;
	console.log("Exiting...");
	process.exit(0);
});

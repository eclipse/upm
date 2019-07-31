/*
* Author: Zion Orent <zorent@ics.com>
* Copyright (c) 2014 Intel Corporation.
*
* This program and the accompanying materials are made available under the
* terms of the The MIT License which is available at
* https://opensource.org/licenses/MIT.
*
* SPDX-License-Identifier: MIT
*/

//Load Speaker module
var Speaker = require('jsupm_speaker');
// Instantiate a Speaker on digital pin D2
var mySpeaker = new Speaker.Speaker(2);

// Play all 7 of the lowest notes
mySpeaker.playAll();

// Play a medium C-sharp
mySpeaker.playSound('c', true, "med");

// Print message when exiting
process.on('SIGINT', function()
{
	console.log("Exiting...");
	process.exit(0);
});

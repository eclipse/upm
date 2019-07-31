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

var FM_receiver_lib = require('jsupm_sx6119');

// Instantiate a SX6119 on digital pins 2 (power) and 3 (seek)
// This example was tested on the Grove FM Receiver.
var myFM_receiver_obj = new FM_receiver_lib.SX6119(2, 3);

// if an argument was specified (any argument), seek to the next
// station, else just toggle the power.


console.log("Supply any argument to the command line to seek to the");
console.log("next station.");
console.log("Running the example without an argument will toggle the");
console.log("power on or off.\n");

var doSeek = false;

if (process.argv.length > 2)
	doSeek = true;

// depending on what was selected, do it
if (doSeek)
	myFM_receiver_obj.seek();
else
	myFM_receiver_obj.togglePower();

console.log("Exiting");

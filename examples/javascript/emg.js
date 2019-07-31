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
var emg_lib = require("jsupm_emg");

// The was tested with the EMG Muscle Signal Reader Sensor Module
// Instantiate a EMG on analog pin A0
var emg_obj = new emg_lib.EMG(0);
console.log("Calibrating....");
emg_obj.calibrate();

var myInterval = setInterval(function()
{
	console.log(emg_obj.value());
}, 100);

// When exiting: clear interval, and print message
process.on('SIGINT', function()
{
	clearInterval(myInterval);
	emg_obj = null;
	emg_lib.cleanUp();
	emg_lib = null;
	console.log("Exiting...");
	process.exit(0);
});

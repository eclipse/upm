/*
* Author: Ron Evans (@deadprogram)
* Copyright (c) 2016 Intel Corporation.
*
* This program and the accompanying materials are made available under the
* terms of the The MIT License which is available at
* https://opensource.org/licenses/MIT.
*
* SPDX-License-Identifier: MIT
*/

var mraa = require('mraa');
console.log('MRAA Version: ' + mraa.getVersion());

// open connection to Firmata
mraa.addSubplatform(mraa.GENERIC_FIRMATA, "/dev/ttyACM0");

var curieImu = require('jsupm_curieimu');
var myCurie = new curieImu.CurieImu();

var outputStr;
var myInterval = setInterval(function()
{
	myCurie.updateAccel();
	outputStr = "accel: x " + myCurie.getAccelX()
				+ " - y " + myCurie.getAccelY()
				+ " - z " + myCurie.getAccelZ();
	console.log(outputStr);
}, 500);

// Print message when exiting
process.on('SIGINT', function()
{
	clearInterval(myInterval);
	console.log("Exiting");
	process.exit(0);
});

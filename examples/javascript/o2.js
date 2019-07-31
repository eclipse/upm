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
var O2_lib = require("jsupm_o2");

// The was tested with the O2 Oxygen Concentration Sensor Module
// Instantiate a O2 on analog pin A0
var O2_obj = new O2_lib.O2(0);

var myInterval = setInterval(function()
{
	console.log("The output voltage is: " + roundNum(O2_obj.voltageValue(), 5) + "mV");
}, 100);

function roundNum(num, decimalPlaces)
{
	var extraNum = (1 / (Math.pow(10, decimalPlaces) * 1000));
	return (Math.round((num + extraNum) * (Math.pow(10, decimalPlaces))) / Math.pow(10, decimalPlaces));
}

// When exiting: clear interval, and print message
process.on('SIGINT', function()
{
	clearInterval(myInterval);
	O2_obj = null;
	O2_lib.cleanUp();
	O2_lib = null;
	console.log("Exiting...");
	process.exit(0);
});

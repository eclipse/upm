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
var differential_amplifier_lib = require("jsupm_ina132");

// Tested with the INA132 Differential Amplifier Sensor module.
// Instantiate an INA132 on analog pin A0
var differential_amplifier_obj = new differential_amplifier_lib.INA132(0);

var myInterval = setInterval(function()
{
	console.log(roundNum(differential_amplifier_obj.value(), 5));
}, 1000);

function roundNum(num, decimalPlaces)
{
	var extraNum = (1 / (Math.pow(10, decimalPlaces) * 1000));
	var numerator = Math.round((num + extraNum) * (Math.pow(10, decimalPlaces)));
	var denominator = Math.pow(10, decimalPlaces);
	return (numerator / denominator);
}

// When exiting: clear interval, and print message
process.on('SIGINT', function()
{
	clearInterval(myInterval);
	differential_amplifier_obj = null;
	differential_amplifier_lib.cleanUp();
	differential_amplifier_lib = null;
	console.log("Exiting");
	process.exit(0);
});

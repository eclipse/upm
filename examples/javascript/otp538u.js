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

// analog voltage, usually 3.3 or 5.0
var OTP538U_AREF = 5.0;

var tempIRSensor_lib = require('jsupm_otp538u');

// Instantiate a OTP538U on analog pins A0 and A1
// A0 is used for the Ambient Temperature and A1 is used for the
// Object temperature.
var tempIRSensor_obj = new tempIRSensor_lib.OTP538U(0, 1, OTP538U_AREF);


function checkTemp()
{
	var outputStr = "Ambient temp: " +
		roundNum(tempIRSensor_obj.ambientTemperature(), 2) +
		" C, Object temp: " +
		roundNum(tempIRSensor_obj.objectTemperature(), 2) +
		" C";
	console.log(outputStr);
}

var myInterval = setInterval(checkTemp, 1000);

function roundNum(num, decimalPlaces)
{
	var extraNum = (1 / (Math.pow(10, decimalPlaces) * 1000));
	return (Math.round((num + extraNum) *
		(Math.pow(10, decimalPlaces))) / Math.pow(10, decimalPlaces));
}

// When exiting: clear interval and print message
process.on('SIGINT', function()
{
	clearInterval(myInterval);
	console.log("Exiting...");
	process.exit(0);
});

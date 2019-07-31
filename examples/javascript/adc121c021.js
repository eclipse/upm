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

// Load ADC121C021 module
var I2C_ADC = require('jsupm_adc121c021');
// Instantiate an ADC121C021 on I2C bus 0
var busID = I2C_ADC.ADC121C021_I2C_BUS;
var I2CAddr = I2C_ADC.ADC121C021_DEFAULT_I2C_ADDR;
var myI2C_ADC = new I2C_ADC.ADC121C021(busID, I2CAddr);

// get the data every 50 milliseconds
var myInterval = setInterval(function()
{
	var val = myI2C_ADC.value();
	var voltsVal = myI2C_ADC.valueToVolts(val);
	console.log("ADC value: " + val + " Volts = " + voltsVal);
}, 50);


// Print message when exiting
process.on('SIGINT', function()
{
	clearInterval(myInterval);
	myI2C_ADC = null;
	I2C_ADC.cleanUp();
	I2C_ADC = null;
	console.log("Exiting...");
	process.exit(0);
});

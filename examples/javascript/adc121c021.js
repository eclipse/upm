/*jslint node:true, vars:true, bitwise:true, unparam:true */
/*jshint unused:true */
/*global */
/*
* Author: Zion Orent <zorent@ics.com>
* Copyright (c) 2015 Intel Corporation.
*
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to
* permit persons to whom the Software is furnished to do so, subject to
* the following conditions:
*
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
* LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
* OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
* WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
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

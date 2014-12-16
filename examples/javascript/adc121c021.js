/*jslint node:true, vars:true, bitwise:true, unparam:true */
/*jshint unused:true */
/*global */
/*
* Author: Zion Orent <zorent@ics.com>
* Copyright (c) 2014 Intel Corporation.
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
var myI2C_ADC = new I2C_ADC.ADC121C021(I2C_ADC.ADC121C021_I2C_BUS, 
                I2C_ADC.ADC121C021_DEFAULT_I2C_ADDR);
var g_sum, g_iteration, g_intervalObj;
var g_total_iteration_count = 30;
// An analog sensor, such as a Grove light sensor,
// must be attached to the adc
main();

// this function will be called by itself
function main()
{
	g_sum = 0;
	g_iteration = 0;
	// Sum the value every 50 milliseconds for g_total_iteration_count times
	// Then print the average value and corresponding voltage
	g_intervalObj = setInterval(function(I2CADC_val)
	{
		g_sum += I2CADC_val;
		if (g_iteration >= g_total_iteration_count)
		{
			g_iteration = 0;
			var val = (g_sum / g_total_iteration_count);
			console.log("ADC value: " + val + " Volts = " +
			            myI2C_ADC.valueToVolts(val));
			clearInterval(g_intervalObj);
			// Now that we're done with this iteration, go to the next iteration
			main();
		}
		g_iteration++;
	}, 50, myI2C_ADC.value());
}

// Print message when exiting
process.on('SIGINT', function()
{
	console.log("Exiting...");
	process.exit(0);
});

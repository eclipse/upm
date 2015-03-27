/*jslint node:true, vars:true, bitwise:true, unparam:true */
/*jshint unused:true */
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

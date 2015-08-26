/*jslint node:true, vars:true, bitwise:true, unparam:true */
/*jshint unused:true */
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

var voltageDivider = require('jsupm_grovevdiv');
// Instantiate a Grove Voltage Divider sensor on analog pin A0
var myVoltageDivider = new voltageDivider.GroveVDiv(0);

// collect data and output measured voltage according to the setting
// of the scaling switch (3 or 10)
var val, gain3val, gain10val;
function getVoltageInfo()
{
	val = myVoltageDivider.value(100);
	gain3val = myVoltageDivider.computedValue(3, val);
	gain10val = myVoltageDivider.computedValue(10, val);
	console.log("ADC value: " + val + " Gain 3: " + gain3val 
				+ "v Gain 10: " + gain10val + "v");
}

setInterval(getVoltageInfo, 1000);

// Print message when exiting
process.on('SIGINT', function()
{
	myVoltageDivider = null;
	voltageDivider.cleanUp();
	voltageDivider = null;
	console.log("Exiting...");
	process.exit(0);
});

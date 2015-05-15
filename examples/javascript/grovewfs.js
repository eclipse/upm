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

var waterFlow_lib = require('jsupm_grovewfs');

// Instantiate a Grove Water Flow Sensor on digital pin D2
var myWaterFlow_obj = new waterFlow_lib.GroveWFS(2);

// set the flow counter to 0 and start counting
myWaterFlow_obj.clearFlowCounter();
myWaterFlow_obj.startFlowCounter();


var millis, flowCount, fr;
var myInterval = setInterval(function()
{
	// we grab these (millis and flowCount) just for display
	// purposes in this example
	millis = myWaterFlow_obj.getMillis();
	flowCount = myWaterFlow_obj.flowCounter();

	fr = myWaterFlow_obj.flowRate();

	// output milliseconds passed, flow count, and computed flow rate
	outputStr = "Millis: " + millis + " Flow Count: " + flowCount +
	" Flow Rate: " + fr + " LPM";
	console.log(outputStr);

	// best to gather data for at least one second for reasonable
	// results.
}, 2000);


// When exiting: clear interval and print message
process.on('SIGINT', function()
{
	clearInterval(myInterval);
	myWaterFlow_obj.stopFlowCounter();
	myWaterFlow_obj = null
	waterFlow_lib.cleanUp();
	waterFlow_lib = null;

	console.log("Exiting");
	process.exit(0);
});

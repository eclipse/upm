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

var upmMQ3 = require("jsupm_gas");

// Attach gas sensor to AIO0
var myMQ3 = new upmMQ3.MQ3(0);

var threshContext = new upmMQ3.thresholdContext;
threshContext.averageReading = 0;
threshContext.runningAverage = 0;
threshContext.averagedOver = 2;

// Infinite loop, ends when script is cancelled
// Repeatedly, take a sample every 2 milliseconds;
// find the average of 128 samples; and
// print a running graph of the averages using a resolution of 5
while(1)
{
    var buffer = new upmMQ3.uint16Array(128);
    var len = myMQ3.getSampledWindow(2, 128, buffer);
    if (len)
    {
        var thresh = myMQ3.findThreshold(threshContext, 30, buffer, len);
        myMQ3.printGraph(threshContext, 5);
        //if (thresh)
        //    console.log("Threshold is " + thresh);
    }
}

// Print message when exiting
process.on('SIGINT', function()
{
	console.log("Exiting...");
	process.exit(0);
});

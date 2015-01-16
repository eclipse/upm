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

var upmMQ5 = require("jsupm_gas");

// Attach gas sensor to AIO0
var myMQ5 = new upmMQ5.MQ5(0);

var threshContext = new upmMQ5.thresholdContext;
threshContext.averageReading = 0;
threshContext.runningAverage = 0;
threshContext.averagedOver = 2;

// Infinite loop, ends when script is cancelled
while(1)
{
    var buffer = new upmMQ5.uint16Array(128);
    var len = myMQ5.getSampledWindow(2, 128, buffer);
    if (len)
    {
        var thresh = myMQ5.findThreshold(threshContext, 30, buffer, len);
        myMQ5.printGraph(threshContext, 5);
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

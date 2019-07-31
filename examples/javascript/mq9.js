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

var upmMQ9 = require("jsupm_gas");

// Attach gas sensor to AIO0
var myMQ9 = new upmMQ9.MQ9(0);

var threshContext = new upmMQ9.thresholdContext;
threshContext.averageReading = 0;
threshContext.runningAverage = 0;
threshContext.averagedOver = 2;

// Infinite loop, ends when script is cancelled
while(1)
{
    var buffer = new upmMQ9.uint16Array(128);
    var len = myMQ9.getSampledWindow(2, 128, buffer);
    if (len)
    {
        var thresh = myMQ9.findThreshold(threshContext, 30, buffer, len);
        myMQ9.printGraph(threshContext, 5);
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

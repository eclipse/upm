/*
* Author: Zion Orent <zorent@ics.com>
* Copyright (c) 2014 Intel Corporation.
*
* This program and the accompanying materials are made available under the
* terms of the The MIT License which is available at
* https://opensource.org/licenses/MIT.
*
* SPDX-License-Identifier: MIT
*/

var upmMicrophone = require("jsupm_mic");

// Attach microphone to analog port A0
var myMic = new upmMicrophone.Microphone(0);

var threshContext = new upmMicrophone.thresholdContext;
threshContext.averageReading = 0;
threshContext.runningAverage = 0;
threshContext.averagedOver = 2;

var is_running = false;
// Infinite loop, ends when script is cancelled
// Repeatedly, take a sample every 2 microseconds;
// find the average of 128 samples; and
// print a running graph of the averages
while(1)
{
    var buffer = new upmMicrophone.uint16Array(128);
    var len = myMic.getSampledWindow(2, 128, buffer);
    if (len)
    {
        var thresh = myMic.findThreshold(threshContext, 30, buffer, len);
        myMic.printGraph(threshContext);
        if (thresh)
            console.log("Threshold is " + thresh);
    }
}

// Print message when exiting
process.on('SIGINT', function()
{
	console.log("Exiting...");
	process.exit(0);
});

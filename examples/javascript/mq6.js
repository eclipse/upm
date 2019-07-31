/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

var upmGAS = require("jsupm_gas");

// Attach gas sensor to Analog A0
var sensor = new upmGAS.MQ6(0);

var threshContext = new upmGAS.thresholdContext;
threshContext.averageReading = 0;
threshContext.runningAverage = 0;
threshContext.averagedOver = 2;

// Infinite loop, ends when script is cancelled
// Repeatedly, take a sample every 2 microseconds;
// find the average of 128 samples; and
// print a running graph of asteriskss as averages

while(1)
{
    var buffer = new upmGAS.uint16Array(128);
    var len = sensor.getSampledWindow(2, 128, buffer);
    if (len)
    {
        var thresh = sensor.findThreshold(threshContext, 30, buffer, len);
        sensor.printGraph(threshContext, 5);
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

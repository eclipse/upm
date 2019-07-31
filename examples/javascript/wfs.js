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

var waterFlow_lib = require('jsupm_wfs');

// Instantiate a Water Flow Sensor on digital pin D2.  This must be an
// interrupt capable pin.
var myWaterFlow_obj = new waterFlow_lib.WFS(2);

// set the flow counter to 0 and start counting
myWaterFlow_obj.clearFlowCounter();
myWaterFlow_obj.startFlowCounter();


var millis, flowCount, fr;
var myInterval = setInterval(function() {
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
process.on('SIGINT', function() {
    clearInterval(myInterval);
    myWaterFlow_obj.stopFlowCounter();
    myWaterFlow_obj = null
    waterFlow_lib.cleanUp();
    waterFlow_lib = null;

    console.log("Exiting");
    process.exit(0);
});

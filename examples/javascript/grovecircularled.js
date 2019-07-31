/*
 * Author: Jun Kato
 * Contributions: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

// Instantiate a Grove Circular LED on gpio pins 9 and 8
var GCL = require("jsupm_my9221");
var circle = new GCL.GroveCircularLED(9, 8);
var level = 0;

// Show a spinner.
var myInterval = setInterval(function()
{
    circle.setSpinner(level);
    level = (level + 1) % 24;
}, 100);

// When exiting, clear interval and print message.
process.on('SIGINT', function()
{
    clearInterval(myInterval);
    circle.setLevel(0);
    circle = null;
    GCL.cleanUp();
    console.log("Exiting...");
    process.exit(0);
});

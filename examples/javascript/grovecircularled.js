/*
 * Author: Jun Kato
 * Contributions: Jon Trulson <jtrulson@ics.com>
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

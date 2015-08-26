/*jslint node:true, vars:true, bitwise:true, unparam:true */
/*jshint unused:true */

/*
 * Author: Jon Trulson <jtrulson@ics.com>
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

var cameraObj = require('jsupm_grovescam');

// Instantiate a Grove Serial Camera on UART 0
var camera = new cameraObj.GROVESCAM(0);

// make sure port is initialized properly. 115200 baud is the default.
if (!camera.setupTty())
{
    console.log("Failed to setup tty port parameters");
    process.exit(1);
}

if (camera.init())
    console.log("Initialized...");
else
    console.log("init() failed");

if (camera.preCapture())
    console.log("preCapture succeeded...");
else
    console.log("preCapture failed.");

if (camera.doCapture())
    console.log("doCapture succeeded...");
else
    console.log("doCapture failed.");

console.log("Image size is " + camera.getImageSize() + " bytes");

if (camera.getImageSize() > 0)
{
    console.log("Storing image.jpg...");
    if (camera.storeImage("image.jpg"))
        console.log("storeImage succeeded...");
    else
        console.log("storeImage failed.");
}


camera = null;
cameraObj.cleanUp();
cameraObj = null;
console.log("Exiting.");
process.exit(0);


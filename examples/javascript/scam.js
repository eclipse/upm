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

var cameraObj = require('jsupm_scam');

// Instantiate a Serial Camera on UART 0
var camera = new cameraObj.SCAM(0);

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


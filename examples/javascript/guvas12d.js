/*
* Author: Zion Orent <zorent@ics.com>
*         Jon Trulson <jtrulson@ics.com>
* Copyright (c) 2014-2016 Intel Corporation.
*
* This program and the accompanying materials are made available under the
* terms of the The MIT License which is available at
* https://opensource.org/licenses/MIT.
*
* SPDX-License-Identifier: MIT
*/

var UVSensor = require('jsupm_guvas12d');

// analog voltage, usually 3.3 or 5.0
var g_GUVAS12D_AREF = 5.0;

// Instantiate a UV sensor on analog pin A0
var myUVSensor = new UVSensor.GUVAS12D(0, g_GUVAS12D_AREF);

setInterval(function()
{
    var outputStr = "Volts: "
        + myUVSensor.volts()
	+ ", Intensity: "
        + myUVSensor.intensity()
        + " mW/m^2";

    console.log(outputStr);
}, 1000);

// Print message when exiting
process.on('SIGINT', function()
{
	console.log("Exiting...");
	process.exit(0);
});

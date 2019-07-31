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


var barometerObj = require('jsupm_hp20x');

// Instantiate an HP20X on default I2C bus and address
var bar = new barometerObj.HP20X();

// Initialize the device with default values
bar.init();

// Output data every second until interrupted
setInterval(function()
{
    console.log("Temperature: " + bar.getTemperature() + " Celsius");
    console.log("Pressure:    " + bar.getPressure() + " Millibars");
    console.log("Altitude:    " + bar.getAltitude() + " Meters");
    console.log("");
}, 1000);

// exit on ^C
process.on('SIGINT', function()
{
    bar = null;
    barometerObj.cleanUp();
    barometerObj = null;
    console.log("Exiting.");
    process.exit(0);
});


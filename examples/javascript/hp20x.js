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


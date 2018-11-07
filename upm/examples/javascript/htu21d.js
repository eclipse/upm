/*
 * Author: Mihai Tudor Panu <mihai.tudor.panu@intel.com>
 * Copyright (c) 2017 Intel Corporation.
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

//Load the library
var upm = require('jsupm_htu21d');

//Create a sensor instance
var sensor = new upm.HTU21D(0);

//Update sensor data and print the values every second
var ival = setInterval(function () {
    sensor.sampleData();
    console.log("Relative Humidity: " + sensor.getHumidity() + "%");
    console.log("Compensated RH: " + sensor.getCompRH() + "%");
    console.log("Temperature: " + sensor.getTemperature() + "C");
    console.log("Dew Point: " + sensor.getDewPoint() + "C");
}, 1000);

//Clear interval and exit on ^C
process.on('SIGINT', function()
{
    clearInterval(ival);
    process.exit(0);
});

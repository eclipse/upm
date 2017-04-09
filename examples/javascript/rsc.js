/*
* Author: Abhishek Malik <abhishek.malik@intel.com>
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

var rsc = require("jsupm_rsc");

// Instantiate a Honeywell RSC Pressure Sensor at bus 0
var rsc_sensor = new rsc.RSC(0, 9, 8);

console.log("Sensor Name: " + rsc_sensor.getSensorName());
console.log("Sensor Serial No.: " + rsc_sensor.getSensorSerialNumber());

var myInterval = setInterval(function()
{
    console.log("Pressure: " + rsc_sensor.getPressure() + " " + rsc_sensor.getPressureUnit());
    console.log("Temperature: " + rsc_sensor.getTemperature() + " C");
}, 100);

// When exiting: clear interval and print message
process.on('SIGINT', function()
{
    clearInterval(myInterval);
    console.log("Exiting...");
    process.exit(0);
});

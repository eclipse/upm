/*
* Author: Mihai Tudor Panu <mihai.tudor.panu@intel.com>
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

// Load accelerometer
var adxl345 = require('jsupm_adxl345');

// Instantiate on I2C bus
var adxl = new adxl345.Adxl345(0);

setInterval(function()
{
    adxl.update(); // Update the data
    var raw = adxl.getRawValues(); // Read raw sensor data
    var force = adxl.getAcceleration(); // Read acceleration force (g)
    var rawvalues = raw.getitem(0) + " " + raw.getitem(1) + " " + raw.getitem(2);
    console.log("Raw Values: " + rawvalues);
    console.log("ForceX: " + force.getitem(0).toFixed(2) + " g");
    console.log("ForceY: " + force.getitem(1).toFixed(2) + " g");
    console.log("ForceZ: " + force.getitem(2).toFixed(2) + " g");
}, 1000);

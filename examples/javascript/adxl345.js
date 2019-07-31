/*
* Author: Mihai Tudor Panu <mihai.tudor.panu@intel.com>
* Copyright (c) 2015 Intel Corporation.
*
* This program and the accompanying materials are made available under the
* terms of the The MIT License which is available at
* https://opensource.org/licenses/MIT.
*
* SPDX-License-Identifier: MIT
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

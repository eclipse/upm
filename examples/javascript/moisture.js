/*
 * Author: Zion Orent <zorent@ics.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

//Load UPM module
var moisture = require('jsupm_moisture');

// Instantiate a Moisture sensor on analog pin A0
var myMoistureObj = new moisture.Moisture(0);

// Values (approximate):
// 0-300,   sensor in air or dry soil
// 300-600, sensor in humid soil
// 600+,    sensor in wet soil or submerged in water
// Read the value every second and print the corresponding moisture level
setInterval(function() {
    var result;
    var moisture_val = parseInt(myMoistureObj.value());
    if (moisture_val >= 0 && moisture_val < 300)
        result = "Dry";
    else if (moisture_val >= 300 && moisture_val < 600)
        result = "Moist";
    else
        result = "Wet";
    console.log("Moisture value: " + moisture_val + ", " + result);
}, 1000);

// Print message when exiting
process.on('SIGINT', function() {
    console.log("Exiting...");
    process.exit(0);
});

/*
 * Author: Mihai Tudor Panu <mihai.tudor.panu@intel.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

/* This example demonstrates how to use one the ADS1015 ADC on the Grove Joule
 * Shield or the Sparkfun ADC Block for Edison with devices that output a small
 * differential voltage (e.g. geophones, piezoelectric bands or pads,
 * thermocouples).
 */
var mraa = require('mraa'); // Not used directly but required if jsupm_ads1x15 was installed from NPM
var upm = require('jsupm_ads1x15');
var fs = require('fs'); // Values will be written to a file
var file = "./ads1015.data" // Output filename
var id = 0; // Sample number
var ival; // Interval function for sampling

// Initialize and configure the ADS1015
var ads1015 = new upm.ADS1015(0, 0x48);

// Put the ADC into differential mode for pins A0 and A1
ads1015.getSample(upm.ADS1X15_MUX_DIFF_0_1);

// Set the gain based on expected VIN range to -/+ 2.048 V
// Can be adjusted based on application to as low as -/+ 0.256 V, see API
// documentation for details
ads1015.setGain(upm.ADS1X15.GAIN_TWO);

// Set the sample rate to 3300 samples per second (max) and turn on continuous
// sampling
ads1015.setSPS(upm.ADS1015.SPS_3300);
ads1015.setContinuous(true);

// Open the output file and start reading data once open
// Will overwrite file if it already exists
var stream = fs.createWriteStream(file);
stream.once('open', function() {
    ival = setInterval(function() {
        // Write data pairs of ID + Value for easy plotting
        stream.write(id + " " + ads1015.getLastSample().toFixed(7) + "\n");
        id++;
    }, 1); // Wait 1 ms between reads, (actuall call adds another ~1 ms overhead)
});

// Stop after 10 seconds and close the file
setTimeout(function() {
    clearInterval(ival);
    stream.end();
    console.log("Wrote " + id + " samples to file: " + file);
}, 10000);


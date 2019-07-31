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

// Load UPM module
var waterSensor = require('jsupm_water');

// Instantiate a Water sensor on digital pin D2
var water = new waterSensor.Water(2);

// Read whether the sensor is wet/dry, waiting one second between readings
function readWaterState() {
    if (water.isWet())
        console.log("Sensor is wet");
    else
        console.log("Sensor is dry");
}
setInterval(readWaterState, 1000);

// Print message when exiting
process.on('SIGINT', function() {
    console.log("Exiting...");
    process.exit(0);
});

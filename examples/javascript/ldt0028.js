/*
 * Author: Sarah Knepper <sarah.knepper@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

// Load Grove module
var sensorModule = require('jsupm_ldt0028');

var NUMBER_OF_SECONDS = 10;
var MILLISECONDS_PER_SECOND = 1000;
var SAMPLES_PER_SECOND = 50;
var THRESHOLD = 100;

// Create the LDT0-028 Piezo Vibration Sensor object using AIO pin 0
var sensor = new sensorModule.LDT0028(0);

// Read the signal every 20 milliseconds for 10 seconds
console.log("For the next " + NUMBER_OF_SECONDS + " seconds, " +
            SAMPLES_PER_SECOND + " samples will be taken every second.");
console.log("");
var buffer = [];
for (var i=0; i < NUMBER_OF_SECONDS * SAMPLES_PER_SECOND; i++) {
    buffer.push(sensor.getSample());
    delay(MILLISECONDS_PER_SECOND / SAMPLES_PER_SECOND );
}

// Print the number of times the reading was greater than the threshold
var count = 0;
for (var i=0; i < NUMBER_OF_SECONDS * SAMPLES_PER_SECOND; i++) {
    if (buffer[i] > THRESHOLD) {
        count++;
    }
}
console.log(sensor.name() + " exceeded the threshold value of " +
        THRESHOLD + " a total of " + count + " times,");
console.log("out of a total of " + NUMBER_OF_SECONDS*SAMPLES_PER_SECOND +
            " readings.");
console.log("");

// Print a graphical representation of the average value sampled
// each second for the past 10 seconds, using a scale factor of 15
console.log("Now printing a graphical representation of the average reading ");
console.log("each second for the last " + NUMBER_OF_SECONDS + " seconds.");
var SCALE_FACTOR = 15;
for (var i=0; i < NUMBER_OF_SECONDS; i++) {
    var sum = 0;
    for (var j=0; j < SAMPLES_PER_SECOND; j++) {
        sum += buffer[i*SAMPLES_PER_SECOND+j];
    }
    var average = sum / SAMPLES_PER_SECOND;
    var stars_to_print = Math.round(average / SCALE_FACTOR);
    var string = "(" + ("    " + Math.round(average)).slice(-4) + ") | ";
    for (var j=0; j < stars_to_print; j++) {
        string += "*";
    }
    console.log(string);
}

function delay( milliseconds ) {
    var startTime = Date.now();
    while (Date.now() - startTime < milliseconds);
}


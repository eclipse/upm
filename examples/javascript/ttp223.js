/*
 * Author: Sarah Knepper <sarah.knepper@intel.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

// Load TTP223 touch sensor module
var sensorModule = require('jsupm_ttp223');

// Create the TTP223 touch sensor object using GPIO pin 0
var touch = new sensorModule.TTP223(0);

// Check whether or not a finger is near the touch sensor and
// print accordingly, waiting one second between readings
function readSensorValue() {
    if ( touch.isPressed() ) {
        console.log(touch.name() + " is pressed");
    } else {
        console.log(touch.name() + " is not pressed");
    }
}
setInterval(readSensorValue, 1000);

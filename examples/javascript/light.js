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

// Load light module
var sensor = require('jsupm_light');

// Create the light sensor object using AIO pin 0
var light = new sensor.Light(0);

// Read the input and print both the normalized ADC value and a rough
// lux value, waiting one second between readings
function readLightSensorValue() {
    console.log(light.name() + " normalized value is " + light.getNormalized() +
            ", which is roughly " + light.value() + " lux");
}
setInterval(readLightSensorValue, 1000);

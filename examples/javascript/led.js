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

// Load UPM module
var ledSensor = require('jsupm_led');

// Create the LED object using GPIO pin 2
var led = new ledSensor.Led(2);

// Print the name
console.log(led.name());

// Turn the LED on and off 10 times, pausing one second
// between transitions
var i = 0;
var waiting = setInterval(function() {
        if ( i % 2 == 0 ) {
            led.on();
        } else {
            led.off();
        }
        i++;
        if ( i == 20 ) clearInterval(waiting);
        }, 1000);
            

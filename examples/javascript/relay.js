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
var upm = require('jsupm_relay');

// Create the relay switch object using GPIO pin 0
var relay = new upm.Relay(0);

// Close and then open the relay switch 3 times,
// waiting one second each time. The LED on the relay switch
// will light up when the switch is on (closed).
// The switch will also make a noise between transitions.
var i = 0;
var waiting = setInterval(function() {
    if (i % 2 == 0) {
        relay.on();
        if (relay.isOn())
            console.log(relay.name() + " is on");
    } else {
        relay.off();
        if (relay.isOff())
            console.log(relay.name() + " is off");
    }
    i++;
    if (i == 6) clearInterval(waiting);
}, 1000);

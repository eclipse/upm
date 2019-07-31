/*
 * Author: Mihai Tudor Panu <mihai.tudor.panu@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

//initialization
var upm = require('jsupm_rotary');

//setup access analog input Analog pin #0 (A0)
var rotary = new upm.Rotary(0);

loop();

function loop() {
    var abs = rotary.abs_value();
    var absdeg = rotary.abs_deg();
    var absrad = rotary.abs_rad();

    var rel = rotary.rel_value();
    var reldeg = rotary.rel_deg();
    var relrad = rotary.rel_rad();

    //write the knob value to the console in different formats
    console.log("Abs: " + abs + " " + Math.round(parseInt(absdeg)) + " " + absrad.toFixed(3));
    console.log("Rel: " + rel + " " + Math.round(parseInt(reldeg)) + " " + relrad.toFixed(3));

    //wait 2s and call function again
    setTimeout(loop, 2000);
}

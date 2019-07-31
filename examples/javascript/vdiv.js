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

var voltageDivider = require('jsupm_vdiv');
// Instantiate a Voltage Divider sensor on analog pin A0
var myVoltageDivider = new voltageDivider.VDiv(0);

// collect data and output measured voltage according to the setting
// of the scaling switch (3 or 10)
var val, gain3val, gain10val;

function getVoltageInfo() {
    val = myVoltageDivider.value(100);
    gain3val = myVoltageDivider.computedValue(3, val);
    gain10val = myVoltageDivider.computedValue(10, val);
    console.log("ADC value: " + val + " Gain 3: " + gain3val + "v Gain 10: " + gain10val + "v");
}

setInterval(getVoltageInfo, 1000);

// Print message when exiting
process.on('SIGINT', function() {
    myVoltageDivider = null;
    voltageDivider.cleanUp();
    voltageDivider = null;
    console.log("Exiting...");
    process.exit(0);
});

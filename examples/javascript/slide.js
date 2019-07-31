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

var upm_slide = require('jsupm_slide');

//setup access analog input Analog pin #0 (A0)
var Slide = new upm_slide.Slide(0);

loop();

function loop()
{
    var raw = Slide.raw_value();
    var volts = Slide.voltage_value();

    //write the slider values to the console
    console.log("Slider Value: " + raw + " = " + volts.toFixed(2) + " V");

    //wait 2 s then call function again
    setTimeout(loop, 2000);
}

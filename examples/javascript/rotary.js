/*
 * Author: Mihai Tudor Panu <mihai.tudor.panu@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
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

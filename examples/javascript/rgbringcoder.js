/*jslint node:true, vars:true, bitwise:true, unparam:true */
/*jshint unused:true */

/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015 Intel Corporation.
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

var ringcoderObj = require('jsupm_rgbringcoder');

// There are a lot of pins to hook up.  These pins are valid for the
// Edison board, but may need to be adjusted for other platforms.

// In order:
// enable      - 4
// latch       - 10
// clear       - 11
// clock       - 2
// data        - 9
// switch      - 7

// red pwm     - 3
// green pwm   - 5
// blue pwm    - 6

// encA        - 12
// encB        - 13
var ringCoder = new ringcoderObj.RGBRingCoder(4, 10, 11, 2, 9, 7, 12, 13, 3, 
                                              5, 6);

var spin = 0x0001;
var oldState = false;
var oldPos = 0;

// Lets go green
ringCoder.setRGBLED(0.99, 0.01, 0.99);

setInterval(function()
{
    // you spin me round...
    if ((spin & 0xffff) == 0)
        spin = 0x0001;
    
    ringCoder.setRingLEDS(spin);
    spin <<= 1;
    
    // check button state
    var bstate = ringCoder.getButtonState();
    if (bstate != oldState)
    {
        console.log("Button state changed from " + oldState + " to "
            + bstate);
        oldState = bstate;
    }
    
    // check encoder position
    var epos = ringCoder.getEncoderPosition();
    if (epos != oldPos)
    {
        console.log("Encoder position changed from " + oldPos + " to "
            + epos);
        oldPos = epos;
    }
}, 100);


// exit on ^C
process.on('SIGINT', function()
{
    ringCoder = null;
    ringcoderObj.cleanUp();
    ringcoderObj = null;
    console.log("Exiting.");
    process.exit(0);
});





/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
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





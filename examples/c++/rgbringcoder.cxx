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

#include <iostream>
#include <signal.h>

#include "rgbringcoder.hpp"
#include "upm_utilities.h"

using namespace std;

int shouldRun = true;

void
sig_handler(int signo)
{
    if (signo == SIGINT)
        shouldRun = false;
}

int
main(int argc, char** argv)
{
    signal(SIGINT, sig_handler);

    //! [Interesting]

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

    upm::RGBRingCoder ringCoder(4, 10, 11, 2, 9, 7, 12, 13, 3, 5, 6);

    uint16_t spin = 0x0001;
    bool oldState = false;
    int oldPos = 0;

    // Lets go green
    ringCoder.setRGBLED(0.99, 0.01, 0.99);

    while (shouldRun) {
        // you spin me round...
        if (spin == 0)
            spin = 0x0001;

        ringCoder.setRingLEDS(spin);
        spin <<= 1;

        // check button state
        bool bstate = ringCoder.getButtonState();
        if (bstate != oldState) {
            cout << "Button state changed from " << oldState << " to " << bstate << endl;
            oldState = bstate;
        }

        // check encoder position
        int epos = ringCoder.getEncoderPosition();
        if (epos != oldPos) {
            cout << "Encoder position changed from " << oldPos << " to " << epos << endl;
            oldPos = epos;
        }

        upm_delay_us(100000);
    }

    //! [Interesting]

    return 0;
}

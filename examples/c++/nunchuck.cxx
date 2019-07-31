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

#include "nunchuck.hpp"
#include "upm_utilities.h"

using namespace std;

bool shouldRun = true;

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
    // Instantiate a nunchuck controller bus 3
    upm::NUNCHUCK nunchuck(3);

    while (shouldRun) {
        nunchuck.update();

        cout << "stickX: " << nunchuck.stickX << ", stickY: " << nunchuck.stickY << endl;
        cout << "accelX: " << nunchuck.accelX << ", accelY: " << nunchuck.accelY
             << ", accelZ: " << nunchuck.accelZ << endl;

        cout << "button C: " << ((nunchuck.buttonC) ? "pressed" : "not pressed") << endl;
        cout << "button Z: " << ((nunchuck.buttonZ) ? "pressed" : "not pressed") << endl;
        cout << endl;

        upm_delay_us(100000);
    }
    //! [Interesting]

    return 0;
}

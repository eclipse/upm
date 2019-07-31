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

#include "rotaryencoder.hpp"
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
main()
{
    signal(SIGINT, sig_handler);

    //! [Interesting]
    // Instantiate a Grove Rotary Encoder, using signal pins D2 and D3
    upm::RotaryEncoder rotaryencoder(2, 3);

    while (shouldRun) {
        cout << "Position: " << rotaryencoder.position() << endl;
        upm_delay_us(100000);
    }

    //! [Interesting]

    cout << "Exiting..." << endl;

    return 0;
}

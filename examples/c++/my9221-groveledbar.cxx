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

#include "groveledbar.hpp"
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

    // Instantiate a GroveLEDBar, we use D8 for the data, and D9 for the
    // clock.  This was tested with a Grove LED bar.
    upm::GroveLEDBar bar(8, 9);

    while (shouldRun) {
        // count up from green to red
        for (int i = 0; i <= 10; i++) {
            bar.setBarLevel(i, true);
            upm_delay_us(100000);
        }
        upm_delay(1);

        // count down from red to green
        for (int i = 0; i <= 10; i++) {
            bar.setBarLevel(i, false);
            upm_delay_us(100000);
        }
        upm_delay(1);
    }
    //! [Interesting]

    cout << "Exiting..." << endl;
    // turn off the LED's
    bar.setBarLevel(0);

    return 0;
}

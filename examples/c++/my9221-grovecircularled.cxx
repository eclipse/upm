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

#include "grovecircularled.hpp"
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
main()
{
    signal(SIGINT, sig_handler);

    //! [Interesting]

    // Instantiate a Grove Circular LED on D9 for data, D8 for clock

    upm::GroveCircularLED circle(9, 8);

    int level = 0;
    while (shouldRun) {
        circle.setSpinner(level);
        level = (level + 1) % 24;
        upm_delay_us(100000);
    }

    //! [Interesting]

    cout << "Exiting" << endl;

    return 0;
}

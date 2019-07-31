/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <iostream>
#include <signal.h>

#include "linefinder.hpp"
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
    // Instantiate a  Line Finder sensor on digital pin D2
    upm::LineFinder finder(2);

    // check every second for the presence of white detection
    while (shouldRun) {
        bool val = finder.whiteDetected();
        if (val)
            cout << "White detected." << endl;
        else
            cout << "Black detected." << endl;

        upm_delay(1);
    }
    //! [Interesting]

    cout << "Exiting..." << endl;

    return 0;
}

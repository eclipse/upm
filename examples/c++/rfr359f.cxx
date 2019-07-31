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

#include "rfr359f.hpp"
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
    // Instantiate an RFR359F digital pin D2
    // This was tested on the Grove IR Distance Interrupter

    upm::RFR359F dInterruptor(2);

    // The range appears to be about 4 inches, depending on adjustment
    while (shouldRun) {
        if (dInterruptor.objectDetected())
            cout << "Object detected" << endl;
        else
            cout << "Area is clear" << endl;

        upm_delay_us(100000); // 100ms
    }
    //! [Interesting]

    cout << "Exiting..." << endl;

    return 0;
}

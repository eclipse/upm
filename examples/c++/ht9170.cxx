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

#include "ht9170.hpp"
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

    // Instantiate a DTMF decoder
    upm::HT9170 dtmf(12, 11, 10, 9, 8);

    // Now we just spin in a loop, upm_delaying every 100ms, checking to see
    // if a digit is available.  If so, we decode and print the digit,
    // and continue looping.

    while (shouldRun) {
        if (dtmf.digitReady()) {
            cout << "Got DTMF code: " << dtmf.decodeDigit() << endl;
            // now spin until digitReady() goes false again
            while (dtmf.digitReady())
                ;
        }

        upm_delay_us(100000);
    }

    //! [Interesting]

    cout << "Exiting..." << endl;

    return 0;
}

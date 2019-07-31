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

#include "loudness.hpp"
#include "upm_utilities.h"

using namespace std;

bool shouldRun = true;

#define LOUDNESS_AREF 5.0

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

    // Instantiate a Loudness sensor on analog pin A0, with an analog
    // reference voltage of LOUDNESS_AREF
    upm::Loudness loud(0, LOUDNESS_AREF);

    // Every tenth of a second, sample the loudness and output it's
    // corresponding analog voltage.

    while (shouldRun) {
        cout << "Detected loudness (volts): " << loud.loudness() << endl;

        upm_delay_us(100000);
    }

    //! [Interesting]

    cout << "Exiting" << endl;

    return 0;
}

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

#include "ehr.hpp"
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
    // Instantiate a  Ear-clip Heart Rate sensor on digital pin D2
    upm::EHR heart(2);

    // set the beat counter to 0, init the clock and start counting beats
    heart.clearBeatCounter();
    heart.initClock();
    heart.startBeatCounter();

    while (shouldRun) {
        // we grab these just for display purposes in this example
        uint32_t millis = heart.getMillis();
        uint32_t beats = heart.beatCounter();

        // heartRate() requires that at least 5 seconds pass before
        // returning anything other than 0
        int hr = heart.heartRate();

        // output milliseconds passed, beat count, and computed heart rate
        cout << "Millis: " << millis << " Beats: " << beats;
        cout << " Heart Rate: " << hr << endl;

        upm_delay(1);
    }

    heart.stopBeatCounter();
    //! [Interesting]

    cout << "Exiting..." << endl;

    return 0;
}

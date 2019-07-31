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

#include "bh1750.hpp"
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

    // Instantiate a BH1750 sensor using defaults (I2C bus (0), using
    // the default I2C address (0x23), and setting the mode to highest
    // resolution, lowest power mode).
    upm::BH1750 sensor;

    // Every second, sample the BH1750 and output the measured lux value

    while (shouldRun) {
        cout << "Detected Light Level (lux): " << sensor.getLux() << endl;
        upm_delay(1);
    }

    //! [Interesting]

    cout << "Exiting" << endl;

    return 0;
}

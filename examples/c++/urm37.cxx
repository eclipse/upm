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

#include "upm_utilities.h"
#include "urm37.hpp"

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

    // Instantiate a URM37 sensor on analog pin A0, reset pin on D2,
    // trigger pin on D3 with an analog reference voltage of 5.0
    upm::URM37 sensor(0, 2, 3, 5.0);

    // Every half a second, sample the URM37 and output the measured
    // distance in cm.

    while (shouldRun) {
        cout << "Detected distance (cm): " << sensor.getDistance() << endl;
        upm_delay_us(500000);
    }

    //! [Interesting]

    cout << "Exiting" << endl;

    return 0;
}

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

#include "dfrph.hpp"
#include "upm_utilities.h"

using namespace std;

bool shouldRun = true;

#define DFRPH_AREF 5.0

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

    // Instantiate a DFRPH sensor on analog pin A0, with an analog
    // reference voltage of DFRPH_AREF
    upm::DFRPH sensor(0, DFRPH_AREF);

    // After calibration, set the offset (based on calibration with a pH
    // 7.0 buffer solution).  See the UPM sensor documentation for
    // calibrations instructions.
    sensor.setOffset(0.065);

    // Every second, sample the pH and output it's corresponding
    // analog voltage.

    while (shouldRun) {
        cout << "Detected volts: " << sensor.volts() << endl;
        cout << "pH value: " << sensor.pH() << endl;
        cout << endl;

        upm_delay(1);
    }

    //! [Interesting]

    cout << "Exiting" << endl;

    return 0;
}

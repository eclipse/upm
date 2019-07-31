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

#include "mg811.hpp"
#include "upm_utilities.h"

using namespace std;

bool shouldRun = true;

#define MG811_AREF 5.0

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

    // Instantiate an MG811 on analog pin A0, and digital pin D2 with an
    // analog reference voltage of MG811_AREF (5.0)
    upm::MG811 sensor(0, 2, MG811_AREF);

    // Every tenth of a second, sample the sensor and output it's
    // detected CO2 concentration in parts per million (ppm)

    while (shouldRun) {
        cout << "CO2 concentration in PPM: " << sensor.ppm() << endl;

        upm_delay_us(100000);
    }

    //! [Interesting]

    cout << "Exiting" << endl;

    return 0;
}

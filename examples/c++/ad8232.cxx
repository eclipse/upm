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

#include "ad8232.hpp"
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
    // Instantiate a Ad8232 sensor on digital pins 10 (LO+), 11 (LO-)
    // and an analog pin, 0 (OUTPUT)
    upm::AD8232 ad8232(10, 11, 0);

    // Output the raw numbers from the ADC, for plotting elsewhere.
    // A return of 0 indicates a Lead Off (LO) condition.
    // In theory, this data could be fed to software like Processing
    // (https://www.processing.org/) to plot the data just like an
    // EKG you would see in a hospital.
    while (shouldRun) {
        cout << ad8232.value() << endl;
        upm_delay_us(1000);
    }
    //! [Interesting]

    cout << "Exiting" << endl;

    return 0;
}

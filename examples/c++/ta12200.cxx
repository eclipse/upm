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

#include "ta12200.hpp"
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
    // Instantiate a TA12-200 sensor on analog pin A0
    upm::TA12200 electricity(0);

    while (shouldRun) {
        unsigned int maxVal = electricity.highestValue();
        float current = electricity.milliAmps(maxVal);

        cout << "Max ADC Value: " << maxVal << ", current: " << current << "mA" << endl;
        upm_delay_us(100000);
    }

    //! [Interesting]

    cout << "Exiting..." << endl;

    return 0;
}

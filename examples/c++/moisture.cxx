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

#include "moisture.hpp"
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
    // Instantiate a Moisture sensor on analog pin A0
    upm::Moisture moisture(0);

    // Values (approximate):
    // 0-300, sensor in air or dry soil
    // 300-600, sensor in humid soil
    // 600+, sensor in wet soil or submerged in water.
    // Read the value every second and print the corresponding moisture level
    while (shouldRun) {
        int val = moisture.value();
        cout << "Moisture value: " << val << ", ";
        if (val >= 0 && val < 300)
            cout << "dry";
        else if (val >= 300 && val < 600)
            cout << "moist";
        else
            cout << "wet";

        cout << endl;

        upm_delay(1);
    }
    //! [Interesting]

    cout << "Exiting" << endl;

    return 0;
}

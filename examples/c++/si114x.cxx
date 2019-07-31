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

#include "si114x.hpp"
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
    // Instantiate a SI114x UV Sensor on I2C bus 0
    upm::SI114X uvi(0);

    // First initialize it
    uvi.initialize();

    cout << "UV Index Scale:" << endl;
    cout << "---------------" << endl;
    cout << "11+        Extreme" << endl;
    cout << "8-10       Very High" << endl;
    cout << "6-7        High" << endl;
    cout << "3-5        Moderate" << endl;
    cout << "0-2        Low" << endl;
    cout << endl;

    // update every second and print the currently measured UV Index
    while (shouldRun) {
        // update current value(s)
        uvi.update();

        // print detected value
        cout << "UV Index: " << uvi.getUVIndex() << endl;

        upm_delay(1);
    }
    //! [Interesting]

    cout << "Exiting..." << endl;

    return 0;
}

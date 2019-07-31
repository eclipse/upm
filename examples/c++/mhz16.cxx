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
#include <string>

#include "mhz16.hpp"
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
main(int argc, char** argv)
{
    signal(SIGINT, sig_handler);

    //! [Interesting]
    // Instantiate a MHZ16 serial CO2 sensor on uart 0.
    // This example was tested on the Grove CO2 sensor module.

    upm::MHZ16 co2(0);

    // make sure port is initialized properly.  9600 baud is the default.
    if (!co2.setupTty(B9600)) {
        cerr << "Failed to setup tty port parameters" << endl;
        return 1;
    }

    cout << "Make sure that the sensor has had at least 3 minutes to warm up" << endl;
    cout << "or you will not get valid results." << endl;
    cout << "The temperature reported is not the ambient temperature, " << endl;
    cout << "but rather the temperature of the sensor elements." << endl;

    upm_delay(1);

    while (shouldRun) {
        if (!co2.getData()) {
            cerr << "Failed to retrieve data" << endl;
            continue;
        }

        cout << "CO2 concentration: " << co2.getGas() << " PPM, "
             << "Temperature (in C): " << co2.getTemperature() << endl;

        upm_delay(2);
    }

    //! [Interesting]

    cout << "Exiting..." << endl;

    return 0;
}

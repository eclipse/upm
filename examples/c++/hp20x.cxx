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
#include <stdio.h>

#include "hp20x.hpp"
#include "upm_utilities.h"

using namespace std;
using namespace upm;

int shouldRun = true;

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
    // Instantiate an HP20X on default I2C bus and address

    upm::HP20X bar;

    // Initialize the device with default values
    bar.init();

    // Output data every second until interrupted
    while (shouldRun) {
        printf("Temperature: %f Celsius\n", bar.getTemperature());
        printf("Pressure:    %f Millibars\n", bar.getPressure());
        printf("Altitude:    %f Meters\n", bar.getAltitude());

        printf("\n");

        upm_delay(1);
    }
    //! [Interesting]

    cout << "Exiting..." << endl;

    return 0;
}

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
#include "wheelencoder.hpp"

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
    // Instantiate a DFRobot Wheel Encoder on digital pin D2
    upm::WheelEncoder sensor(2);

    // set the counter to 0 and start counting
    sensor.clearCounter();
    sensor.startCounter();

    while (shouldRun) {
        // output milliseconds passed and current sensor count
        cout << "Millis: " << sensor.getMillis() << " Count: " << sensor.counter() << endl;

        upm_delay(1);
    }

    sensor.stopCounter();
    //! [Interesting]

    cout << "Exiting..." << endl;

    return 0;
}

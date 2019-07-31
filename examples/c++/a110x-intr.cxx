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
#include <stddef.h>

#include "a110x.hpp"
#include "upm_utilities.h"

using namespace std;

int shouldRun = true;

void
sig_handler(int signo)
{
    if (signo == SIGINT)
        shouldRun = false;
}

// Our pulse counter
volatile unsigned int counter = 0;

// Our interrupt handler
void
hallISR(void* arg)
{
    counter++;
}

int
main()
{
    signal(SIGINT, sig_handler);

    //! [Interesting]
    // Instantiate an A110X sensor on digital pin D2
    upm::A110X hall(2);

    // This example uses a user-supplied interrupt handler to count
    // pulses that occur when a magnetic field of the correct polarity
    // is detected.  This could be used to measure the rotations per
    // minute (RPM) of a rotor for example.

    hall.installISR(hallISR, NULL);

    while (shouldRun) {
        cout << "Pulses detected: " << counter << endl;

        upm_delay(1);
    }
    //! [Interesting]

    cout << "Exiting..." << endl;

    return 0;
}

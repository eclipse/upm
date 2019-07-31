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

#include "rpr220.hpp"
#include "upm_utilities.h"

using namespace std;

int shouldRun = true;

void
sig_handler(int signo)
{
    if (signo == SIGINT)
        shouldRun = false;
}

volatile unsigned int counter = 0;

// Our interrupt handler
void
rprISR(void* arg)
{
    counter++;
}

int
main()
{
    signal(SIGINT, sig_handler);

    //! [Interesting]
    // This example uses an interrupt handler to increment a counter

    // Instantiate an RPR220 digital pin D2
    // This was tested on the Grove IR Reflective Sensor

    upm::RPR220 rpr220(2);

    // Here, we setup our Interupt Service Routine (ISR) to count
    // 'black' pulses detected.

    rpr220.installISR(rprISR, NULL);

    while (shouldRun) {
        cout << "Counter: " << counter << endl;

        upm_delay(1);
    }
    //! [Interesting]

    cout << "Exiting..." << endl;

    return 0;
}

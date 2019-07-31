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

#include "mq303a.hpp"
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
    // Instantiate an mq303a sensor on analog pin A0

    // This device uses a heater powered from an analog I/O pin.
    // If using A0 as the data pin, then you need to use A1, as the heater
    // pin (if using a grove mq303a).  For A1, we can use the D15 gpio,
    // setup as an output, and drive it low to power the heater.

    upm::MQ303A mq303a(0, 15);

    cout << "Enabling heater and waiting 2 minutes for warmup." << endl;
    mq303a.heaterEnable(true);
    upm_delay(120);

    cout << "This sensor may need to warm until the value drops below about 450." << endl;

    // Print the detected alcohol value every second
    while (shouldRun) {
        int val = mq303a.value();

        cout << "Alcohol detected (higher means stronger alcohol): " << val << endl;

        upm_delay(1);
    }
    //! [Interesting]

    cout << "Exiting" << endl;
    mq303a.heaterEnable(false);

    return 0;
}

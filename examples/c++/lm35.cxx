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

#include "lm35.hpp"
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

    // Instantiate a LM35 on analog pin A0, with a default analog
    // reference voltage of 5.0
    upm::iTemperature* sensor = new upm::LM35(0);

    // Every half second, sample the sensor and output the temperature

    while (shouldRun) {
        cout << "Temperature: " << sensor->getTemperature() << " C" << endl;

        upm_delay_us(500000);
    }

    //! [Interesting]

    cout << "Exiting" << endl;
    delete sensor;

    return 0;
}

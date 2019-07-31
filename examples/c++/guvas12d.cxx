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

#include "guvas12d.hpp"
#include "upm_utilities.h"

using namespace std;

bool shouldRun = true;

// analog voltage, usually 3.3 or 5.0
#define GUVAS12D_AREF 5.0

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
    // This was tested with the Grove UV Sensor module.
    // It has a sensing range from between 240-370nm.  It's strongest
    // response is around 320-360nm.

    // Instantiate a GUVAS12D on analog pin A0
    upm::GUVAS12D volts(0);

    // The higher the voltage the more intense the UV radiation.

    while (shouldRun) {
        cout << "Volts: " << volts.volts() << ", Intensity: " << volts.intensity() << " mW/m^2"
             << endl;

        upm_delay(1);
    }
    //! [Interesting]

    cout << "Exiting" << endl;

    return 0;
}

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

#include "gp2y0a.hpp"
#include "upm_utilities.h"

using namespace std;

bool shouldRun = true;

// analog voltage, usually 3.3 or 5.0
#define GP2Y0A_AREF 5.0
#define SAMPLES_PER_QUERY 20

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
    // Note, for the Grove 80cm version of this sensor, due to the way
    // it is wired, you need to plug this into the A0 port, where it
    // will use the available A1 pin for data.

    // Instantiate a GP2Y0A on analog pin A1
    upm::GP2Y0A volts(1);

    // The higher the voltage (closer to AREF) the closer the object is.  NOTE:
    // the measured voltage will probably not exceed 3.3 volts.
    // Every second, print the averaged voltage value (averaged over 20 samples).
    while (shouldRun) {
        cout << "AREF: " << GP2Y0A_AREF << ", Voltage value (higher means closer): "
             << volts.value(GP2Y0A_AREF, SAMPLES_PER_QUERY) << endl;

        upm_delay(1);
    }
    //! [Interesting]

    cout << "Exiting" << endl;

    return 0;
}

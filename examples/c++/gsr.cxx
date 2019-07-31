/*
 * Author: Zion Orent <zorent@ics.com>
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

#include "gsr.hpp"
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
    // The was tested with the GSR Galvanic Skin Response Sensor module.

    // Instantiate a GSR on analog pin A0
    upm::GSR gsr(0);
    cout << "Calibrating...." << endl;
    gsr.calibrate();

    while (shouldRun) {
        cout << gsr.value() << endl;
        upm_delay_us(500000);
    }
    //! [Interesting]

    cout << "Exiting" << endl;

    return 0;
}

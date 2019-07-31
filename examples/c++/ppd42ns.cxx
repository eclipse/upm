/*
 * Author: Zion Orent <zorent@ics.com>
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
#include <string>

#include "ppd42ns.hpp"

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
    // Instantiate a dust sensor on GPIO pin D8
    upm::PPD42NS dust(8);
    ppd42ns_dust_data data;
    cout << "This program will give readings every 30 seconds until "
         << "you stop it" << endl;
    while (shouldRun) {
        data = dust.getData();
        cout << "Low pulse occupancy: " << data.lowPulseOccupancy << endl;
        cout << "Ratio: " << data.ratio << endl;
        cout << "Concentration: " << data.concentration << endl;
        cout << endl;
    }
    //! [Interesting]

    cout << "Exiting" << endl;

    return 0;
}

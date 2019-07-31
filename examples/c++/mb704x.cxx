/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <iostream>
#include <mb704x.hpp>
#include <signal.h>
#include <upm_utilities.h>

using namespace std;
using namespace upm;

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

    // Instantiate a MB704X sensor using default parameters (bus 0,
    // address 112)
    upm::MB704X sensor;

    while (shouldRun) {
        cout << "Range: " << sensor.getRange() << " cm" << endl;

        upm_delay_ms(500);
    }

    cout << "Exiting..." << endl;

    //! [Interesting]

    return 0;
}

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
#include <signal.h>

#include "hka5.hpp"
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

    // Instantiate a HKA5 sensor on uart 0.  We don't use the set or
    // reset pins, so we pass -1 for them.
    upm::HKA5 sensor(0, -1, -1);

    // update once every 2 seconds and output data
    while (shouldRun) {
        sensor.update();

        cout << "PM 1  : " << sensor.getPM1() << " ug/m3" << endl;
        cout << "PM 2.5: " << sensor.getPM2_5() << " ug/m3" << endl;
        cout << "PM 10 : " << sensor.getPM10() << " ug/m3" << endl;
        cout << endl;

        upm_delay(2);
    }

    if (shouldRun)
        cerr << "Timed out" << endl;

    //! [Interesting]

    cout << "Exiting" << endl;

    return 0;
}

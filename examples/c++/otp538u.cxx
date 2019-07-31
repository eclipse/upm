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

#include <iomanip>
#include <iostream>
#include <signal.h>
#include <stdexcept>
#include <string>

#include "otp538u.hpp"
#include "upm_utilities.h"

using namespace std;

bool shouldRun = true;

// analog voltage, usually 3.3 or 5.0
#define OTP538U_AREF 5.0

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

    // Instantiate a OTP538U on analog pins A0 and A1
    // A0 is used for the Ambient Temperature and A1 is used for the
    // Object temperature.
    upm::OTP538U temps(0, 1, OTP538U_AREF);

    // enable debugging if you would like
    // temps.setDebug(true);

    // Output ambient and object temperatures
    while (shouldRun) {
        try {
            cout << "Ambient temp: " << std::fixed << setprecision(2) << temps.ambientTemperature()
                 << " C, Object temp: " << temps.objectTemperature() << " C" << endl;
        } catch (std::out_of_range& e) {
            cerr << "Temperature(s) are out of range: " << e.what() << endl;
        }

        cout << endl;
        upm_delay(1);
    }
    //! [Interesting]

    cout << "Exiting" << endl;

    return 0;
}

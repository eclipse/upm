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

#include "upm_utilities.h"
#include "vdiv.hpp"

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
    // Instantiate a Grove Voltage Divider sensor on analog pin A0
    upm::VDiv vDiv(0);

    // collect data and output measured voltage according to the setting
    // of the scaling switch (3 or 10)
    while (shouldRun) {
        unsigned int val = vDiv.value(100);
        float gain3val = vDiv.computedValue(3, val);
        float gain10val = vDiv.computedValue(10, val);
        cout << "ADC value: " << val << " Gain 3: " << gain3val << "v Gain 10: " << gain10val << "v"
             << endl;

        upm_delay(1);
    }
    //! [Interesting]

    cout << "Exiting..." << endl;

    return 0;
}

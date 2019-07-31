/*
 * Author: Abhishek Malik <abhishek.malik@intel.com>
 * Copyright (c) 2017 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <iostream>
#include <signal.h>

#include "abp.hpp"
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
    // Instantiate an ABP sensor on i2c bus 0
    upm::ABP abp(0, ABP_DEFAULT_ADDRESS);
    while (shouldRun) {
        abp.update();
        cout << "Retrieved pressure: " << abp.getPressure() << endl;
        cout << "Retrieved Temperature: " << abp.getTemperature() << endl;

        upm_delay(1);
    }
    //! [Interesting]

    cout << "Exiting..." << endl;

    return 0;
}

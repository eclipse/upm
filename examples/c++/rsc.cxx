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

#include "rsc.hpp"
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

    upm::RSC rsc(0, 9, 8);

    cout << "Sensor Name: " << rsc.getSensorName() << endl;
    rsc.setMode(NORMAL_MODE);
    rsc.setDataRate(N_DR_330_SPS);
    while (shouldRun) {
        cout << "inH2O pressure: " << rsc.getPressure() << endl;
        cout << "Temp (C): " << rsc.getTemperature() << endl;

        upm_delay(1);
    }
    //! [Interesting]

    cout << "Exiting..." << endl;

    return 0;
}

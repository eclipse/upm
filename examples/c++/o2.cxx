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

#include "o2.hpp"
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
main(int argc, char** argv)
{
    signal(SIGINT, sig_handler);

    //! [Interesting]
    // The was tested with the O2 Oxygen Concentration Sensor Module
    // Instantiate a O2 on analog pin A0
    upm::O2 O2(0);
    while (shouldRun) {
        cout << "The output voltage is: " << O2.voltageValue() << "mV" << endl;
        upm_delay_us(100000);
    }

    //! [Interesting]
    cout << "Exiting" << endl;
    return 0;
}

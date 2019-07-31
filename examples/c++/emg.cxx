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

#include "emg.hpp"
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
    // The was tested with the EMG Muscle Signal Reader Sensor Module
    // Instantiate a EMG on analog pin A0
    upm::EMG emg(0);
    cout << "Calibrating...." << endl;
    emg.calibrate();

    while (shouldRun) {
        cout << emg.value() << endl;
        upm_delay_us(100000);
    }

    //! [Interesting]
    cout << "Exiting" << endl;
    return 0;
}

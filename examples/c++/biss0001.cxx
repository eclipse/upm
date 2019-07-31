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

#include "biss0001.hpp"
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
    // Instantiate a Grove Motion sensor on GPIO pin D2
    upm::BISS0001 motion(2);

    while (shouldRun) {
        bool val = motion.value();

        if (val)
            cout << "Detecting moving object";
        else
            cout << "No moving objects detected";

        cout << endl;

        upm_delay(1);
    }
    //! [Interesting]

    cout << "Exiting" << endl;

    return 0;
}

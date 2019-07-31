/*
 * Author: Jon Trulson <jtrulson@ics.com>
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

#include "rpr220.hpp"
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
    // This example uses a simple method to determine current status

    // Instantiate an RPR220 digital pin D2
    // This was tested on the Grove IR Reflective Sensor

    upm::RPR220 rpr220(2);

    while (shouldRun) {
        if (rpr220.blackDetected())
            cout << "Black detected" << endl;
        else
            cout << "Black NOT detected" << endl;

        upm_delay_us(100000); // 100ms
    }
    //! [Interesting]

    cout << "Exiting..." << endl;

    return 0;
}

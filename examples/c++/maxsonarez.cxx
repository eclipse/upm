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

#include "maxsonarez.hpp"
#include "upm_utilities.h"

using namespace std;

bool shouldRun = true;

#define MAXSONAREZ_AREF 5.0

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

    // Instantiate a MaxSonarEZ on analog pin A1, with an analog
    // reference voltage of MAXSONAREZ_AREF
    upm::MAXSONAREZ sonar(1, MAXSONAREZ_AREF);

    // Every second, sample the sonar and output the distance to an
    // object in inches.

    // With the EZ3 version, the minimum and maximum ranges seemed to be
    // between 6 and 33 inches

    while (shouldRun) {
        cout << "AREF: " << MAXSONAREZ_AREF << ", distance in inches: " << sonar.inches() << endl;

        upm_delay(1);
    }

    //! [Interesting]

    cout << "Exiting" << endl;

    return 0;
}

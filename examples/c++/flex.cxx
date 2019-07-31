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

#include "flex.hpp"
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
    // The was tested with a Spectra Symbol flex sensor.
    // We attached a 22K resistor to a breadboard,
    // with 1 end attached to GND and the other connected to
    // both the flex sensor and A0.
    // The flex sensor was connected on 1 pin to the 22K resistor and A0
    // and on the other pin to 5V.

    // Instantiate a Flex sensor on analog pin A0
    upm::Flex flex(0);

    while (shouldRun) {
        cout << "Flex value: " << flex.value() << endl;
        upm_delay(1);
    }
    //! [Interesting]

    cout << "Exiting" << endl;

    return 0;
}

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

#include "l298.hpp"
#include "upm_utilities.h"

using namespace std;

int
main()
{
    //! [Interesting]

    // Instantiate one of the 2 possible DC motors on a L298 Dual
    // H-Bridge.  For controlling a stepper motor, see the l298-stepper
    // example.
    upm::L298 l298(3, 4, 7);

    cout << "Starting motor at 50% for 3 seconds..." << endl;
    l298.setSpeed(50);
    l298.setDirection(upm::L298::DIR_CW);
    l298.enable(true);

    upm_delay(3);

    cout << "Reversing direction..." << endl;
    l298.setDirection(upm::L298::DIR_NONE); // fast stop
    l298.setDirection(upm::L298::DIR_CCW);
    upm_delay(3);

    l298.setSpeed(0);
    l298.enable(false);

    //! [Interesting]

    cout << "Exiting..." << endl;

    return 0;
}

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

    // Instantiate a Stepper motor on a L298 Dual H-Bridge.

    // This was tested with the NEMA-17 12V, 350mA, with 200 steps per rev.
    upm::L298 l298(200, 3, 4, 7, 8, 9);

    l298.setSpeed(10); // 10 RPMs
    l298.setDirection(upm::L298::DIR_CW);
    l298.enable(true);
    cout << "Rotating 1 full revolution at 10 RPM speed." << endl;
    // move 200 steps, a full rev
    l298.stepperSteps(200);
    cout << "Sleeping for 2 seconds..." << endl;
    upm_delay(2);
    cout << "Rotating 1/2 revolution in opposite direction at 10 RPM speed." << endl;
    l298.setDirection(upm::L298::DIR_CCW);
    l298.stepperSteps(100);
    // release
    l298.enable(false);

    //! [Interesting]

    cout << "Exiting..." << endl;

    return 0;
}

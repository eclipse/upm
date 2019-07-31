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

#include "uln200xa.hpp"
#include "upm_utilities.h"

using namespace std;

int
main()
{
    //! [Interesting]

    // Instantiate a Stepper motor on a ULN200XA Darlington controller.

    // This was tested with the Grove Gear Stepper Motor with Driver

    // Wire the pins so that I1 is pin D8, I2 is pin D9, I3 is pin D10 and
    // I4 is pin D11
    upm::ULN200XA uln200xa(4096, 8, 9, 10, 11);

    uln200xa.setSpeed(5);
    uln200xa.setDirection(ULN200XA_DIR_CW);
    cout << "Rotating 1 revolution clockwise." << endl;
    uln200xa.stepperSteps(4096);
    cout << "Sleeping for 2 seconds..." << endl;
    upm_delay(2);
    cout << "Rotating 1/2 revolution counter clockwise." << endl;
    uln200xa.setDirection(ULN200XA_DIR_CCW);
    uln200xa.stepperSteps(2048);

    // turn off the power
    uln200xa.release();

    //! [Interesting]
    cout << "Exiting..." << endl;

    return 0;
}

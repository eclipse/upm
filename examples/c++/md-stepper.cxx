/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015-2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <iostream>

#include "md.hpp"
#include "md_defs.h"
#include "upm_utilities.h"

using namespace std;

int
main(int argc, char** argv)
{
    //! [Interesting]
    // Instantiate an I2C Motor Driver on I2C bus 0

    upm::MD motors(MD_I2C_BUS, MD_DEFAULT_I2C_ADDR);

    // This example demonstrates using the MD to drive a stepper motor

    // configure it, for this example, we'll assume 200 steps per rev
    motors.configStepper(200);

    // set for half a rotation
    motors.setStepperSteps(100);

    // let it go - clockwise rotation, 10 RPM speed
    motors.enableStepper(MD_STEP_DIR_CW, 10);

    upm_delay(3);

    // Now do it backwards...
    motors.setStepperSteps(100);
    motors.enableStepper(MD_STEP_DIR_CCW, 10);

    // now disable
    motors.disableStepper();

    //! [Interesting]

    cout << "Exiting..." << endl;

    return 0;
}

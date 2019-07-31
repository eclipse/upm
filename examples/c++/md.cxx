/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2014-2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <iostream>

#include "md.hpp"
#include "upm_utilities.h"

using namespace std;

int
main(int argc, char** argv)
{
    //! [Interesting]
    // Instantiate an I2C Motor Driver on I2C bus 0

    upm::MD motors(MD_I2C_BUS, MD_DEFAULT_I2C_ADDR);

    // set direction to CW and set speed to 50%
    cout << "Spin M1 and M2 at half speed for 3 seconds" << endl;
    motors.setMotorDirections(MD_DIR_CW, MD_DIR_CW);
    motors.setMotorSpeeds(127, 127);

    upm_delay(3);
    // counter clockwise
    cout << "Reversing M1 and M2 for 3 seconds" << endl;
    motors.setMotorDirections(MD_DIR_CCW, MD_DIR_CCW);
    upm_delay(3);

    //! [Interesting]

    cout << "Stopping motors" << endl;
    motors.setMotorSpeeds(0, 0);

    cout << "Exiting..." << endl;

    return 0;
}

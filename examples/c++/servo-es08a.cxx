/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <iostream>

#include "es08a.hpp"
#include "upm_utilities.h"

int
main(int argc, char** argv)
{
    //! [Interesting]
    upm::ES08A servo(5);

    // Sets the shaft to 180, then to 90, then to 0,
    // then back to 90, and finally back to 180,
    // pausing for a second in between each angle
    servo.setAngle(180);
    std::cout << "Set angle to 180" << std::endl;
    upm_delay(1);
    servo.setAngle(90);
    std::cout << "Set angle to 90" << std::endl;
    upm_delay(1);
    servo.setAngle(0);
    std::cout << "Set angle to 0" << std::endl;
    upm_delay(1);
    servo.setAngle(90);
    std::cout << "Set angle to 90" << std::endl;
    upm_delay(1);
    servo.setAngle(180);
    std::cout << "Set angle to 180" << std::endl;
    //! [Interesting]

    std::cout << "exiting application" << std::endl;

    return 0;
}

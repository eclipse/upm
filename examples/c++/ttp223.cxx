/*
 * Author: Sarah Knepper <sarah.knepper@intel.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <iostream>

#include "ttp223.hpp"
#include "upm_utilities.h"

int
main(int argc, char** argv)
{
    // This example uses GPIO 0
    //! [Interesting]

    // Create the TTP223 touch sensor object using GPIO pin 0
    upm::TTP223 touch(0);

    // Check whether or not a finger is near the touch sensor and
    // print accordingly, waiting one second between readings
    while (1) {
        if (touch.isPressed()) {
            std::cout << touch.name() << " is pressed" << std::endl;
        } else {
            std::cout << touch.name() << " is not pressed" << std::endl;
        }
        upm_delay(1);
    }

    // Delete the touch sensor object
    //! [Interesting]

    return 0;
}

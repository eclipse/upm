/*
 * Author: Brendan Le Foll <brendan.le.foll@intel.com>
 * Contributions: Sarah Knepper <sarah.knepper@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <iostream>

#include "led.hpp"
#include "upm_utilities.h"

int
main(int argc, char** argv)
{
    //! [Interesting]

    // Create the Grove LED object using GPIO pin 2
    upm::Led led(2);

    // Print the name
    std::cout << led.name() << std::endl;

    // Turn the LED on and off 10 times, pausing one second
    // between transitions
    for (int i = 0; i < 10; i++) {
        led.on();
        upm_delay(1);
        led.off();
        upm_delay(1);
    }

    // Delete the Grove LED object
    //! [Interesting]

    return 0;
}

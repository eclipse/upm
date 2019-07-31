/*
 * Author: Sarah Knepper <sarah.knepper@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <iostream>

#include "button.hpp"
#include "upm_utilities.h"

int
main(int argc, char** argv)
{
    // This example uses GPIO 0
    //! [Interesting]

    // Create the button object using GPIO pin 0
    upm::Button button(0);

    // Read the input and print, waiting one second between readings
    while (1) {
        std::cout << button.name() << " value is " << button.value() << std::endl;
        upm_delay(1);
    }

    // Delete the button object
    //! [Interesting]

    return 0;
}

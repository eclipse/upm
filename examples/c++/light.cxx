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

#include "light.hpp"
#include "upm_utilities.h"

int
main(int argc, char** argv)
{
    //! [Interesting]
    // Create the light sensor object using AIO pin 0
    upm::Light light(0);

    // Read the input and print both the normalized ADC value and a
    // rough lux value, waiting one second between readings
    while (1) {
        std::cout << light.name() << " normalized value is " << light.getNormalized()
                  << ", which is roughly " << light.value() << " lux" << std::endl;
        upm_delay(1);
    }

    // Delete the light sensor object
    //! [Interesting]
    return 0;
}

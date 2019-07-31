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
#include <stdio.h>

#include "temperature.hpp"
#include "upm_utilities.h"

using namespace std;

int
main(int argc, char** argv)
{
    //! [Interesting]

    // Create the temperature sensor object using AIO pin 0
    upm::Temperature temp(0);
    std::cout << temp.name() << std::endl;

    // Read the temperature ten times, printing both the Celsius and
    // equivalent Fahrenheit temperature, waiting one second between readings
    for (int i = 0; i < 10; i++) {
        int celsius = temp.value();
        int fahrenheit = (int) (celsius * 9.0 / 5.0 + 32.0);
        printf("%d degrees Celsius, or %d degrees Fahrenheit\n", celsius, fahrenheit);
        upm_delay(1);
    }

    // Delete the temperature sensor object
    //! [Interesting]

    return 0;
}

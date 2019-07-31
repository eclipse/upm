/*
 * The MIT License (MIT)
 *
 * Author: Your Full Name <your@email.address>
 * Copyright (c) <year> <copyright holder>
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <iostream>
#include <string>

#include "sensortemplate.hpp"
#include "upm_utilities.h"

int
main()
{
    //! [Interesting]
    // Create an instance of SensorTemplate
    upm::SensorTemplate sensor(0);

    while (true) {
        std::cout << "SensorTemplate says: " << sensor.helloWorld() << std::endl;

        // Repeat every 2 seconds
        upm_delay_us(2000000);
    }
    //! [Interesting]
    return 0;
}

/*
 * Author: Henry Bruce <henry.bruce@intel.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <exception>
#include <iostream>
#include <stddef.h>
#include <stdint.h>

#include "mraa/common.h"
#include "t6713.hpp"
#include "upm_utilities.h"

#define EDISON_I2C_BUS 1
#define FT4222_I2C_BUS 0

//! [Interesting]
int
main()
{
    /* Create an instance of the T6713 sensor */
    upm::T6713 cO2Sensor(EDISON_I2C_BUS);

    // if (cO2Sensor == NULL) {
    //     std::cout << "CO2 sensor not detected" << std::endl;
    //     return 1;
    // }
    std::cout << "CO2 sensor " << cO2Sensor.getModuleName() << " detected" << std::endl;
    while (true) {
        try {
            uint16_t value = cO2Sensor.getPpm();
            std::cout << "CO2 level = " << value << " ppm" << std::endl;
        } catch (std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
        upm_delay(1);
    }

    return 0;
}

//! [Interesting]

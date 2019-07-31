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

#include <iostream>

#include "mraa/common.h"
#include "t6713.hpp"
#include "upm_utilities.h"

#define FT4222_I2C_BUS  0
#define DEFAULT_I2C_BUS 0

int
main()
{
    //! [Interesting]
    upm::T6713 cO2Sensor(DEFAULT_I2C_BUS);

    while (true) {
        uint16_t value = cO2Sensor.getPpm();
        std::cout << "CO2 level = " << value << " ppm" << std::endl;
        upm_delay(1);
    }
    //! [Interesting]
    return 0;
}

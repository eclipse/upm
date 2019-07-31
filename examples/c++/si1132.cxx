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
#include "si1132.hpp"
#include "upm_utilities.h"

#define FT4222_I2C_BUS  1
#define DEFAULT_I2C_BUS 0

int
main()
{
    //! [Interesting]
    upm::SI1132 lightSensor(DEFAULT_I2C_BUS);
    while (true) {
        float value = lightSensor.getVisibleLux();
        std::cout << "Light level = " << value << " lux" << std::endl;
        upm_delay(1);
    }
    //! [Interesting]
    return 0;
}

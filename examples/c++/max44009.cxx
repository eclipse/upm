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

#include "max44009.hpp"
#include "upm_utilities.h"

#define EDISON_I2C_BUS 1

int
main()
{
    //! [Interesting]
    upm::MAX44009 lightSensor(EDISON_I2C_BUS);
    while (true) {
        float value = lightSensor.getVisibleLux();
        std::cout << "Light level = " << value << " lux" << std::endl;
        upm_delay(1);
    }
    //! [Interesting]
    return 0;
}

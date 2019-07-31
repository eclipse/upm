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

#include "si7005.hpp"
#include "upm_utilities.h"

#define EDISON_I2C_BUS 1
#define EDISON_GPIO_SI7005_CS 20

//! [Interesting]
int
main()
{
    try {
        upm::SI7005 sensor(EDISON_I2C_BUS, EDISON_GPIO_SI7005_CS);
        while (true) {
            int temperature = sensor.getTemperatureCelsius();
            int humidity = sensor.getHumidityRelative();
            std::cout << "Temperature = " << temperature << "C" << std::endl;
            std::cout << "Humidity    = " << humidity << "%" << std::endl;
            upm_delay(1);
        }
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
//! [Interesting]

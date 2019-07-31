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

#include "ads1015.hpp"
#include "mraa/gpio.hpp"
#include "upm_utilities.h"

#define EDISON_I2C_BUS 1
#define FT4222_I2C_BUS 0
#define EDISON_GPIO_SI7005_CS 20

int
main()
{
    /* Create an instance of the ADS1015 sensor */
    upm::ADS1015 adc(EDISON_I2C_BUS);
    mraa::Gpio gpio(EDISON_GPIO_SI7005_CS);
    gpio.dir(mraa::DIR_OUT_HIGH);

    // if (adc == NULL) {
    //     std::cout << "ADC not detected" << std::endl;
    //     return 1;
    // }
    std::cout << "ADC " << adc.getModuleName() << " detected. ";
    std::cout << adc.getNumInputs() << " inputs available" << std::endl;
    while (true) {
        for (unsigned int i = 0; i < adc.getNumInputs(); ++i) {
            std::cout << "Input " << i;
            try {
                float voltage = adc.getVoltage(i);
                std::cout << ": Voltage = " << voltage << "V" << std::endl;
            } catch (std::exception& e) {
                std::cerr << e.what() << std::endl;
            }
        }
        upm_delay(1);
    }

    return 0;
}

//! [Interesting]

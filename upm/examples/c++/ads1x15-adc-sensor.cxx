/*
 * Author: Henry Bruce <henry.bruce@intel.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <exception>
#include <iostream>
#include <stddef.h>

#include "ads1015.hpp"
#include "iADC.hpp"
#include "mraa/gpio.hpp"
#include "upm_utilities.h"

#define EDISON_I2C_BUS 1
#define FT4222_I2C_BUS 0
#define EDISON_GPIO_SI7005_CS 20

int
main()
{
    /* Create an instance of the ADS1015 sensor */
    upm::ADS1015 sensor(EDISON_I2C_BUS);
    mraa::Gpio gpio(EDISON_GPIO_SI7005_CS);
    gpio.dir(mraa::DIR_OUT_HIGH);

    /* Show usage from the IADC interface */
    upm::IADC* adc = static_cast<upm::IADC*>(&sensor);

    if (adc == NULL) {
        std::cout << "ADC not detected" << std::endl;
        return 1;
    }
    std::cout << "ADC " << adc->getModuleName() << " detected. ";
    std::cout << adc->getNumInputs() << " inputs available" << std::endl;
    while (true) {
        for (unsigned int i = 0; i < adc->getNumInputs(); ++i) {
            std::cout << "Input " << i;
            try {
                float voltage = adc->getVoltage(i);
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

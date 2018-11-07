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
#include <stdint.h>

#include "iCO2Sensor.hpp"
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
    upm::T6713 sensor(EDISON_I2C_BUS);

    /* Show usage from the ICO2Sensor interface */
    upm::ICO2Sensor* cO2Sensor = static_cast<upm::ICO2Sensor*>(&sensor);

    if (cO2Sensor == NULL) {
        std::cout << "CO2 sensor not detected" << std::endl;
        return 1;
    }
    std::cout << "CO2 sensor " << cO2Sensor->getModuleName() << " detected" << std::endl;
    while (true) {
        try {
            uint16_t value = cO2Sensor->getPpm();
            std::cout << "CO2 level = " << value << " ppm" << std::endl;
        } catch (std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
        upm_delay(1);
    }

    return 0;
}

//! [Interesting]

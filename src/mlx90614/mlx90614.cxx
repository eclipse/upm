/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
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

#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <stdexcept>

#include "mlx90614.hpp"

using namespace upm;

MLX90614::MLX90614 (int bus, int devAddr) : m_i2Ctx(bus) {
    m_name = "MLX90614";

    m_i2cAddr = devAddr;
    m_bus = bus;

    mraa::Result ret = m_i2Ctx.address(m_i2cAddr);
    if (ret != mraa::SUCCESS) {
        throw std::invalid_argument(std::string(__FUNCTION__) + 
                                    ": address() failed");
    }

    if ((ret = m_i2Ctx.frequency(mraa::I2C_STD)) != mraa::SUCCESS ) {
      throw std::invalid_argument(std::string(__FUNCTION__) +
                                  ": I2c.frequency(I2C_STD) failed");
      return;
    }
}

float
MLX90614::readObjectTempF(int objAddr) {
    return (readTemperature(objAddr) * 9 / 5) + 32;
}

float
MLX90614::readAmbientTempF(void) {
    return (readTemperature(MLX90614_TA) * 9 / 5) + 32;
}

float
MLX90614::readObjectTempC(int objAddr) {
    return readTemperature(objAddr);
}

float
MLX90614::readAmbientTempC(void) {
    return readTemperature(MLX90614_TA);
}

/*
 * **************
 *  private area
 * **************
 */
float
MLX90614::readTemperature (uint8_t address) {
    return m_i2Ctx.readWordReg(address) * 0.02 - 273.15;
}

/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
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

float
MLX90614::getTemperature() {
  return readAmbientTempC();
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

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
#include <string>
#include <stdexcept>
#include <unistd.h>
#include <stdlib.h>

#include "max44000.hpp"

using namespace upm;

MAX44000::MAX44000 (int bus, int devAddr) : m_i2cMaxControlCtx(bus) {
    m_name = "MAX44000";

    m_maxControlAddr = devAddr;
    m_bus = bus;

    mraa::Result ret = m_i2cMaxControlCtx.address(m_maxControlAddr);
    if (ret != mraa::SUCCESS) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_i2c_address() failed");
    }

    // i2cWriteReg (MCR, 0x2C);
    // i2cWriteReg (TCR, 0x6);
}

uint16_t
MAX44000::getProximity () {
    uint16_t data = 0;

    data = (i2cReadReg_8 (ALSDATA_HIGH) & 0x7F) << 8;
    data = data | i2cReadReg_8 (ALSDATA_LOW);

    return data;
}

float
MAX44000::getValue() {
    return (float) MAX44000::getProximity();
}

uint16_t
MAX44000::getAmbient () {
    uint16_t data = 0;

    data = (i2cReadReg_8 (ALSDATA_HIGH) & 0x7F) << 8;
    data = data | i2cReadReg_8 (ALSDATA_LOW);

    return data;
}

/*
 * **************
 *  private area
 * **************
 */
uint8_t
MAX44000::i2cReadReg_8 (int reg) {
    uint8_t data;

    m_i2cMaxControlCtx.writeByte(reg);

    m_i2cMaxControlCtx.read(&data, 0x1);

    return data;
}

uint16_t
MAX44000::i2cReadReg_16 (int reg) {
    uint16_t data;

    m_i2cMaxControlCtx.writeByte(reg);

    m_i2cMaxControlCtx.read((uint8_t *)&data, 0x2);

    return data;
}

mraa::Result
MAX44000::i2cWriteReg (uint8_t reg, uint8_t value) {
    mraa::Result error = mraa::SUCCESS;

    uint8_t data[2] = { reg, value };
    error = m_i2cMaxControlCtx.write (data, 2);

    return error;
}

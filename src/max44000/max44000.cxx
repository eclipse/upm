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
#include <string>
#include <stdexcept>
#include <unistd.h>
#include <stdlib.h>

#include "max44000.h"

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

    m_i2cMaxControlCtx.address(m_maxControlAddr);
    m_i2cMaxControlCtx.writeByte(reg);

    m_i2cMaxControlCtx.address(m_maxControlAddr);
    m_i2cMaxControlCtx.read(&data, 0x1);

    return data;
}

uint16_t
MAX44000::i2cReadReg_16 (int reg) {
    uint16_t data;

    m_i2cMaxControlCtx.address(m_maxControlAddr);
    m_i2cMaxControlCtx.writeByte(reg);

    m_i2cMaxControlCtx.address(m_maxControlAddr);
    m_i2cMaxControlCtx.read((uint8_t *)&data, 0x2);

    return data;
}

mraa::Result
MAX44000::i2cWriteReg (uint8_t reg, uint8_t value) {
    mraa::Result error = mraa::SUCCESS;

    uint8_t data[2] = { reg, value };
    error = m_i2cMaxControlCtx.address (m_maxControlAddr);
    error = m_i2cMaxControlCtx.write (data, 2);

    return error;
}

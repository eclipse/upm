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

#include "max44000.h"

using namespace upm;

MAX44000::MAX44000 (int bus, int devAddr) {
    m_name = "MAX44000";

    m_maxControlAddr = devAddr;
    m_bus = bus;

    m_i2cMaxControlCtx = maa_i2c_init(m_bus);

    maa_result_t ret = maa_i2c_address(m_i2cMaxControlCtx, m_maxControlAddr);
    if (ret != MAA_SUCCESS) {
        fprintf(stderr, "Messed up i2c bus\n");
    }

    i2cWriteReg (MCR, 0x2C);
    i2cWriteReg (TCR, 0x6);
}

MAX44000::~MAX44000() {
    maa_i2c_stop(m_i2cMaxControlCtx);
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

    maa_i2c_address(m_i2cMaxControlCtx, m_maxControlAddr);
    maa_i2c_write_byte(m_i2cMaxControlCtx, reg);

    maa_i2c_address(m_i2cMaxControlCtx, m_maxControlAddr);
    maa_i2c_read(m_i2cMaxControlCtx, &data, 0x1);

    return data;
}

uint16_t
MAX44000::i2cReadReg_16 (int reg) {
    uint16_t data;

    maa_i2c_address(m_i2cMaxControlCtx, m_maxControlAddr);
    maa_i2c_write_byte(m_i2cMaxControlCtx, reg);

    maa_i2c_address(m_i2cMaxControlCtx, m_maxControlAddr);
    maa_i2c_read(m_i2cMaxControlCtx, (uint8_t *)&data, 0x2);

    return data;
}

maa_result_t
MAX44000::i2cWriteReg (uint8_t reg, uint8_t value) {
    maa_result_t error = MAA_SUCCESS;

    uint8_t data[2] = { reg, value };
    error = maa_i2c_address (m_i2cMaxControlCtx, m_maxControlAddr);
    error = maa_i2c_write (m_i2cMaxControlCtx, data, 2);

    return error;
}

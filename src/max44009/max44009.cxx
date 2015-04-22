/*
 * Author: Scott Ware <scott.r.ware@intel.com>
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
#include <math.h>

#include "max44009.h"

using namespace upm;

MAX44009::MAX44009 (int bus, int devAddr) {

    m_maxControlAddr = devAddr;
    m_bus = bus;
    configured = false;

    m_i2cMaxControlCtx = mraa_i2c_init(m_bus);

    mraa_result_t status = mraa_i2c_address(m_i2cMaxControlCtx, m_maxControlAddr);
    if (status != MRAA_SUCCESS) {
        fprintf(stderr, "MAX44009: I2C bus failed to initialise.\n");
        configured = false;
        return;
    }

    // Reset chip to defaults
    if (reset() != MRAA_SUCCESS) {
        configured = false;
        return;
    }

    configured = true;
}

MAX44009::~MAX44009() {
    mraa_i2c_stop(m_i2cMaxControlCtx);
}

mraa_result_t
MAX44009::reset() {
    uint8_t address[5] = {MAX44009_INT_ENABLE_ADDR, MAX44009_CONFIG_ADDR, \
                                   MAX44009_THR_HIGH_ADDR, MAX44009_THR_LOW_ADDR, \
                                   MAX44009_THR_TIMER_ADDR};
    uint8_t value[5]   = {MAX44009_INT_DISABLED, MAX44009_DEFAULT_CONFIGURATION, \
                                   0xFF, 0x00, 0xFF};
    uint8_t data[2];

    uint8_t i;
    mraa_result_t status = MRAA_SUCCESS;

    for (i = 0; i < sizeof(address); i++) {
        data[0] = address[i];
        data[1] = value[i];
        mraa_i2c_address(m_i2cMaxControlCtx, m_maxControlAddr);
        status = mraa_i2c_write (m_i2cMaxControlCtx, data, sizeof(data));
    }

    return status;
}

mraa_result_t
MAX44009::getValue(float* value) {
    uint8_t exponent, mantissa;
    uint8_t data[MAX44009_LUX_LENGTH];

    mraa_i2c_address(m_i2cMaxControlCtx, m_maxControlAddr);
    int length = mraa_i2c_read_bytes_data(m_i2cMaxControlCtx, MAX44009_LUX_START_ADDR, data, MAX44009_LUX_LENGTH);

    if(length != MAX44009_LUX_LENGTH) { return MRAA_ERROR_INVALID_RESOURCE; }

    exponent = (( data[MAX44009_LUX_HIGH] & 0xF0 ) >> 4);
    mantissa = (( data[MAX44009_LUX_HIGH] & 0x0F ) << 4 ) | ( data[MAX44009_LUX_LOW] & 0x0F );

    // Check for overrange condition
    if(exponent == MAX44009_OVERRANGE_CONDITION) { return MRAA_ERROR_INVALID_RESOURCE; }

    *value = pow((double)2,(double)exponent) * mantissa * 0.045;

    return MRAA_SUCCESS;
}

bool
MAX44009::isConfigured() {
    return configured;
}

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
    if (status == MRAA_SUCCESS) {
        configured = true;
    }
    else {
        fprintf(stderr, "MAX44009: I2C bus failed to initialise.\n");
    }
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
MAX44009::getValue(uint16_t* value) {
    uint8_t exponent, mantissa;
    uint8_t data[2];
    uint16_t result;

    mraa_i2c_address(m_i2cMaxControlCtx, m_maxControlAddr);
    data[0] = mraa_i2c_read_byte_data(m_i2cMaxControlCtx, MAX44009_LUX_HIGH_ADDR);
    mraa_i2c_address(m_i2cMaxControlCtx, m_maxControlAddr);
    data[1] = mraa_i2c_read_byte_data(m_i2cMaxControlCtx, MAX44009_LUX_LOW_ADDR);

    if(data[0] == -1 || data[1] == -1) {
        return MRAA_ERROR_INVALID_RESOURCE;
    }

    exponent = (( data[0] >> 4 ) & 0x0F);
    mantissa = (( data[0] & 0x0F ) << 4 );
    mantissa |= ( data[1] & 0x0F );

    // Check for overrange condition
    if(exponent == MAX44009_OVERRANGE_CONDITION) { return MRAA_ERROR_INVALID_RESOURCE; }

    result = ( (uint16_t) exponent << 8 );
    result |= ( (uint16_t) mantissa << 0);
    *value = result;

    return MRAA_SUCCESS;
}

float
MAX44009::convertToLux(uint16_t value) {
    uint8_t exponent, mantissa;
    float result = 0.045;

    exponent = ( value >> 8 ) & 0xFF;
    mantissa = (value >> 0) & 0xFF;

    result *= 2^exponent * mantissa;

    return result;
}

bool
MAX44009::isConfigured() {
    return configured;
}

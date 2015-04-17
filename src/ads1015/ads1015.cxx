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
#include <string>

#include "ads1015.h"

using namespace upm;
using namespace std;


ADS1015::ADS1015(int bus, uint8_t address, float vref)
{
    // Setup i2c
    m_bus = bus;
    m_addr = address;
    m_vref = vref;
    configured = false;

    m_i2c = mraa_i2c_init(m_bus);

    mraa_result_t status = mraa_i2c_address(m_i2c, m_addr);
    if (status == MRAA_SUCCESS) {
        configured = true;
    }
    else {
        fprintf(stderr, "ADS1015: I2C bus failed to initialise.\n");
    }

    // Set parameters
    m_gain = GAIN_TWO;
}

ADS1015::~ADS1015()
{
    mraa_i2c_stop(m_i2c);
}

mraa_result_t
ADS1015::getValue(int input, uint16_t *value) {

    mraa_result_t status;
    uint16_t result;

    // Check input is in range
    if (input > 3) { return MRAA_ERROR_INVALID_PARAMETER; }

    // Start with default values
    uint16_t config = ADS1015_REG_CONFIG_CQUE_NONE    | // Disable the comparator (default val)
                      ADS1015_REG_CONFIG_CLAT_NONLAT  | // Non-latching (default val)
                      ADS1015_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
                      ADS1015_REG_CONFIG_CMODE_TRAD   | // Traditional comparator (default val)
                      ADS1015_REG_CONFIG_DR_1600SPS   | // 1600 samples per second (default)
                      ADS1015_REG_CONFIG_MODE_SINGLE;   // Single-shot mode (default)

    // Set PGA/voltage range
    config |= m_gain;

    // Set single-ended input channel
    switch (input)
    {
    case (0):
        config |= ADS1015_REG_CONFIG_MUX_SINGLE_0;
        break;
    case (1):
        config |= ADS1015_REG_CONFIG_MUX_SINGLE_1;
        break;
    case (2):
        config |= ADS1015_REG_CONFIG_MUX_SINGLE_2;
        break;
    case (3):
        config |= ADS1015_REG_CONFIG_MUX_SINGLE_3;
        break;
    }

    // Set 'start single-conversion' bit
    config |= ADS1015_REG_CONFIG_OS_SINGLE;

    // Write config register to the ADC
    mraa_i2c_address(m_i2c, m_addr);
    status = mraa_i2c_write_word_data(m_i2c, config, ADS1015_REG_POINTER_CONFIG);

    if(status != MRAA_SUCCESS) { return status; }

    // Write to pointer register to point to the conversion register
    status = mraa_i2c_write(m_i2c, ADS1015_REG_POINTER_CONVERT, 2);

    if(status != MRAA_SUCCESS) { return status; }

    // Read conversion result
    result = mraa_i2c_read_word_data(m_i2c, ADS1015_REG_POINTER_CONVERT) >> ADS1015_BITSHIFT;

    if(result == -1) { return MRAA_ERROR_INVALID_RESOURCE; }

    *value = result;
}

void
ADS1015::setGain(adsGain_t gain) { m_gain = gain; }

adsGain_t
ADS1015::getGain() { return m_gain; }

float
ADS1015::convertToVolts(uint16_t value) { return (value * m_vref / ADS1015_RESOLUTION); }

bool
ADS1015::isConfigured() { return configured; }

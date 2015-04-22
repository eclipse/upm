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
    m_gain = GAIN_TWOTHIRDS;
}

ADS1015::~ADS1015()
{
    mraa_i2c_stop(m_i2c);
}

mraa_result_t
ADS1015::getValue(int input, uint16_t *value) {

    uint8_t result[ADS1015_CONVERSION_REG_LENGTH];

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
    uint8_t configData[3];
    configData[0] = ADS1015_REG_POINTER_CONFIG;
    configData[1] = (uint8_t)(config >> 8);
    configData[2]= (uint8_t)(config & 0xFF);

    mraa_i2c_address(m_i2c, m_addr);
    mraa_result_t status = mraa_i2c_write(m_i2c, configData, 3);

    if(status != MRAA_SUCCESS) { fprintf(stderr, "ADS1015: Failed to write config.\n"); return status; }

    // Wait 1ms
    usleep(1000);

    // Write to pointer register
    mraa_i2c_address(m_i2c, m_addr);
    status = mraa_i2c_write_byte(m_i2c, ADS1015_REG_POINTER_CONVERT);

    if(status != MRAA_SUCCESS) { return status; }

    // Read conversion result
    mraa_i2c_address(m_i2c, m_addr);
    int length = mraa_i2c_read(m_i2c, result, ADS1015_CONVERSION_REG_LENGTH);

    if(length != ADS1015_CONVERSION_REG_LENGTH) { fprintf(stderr, "ADS1015: Failed to read conversion register.\n"); return MRAA_ERROR_INVALID_RESOURCE; }

    *value = (uint16_t)(result[ADS1015_CONVERSION_MSB] << 8) | (uint16_t)(result[ADS1015_CONVERSION_LSB]);
    *value = *value >> ADS1015_BITSHIFT;
}

void
ADS1015::setGain(adsGain_t gain) { m_gain = gain; }

adsGain_t
ADS1015::getGain() { return m_gain; }

float
ADS1015::convertToVolts(uint16_t value) { return ((float)value * m_vref / ADS1015_RESOLUTION); }

bool
ADS1015::isConfigured() { return configured; }

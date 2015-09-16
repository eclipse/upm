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

#include "max31723.h"

using namespace upm;

MAX31723::MAX31723 (int csn) : m_csnPinCtx(csn), m_spi(0) {
    mraa::Result error = mraa::SUCCESS;
    m_name = "MAX31723";

    error = m_csnPinCtx.dir (mraa::DIR_OUT);
    if (error != mraa::SUCCESS) {
        throw std::invalid_argument(std::string(__FUNCTION__) + 
                                    ": m_csnPinCtx.dir() failed");
    }

    CSOff ();

    // set spi mode to mode2 (CPOL = 1, CPHA = 0)
    m_spi.mode (mraa::SPI_MODE2);
    // set ontinuously perform temperature conversions
    writeRegister (R_STS_WRITE_CMD, B_CONT_READING);
}

short
MAX31723::getTemperature () {
    uint8_t lsb = 0;
    uint8_t msb = 0;
    short temperature = 0;

    lsb = readRegister (R_TEMPERATURE_LSB);
    msb = readRegister (R_TEMPERATURE_MSB);

    if ((msb & 0x80) != 0) {
        msb &= 0x7F;
        temperature = 0 - msb;

    } else {
        temperature = msb;
    }

    return temperature;
}

/*
 * **************
 *  private area
 * **************
 */

uint8_t
MAX31723::readRegister (uint8_t reg) {
    uint8_t     data[2]     = { 0x00, 0x00 };
    uint8_t*    sensorData  = NULL;

    CSOn ();
    data[0] = reg;
    sensorData = m_spi.write(data, 2);
    CSOff ();

    return sensorData[1];
}

void
MAX31723::writeRegister (uint8_t reg, uint8_t data) {
    uint8_t     buffer[2]   = { 0x00, 0x00 };
    uint8_t*    sensorData  = NULL;

    CSOn ();
    buffer[0] = reg;
    buffer[1] = data;
    sensorData = m_spi.write(buffer, 2);
    CSOff ();
}

mraa::Result
MAX31723::CSOn () {
    return m_csnPinCtx.write (HIGH);
}

mraa::Result
MAX31723::CSOff () {
    return m_csnPinCtx.write (LOW);
}
